image=imread('img008.bmp');

windowLocation = [35,50];
windowSize=[32,32];
[row,col] = size(image); % ����ͼ��ߴ�
x = windowLocation(1);%���ο�λ�����꣬���ʽΪ[x,y]
y = windowLocation(2);
height = windowSize(1);%���ο�ߴ磬���ʽΪ[height,width]����[�߶�,���]
width = windowSize(2);
if((x<=row && y<=col)&&(height<=row && width<=col))
    disp('���ο�Ϸ���');
    figure;imshow(image);
    hold on
    drawRectangleImage = rectangle('Position',[y,x,width,height],'LineWidth',4,'EdgeColor','r');
    hold off
else
    disp('���ο򲻺Ϸ���');
end
