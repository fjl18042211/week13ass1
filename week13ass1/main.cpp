#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;


//Ϊ���ڱ��ⶨ��ĺ� 
#define WINDOW_NAME1 "��ԭʼͼƬ��"        
#define WINDOW_NAME2 "��ƥ�䴰�ڡ�"        



Mat g_srcImage; Mat g_templateImage; Mat g_resultImage;
int g_nMatchMethod;
int g_nMaxTrackbarNum = 5;
//��������
void on_Matching(int, void*);
void ShowHelpText();
int main()
{

	ShowHelpText();
	g_srcImage = imread("D:\\img.png", 0);
	g_templateImage = imread("D:\\template.png", 0);
	//��������
	namedWindow(WINDOW_NAME1, WINDOW_AUTOSIZE);
	namedWindow(WINDOW_NAME2, WINDOW_AUTOSIZE);
	//���������������㷨ѡ��
	createTrackbar("�㷨", WINDOW_NAME1, &g_nMatchMethod, g_nMaxTrackbarNum, on_Matching);
	on_Matching(0, 0);
	waitKey(0);
	return 0;

}



void on_Matching(int, void*)
{

	Mat srcImage;
	g_srcImage.copyTo(srcImage);

	//��ʼ�����ڽ������ľ���
	int resultImage_rows = g_srcImage.rows - g_templateImage.rows + 1;
	int resultImage_cols = g_srcImage.cols - g_templateImage.cols + 1;
	g_resultImage.create(resultImage_rows, resultImage_cols, CV_32FC1);


	matchTemplate(g_srcImage, g_templateImage, g_resultImage, g_nMatchMethod);
	normalize(g_resultImage, g_resultImage, 0, 1, NORM_MINMAX, -1, Mat());

	//ƥ���λ�ö�λ
	double minValue; double maxValue; Point minLocation; Point maxLocation;
	Point matchLocation;
	minMaxLoc(g_resultImage, &minValue, &maxValue, &minLocation, &maxLocation, Mat());


	if (g_nMatchMethod == TM_SQDIFF || g_nMatchMethod == TM_SQDIFF_NORMED)
	{
		matchLocation = minLocation;
	}
	else
	{
		matchLocation = maxLocation;
	}

	//���Ƴ����Σ�����ʾ���ս��
	rectangle(srcImage, matchLocation, Point(matchLocation.x + g_templateImage.cols, matchLocation.y + g_templateImage.rows), Scalar(0, 0, 255), 2, 8, 0);
	rectangle(g_resultImage, matchLocation, Point(matchLocation.x + g_templateImage.cols, matchLocation.y + g_templateImage.rows), Scalar(0, 0, 255), 2, 8, 0);

	imshow(WINDOW_NAME1, srcImage);
	imshow(WINDOW_NAME2, g_resultImage);

}




static void ShowHelpText()
{


	printf("\n\n\t������������۲�ͼ��Ч��\n\n");
	printf("\n\t��������Ӧ�ķ�����ֵ˵��: \n\n"
		"\t\t������0��- ƽ����ƥ�䷨(SQDIFF)\n"
		"\t\t������1��- ��һ��ƽ����ƥ�䷨(SQDIFF NORMED)\n"
		"\t\t������2��- ���ƥ�䷨(TM CCORR)\n"
		"\t\t������3��- ��һ�����ƥ�䷨(TM CCORR NORMED)\n"
		"\t\t������4��- ���ϵ��ƥ�䷨(TM COEFF)\n"
		"\t\t������5��- ��һ�����ϵ��ƥ�䷨(TM COEFF NORMED)\n");
}