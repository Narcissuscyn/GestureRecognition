
// this is the function to judge single click or double click

#include "gesture.h"
#include "utils.h"
#include <iostream>
#include <string>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace std;
using namespace op;
using namespace cv;


int diff_click(std::vector<cv::Point>& cli_seq)
{

    if (cli_seq.size()<1)
    {
        cout<<"click sequence is null!"<<endl;
        return -1;
    }
    std::vector<int> dy_seq;

    int length=cli_seq.size();

    int last_dy=-1;
    int interval=0;
    if(cli_seq[1].y-cli_seq[0].y>0)
     {
         last_dy=0;
     }
     else
    {
        last_dy=1;
    }

    for(int i=1;i<length-1;i++)
    {
        if(cli_seq[i+1].y-cli_seq[i].y>0)
        {
            dy_seq.push_back(0);
        }
        else
        {
            dy_seq.push_back(1);
        }
    }

	length=dy_seq.size();
    std::string str="";
    int  lst_dy=dy_seq[0];
    int min_cnt=0;

    for (int i = 0; i < length; i++)
    {

        int cur_dy=dy_seq[i];
        if (cur_dy==lst_dy)
        {
            min_cnt++;
        }
        else
        {

            if (min_cnt>0)
            {
                if (lst_dy==0)
                    str+="0";
                else
                    str+="1";
            }
            lst_dy=cur_dy;
            min_cnt=1;
        }

        if(i==length-1)
        {
            if (min_cnt>0&&lst_dy==0)
                str+="0";
            if (min_cnt>0&&lst_dy==1)
                str+="1";
        }
    }

    cout<<"str="<<str<<endl;
    if (str=="01010"||str=="0101"||str=="101")
        return 1;
    if (str=="010"||str=="01") {
        return 0;
    }

//    cout<<str<<endl;
    return -1;
}


