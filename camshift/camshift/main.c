#include  <stdio.h>
#include <stdint.h>
#include  "camshift.h"
//https://github.com/FLYYZJ/CV_YZJ/blob/master/Camshift_with_demo/Feature_caculation.py
int read2array_temp(int size_h, int size_w, int *r, int *g, int *b)
{
	//下面是读数据，
	//模板截图的是48*43 模板在第一张图中的位置由0索引为，（25，38）
	FILE *fpRead = fopen(".\\rgb\\r.txt", "r");
	if (fpRead == NULL)
	{
		return 0;
	}
	for (int i = 0; i<size_h*size_w; i++)
	{
		fscanf(fpRead, "%d", &r[i]);
	}
	

	FILE *fpRead2 = fopen(".\\rgb\\g.txt", "r");
	if (fpRead2 == NULL)
	{
		return 0;
	}
	for (int i = 0; i<size_h*size_w; i++)
	{
		fscanf(fpRead2, "%d", &g[i]);
	}

	
	FILE *fpRead3 = fopen(".\\rgb\\b.txt", "r");
	if (fpRead3 == NULL)
	{
		return 0;
	}
	for (int i = 0; i<size_h*size_w; i++)
	{
		fscanf(fpRead3, "%d", &b[i]);
	}

}

int read2array(int size_h, int size_w, int *r, int *g, int *b)
{
	//下面是读数据，
	//一帧是96*128
	FILE *fpRead = fopen(".\\rgb\\r1.txt", "r");
	if (fpRead == NULL)
	{
		return 0;
	}
	for (int i = 0; i<size_h*size_w; i++)
	{
		fscanf(fpRead, "%d", &r[i]);
	}


	FILE *fpRead2 = fopen(".\\rgb\\g1.txt", "r");
	if (fpRead2 == NULL)
	{
		return 0;
	}
	for (int i = 0; i<size_h*size_w; i++)
	{
		fscanf(fpRead2, "%d", &g[i]);
	}


	FILE *fpRead3 = fopen(".\\rgb\\b1.txt", "r");
	if (fpRead3 == NULL)
	{
		return 0;
	}
	for (int i = 0; i<size_h*size_w; i++)
	{
		fscanf(fpRead3, "%d", &b[i]);
	}

}


int main()
{


	int size_h = 48, size_w = 43;
	int r[48][43], g[48][43], b[48][43];
	int	h[48][43], v[48][43];
	float s[48][43];
	int histTmp[6];
	read2array_temp(size_h, size_w, &r[0][0], &g[0][0], &b[0][0]);

	//获取到模板的h分量,获取模板直方图
	rgb2hsv_all(&r[0][0], &g[0][0], &b[0][0], *&h, *&s, *&v, size_h, size_w);
	hist(&h[0][0], 6, &histTmp[0], size_h, size_w);
	
	//获取一帧图像数据
	int r1[96][128], g1[96][128], b1[96][128];
	int	h1[96][128], v1[96][128];
	float s1[96][128];
	int newIma[96][128], newImaPost[96][128];
	int newx=25,newy=38,nsize_xt=48,nsize_yt=43;

	read2array(96, 128, &r1[0][0], &g1[0][0], &b1[0][0]);
	rgb2hsv_all(&r1[0][0], &g1[0][0], &b1[0][0], *&h1, *&s1, *&v1, 96, 128);
	Backprojection(&histTmp[0], &h1[0][0], &newIma[0][0], 6, 96, 128);//反向投影

	threshold(&newIma[0][0], &newIma[0][0], 180, 255, 96, 128);//阈值
	MorphOpen(&newIma[0][0], &newImaPost[0][0], 128, 96, 2, 2);//开运算
	Meanshift(&newImaPost[0][0], &newx, &newy, &nsize_xt, &nsize_yt, 96, 128, 2, 10);

	return 1;



}