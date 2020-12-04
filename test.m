image=imread('img008.bmp');

windowLocation = [35,50];
windowSize=[32,32];
[row,col] = size(image); % 输入图像尺寸
x = windowLocation(1);%矩形框位置坐标，其格式为[x,y]
y = windowLocation(2);
height = windowSize(1);%矩形框尺寸，其格式为[height,width]，即[高度,宽度]
width = windowSize(2);
if((x<=row && y<=col)&&(height<=row && width<=col))
    disp('矩形框合法！');
    figure;imshow(image);
    hold on
    drawRectangleImage = rectangle('Position',[y,x,width,height],'LineWidth',4,'EdgeColor','r');
    hold off
else
    disp('矩形框不合法！');
end
