#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <algorithm>
const double PI = 3.1415927;
using namespace std;
#include "bmpFile.h"
#include "LinPreprocess.h"
#include "LinHoughTransform.h"
#include "LinCoinCharacter.h"
#include "LinRotate.h"
#include "RmwSobel.h"
#include "LinJudge.h"
#include "LinImgProcFrame.h"


void main()
{
	clock_t t1, t2;
	t1 = clock();
	int r = 0, code=0, ro =0;
	int numflag = 0;
	char InputPath[100] = { 0 }, OutputPath[100] = { 0 };


	for (int num = 1;num < 50;num++)
	{
		sprintf_s(InputPath, "..\\LinCoin_Object_Oriented\\src\\%d.bmp", num);
		LinImgProcFrame Img;
		//////////////////////////////////////////
		// 初始化。读图，初始化各参数
		//////////////////////////////////////////
		Img.Init(InputPath);

		//////////////////////////////////////////
		// 设置采样倍数       
		//////////////////////////////////////////
		Img.SetRatio(2);

		//////////////////////////////////////////
		// 为处理过程中的图申请内存 
		//////////////////////////////////////////
		Img.RequestMemory();

		//////////////////////////////////////////
		// 采样            
		//////////////////////////////////////////
		Img.Sample();

		//////////////////////////////////////////
		// 对采样后的图做sobel算子
		//////////////////////////////////////////
		Img.SampleSobel();

		//////////////////////////////////////////
		// 对sobel后的图二值化    
		//////////////////////////////////////////
		Img.SampleBin();

		//////////////////////////////////////////
		// 从左往右，从右往左，从上往下,从下往上
		// 扫描，获取硬币的边缘
		//////////////////////////////////////////
		Img.SampleInit();
		Img.SampleGetEdge();

		//////////////////////////////////////////
		// 通过圆的Hough变换得到圆心位置和半径
		// 得到的参数全部乘以采样倍数来得到原图
		// 中圆心的位置和半径
		//////////////////////////////////////////
		Img.HoughTransform();

		//////////////////////////////////////////
		// 为后面截出硬币的图像和处理过程申请内存
		//////////////////////////////////////////
		Img.RequestMemory2();

        //////////////////////////////////////////
		// 得到圆的半径，根据半径判断币值
		// numflag=0， 1圆（老版）
		//         1， 1毛
		//         2， 5毛
		//////////////////////////////////////////
		r = Img.GetRadius();
		if (r <= 86) numflag = 1;              //1角
		else if (r > 100) numflag = 0;         //1元
		else numflag = 2;                      //5毛

		//////////////////////////////////////////
		// 从原图中截取出硬币的图像，并做sobel变换
		//////////////////////////////////////////
		Img.GetCircleImg();
		Img.Sobel();

		//////////////////////////////////////////
		// 一度隔一度，提取sobel图的特征，特征为
		// 0.7倍到0.85倍半径的圆环上的点的像素和。
		// 将该特征存放在一个180维的数组中，根据
		// 币值的不同，和事先取好的标准的图的特征
		// 相比较，取相关系数最大时的值，即可知道
		// 正反面和应该旋转的度数。
		//////////////////////////////////////////
		Img.GetCharacter();
		Img.Judge(numflag);

		//////////////////////////////////////////
		// code为硬币的正反面，0为正，1为反
		// ro表示硬币应该旋转多少度得以转正
		//////////////////////////////////////////
		code = Img.GetCode();
		ro = Img.GetTheta();

		//////////////////////////////////////////
		// 初始化旋转图，背景置为黑色，将硬币转正
		//////////////////////////////////////////
		Img.InitRotateImg();
		Img.RotateImg();

		//////////////////////////////////////////
		// 根据币值、正反面和应旋转的度数为图像命名
		// 并输出到指定的路径
		//////////////////////////////////////////
		switch (numflag)
		{
		case 0:
			if (code == 0)
			{
				sprintf_s(OutputPath, "..\\LinCoin_Object_Oriented\\ans\\%d_1圆正面_旋转%d度后转正.bmp",num, ro);
				Img.Output(OutputPath);
			}
			else
			{
				sprintf_s(OutputPath, "..\\LinCoin_Object_Oriented\\ans\\%d_1圆反面_旋转%d度后转正.bmp", num, ro);
				Img.Output(OutputPath);
			}
			break;
		case 1:
			if (code == 0)
			{
				sprintf_s(OutputPath, "..\\LinCoin_Object_Oriented\\ans\\%d_1毛正面_旋转%d度后转正.bmp", num, ro);
				Img.Output(OutputPath);
			}
			else
			{
				sprintf_s(OutputPath, "..\\LinCoin_Object_Oriented\\ans\\%d_1毛反面_旋转%d度后转正.bmp", num, ro);
				Img.Output(OutputPath);
			}
			break;
		case 2:
			if (code == 0)
			{
				sprintf_s(OutputPath, "..\\LinCoin_Object_Oriented\\ans\\%d_5毛正面_旋转%d度后转正.bmp", num, ro);
				Img.Output(OutputPath);
			}
			else
			{
				sprintf_s(OutputPath, "..\\LinCoin_Object_Oriented\\ans\\%d_5毛反面_旋转%d度后转正.bmp", num, ro);
				Img.Output(OutputPath);
			}
			break;
		}
	}
	
	t2 = clock();
	printf_s("\nAvg %lf ms\n", (t2 - t1) / 49.0); //查看每张图的处理时间。
	getchar();
	getchar();
	return;
}
