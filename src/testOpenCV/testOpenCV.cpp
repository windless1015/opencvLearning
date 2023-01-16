#include <windows.h>
#include <shobjidl.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace std;

/*
//	cv::Point center = cv::Point(warp_dst.cols / 2, warp_dst.rows / 2);
//	double angle = -50.0;
//	double scale = 0.6;
//	cv::Mat rot_mat = cv::getRotationMatrix2D(center, angle, scale);
//	cv::Mat warp_rotate_dst;
//	warpAffine(warp_dst, warp_rotate_dst, rot_mat, warp_dst.size());
*/
std::string storedFilePath = "";

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

std::string openFileWindow()
{
    PWSTR pszFilePath = L"";
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileOpenDialog *pFileOpen;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
            IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem *pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    // Display the file name to the user.
                    /*if (SUCCEEDED(hr))
                    {
                        MessageBoxW(NULL, pszFilePath, L"File Path", MB_OK);
                        CoTaskMemFree(pszFilePath);
                    }*/
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }
    if ((wcslen(pszFilePath)) == 0)
        return "";
    //return string, need to convert to std::string
    string str;
    DWORD dwMinSize = 0;
    LPSTR lpszStr = NULL;
    dwMinSize = WideCharToMultiByte(CP_OEMCP, NULL, pszFilePath, -1, NULL, 0, NULL, FALSE);
    if (0 == dwMinSize)
    {
        return FALSE;
    }
    lpszStr = new char[dwMinSize];
    WideCharToMultiByte(CP_OEMCP, NULL, pszFilePath, -1, lpszStr, dwMinSize, NULL, FALSE);
    str = lpszStr;
    delete[] lpszStr;
    CoTaskMemFree(pszFilePath);
    return str;

}

void transparentizeImage(cv::Mat& image);

int main(int argc, char* argv[]) 
{
    cv::Mat movingImage, fixedImage;

    std::string movingPath = openFileWindow();
    if (movingPath == "")
    {
        std::cout << "Error: No Moving Image" << std::endl;
        return -1;
    }

    //"E:\\code\\UltraSound\\Registration\\MedicalImageRegistration\\src\\testMedicalRegistrationFromSrc\\testData\\t2.png";
    movingImage = cv::imread(movingPath);
    /*movingImage = cv::imread(movingPath, cv::IMREAD_COLOR);*/
    if (!movingImage.data) {
        printf("No image data \n");
        return -1;
    }

    transparentizeImage(movingImage);
    return -1;

    //fixed image reading
    std::string fixedPath = openFileWindow();
    if (fixedPath == "")
    {
        std::cout << "Error: No Fixed Image" << std::endl;
        return -1;
    }
        //"E:\\code\\UltraSound\\Registration\\MedicalImageRegistration\\src\\testMedicalRegistrationFromSrc\\testData\\t1.png";
    fixedImage = cv::imread(fixedPath, cv::IMREAD_COLOR);
    if (!fixedImage.data) {
        printf("No image data \n");
        return -1;
    }

    int a = 1;
	float mat_origin[9];
	float mat[6];
	readMatrixFromFile("D:/testMat.txt", mat_origin);
	for (int i = 0; i < 6; i++)
		mat[i] = mat_origin[i];

	std::cout << "2D image registration" << std::endl;

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

	cv::Mat warpedMovingImage;
	cv::warpAffine(movingImage, warpedMovingImage, applyMatrix_real, movingImage.size());


	cv::Mat overlayImage;
	/*cv::addWeighted(result, 0.3, fixedImage, 0.7, 0, overlayImage);*/
    //cv::addWeighted(fixedImage, 0.8, warpedMovingImage, 0.2, 0, overlayImage);

	cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
	cv::imshow("movingImage", movingImage);
	cv::imshow("fixedImage", fixedImage);
	cv::imshow("warpedMovingImage", warpedMovingImage);
	//cv::imshow("overlay", overlayImage);
	cv::waitKey(0);
	return 0;
}

void transparentizeImage(cv::Mat& image)
{
    cv::Mat rgba;
    cv::cvtColor(image, rgba, cv::COLOR_BGR2BGRA);
    //Split the image for access to alpha channel
    std::vector<cv::Mat> rgbaChannels(4);
    cv::split(rgba, rgbaChannels);
    //Assign the mask to the last channel of the image
    rgbaChannels[3] = 0.2 * rgbaChannels[3];
    ////Finally concat channels for rgba image
    cv::merge(rgbaChannels, rgba);

    //std::vector<cv::Mat> rgbChannels;
    //cv::split(image, rgbChannels);

    //cv::Mat alpha = cv::Mat::zeros(image.rows, image.cols, CV_8UC1);
    //alpha += 128;

    /*cv::Mat alpha = rgbChannels.at(0) + rgbChannels.at(1) + rgbChannels.at(2);
    rgbChannels.push_back(alpha);*/

    /*cv::Mat res0;
    cv::merge(&(rgbChannels[0]), 1, res0);

    cv::Mat res1;
    cv::merge(&(rgbChannels[1]), 1, res1);

    cv::Mat res2;
    cv::merge(&(rgbChannels[2]), 1, res2);

    cv::imshow("res0", res0);
    cv::imshow("res1", res1);
    cv::imshow("res2", res2);

    cv::Mat res_rgb;
    std::vector<cv::Mat> mergeArr = { res0, res1, res2 };
    cv::merge(mergeArr,res_rgb);
    cv::imshow("res_rgb", res_rgb);*/

    cv::imshow("dddd", rgba);
    cv::waitKey(0);
}

void test(cv::Mat& img)
{
    //cv::Mat rgba;
    ////First create the image with alpha channel
    //cv::cvtColor(img, rgba, cv::COLOR_RGB2RGBA);
    ////Split the image for access to alpha channel
    //std::vector<cv::Mat>channels(4);
    //cv::split(rgba, channels);
    ////Assign the mask to the last channel of the image
    //channels[3] = alpha_data;
    ////Finally concat channels for rgba image
    //cv::merge(channels, 4, rgba);

}