#define LEVEL 255

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
int low_matrix[5][5] = { { 1, 1, 1, 1, 1 },
                         { 1, 1, 1, 1, 1 },
                         { 1, 1, 5, 1, 1 },
                         { 1, 1, 1, 1, 1 },
                         { 1, 1, 1, 1, 1 } };

int high_matrix[5][5] = { { 0, 0, 0, 0, 0 },
                          { 0, -1, -1, -1, 0 },
                          { 0, -1, 9, -1, 0 },
                          { 0, -1, -1, -1, 0 },
                          { 0, 0, 0, 0, 0 } };

int sum__matrix(int (*ptr)[5])
{
    int sum = 0;
    for (int i = 0; i < 5;++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            sum += ptr[i][j];
        }
    }
    return sum;
}

int get_pixel_brightness(int x, int y, int z)
{
    int brightness = (x+y+z)/3;
    return brightness;
}

void do_filtering(cv::Mat_<cv::Vec3b>& _F, cv::Mat_<cv::Vec3b>& _I, int x, int y, int sum, int (*mask)[5]){
    int val = 0;

    if(sum == 0)
        sum = 1;

    if(x+5>=_I.cols || x<0 || y+5>=_I.rows || y<0)
        return;

    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
        {
            val += _I(x + i, y + j)[0] * mask[i][j];
        }
    val = abs(val / sum);
    if(val>255)
        val = 255;
    _F(x + 2, y + 2)[0] = val;

    val=0;
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
        {
            val += _I(x + i, y + j)[1] * mask[i][j];
        }
    val = abs(val / sum);
    if(val>255)
        val = 255;
    _F(x + 2, y + 2)[1] = val;

    val=0;
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
        {
            val += _I(x + i, y + j)[2] * mask[i][j];
        }
    val = abs(val / sum);
    if(val>255)
        val = 255;
    _F(x + 2, y + 2)[2] = val;

}

void transform(cv::Mat &image, cv::Mat &filtered){
    int hp_sum = sum__matrix(high_matrix);
    int lp_sum = sum__matrix(low_matrix);
    std::cout<<"SUMA "<<hp_sum<<std::endl;
    std::cout<<"SUMA"<<lp_sum<<std::endl;
    int average = 0;

    cv::Mat_<cv::Vec3b> _I = filtered;
    cv::Mat_<cv::Vec3b> _F = image;

    for (int i = 0; i < _I.rows; ++i)
        for (int j = 0; j < _I.cols; ++j)
        {
            average = get_pixel_brightness(_I(i,j)[0], _I(i,j)[1], _I(i,j)[2]);

            if (average > LEVEL)
                do_filtering(_I, _F, i, j, lp_sum, low_matrix);
            else
                do_filtering(_I, _F, i, j, hp_sum, high_matrix);
        }
    filtered = _I;

}

int main(int, char *[]) {
    cv::Mat image = cv::imread("Lena.png");
    cv::Mat image2 = cv::imread("Lena.png");
    transform(image, image2);

    cv::imshow( "Oryginalny obraz", image );
    cv::imshow("Obraz po przekształceniu",image2);
    cv::waitKey(-1);
    return 0;
}