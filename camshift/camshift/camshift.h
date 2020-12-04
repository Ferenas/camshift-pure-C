#include  <stdio.h>
#include <stdint.h>
#include <math.h>
int findmax(int *a,int size)
{

	int max=0;
	for (int i = 0; i<size; i++)
	{
		if (a[i]>max)
		{
			max = a[i];
	    }
	}
	return max;

}
static int rgb2hsv_max(int a, int b, int c)
{
	int max = a;
	if (b > max) max = b;
	if (c > max) max = c;
	return max;
}

static int rgb2hsv_min(int a, int b, int c)
{
	int min = a;
	if (b < min) min = b;
	if (c < min) min = c;
	return min;
}


static int rgb2hsv(int r, int g, int b, int *h, float *s, int *v)
{
	int imax, imin;
	float diff;
	imax = rgb2hsv_max(r, g, b);
	imin = rgb2hsv_min(r, g, b);
	diff = imax - imin;
	
	*v = imax;
	if (imax == 0)
		*s = 0;
	else
		*s = (diff/imax);

	if (diff != 0)
	{
		if (r == imax)
		{
			*h = 60 * (g - b) / diff;
		}
		else if (g == imax)
		{
			*h = 60 * (b - r) / diff + 120;
		}
		else
		{
			*h = 60 * (r - g) / diff + 240;
		}

		if (*h < 0)
			*h = *h + 360;
	}
	else
		*h = 0;

	return 0;
}

void rgb2hsv_all(int *r, int *g, int *b, int *h, int *s, int *v,int size_h,int size_w)
{
	for (int i = 0; i<size_h*size_w; i++)
	{
		rgb2hsv(r[i], g[i], b[i], &h[i], &s[i], &v[i]);
	}

}


void hist(int *h, int bins, int *hist,int size_h,int size_w)//bins默认是6格
{
	int k = 360 / bins;
	for (int i = 0; i<size_h*size_w; i++)
	{
		if (h[i] <= k)
		{
			hist[0] += 1;
	    }
		else if (h[i] <= 2*k)
		{
			hist[1] += 1;
		}
		else if (h[i] <= 3 * k)
		{
			hist[2] += 1;
		}
		else if (h[i] <= 4 * k)
		{
			hist[3] += 1;
		}
		else if (h[i] <= 5 * k)
		{
			hist[4] += 1;
		}
		else if (h[i] <= 6 * k)
		{
			hist[5] += 1;
		}
		
	}


}


void Backprojection(int *hist, int *h, int *newimg, int bins,int size_h, int size_w)
{
	int max = findmax(hist, 6);
	float scale = 255.0 / max;
	
	int k = 360 / bins;
	for (int i = 0; i<size_h*size_w; i++)
	{
		if (h[i] <= k)
		{
			newimg[i] = hist[0]*scale;
		}
		else if (h[i] <= 2 * k)
		{
			newimg[i] = hist[1] * scale;
		}
		else if (h[i] <= 3 * k)
		{
			newimg[i] = hist[2] * scale;
		}
		else if (h[i] <= 4 * k)
		{
			newimg[i] = hist[3] * scale;
		}
		else if (h[i] <= 5 * k)
		{
			newimg[i] = hist[4] * scale;
		}
		else if (h[i] <= 6 * k)
		{
			newimg[i] = hist[5] * scale;
		}

	}


}

void threshold(int *h, int *newimage, int thre, int max,int size_h, int size_w) //默认二阈值
{
	if (thre > max)
	{
		return 1;
	}
	for (int i = 0; i<size_h*size_w; i++)
	{
		if (h[i]>thre)
		{
			newimage[i] = max;
		}
		else
		{
			newimage[i] = 0;
		}
	}

}

void MorphErosion(int* src, int* dst, int width, int height, int strutWidth, int structHeight)
{
	if (width - strutWidth < 0 && height - structHeight < 0)return;
	if (strutWidth != structHeight)return;

	int mid = (strutWidth + 1) / 2 - 1;
	int val = 255;
	for (int i = mid; i < height - mid; i++)
	{
		for (int j = mid; j < width - mid; j++)
		{
			for (int m = 0; m < strutWidth; m++)
			{
				for (int n = 0; n < structHeight; n++)
				{
					if (m == mid && n == mid)continue;
					val &= src[(i + m) * width + j + n];
				}
			}

			dst[i * width + j] = val;
			val = 255;
		}
	}
}
//膨胀
void MorphDilition(int* src, int* dst, int width, int height, int strutWidth, int structHeight)
{
	if (width - strutWidth < 0 && height - structHeight < 0)return;
	if (strutWidth != structHeight)return;

	int mid = (strutWidth + 1) / 2 - 1;
	int val = 0;
	for (int i = mid; i < height - mid; i++)
	{
		for (int j = mid; j < width - mid; j++)
		{
			for (int m = 0; m < strutWidth; m++)
			{
				for (int n = 0; n < structHeight; n++)
				{
					if (m == mid && n == mid)continue;
					val |= src[(i + m) * width + j + n];
				}
			}

			dst[i * width + j] = val;
			val = 0;
		}
	}
}
//先腐蚀后膨胀
void MorphOpen(int* src, int* tmp, int width, int height, int strutWidth, int structHeight)
{
	MorphErosion(src, tmp, width, height, strutWidth, structHeight);
	MorphDilition(tmp, src, width, height, strutWidth, structHeight);
}
//二阶矩计算一帧图像中的目标质心,输入的是模板大小的h和w
void CentralPoint(int *newIma, int x, int y, int size_ht, int size_wt,int size_h,int size_w,int *newx,int *newy,int *S)
{
	int M00 = 0, M01 = 0, M10 = 0;
	for (int i = x; i < size_ht+x; i++)
	{
		for (int j = y; j < size_wt+y; j++)
		{
			M00 = M00 + newIma[i*size_w + j];//注意这里指针访问的是那一帧图像，因此取图像的width	
			M01 = M01 + j*newIma[i*size_w + j];
			M10 = M10 + i*newIma[i*size_w + j];
		}

	}
	
	*newx = M10 / M00;
	*newy = M01 / M00;
	*S = 2 * sqrt(M00);

}

void Meanshift(int *newImaPost, int *x, int *y, int *size_ht, int *size_wt, int size_h,int size_w,int Distance, int iter)
{
	int xc = 0, yc = 0, S = 0;
	int xc_ = 0, yc_ = 0,ht_ = 0, wt_ = 0;
	int i = 0;
	xc_ = (*x + *size_ht) / 2;
	yc_ = (*y + *size_wt) / 2;
	ht_ = *size_ht;
	wt_ = *size_wt;
	int xx = 0, yy = 0;
	for (int j =0; j < iter; j++)
	{
		int i = 0;
		while (1)
		{
			i++;
			CentralPoint(newImaPost, *x, *y, *size_ht, *size_wt, size_h, size_w, &xc, &yc, &S);
			if ((xc>0) & (yc > 0)) 
				break;
			else
			{
				*size_ht = *size_ht + 5;
				*size_wt = *size_wt + 5;
			}
			if (i > 5)
			{
				xc = xc_;
				yc = yc_;
				*size_ht = ht_;
				*size_wt = wt_;
			}
		}
		
		xx = *x + *size_ht / 2 - xc;
		yy = *y + *size_wt / 2 - yc;
		if (sqrt(xx*xx + yy*yy) < Distance)break;
		*x = xc-sqrt(S)/2;
		*y = yc - sqrt(S) / 2;
		*size_wt = sqrt(S);
		*size_ht = sqrt(S);

	}

}
