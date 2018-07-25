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
		// ��ʼ������ͼ����ʼ��������
		//////////////////////////////////////////
		Img.Init(InputPath);

		//////////////////////////////////////////
		// ���ò�������       
		//////////////////////////////////////////
		Img.SetRatio(2);

		//////////////////////////////////////////
		// Ϊ��������е�ͼ�����ڴ� 
		//////////////////////////////////////////
		Img.RequestMemory();

		//////////////////////////////////////////
		// ����            
		//////////////////////////////////////////
		Img.Sample();

		//////////////////////////////////////////
		// �Բ������ͼ��sobel����
		//////////////////////////////////////////
		Img.SampleSobel();

		//////////////////////////////////////////
		// ��sobel���ͼ��ֵ��    
		//////////////////////////////////////////
		Img.SampleBin();

		//////////////////////////////////////////
		// �������ң��������󣬴�������,��������
		// ɨ�裬��ȡӲ�ҵı�Ե
		//////////////////////////////////////////
		Img.SampleInit();
		Img.SampleGetEdge();

		//////////////////////////////////////////
		// ͨ��Բ��Hough�任�õ�Բ��λ�úͰ뾶
		// �õ��Ĳ���ȫ�����Բ����������õ�ԭͼ
		// ��Բ�ĵ�λ�úͰ뾶
		//////////////////////////////////////////
		Img.HoughTransform();

		//////////////////////////////////////////
		// Ϊ����س�Ӳ�ҵ�ͼ��ʹ�����������ڴ�
		//////////////////////////////////////////
		Img.RequestMemory2();

        //////////////////////////////////////////
		// �õ�Բ�İ뾶�����ݰ뾶�жϱ�ֵ
		// numflag=0�� 1Բ���ϰ棩
		//         1�� 1ë
		//         2�� 5ë
		//////////////////////////////////////////
		r = Img.GetRadius();
		if (r <= 86) numflag = 1;              //1��
		else if (r > 100) numflag = 0;         //1Ԫ
		else numflag = 2;                      //5ë

		//////////////////////////////////////////
		// ��ԭͼ�н�ȡ��Ӳ�ҵ�ͼ�񣬲���sobel�任
		//////////////////////////////////////////
		Img.GetCircleImg();
		Img.Sobel();

		//////////////////////////////////////////
		// һ�ȸ�һ�ȣ���ȡsobelͼ������������Ϊ
		// 0.7����0.85���뾶��Բ���ϵĵ�����غ͡�
		// �������������һ��180ά�������У�����
		// ��ֵ�Ĳ�ͬ��������ȡ�õı�׼��ͼ������
		// ��Ƚϣ�ȡ���ϵ�����ʱ��ֵ������֪��
		// �������Ӧ����ת�Ķ�����
		//////////////////////////////////////////
		Img.GetCharacter();
		Img.Judge(numflag);

		//////////////////////////////////////////
		// codeΪӲ�ҵ������棬0Ϊ����1Ϊ��
		// ro��ʾӲ��Ӧ����ת���ٶȵ���ת��
		//////////////////////////////////////////
		code = Img.GetCode();
		ro = Img.GetTheta();

		//////////////////////////////////////////
		// ��ʼ����תͼ��������Ϊ��ɫ����Ӳ��ת��
		//////////////////////////////////////////
		Img.InitRotateImg();
		Img.RotateImg();

		//////////////////////////////////////////
		// ���ݱ�ֵ���������Ӧ��ת�Ķ���Ϊͼ������
		// �������ָ����·��
		//////////////////////////////////////////
		switch (numflag)
		{
		case 0:
			if (code == 0)
			{
				sprintf_s(OutputPath, "..\\LinCoin_Object_Oriented\\ans\\%d_1Բ����_��ת%d�Ⱥ�ת��.bmp",num, ro);
				Img.Output(OutputPath);
			}
			else
			{
				sprintf_s(OutputPath, "..\\LinCoin_Object_Oriented\\ans\\%d_1Բ����_��ת%d�Ⱥ�ת��.bmp", num, ro);
				Img.Output(OutputPath);
			}
			break;
		case 1:
			if (code == 0)
			{
				sprintf_s(OutputPath, "..\\LinCoin_Object_Oriented\\ans\\%d_1ë����_��ת%d�Ⱥ�ת��.bmp", num, ro);
				Img.Output(OutputPath);
			}
			else
			{
				sprintf_s(OutputPath, "..\\LinCoin_Object_Oriented\\ans\\%d_1ë����_��ת%d�Ⱥ�ת��.bmp", num, ro);
				Img.Output(OutputPath);
			}
			break;
		case 2:
			if (code == 0)
			{
				sprintf_s(OutputPath, "..\\LinCoin_Object_Oriented\\ans\\%d_5ë����_��ת%d�Ⱥ�ת��.bmp", num, ro);
				Img.Output(OutputPath);
			}
			else
			{
				sprintf_s(OutputPath, "..\\LinCoin_Object_Oriented\\ans\\%d_5ë����_��ת%d�Ⱥ�ת��.bmp", num, ro);
				Img.Output(OutputPath);
			}
			break;
		}
	}
	
	t2 = clock();
	printf_s("\nAvg %lf ms\n", (t2 - t1) / 49.0); //�鿴ÿ��ͼ�Ĵ���ʱ�䡣
	getchar();
	getchar();
	return;
}
