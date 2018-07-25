#ifndef Lin_HoughTransform_H
#define Lin_HoughTransform_H

void LinHoughCircle(BYTE *pEdgeImg, int width, int height, int *A, int *B, int *Radius, int *Err)
{

	BYTE *L, *R, *U, *D, *left, *right, *up, *down;
	int *a = new int[width];
	int *b = new int[height];
	memset(a, 0, sizeof(int)*width);
	memset(b, 0, sizeof(int)*height);
	int i, j, loca = 0, locb = 0, radius = 0, MAX = -1;


	//ÕÒÔ²ÐÄºá×ø±ê
	L = pEdgeImg;
	R = pEdgeImg + width - 1;
	left = L, right = R;

	int templeft, tempright, avg;
	for (i = 0;i < height;i++)
	{
		templeft = 0; tempright = width - 1;
		for (j = 0; j < width; j++)
		{
			if (*left == 0)
			{
				templeft = j;
				break;
			}
			left++;
		}
		for (j = width - 1; j > 0; j--)
		{
			if (*right == 0)
			{
				tempright = j;
				break;
			}
			right--;
		}
		L += width;
		R += width;
		left = L;
		right = R;
		if (templeft == 0 || tempright == width - 1)continue;
		else
		{
			avg = (templeft + tempright) / 2;
			a[avg]++;
		}
	}

	MAX = -1;
	for (i = 0;i < width;i++)
	{
		if (a[i] > MAX)
		{
			MAX = a[i];
			loca = i;
		}
	}
	//printf_s("%d\n", loca);

	//ÕÒÔ²ÐÄ×Ý×ø±ê
	U = pEdgeImg;
	D = pEdgeImg + (height - 1)*width;
	up = U; down = D;
	int tempup, tempdown;
	for (i = 0; i < width; i++)
	{
		tempup = 0; tempdown = height - 1;
		for (j = 0; j < height;j++)
		{
			if (*up == 0)
			{
				tempup = j;
				break;
			}
			up += width;
		}
		for (j = height - 1; j > 0; j--)
		{
			if (*down == 0)
			{
				tempdown = j;
				break;
			}
			down -= width;
		}
		U++;
		D++;
		up = U;
		down = D;
		if (tempup == 0 || tempdown == height - 1)continue;
		else
		{
			avg = (tempup + tempdown) / 2;
			b[avg]++;
		}

	}
	MAX = -1;
	for (i = 0;i < height;i++)
	{
		if (b[i] > MAX)
		{
			MAX = b[i];
			locb = i;
		}
	}
	//printf_s("%d\n", locb);

	//ÕÒ°ë¾¶
	BYTE *pEdge;
	pEdge = pEdgeImg;
	int MaxSqrRadius = (width)*(width)+(height)*(height);
	int *r = new int[MaxSqrRadius];
	memset(r, 0, sizeof(int)*MaxSqrRadius);
	int tempsqr, err = 100, k;

	for (i = 0;i < height;i++)
	{
		for (j = 0;j < width;j++)
		{
			if (*pEdge == 0)
			{
				tempsqr = (j - loca)*(j - loca) + (i - locb)*(i - locb);
				for (k = tempsqr - err;k <= tempsqr + err;k++)
				{
					k = max(0, k);
					r[k]++;
				}
			}
			pEdge++;
		}
	}

	MAX = -1;
	for (i = 0;i < MaxSqrRadius;i++)
	{
		if (r[i] > MAX)
		{
			MAX = r[i];
			radius = i;
		}
	}
	radius = sqrt(radius);


	*A = loca;
	*B = locb;
	*Radius = radius;
	*Err = err;

	delete a;
	delete b;
	delete r;
	return;
}
void LinGetCircle(BYTE *pGryImg, BYTE *pCircleImg, int A, int B, int Radius, int width, int height)
{
	BYTE *pGry, *pG, *pC;
	pG = pGry = pGryImg + (B - Radius)*width + A - Radius;
	pC = pCircleImg;
	int i, j, D = 2 * Radius;
	for (i = 0;i <= D;i++)
	{
		for (j = 0;j <= D;j++)
		{
			*(pC++) = *(pGry++);
		}
		pG += width;
		pGry = pG;
	}

	return;
}

#endif