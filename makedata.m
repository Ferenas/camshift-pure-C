I=imread('tmp.bmp');
I_r=I(:,:,1);
I_g=I(:,:,2);
I_b=I(:,:,3);
fid=fopen('r.txt','wt');%写入文件路径
[m,n]=size(I_r);
 for i=1:1:m
    for j=1:1:n
       if j==n
         fprintf(fid,'%g\n',I_r(i,j));
      else
        fprintf(fid,'%g\t',I_r(i,j));
       end
    end
end
fclose(fid);
fid=fopen('g.txt','wt');%写入文件路径
[m,n]=size(I_g);
 for i=1:1:m
    for j=1:1:n
       if j==n
         fprintf(fid,'%g\n',I_g(i,j));
      else
        fprintf(fid,'%g\t',I_g(i,j));
       end
    end
end
fclose(fid);
fid=fopen('b.txt','wt');%写入文件路径
[m,n]=size(I_g);
 for i=1:1:m
    for j=1:1:n
       if j==n
         fprintf(fid,'%g\n',I_b(i,j));
      else
        fprintf(fid,'%g\t',I_b(i,j));
       end
    end
end
fclose(fid);