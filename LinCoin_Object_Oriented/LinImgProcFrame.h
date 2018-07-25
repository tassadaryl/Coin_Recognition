#ifndef LinImgProcFrame_H
#define LinImgProcFrame_H
class LinImgProcFrame {
public:
	//��������
	LinImgProcFrame();
	~LinImgProcFrame();
	//��ͼ
	void Init(char *s);
	//�����ڴ�
	void RequestMemory();
	void RequestMemory2();
	//��ȡ��͸�
	int GetWidth();
	int GetHeight();
	//���ò�������
	void SetRatio(int x);
	//����
	void Sample();
	//�������ͼSobel����ֵ�����õ���Եͼ
	void SampleSobel();
	void SampleBin();
	void SampleInit();
	void SampleGetEdge();
	//Hough�任���õ�����ͼ�е�Բ�Ĳ���
	void HoughTransform();
	//����Բ�ĺͰ뾶��ԭͼ��ȡ��Ӳ��
	void GetCircleImg();
	//�õ��뾶
	int GetRadius();
	//Sobel���ӣ���ȡ����
	void Sobel();
	//��ȡ180���������Ƚ�
	void GetCharacter();
	void Judge(int numflag);
	//��ʼ����תͼ��
	void InitRotateImg();
	//��תԭͼ����ת��
	void RotateImg();
	//�õ�Ӳ����������ת�Ĳ���
	int GetCode();
	int GetTheta();
	//���ͼ��
	void Output(char *s);
	void OutputCircleImg(char *s);

	int GetRatio();

private:
	//�ڴ��ͷ�
	void Dump();

private:
	//ͼ������
	int width, height;     //ԭͼ����
	int nwidth, nheight;   //����ͼ����
	int ratio;             //��������
	int A, B, Radius, Err, d; //Բ�ĺ����ꡢ�����ꡢ�뾶��������ֱ��
	int Character[180] = { 0 };//ͼ�������
	int code, theta;              //codeΪ0��ʾӲ�����棬1��ʾ���棬theta��ʾӦ��ת�Ķ���

	BYTE *pGryImg;
	BYTE *pSampleImg;
	BYTE *pSampleSbImg;
	BYTE *pSampleBinImg;
	BYTE *pSampleEdgeImg;
	BYTE *pCircleImg;
	BYTE *pCircleSbImg;
	BYTE *pRotateImg;

	//��������ڴ��ֽ���                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
	int m_memSize;
	//�ڲ�������
	int m_nFrameID;
	//ͼ��·��
	//char *Path;
};

LinImgProcFrame::LinImgProcFrame() {
	//<�ڹ��캯���а����еĳ�Ա��������ֵ>
	//ͼ������
	width = 0;
	height = 0;

	pGryImg = NULL;
	pSampleImg = NULL;
	pSampleSbImg = NULL;
	pSampleBinImg = NULL;
	pSampleEdgeImg = NULL;
	pCircleImg = NULL;
	pCircleSbImg = NULL;
	pRotateImg = NULL;

	m_memSize = 0;
	m_nFrameID = 0;
}

LinImgProcFrame::~LinImgProcFrame() {
	Dump();
}

void LinImgProcFrame::Dump()
{
	if (pGryImg) { delete pGryImg; pGryImg = NULL; }
	if (pSampleImg) { delete pSampleImg; pSampleImg = NULL; }
	if (pSampleSbImg) { delete pSampleSbImg;pSampleSbImg = NULL; }
	if (pSampleBinImg) { delete pSampleBinImg;pSampleBinImg = NULL; }
	if (pSampleEdgeImg) { delete pSampleEdgeImg; pSampleEdgeImg = NULL; }
	if (pCircleImg) { delete pCircleImg; pCircleImg = NULL; }
	if (pCircleSbImg) { delete pCircleSbImg; pCircleSbImg = NULL; }
	if (pRotateImg) { delete pRotateImg;pRotateImg = NULL; }
	m_memSize = 0;
}

void LinImgProcFrame::Init(char *s)
{
	pGryImg = RmwRead8BitBmpFile2Img(s, &width, &height);
	A = B = Radius = Err = d = -1;
	memset(Character, 0, sizeof(int) * 180);
	ratio = 0;
	code = 0;
	theta = 0;
}

void LinImgProcFrame::RequestMemory() 
{
	//���ͷ�
	//Dump();
	//������
	//pGryImg = new BYTE[width*height];
	pSampleImg = new BYTE[nwidth*nheight];
	pSampleSbImg = new BYTE[nwidth*nheight];
	pSampleBinImg = new BYTE[nwidth*nheight];
	pSampleEdgeImg = new BYTE[nwidth*nheight];
	m_memSize = width*height + 4 * nwidth*nheight + 100; 
}

void LinImgProcFrame::RequestMemory2()
{
	pCircleImg = new BYTE[d*d];
	pCircleSbImg = new BYTE[d*d];
	pRotateImg = new BYTE[d*d];
	m_memSize += 3 * d * d;
}

int LinImgProcFrame::GetWidth()
{
	return width;
}

int LinImgProcFrame::GetHeight()
{
	return height;
}

void LinImgProcFrame::SetRatio(int x)
{
	ratio = x;
	nwidth = width / x;
	nheight = height / x;
}

void LinImgProcFrame::Sample()
{
	LinSample(pGryImg, pSampleImg, width, height, ratio);
}

void LinImgProcFrame::SampleSobel()
{
	RmwSobelGrayImage(pSampleImg, nwidth, nheight, pSampleSbImg);
}

void LinImgProcFrame::SampleBin()
{
	LinGetBitImg(pSampleSbImg, pSampleBinImg, nwidth, nheight, 100);
}

void LinImgProcFrame::SampleInit()
{
	LinInitial(pSampleEdgeImg, nwidth, nheight);
}

void LinImgProcFrame::SampleGetEdge()
{
	LinGetEdge(pSampleBinImg, pSampleEdgeImg, nwidth, nheight);
}

void LinImgProcFrame::HoughTransform()
{
	LinHoughCircle(pSampleEdgeImg, nwidth, nheight, &A, &B, &Radius, &Err);
	A *= ratio;
	B *= ratio;
	Radius *= ratio;
	d = 2 * Radius + 1;
}

int LinImgProcFrame::GetRadius()
{
	return Radius;
}

void LinImgProcFrame::GetCircleImg()
{
	LinGetCircle(pGryImg, pCircleImg, A, B, Radius, width, height);
}

void LinImgProcFrame::Sobel()
{
	RmwSobelGrayImage(pCircleImg, d, d, pCircleSbImg);
}

void LinImgProcFrame::GetCharacter()
{
	LinGetCharacter(pCircleSbImg, Character, Radius);
}

void LinImgProcFrame::Judge(int numflag)
{
	judge(Character, &code, &theta, numflag);
	theta = (360 - theta) % 360;
}

int LinImgProcFrame::GetCode()
{
	return code;
}

int LinImgProcFrame::GetTheta()
{
	return theta;
}

void LinImgProcFrame::InitRotateImg()
{
	LinInitial2(pRotateImg, d, d);
}

void LinImgProcFrame::RotateImg()
{
	LinRotate2(pCircleImg, d, d, pRotateImg, theta, Radius, Radius);
}

void LinImgProcFrame::Output(char *s)
{
	RmwWrite8BitImg2BmpFile(pRotateImg, d, d, s);
}

void LinImgProcFrame::OutputCircleImg(char *s)
{
	RmwWrite8BitImg2BmpFile(pGryImg, width, height, s);
}

int LinImgProcFrame::GetRatio()
{
	return ratio;
}
#endif
