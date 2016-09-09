function [S,density,algd] = run_densest_subgraph(A)
% Write output and run the commands

% assert(issymmetric(A));
% assert(~any(diag(A)));

mydir = fileparts(mfilename('fullpath'));
mygfile = fullfile(mydir,'run_dsubgraph.smat');
myoutfile = fullfile(mydir,'run-dsubgraph.out');

writeSMAT(mygfile, A);

status = system(sprintf(...
    '"%s" "%s" "%s"', fullfile(mydir, 'find_densest_subgraph'), ...
    mygfile,myoutfile));

if status == 0
    try
        fid = fopen(myoutfile, 'r');
        algd = textscan(fid, '%f', 1);
        algd = algd{1};
        num = textscan(fid, '%d', 1);
        vals = textscan(fid, '%d', num{1});
        S = vals{1}+1;
        density = full(sum(sum(A(S,S)))/numel(S));
        fclose(fid);
    catch me
        fclose(fid);
        rethrow(me)
    end
end

if nargout < 2
    fprintf('   Output density: %f\n', algd);
    fprintf(' Computed density: %f\n', density);
end
    


function writeSMAT(filename, A, optionsu)
% WRITESMAT writes a matrix in a sparse matrix (SMAT) form.
%
%   WRITESMAT(filename, A, options)
%   filename - the filename to write
%   A - the matrix 
%
%   options.format value format (e.g. %i or %f)
%   options.ut     upper triangular (only print the upper triangular part)
%   options.graph  assume the column size is the same as the row size
%
%   options.blocksize blocksize to write out extremely large matrices.
%

% David Gleich
% Copyright, 2005-2010

options = struct('format', '%f', 'ut', 'no', 'graph', 'no', ...
    'onebased', 'no', 'blocksize', 1000000);

if (nargin > 2)
    options = merge_structs(optionsu, options);
end;



fid = fopen(filename, 'wt');

if (strcmp(options.ut', 'yes'))
    A = triu(A);
    [i, j, v] = find(A);
else
    [i, j, v] = find(A);
end

nz = length(i);
[m n] = size(A);

if m==1,
    i = i';
    j = j';
    v = v';
end

if (strcmp(options.graph, 'yes'))
    fprintf(fid, '%i %i\n', m, nz);
else
    
    fprintf(fid, '%i %i %i\n', m, n, nz);
end;

fmt = sprintf('%s %s %s\\n', '%i', '%i', options.format);

if (~strcmp(options.onebased, 'yes'))
    i = i-1;
    j = j-1;
end;

try
    fprintf(fid, fmt, [i j v]');
catch
    fprintf('... trying block write ...\n');
    blocksize = options.blocksize;
    startblock = 1;
    blocknum = 1;
    nzleft = nz;
    while (nzleft > 0)
        curblock = min(nzleft, blocksize);
        fprintf('block %i (%i - %i)\n', blocknum, startblock, startblock+curblock-1);
        curpart = startblock:(startblock+curblock-1);
        fprintf(fid, fmt, [i(curpart) j(curpart) v(curpart)]');
        
        nzleft = nzleft - curblock;
        startblock = startblock+curblock;
        blocknum = blocknum+1;
    end;
end;




%for ii=1:length(i)
%    fprintf(fid, '%i %i %f\n', i(ii)-1, j(ii)-1, v(ii));
%end
fclose(fid);

function S = merge_structs(A, B)
% MERGE_STRUCTS Merge two structures.
%
% S = merge_structs(A, B) makes the structure S have all the fields from A
% and B.  Conflicts are resolved by using the value in A.
%

%
% merge_structs.m
% David Gleich
%
% Revision 1.00
% 19 Octoboer 2005
%

S = A;

fn = fieldnames(B);

for ii = 1:length(fn)
    if ~isfield(A, fn{ii})
        S.(fn{ii}) = B.(fn{ii});
    end
end
