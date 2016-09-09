A=zeros(6);
for i=1:32767
    count=1;
    temp=bitget(i,15:-1:1);
    for j=1:6
        for k=j+1:6
            A(j,k)=temp(count);
            count=count+1;
        end
    end
    A=triu(A,1)+triu(A,1)';
    [S,density,algd] = run_densest_subgraph(A);
    [cal_density,cut]=max_density(A);
    if abs(algd-cal_density)>10^-5
        fprintf('error!\n');
        break;
    end
    i
end