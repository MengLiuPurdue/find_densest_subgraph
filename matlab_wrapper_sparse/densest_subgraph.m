function densest_subgraph(A, outfile)
if ~issymmetric(A)
    error('input is not symmetric\n');
end
[ei,ej,w] = find(A);
n = max(ei);
[m,d2] = size(ei);
ei = ei - 1;
ej = ej - 1;
for i = 1:m
    if ei(i)<0 || ej(i)<0 || ei(i)>=n || ej(i)>=n
        error('invalid input in line %d\n', i);
    end
    if w(i)<0
        error('negative weight in line %d\n', i);
    end
end
output = zeros(n,1);
outputlen = 0;
outputlenPtr = libpointer('int64Ptr',outputlen);
eiPtr = libpointer('int64Ptr',ei);
ejPtr = libpointer('int64Ptr',ej);
wPtr = libpointer('doublePtr',w);
outputPtr = libpointer('int64Ptr',output);
loadlibrary('libdensest_subgraph','def.h')
density = calllib('libdensest_subgraph','densest_subgraph',n,m,eiPtr,ejPtr,wPtr,outputPtr,outputlenPtr);
output = get(outputPtr, 'Value');
outputlen = get(outputlenPtr, 'Value');
unloadlibrary libdensest_subgraph;

%%
% save the result to the outfile
wptr = fopen(outfile, 'w');
fprintf(wptr, '%f\n', density);
fprintf(wptr, '%d\n', outputlen);
for i = 1:outputlen
    fprintf(wptr, '%d\n', output(i));
end
fclose(wptr);
