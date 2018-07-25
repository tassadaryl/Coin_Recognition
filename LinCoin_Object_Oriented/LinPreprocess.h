#ifndef Lin_Preprocess_H
#define Lin_Preprocess_H

void LinSample(BYTE *pGryImg, BYTE *pNewImg, int width, int height, int ratio)  //采样，宽和高分别缩小到1/ratio
{
	BYTE *pGry, *pNew;
	pGry = pGryImg;
	pNew = pNewImg;
	int i, j;
	for (i = 0; i < height; i += ratio)
	{
		for (j = 0;j < width; j += ratio)
		{
			*(pNew++) = *pGry;
			pGry += ratio;
		}
		pGry += (ratio - 1)*width;
	}

	return;
}

void LinAvgFilterBySumcol(BYTE *pGryImg, BYTE *pResImg, int width, int height, int nx, int ny)
{
	BYTE *pRes;
	int size = (nx * 2 + 1)*(ny * 2 + 1);
	int x, y, i, j, t, sum = 0;
	//int Sumcol[1024];
	int *Sumcol;
	Sumcol = (int*)malloc(sizeof(int)*width);
	memset(Sumcol, 0, sizeof(int)*width);

	//初始化Sumcol
	for (i = 0;i<width;i++)
	{
		for (j = 0;j <= ny * 2;j++)
		{
			Sumcol[i] += *(pGryImg + j*width + i);
		}
	}


	for (y = ny, pRes = pResImg + y*width;y < height - ny;y++)
	{
		pRes += nx;  //跳过左边界

		for (x = nx;x < width - nx;x++)
		{
			sum = 0;
			for (t = x - nx;t <= x + nx;t++)
			{
				sum += Sumcol[t];
			}
			*(pRes++) = sum / size;
		}

		pRes += nx;  //跳过右边界

					 //换行更新sumcol
		for (i = 0;i < width;i++)
		{
			Sumcol[i] -= *(pGryImg + (y - ny)*width + i);
			Sumcol[i] += *(pGryImg + (y + ny + 1)*width + i);
		}

	}
	free(Sumcol);
}

void LinMinus(BYTE *pGryImg, BYTE *pAvgImg, BYTE *pMinusImg, int width, int height)
{
	BYTE *pG, *pA, *pM;
	pG = pGryImg;
	pA = pAvgImg;
	pM = pMinusImg;
	int i, temp, size = width*height;
	for (i = 0;i < size;i++)
	{
		temp = *(pG++) - *(pA++);
		*(pM++) = max(temp, 0);
	}

	return;
}

void LinGetBitImg(BYTE *pGryImg, BYTE *pBitImg, int width, int height, int threshold)
{
	BYTE *pCur, *pBit;
	pCur = pGryImg;
	pBit = pBitImg;
	int i, imgsize = width*height;
	for (i = 0;i < imgsize;i++)
	{
		if (*pCur > threshold)
		{
			*(pBit++) = 0;
			pCur++;
		}
		else
		{
			*(pBit++) = 255;
			pCur++;
		}
	}

	return;
}

void LinInitial(BYTE *pEdgeImg, int width, int height)
{
	BYTE *pEdge;
	pEdge = pEdgeImg;
	int i, imgsize = width*height;
	for (i = 0;i < imgsize;i++)
	{
		*(pEdge++) = 255;
	}

	return;
}

void LinInitial2(BYTE *pEdgeImg, int width, int height)
{
	BYTE *pEdge;
	pEdge = pEdgeImg;
	int i, imgsize = width*height;
	for (i = 0;i < imgsize;i++)
	{
		*(pEdge++) = 0;
	}

	return;
}
void LinGetEdge(BYTE *pGryImg, BYTE *pEdgeImg, int width, int height)
{
	BYTE *pGry, *pEdge, *pG, *pE;
	int i, j;

	//左边界
	pGry = pGryImg;
	pG = pGryImg;
	pEdge = pEdgeImg;
	pE = pEdgeImg;

	for (i = 0;i < height;i++)
	{
		for (j = 0;j < width;j++)
		{
			if (*pGry == 0)
			{
				*pEdge = 0;
				break;
			}
			else
			{
				pGry++;
				pEdge++;
			}
		}
		pG += width;
		pE += width;
		pGry = pG;
		pEdge = pE;
	}

	//右边界
	pGry = pGryImg + width - 1;
	pG = pGryImg + width - 1;
	pEdge = pEdgeImg + width - 1;
	pE = pEdgeImg + width - 1;
	for (i = 0;i < height;i++)
	{
		for (j = 0;j < width;j++)
		{
			if (*pGry == 0)
			{
				*pEdge = 0;
				break;
			}
			else
			{
				pGry--;
				pEdge--;
			}
		}
		pG += width;
		pE += width;
		pGry = pG;
		pEdge = pE;
	}

	//上边界
	pGry = pGryImg;
	pG = pGryImg;
	pEdge = pEdgeImg;
	pE = pEdgeImg;
	for (j = 0; j < width; j++)
	{
		for (i = 0; i < height;i++)
		{
			if (*pGry == 0)
			{
				*pEdge = 0;
				break;
			}
			else
			{
				pGry += width;
				pEdge += width;
			}
		}
		pG++;
		pE++;
		pGry = pG;
		pEdge = pE;
	}

	//下边界
	pGry = pGryImg + (height - 1)*width;
	pG = pGryImg + (height - 1)*width;
	pEdge = pEdgeImg + (height - 1)*width;
	pE = pEdgeImg + (height - 1)*width;
	for (j = 0; j < width; j++)
	{
		for (i = 0; i < height;i++)
		{
			if (*pGry == 0)
			{
				*pEdge = 0;
				break;
			}
			else
			{
				pGry -= width;
				pEdge -= width;
			}
		}
		pG++;
		pE++;
		pGry = pG;
		pEdge = pE;
	}

	return;
}

void LinMedFilter(BYTE *pGryImg, BYTE *pResImg, int width, int height, int nx, int ny)
{
	BYTE *pRes, *pLeft, *pRight;
	int size = (nx * 2 + 1)*(ny * 2 + 1);
	int x, y, i, j, med, num;
	int histogram[256];
	for (y = ny, pRes = pResImg + y*width;y < height - ny;y++)
	{
		pRes += nx;
		//初始化直方图
		x = nx;
		memset(histogram, 0, sizeof(int) * 256);
		for (i = y - ny;i <= y + ny;i++)
		{
			for (j = x - nx;j <= x + nx;j++)
			{
				histogram[*(pGryImg + i*width + j)]++;
			}
		}
		//初始化中值
		for (num = 0, med = 0;med < 256;med++)
		{
			num += histogram[med];
			if (num * 2 >= size) break;
		}
		//中值赋值
		*(pRes++) = med;
		//对于该行的每个像素进行中值滤波
		for (x = nx + 1;x < width - nx;x++)
		{
			//更新直方图
			for (i = y - ny,
				pLeft = pGryImg + i*width + (x - nx - 1),
				pRight = pGryImg + i*width + (x + nx);
				i <= y + ny;
				i++, pLeft += width, pRight += width)
			{
				if (*pLeft <= med)num--;
				histogram[*pLeft]--;

				if (*pRight <= med)num++;
				histogram[*pRight]++;
			}

			//更新中值
			if (num * 2 > size)
			{
				for (;med >= 0;med--)
				{
					num -= histogram[med];
					if (num * 2 < size)break;
				}
				num += histogram[med];
			}
			else if (num * 2 < size)
			{
				for (med = med + 1;med < 256;med++)
				{
					num += histogram[med];
					if (num * 2 >= size)break;
				}
			}
			//中值赋值
			*(pRes++) = med;
		}
		pRes += nx;
	}
}

#endif