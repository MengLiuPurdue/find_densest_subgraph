%find the subgraph with the maximum density in an undirected graph through brutal search
%the undirected graph is represented by an adjcent matrix A
%density is the maximum density and cut is the corresponding subgraph
function [density,cut]=max_density(A)
density=0;
for i=0:63
   temp=0;
   a=bitget(i,6:-1:1);
   for j=1:6
       for k=1:6
           if a(j)*a(k)*A(j,k)~=0
               temp=temp+A(j,k);
           end
       end
   end
   count=sum(a);
   temp=temp/count;
   if temp>density
       max_cut=a;
       density=temp;
   end
end
j=1;
for i=1:6
    if max_cut(i)==1
        cut(j,1)=i;
        j=j+1;
    end
end
