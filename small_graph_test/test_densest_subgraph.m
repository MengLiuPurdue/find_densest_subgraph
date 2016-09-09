mydir = fileparts(mfilename('fullpath'));
mygfile = fullfile(mydir,'run_dsubgraph.smat');
myoutfile = fullfile(mydir,'run-dsubgraph.out');

for i=1:1253
    system('python writeSMAT.py i');
    status = system(sprintf(...
    '"%s" "%s" "%s"', fullfile(mydir, 'find_densest_subgraph'), ...
    mygfile,myoutfile));
    if status == 0
        try
            fid = fopen(myoutfile, 'r');
            algd = textscan(fid, '%f', 1);
            algd = algd{1};
        catch me
            fclose(fid);
            rethrow(me)
        end
    end
    A = readSMAT(mygfile);
    density = max_density(A);
    if abs(algd - density) > 10^-5
        fprintf('error!\n');
        break;
    end
    i
end
