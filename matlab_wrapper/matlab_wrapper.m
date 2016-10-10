function matlab_wrapper(infile, outfile)

%mex -v COMPFLAGS='$COMPFLAGS -std=c++11' input_test.cpp
mex densest_subgraph.cpp maxflow.cpp

if ~exist(infile,'file') 
    error('readSMAT:fileNotFound', 'Unable to read file %s', infile); 
end
s=load(infile,'-ascii'); 
n = s(1,1);
m = s(1,3);
ei = zeros(m,1);
ej = zeros(m,1);
w = zeros(m,1);
for i = 2:m+1
    ei(i-1) = s(i,1);
    ej(i-1) = s(i,2);
    w(i-1) = s(i,3);
    if ei(i-1) >= n || ej(i-1) >= n
        error('invalid input in line %d\n', i);
    end
    if w(i-1) < 0
        error('negative input in line %d\n', i);
    end
end
% test_result = input_test(n,m,ei,ej,w);
% if(test_result)
%     error('Failling in input test\n');
% end
[density, outputlen, output] = densest_subgraph(n,m,ei,ej,w);
wptr = fopen(outfile, 'w');
fprintf(wptr, '%f\n', density);
fprintf(wptr, '%d\n', outputlen);
for i = 1:outputlen
    fprintf(wptr, '%d\n', output(i));
end
fclose(wptr);

