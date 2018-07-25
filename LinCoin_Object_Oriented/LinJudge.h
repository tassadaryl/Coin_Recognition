#ifndef Lin_Judge_H
#define Lin_Judge_H

//获取180维特征
void LinGetCharacter(BYTE *pMedImg, int *Character, int Radius)
{
	BYTE *pMed;
	int *Cha;
	int down = int(Radius * 0.7);
	int up = int(Radius * 0.85);
	int i, j, h, w, cnt, theta, rsintheta, rcostheta, d = 2 * Radius + 1;
	float archtheta;

	for (i = down; i <= up; i++)
	{
		Cha = Character;
		for (cnt = 0;cnt < 180;cnt++)
		{
			theta = cnt * 2;
			archtheta = theta * PI / 180.0;
			rsintheta = (int)(i*sin(archtheta));
			rcostheta = (int)(i*cos(archtheta));
			h = Radius - rsintheta;
			w = Radius - rcostheta;
			pMed = pMedImg + h*d + w;
			*(Cha++) += *pMed;
		}
	}
	return;
}

//相关系数法
float ppcc(int *Character1, int *Character2)
{
	int *Ch1, *Ch2;
	Ch1 = Character1;
	Ch2 = Character2;
	double sum1 = 0, sum2 = 0, avg1 = 0, avg2 = 0, i, numerator = 0, denominator = 0, den1 = 0, den2 = 0;
	float r = 0;
	for (i = 0;i < 180;i++)
	{
		sum1 += *(Ch1++);
		sum2 += *(Ch2++);
	}
	avg1 = sum1 / 180;
	avg2 = sum2 / 180;

	Ch1 = Character1;
	Ch2 = Character2;
	for (i = 0;i < 180;i++)
	{
		numerator += (*Ch1 - avg1)*(*Ch2 - avg2);
		den1 += (*Ch1 - avg1)*(*Ch1 - avg1);
		den2 += (*Ch2 - avg2)*(*Ch2 - avg2);
		Ch1++;
		Ch2++;
	}
	denominator = sqrt(den1*den2);
	r = (float)numerator / (float)denominator;

	return r;
}

//比较大小，确定正反面和旋转度数
void judge(int *Character, int *code, int *ro, int numflag)
{
	int *Ch;
	int i, j, num, temp, next, maxnum = 0, rotate = 0;
	float  MAX = 0, r[2] = { 0 };
	switch (numflag)
	{
		//1圆
	case 0:                          
		for (i = 0;i < 180;i++)
		{
			r[0] = ppcc(Character, yiyuanzheng);
			r[1] = ppcc(Character, yiyuanfan);
			for (num = 0;num < 2;num++)
			{
				if (r[num] > MAX)
				{
					MAX = r[num];
					maxnum = num;
					rotate = -i * 2;
				}
			}

			//数组全部左移一位
			Ch = Character;
			temp = *Ch;
			for (j = 0;j < 179;j++)
			{
				next = *(Ch + 1);
				*Ch = next;
				Ch++;
			}
			*Ch = temp;
		}
		break;
		//1毛
	case 1:
		for (i = 0;i < 180;i++)
		{
			r[0] = ppcc(Character, yimaozheng);
			r[1] = ppcc(Character, yimaofan);
			for (num = 0;num < 2;num++)
			{
				if (r[num] > MAX)
				{
					MAX = r[num];
					maxnum = num;
					rotate = -i * 2;
				}
			}

			//数组全部左移一位
			Ch = Character;
			temp = *Ch;
			for (j = 0;j < 179;j++)
			{
				next = *(Ch + 1);
				*Ch = next;
				Ch++;
			}
			*Ch = temp;
		}
		break;
		//5毛
	case 2:
		for (i = 0;i < 180;i++)
		{
			r[0] = ppcc(Character, wumaozheng);
			r[1] = ppcc(Character, wumaofan);
			for (num = 0;num < 2;num++)
			{
				if (r[num] > MAX)
				{
					MAX = r[num];
					maxnum = num;
					rotate = -i * 2;
				}
			}
			//printf_s("%f %f %f\n", r[0], r[1], MAX);
			//数组全部左移一位
			Ch = Character;
			temp = *Ch;
			for (j = 0;j < 179;j++)
			{
				next = *(Ch + 1);
				*Ch = next;
				Ch++;
			}
			*Ch = temp;
		}
		break;
	}


	*code = maxnum;
	*ro = rotate;
}
#endif