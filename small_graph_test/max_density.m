function [density,cut]=max_density(A)
density=0;
[n,n]=size(A);
for i=0:(2^n-1)
   temp=0;
   a=bitget(i,n:-1:1);
   for j=1:n
       for k=1:n
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
for i=1:n
    if max_cut(i)==1
        cut(j,1)=i;
        j=j+1;
    end
end