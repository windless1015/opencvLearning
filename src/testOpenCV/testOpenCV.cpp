#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace std;

void printMatrix(cv::Mat& mat)
{
	int h = mat.rows;
	int w = mat.cols;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++)
		{
			std::cout << mat.at<double>(i, j) << " ";
		}
		std::cout << endl;
	}
}

void readMatrixFromFile(std::string matFile, float *mat)
{
	ifstream in;
	in.open(matFile);
	assert(in.is_open());
	int count = 0;
	string s;
	while (getline(in, s, ','))
	{
		s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());
		s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
		if (s == "")
			continue;
		float f = stof(s);
		mat[count++] = f;
		//cout << s ;
	}
	in.close();
}

int main(int argc, char* argv[]) {
	float mat_origin[9];
	float mat[6];
	readMatrixFromFile("D:/testMat.txt", mat_origin);
	for (int i = 0; i < 6; i++)
		mat[i] = mat_origin[i];

	std::cout << "2D image registration" << std::endl;

	/*if (argc != 2) {
		printf("usage: main <Image_Path>\n");
		return -1;
	}*/
	cv::Mat movingImage, fixedImage;
	/*image = cv::imread(argv[1], cv::IMREAD_COLOR);*/
	std::string movingPath = 
		"E:\\code\\UltraSound\\Registration\\MedicalImageRegistration\\src\\testMedicalRegistrationFromSrc\\testData\\t2.png";
	movingImage = cv::imread(movingPath, cv::IMREAD_COLOR);
	if (!movingImage.data) {
		printf("No image data \n");
		return -1;
	}

	std::string fixedPath =
		"E:\\code\\UltraSound\\Registration\\MedicalImageRegistration\\src\\testMedicalRegistrationFromSrc\\testData\\t1.png";
	fixedImage = cv::imread(fixedPath, cv::IMREAD_COLOR);
	if (!fixedImage.data) {
		printf("No image data \n");
		return -1;
	}

	int h = movingImage.size().height;
	int w = movingImage.size().width;

	//cv::Mat applyMatrix(3, 3, CV_32FC1, mat);
	cv::Mat applyMatrix_real(2, 3, CV_64FC1);
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			applyMatrix_real.at<double>(i, j) = mat[i * 3 + j];
		}
	}


	printMatrix(applyMatrix_real);
	/*cv::Point2f center = cv::Point2f(h / 2.0, w / 2.0);
	cv::Mat rot_mat = cv::getRotationMatrix2D(center, 15, 1.0);
	printMatrix(rot_mat);*/

	cv::Mat result;
	cv::warpAffine(movingImage, result, applyMatrix_real, movingImage.size());


	cv::Mat overlayImage;
	cv::addWeighted(result, 0.3, fixedImage, 0.7, 0, overlayImage);

	cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
	cv::imshow("movingImage", movingImage);
	cv::imshow("fixedImage", fixedImage);
	cv::imshow("warped Moving", result);
	cv::imshow("overlay", overlayImage);
	cv::waitKey(0);
	return 0;
}

//int main(int argc, char** argv)
//{
//	std::string path =
//		"E:\\code\\UltraSound\\Registration\\MedicalImageRegistration\\src\\testMedicalRegistrationFromSrc\\testData\\t1.png";
//	cv::Mat src = cv::imread(path);
//	if (src.empty())
//	{
//		cout << "Could not open or find the image!\n" << endl;
//		cout << "Usage: " << argv[0] << " <Input image>" << endl;
//		return -1;
//	}
//	cv::Point2f srcTri[3];
//	srcTri[0] = cv::Point2f(0.f, 0.f);
//	srcTri[1] = cv::Point2f(src.cols - 1.f, 0.f);
//	srcTri[2] = cv::Point2f(0.f, src.rows - 1.f);
//	cv::Point2f dstTri[3];
//	dstTri[0] = cv::Point2f(0.f, src.rows*0.33f);
//	dstTri[1] = cv::Point2f(src.cols*0.85f, src.rows*0.25f);
//	dstTri[2] = cv::Point2f(src.cols*0.15f, src.rows*0.7f);
//	cv::Mat warp_mat = cv::getAffineTransform(srcTri, dstTri);
//	cv::Mat warp_dst = cv::Mat::zeros(src.rows, src.cols, src.type());
//	warpAffine(src, warp_dst, warp_mat, warp_dst.size());
//	cv::Point center = cv::Point(warp_dst.cols / 2, warp_dst.rows / 2);
//	double angle = -50.0;
//	double scale = 0.6;
//	cv::Mat rot_mat = cv::getRotationMatrix2D(center, angle, scale);
//	cv::Mat warp_rotate_dst;
//	warpAffine(warp_dst, warp_rotate_dst, rot_mat, warp_dst.size());
//	cv::imshow("Source image", src);
//	cv::imshow("Warp", warp_dst);
//	cv::imshow("Warp + Rotate", warp_rotate_dst);
//	cv::waitKey();
//	return 0;
//}