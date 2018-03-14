/*
 *
 *

 // for operations on Image
 CvMat* LutMatrix;
 IplImage*  cvImage;
 IplImage*  cvImageTemp;   // this is typically swapped back into cvImage

--
LutMatrix = cvCreateMat(1, 256, CV_8UC1);
cvImage = cvCreateImage(cvSize(settings.image_size_W, settings.image_size_H), IPL_DEPTH_8U, 1);
cvImageTemp = cvCreateImage(cvSize(settings.image_size_W, settings.image_size_H), IPL_DEPTH_8U, 1);

--
cvReleaseMat(&LutMatrix);
cvReleaseImage(&cvImage);
cvReleaseImage(&cvImageTemp);


void AnalysisThread::imageAmplify(ofxCvGrayscaleImage& image, float brightness, float contrast) {
	if (!image.bAllocated) {
		ofLogError("ofxCvGrayscaleImage") << "brightnessContrast(): image not allocated";
		return;
	}

	int i;

	//
	//The algorithm is by Werner D. Streidt
	//(http://visca.com/ffactory/archives/5-99/msg00021.html)
	//(note: uses values between -1 and 1)
	//

	double delta;
	double a;
	double b;
	if (contrast > 0)
	{
		delta = 127.*contrast;
		a = 255. / (255. - delta * 2);
		b = a * (brightness * 100 - delta);
	}
	else {
		delta = -128.*contrast;
		a = (256. - delta * 2) / 255.;
		b = a * brightness*100. + delta;
	}

	for (i = 0; i < 256; i++)
	{
		int v = cvRound(a*i + b);
		if (v < 0)
			v = 0;
		if (v > 255)
			v = 255;
		LutMatrix->data.ptr[i] = (uchar)v;
	}

	cvLUT(cvImage, cvImageTemp, LutMatrix);

	if (cvImageTemp->width*cvImageTemp->nChannels == cvImage->widthStep) {
		image.getPixels().setFromExternalPixels((unsigned char*)cvImageTemp->imageData, cvImageTemp->width, cvImageTemp->height, cvImageTemp->nChannels);
	}
	else {
		image.getPixels().setFromAlignedPixels((unsigned char*)cvImageTemp->imageData, cvImageTemp->width, cvImageTemp->height, cvImageTemp->nChannels, cvImageTemp->widthStep);
	}
}

void AnalysisThread::adjustGamma(ofxCvGrayscaleImage& img, float gamma = 0.5)
{
	auto gammaImage = ofxCv::toCv(img);

	cv::Mat lookUpTable(1, 256, CV_8U);
	unsigned char* p = lookUpTable.ptr();
	for (int i = 0; i < 256; i++) {
		p[i] = cv::saturate_cast<unsigned char>(pow(i / 255.0, gamma) * 255.0);
	}
	cv::LUT(gammaImage, lookUpTable, gammaImage);

	ofxCv::toOf(gammaImage, img.getPixels());
}

void amplification(ofxCvGrayscaleImage& img, float scale = 1.0)
{
	auto cvImage = ofxCv::toCv(img);

	cv::Mat lookUpTable(1, 256, CV_8U);
	unsigned char* p = lookUpTable.ptr();
	for (int i = 0; i < 256; i++) {
		p[i] = cv::saturate_cast<unsigned char>(i * scale);
	}
	cv::LUT(cvImage, lookUpTable, cvImage);

	ofxCv::toOf(cvImage, img.getPixels());
}

*/