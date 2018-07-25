#ifndef Lin_Rotate_H
#define Lin_Rotate_H

typedef struct Location
{
	int x;
	int y;
	int pGry;
}loc;


void LinRotate(BYTE *pCircleImg, BYTE *pRotateImg, int Radius, int theta)
{
	BYTE *pC, *pR;
	pC = pCircleImg;
	pR = pRotateImg;
	int d = 2 * Radius + 1;
	loc *OriPoint = new loc[d*d];
	loc *AfterPoint = new loc[d*d];
	int i, j, temp;
	float costheta, sintheta, xback, yback;
	float archtheta = theta*PI / 180.0;
	int upy, downy, leftx, rightx;

	for (i = 0;i < d;i++)
	{
		for (j = 0;j < d;j++)
		{
			OriPoint[i*d + j].y = Radius - i;
			OriPoint[i*d + j].x = j - Radius;
			OriPoint[i*d + j].pGry = *(pC++);
		}
	}

	//Initial RotateImg
	for (i = 0;i < d;i++)
	{
		for (j = 0;j < d;j++)
		{
			AfterPoint[i*d + j].y = Radius - i;
			AfterPoint[i*d + j].x = j - Radius;
			AfterPoint[i*d + j].pGry = 0;
		}
	}

	for (i = 0;i < d;i++)
	{
		for (j = 0;j < d;j++)
		{
			temp = i*d + j;
			costheta = cos(archtheta);
			sintheta = sin(archtheta);
			xback = AfterPoint[temp].x*costheta - AfterPoint[temp].y*sintheta + 0.0001; //+0.0001防止双线性插值回到整数点出错
			yback = AfterPoint[temp].x*sintheta + AfterPoint[temp].y*costheta + 0.0001;
			if (xback <= -Radius || xback >= Radius || yback <= -Radius || yback >= Radius)
			{
				*(pR++) = AfterPoint[temp].pGry;
			}
			else
			{
				leftx = int(floor(xback));
				rightx = int(ceil(xback));
				upy = int(ceil(yback));
				downy = int(floor(yback));
				int ga = OriPoint[(Radius - downy)*d + Radius + leftx].pGry;       //左下角
				int gb = OriPoint[(Radius - downy)*d + Radius + rightx].pGry;      //右下角
				int gc = OriPoint[(Radius - upy)*d + Radius + leftx].pGry;         //左上角
				int gd = OriPoint[(Radius - upy)*d + Radius + rightx].pGry;        //右上角
				float ge = (rightx - xback)*ga + (xback - leftx)*gb;
				float gf = (rightx - xback)*gc + (xback - leftx)*gd;
				AfterPoint[temp].pGry = (int)((upy - yback)*ge + (yback - downy)*gf);
				*(pR++) = AfterPoint[temp].pGry;
			}
		}
	}
}

void LinRotate2(BYTE *pGryImg, int width, int height, BYTE *pResImg, int theta, int xr, int yr) {
	float archtheta = theta*PI / 180.0;
	int SinTheta = (int)(sin(archtheta)*2048), CosTheta = (int)(cos(archtheta)*2048);
	int x, y, px, py;
	int tx, ty, p1, p2, p3, p4, p1_2, p3_4;
	int ConstX = -xr*CosTheta + yr*SinTheta + (xr << 11);
	int ConstY = -yr*CosTheta - xr*SinTheta + (yr << 11);

	BYTE *pRes = pResImg, *pCur, *p;
	for (y = 0;y<height;y++)
	{
		tx = -y*SinTheta - CosTheta + ConstX;
		ty = y*CosTheta - SinTheta + ConstY;
		for (x = 0;x<width;x++)
		{
			tx += CosTheta; //x*CosTheta - y*SinTheta + ConstX; (x-xr)*CosTheta - (y-yr)*SinTheta + xr 
			ty += SinTheta; //y*CosTheta + x*SinTheta + ConstY; (y-yr)*CosTheta + (x-xr)*SinTheta + yr 
			px = tx >> 11;
			py = ty >> 11;
			if (px<0 || px>width - 2 || py<0 || py>height - 2)
			{
				pRes++;
				continue;
			}
			pCur = pGryImg + py*width + px;
			p1 = *pCur;    //此处求出周围点的值
			p2 = *(pCur + 1);
			p3 = *(pCur + width);
			p4 = *(pCur + width + 1);
			p1_2 = ((p1 << 11) + (tx - (px << 11))*(p2 - p1)) >> 11;
			p3_4 = ((p3 << 11) + (tx - (px << 11))*(p4 - p3)) >> 11;
			*pRes = p1_2 + (((ty - (py << 11))*(p3_4 - p1_2)) >> 11);
			pRes++;
		}
	}
	return;
}

#endif