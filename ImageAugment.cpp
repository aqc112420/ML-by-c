//本程序是用c++来进行数据增强，包括了随机翻转，亮度对比度调整，随机噪声和模糊

#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>
#include<opencv2\core.hpp>

using namespace cv;
using namespace std;

void Resize(Mat &img, int rows, int cols)
{
    resize(img, img, Size(rows, cols), 0, 0, INTER_NEAREST);
}

void random_flip(Mat &img)
{
    //Mat dst;
    const int arrayNum[4] = { 0, 0, 1, 2 };
    int RandIndex = rand() % 4; //generates a random number between 0 and 3
    cout << arrayNum[RandIndex] << endl;
    if (arrayNum[RandIndex] == 1)
    {
        flip(img, img, 1);
        //return dst;
    }
    else if (arrayNum[RandIndex] == 2)
    {
        flip(img, img, 0);
        //return dst;
    }
    else
        ;
        //return img;
}

void on_ContrastAndBright(Mat &img, int contrastValue=80, int brightValue=80)
{
    for (int x = 0; x < img.rows; x++)
    {
        for (int y = 0; y < img.cols; y++)
        {
            for (int z = 0; z < 3; z++)
            {
                img.at<Vec3b>(x, y)[z] =
                    saturate_cast<uchar>((contrastValue*0.01)*(img.at<Vec3b>(x, y)[z]) + brightValue);
            }
        }
    }
}

void RandomBlur(Mat &img, int min = 0, int max = 5)
{
    int radius = rand() % max + min;
    GaussianBlur(img, img, Size(radius, radius), 0, 0);
}

Mat AddRandomNoise(Mat &img, int sd = 5)
{
    int noise_sd = rand() % sd;
    cout << noise_sd << endl;
    Mat dst;
    if (noise_sd > 0)
    {
        Mat noise(img.size(), img.type());
        RNG rng(time(NULL));
        rng.fill(noise, RNG::NORMAL, 0, 36);
        add(img, noise, dst);
    }
    else
    {
        dst = img;
    }
    return dst;
}



int main()
{
    Mat img = imread("E:\\datasets\\aeroscapes\\JPEGImages\\000001_001.jpg");
    if (img.empty())
    {
        cout << "Failed to read img" << endl;
        system("pause");
        exit(0);
    }

    for (int i = 0; i < 10; i++)
    {
        Mat dst = AddRandomNoise(img);
        namedWindow("result", WINDOW_NORMAL);
        imshow("result", dst);
        waitKey(0);
    }
    

   
    system("pause");

    return 0;
}