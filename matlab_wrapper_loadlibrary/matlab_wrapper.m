function matlab_wrapper(infile, outfile)

if ~exist(infile,'file') 
    error('readSMAT:fileNotFound', 'Unable to read file %s', infile); 
end
s=load(infile,'-ascii'); 
n = s(1,1);
m = s(1,3);
ei = zeros(m,1);
ej = zeros(m,1);
w = zeros(m,1);
outputlen = 0;
output = zeros(n,1);
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
outputlenPtr = libpointer('int64Ptr',outputlen);
eiPtr = libpointer('int64Ptr',ei);
ejPtr = libpointer('int64Ptr',ej);
wPtr = libpointer('doublePtr',w);
outputPtr = libpointer('int64Ptr',output);
% loadlibrary('libinput_test','input_test.h')
% unloadlibrary libinput_test;
loadlibrary('libdensest_subgraph','def.h')
density = calllib('libdensest_subgraph','densest_subgraph',n,m,eiPtr,ejPtr,wPtr,outputPtr,outputlenPtr);
output = get(outputPtr, 'Value');
outputlen = get(outputlenPtr, 'Value');
wptr = fopen(outfile, 'w');
fprintf(wptr, '%f\n', density);
fprintf(wptr, '%d\n', outputlen);
for i = 1:outputlen
    fprintf(wptr, '%d\n', output(i));
end
fclose(wptr);
unloadlibrary libdensest_subgraph;
