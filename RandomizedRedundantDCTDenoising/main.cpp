#include "RedundantDXTDenoise.h"

using namespace std;
using namespace cv;
using namespace cp;
using namespace lab;

void fDCT16x16(const float* s, float* d);
void iDCT16x16(const float* s, float* d);
void fDCT16x16_threshold_keep00_iDCT16x16(const float* src, float* dest, float th);
void fDCT8x8(const float* s, float* d);
void iDCT8x8(const float* s, float* d);
void iDCT8x8GT(const float* s, float* d);
void fDCT8x8GT(const float* s, float* d);
int main()
{
	Mat src_ = imread("img/kodim03.png");
	Mat src;
	resize(src_, src, Size(1024, 1024));
	Mat noise;
	Mat dest;
	Mat dest2;
	float sigma = 20.f;
	addNoise(src, noise, sigma);
	int iteration = 100000;

	//cout << YPSNR(src, noise) << endl;
	RedundantDXTDenoise dctDenoise;

	RandomizedRedundantDXTDenoise rrdct;

	//dctDenoise.isSSE = false;
	{
		
		Stat st;
		CalcTime t("fDCT", 0,false);
		for (int i = 0; i < iteration; i++)
		{
			t.start();
			//xphoto::dctDenoising(noise, dest, sigma, 8);
			rrdct.interlace(noise, dest, sigma, Size(16, 16),
			//rrdct.interlace(noise, dest, sigma, Size(8, 8),
			//RandomizedRedundantDXTDenoise::BASIS::DCT, RandomizedRedundantDXTDenoise::SAMPLING::LATTICE, 4);
				RandomizedRedundantDXTDenoise::BASIS::DCT, RandomizedRedundantDXTDenoise::SAMPLING::LATTICE, 8);
				//RandomizedRedundantDXTDenoise::BASIS::DCT, RandomizedRedundantDXTDenoise::SAMPLING::FULL, 1);
				
			//showMatInfo(dest);
			//noise.copyTo(dest);
			//
			//dctDenoise(noise, dest, sigma, Size(8, 8));
			//dctDenoise(noise, dest, sigma, Size(16,16));
			st.push_back(t.getTime());
			
			//guiAlphaBlend(src, dest);
			cout << YPSNR(src, dest) << endl;
			imshow("test", dest); waitKey(1);
			
			cout<<st.getMedian() << "ms"<<endl;
		}
	}
	//getchar();
	cout << YPSNR(src, dest) << endl;
	/*
	for (int i = 0; i < iteration; i++)
	{
		CalcTime t("4x4 DCT");
		dctDenoise(noise, dest, sigma, Size(4, 4));
	}
	cout << YPSNR(src, dest) << endl;
	*/
	for (int i = 0; i < iteration; i++)
	{
		CalcTime t("8x8 DCT");
		dctDenoise(noise, dest2, sigma, Size(8, 8));
	}
	cout << YPSNR(src, dest2) << endl;
	//dest.setTo(0);
//	guiAlphaBlend(dest, dest2);
	//guiAlphaBlend(src, dest);

	for (int i = 0; i < iteration; i++)
	{
		CalcTime t("16x16 DCT");
		dctDenoise(noise, dest, sigma, Size(16, 16));
	}

	cout << YPSNR(src, dest) << endl;

//	dctDenoise.isSSE = false;
	for (int i = 0; i < iteration; i++)
	{
		CalcTime t("16x16 DCT");
		dctDenoise(noise, dest2, sigma, Size(16, 16));
	}
	cout << YPSNR(src, dest2) << endl;
	dctDenoise.isSSE = true;
	for (int i = 0; i < iteration; i++)
	{
		CalcTime t("4x4 DHT");
		dctDenoise(noise, dest, sigma, Size(4, 4), RedundantDXTDenoise::BASIS::DHT);
	}
	cout << YPSNR(src, dest) << endl;
	for (int i = 0; i < iteration; i++)
	{
		CalcTime t("8x8 DHT");
		dctDenoise(noise, dest, sigma, Size(8, 8), RedundantDXTDenoise::BASIS::DHT);
	}
	cout << YPSNR(src, dest) << endl;
	for (int i = 0; i < iteration; i++)
	{
		CalcTime t("16x16 DHT");
		dctDenoise(noise, dest, sigma, Size(16, 16), RedundantDXTDenoise::BASIS::DHT);
	}
	cout << YPSNR(src, dest) << endl;

	{
		CalcTime t("4x4 OpenCV");
		xphoto::dctDenoising(noise, dest2, sigma, 4);
	}
	cout << YPSNR(src, dest2) << endl;
	{
		CalcTime t("8x8 OpenCV");
		xphoto::dctDenoising(noise, dest2, sigma, 8);
	}
	cout << YPSNR(src, dest2) << endl;
	{
		CalcTime t("16x16 OpenCV");
		xphoto::dctDenoising(noise, dest2, sigma, 16);
	}
	cout << YPSNR(src, dest2) << endl;

	//guiAlphaBlend(src, dest);
	return 0;
}