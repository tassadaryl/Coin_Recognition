#ifndef LinImgProcFrame_H
#define LinImgProcFrame_H
class LinImgProcFrame {
public:
	//构造析构
	LinImgProcFrame();
	~LinImgProcFrame();
	//读图
	void Init(char *s);
	//申请内存
	void RequestMemory();
	void RequestMemory2();
	//获取宽和高
	int GetWidth();
	int GetHeight();
	//设置采样倍数
	void SetRatio(int x);
	//采样
	void Sample();
	//采样后的图Sobel、二值化、得到边缘图
	void SampleSobel();
	void SampleBin();
	void SampleInit();
	void SampleGetEdge();
	//Hough变换，得到采样图中的圆心参数
	void HoughTransform();
	//根据圆心和半径在原图中取出硬币
	void GetCircleImg();
	//得到半径
	int GetRadius();
	//Sobel算子，提取特征
	void Sobel();
	//获取180个特征并比较
	void GetCharacter();
	void Judge(int numflag);
	//初始化旋转图像
	void InitRotateImg();
	//旋转原图将其转正
	void RotateImg();
	//得到硬币正反和旋转的参数
	int GetCode();
	int GetTheta();
	//输出图像
	void Output(char *s);
	void OutputCircleImg(char *s);

	int GetRatio();

private:
	//内存释放
	void Dump();

private:
	//图像属性
	int width, height;     //原图宽、高
	int nwidth, nheight;   //采样图宽、高
	int ratio;             //采样倍数
	int A, B, Radius, Err, d; //圆的横坐标、纵坐标、半径和允许误差、直径
	int Character[180] = { 0 };//图像的特征
	int code, theta;              //code为0表示硬币正面，1表示反面，theta表示应该转的度数

	BYTE *pGryImg;
	BYTE *pSampleImg;
	BYTE *pSampleSbImg;
	BYTE *pSampleBinImg;
	BYTE *pSampleEdgeImg;
	BYTE *pCircleImg;
	BYTE *pCircleSbImg;
	BYTE *pRotateImg;

	//已申请的内存字节数                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
	int m_memSize;
	//内部计数器
	int m_nFrameID;
	//图像路径
	//char *Path;
};

LinImgProcFrame::LinImgProcFrame() {
	//<在构造函数中把所有的成员变量赋初值>
	//图像属性
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
	//先释放
	//Dump();
	//后申请
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
