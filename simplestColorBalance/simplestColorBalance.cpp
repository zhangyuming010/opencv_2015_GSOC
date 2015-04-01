#include "simplestColorBalance.h"
#include<iostream>
#include <stdio.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

void computeAccHist(cv::Mat _imSingle, cv::Mat& _hist)
{
	Mat im = _imSingle;
	int channels[] = {1};
	int n_bin = 255;
	float range[] = { 0, 255 } ;
	const float* histRange = { range };
	calcHist(&im,1, 0, Mat(), _hist, 1, &n_bin,&histRange);

	for (int i = 1; i < _hist.rows*_hist.cols; i++)
	{
		_hist.ptr<float>(0)[i] = _hist.ptr<float>(0)[i-1] + _hist.ptr<float>(0)[i];
	}

}
void simplestColorBanlance(cv::InputArray _im, cv::OutputArray _imOut, float s1, float s2)
{
	Mat im = _im.getMat();
	Mat hist_r, hist_g, hist_b;
	_imOut.create(_im.size(), CV_8UC3);
	Mat imOut = _imOut.getMat();
	vector<Mat> rgb;
	split( im, rgb );
	computeAccHist(rgb[0], hist_r);
	computeAccHist(rgb[1], hist_g);
	computeAccHist(rgb[2], hist_b);
	// compute linear mapping
	Mat mapping_r, mapping_g, mapping_b;
	computeMapping(rgb[0], hist_r, mapping_r, s1, s2);
	computeMapping(rgb[1], hist_g, mapping_g, s1, s2);
	computeMapping(rgb[2], hist_b, mapping_b, s1, s2);

	vector<Mat> out(3);
	remapPixValue(mapping_r, rgb[0], out[0]);
	remapPixValue(mapping_g, rgb[1], out[1]);
	remapPixValue(mapping_b, rgb[2], out[2]);
	merge(out, imOut);

}
void computeMapping(const cv::InputArray _imSingle, const cv::InputArray _hist, cv::OutputArray _mapping, float s1, float s2)
{
	
	Mat im = _imSingle.getMat();
	// accumulated histogram
	Mat hist = _hist.getMat();
	_mapping.create(1, 255, CV_8U);
	Mat mapping = _mapping.getMat();
	float vmin = 0;
	Size imSize = im.size();
	float n_pix = imSize.width*imSize.height;
	while (hist.at<float>(vmin) < n_pix*s1)
		vmin ++;
	float vmax = 255 - 1;
	while(hist.at<float>(vmax) > n_pix*(1-s2))
		vmax --;
	// saturate 
	for (int i = 0; i < im.rows; i++)
	{
		uchar* ptrIm = im.ptr<uchar>(i);
		for(int j = 0; j< im.cols; j++)
		{
			if (ptrIm[j] < vmin)
				ptrIm[j] = vmin;
			if (ptrIm[j] > vmax)
				ptrIm[j] = vmax;
		}
	}
	for (int i = 0; i < 255; i++)
	{
		if(i < vmin)
			mapping.ptr<uchar>(0)[i] = 0;
		else if(i >= vmin && i <= vmax)
			mapping.ptr<uchar>(0)[i] = (uchar)((i - vmin)* 255 /(vmax - vmin));
		else
			mapping.ptr<uchar>(0)[i] = 255;
	}
}
void remapPixValue(const cv::InputArray _mapping, cv::InputArray _imSingle, cv::OutputArray _outSingle)
{
	Mat mapping = _mapping.getMat();
	uchar* ptrMap = mapping.data;
	Mat imSingle = _imSingle.getMat();
	_outSingle.create(_imSingle.size(), CV_8U);
	Mat outSingle = _outSingle.getMat();
	for (int i = 0; i < imSingle.rows; i++)
	{
		uchar* ptrIn = imSingle.ptr<uchar>(i);
		uchar* ptrOut = outSingle.ptr<uchar>(i);
		for (int j = 0; j < imSingle.cols; j++)
		{
			ptrOut[j] = ptrMap[ptrIn[j]];
		}
	}
}

int main(int argc, char **argv)
{
	if (3 != argc)
	{
		printf("Useage: %s inputimage outputimage", argv[0]);
	}
	cv::Mat im = imread(argv[1]);
	cv::Mat out;
	simplestColorBanlance(im, out, 0.01, 0.01);
	imwrite(argv[2],out);
}