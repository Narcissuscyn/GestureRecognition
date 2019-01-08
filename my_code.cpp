//
// Created by narcissus on 18-5-22.
//

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <string>
#include<vector>
#include "Point.h"

int BUTTON_NUMBER=12;
using namespace std;

//全剧变量
cv::Rect tar_box(-1,-1,-1,-1);//标志为空的Rect


cv::Rect rightview_click_area[12]={cv::Rect(158,317,38,23),cv::Rect(275,323,40,23),cv::Rect(270,346,46,22),cv::Rect(268,368,46,25),cv::Rect(235,323,40,23)
        ,cv::Rect(229,346,41,22),cv::Rect(224,368,42,25),cv::Rect(196,320,39,22),cv::Rect(187,343,41,22),
                      cv::Rect(180,365,43,25),cv::Rect(142,340,44,23),cv::Rect(133,363,46,25)
};


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//函数部分



/***
 * '''
    :param point: finger tip coordinate
    :param tar_box: the target button
    :return:False:finger tip is not on the target button;True:finger tip is  on the target button;
    '''
 */
 bool is_tip_in_box(Point point,cv::Rect tar_box)
 {
     int x=point.getX();
     int y=point.getY();
     if(((tar_box.x<=x)&&(x<=tar_box.x+tar_box.width)) &&((tar_box.y<=y)&&(y<=tar_box.y+tar_box.height)))

         return true;
     return false;

 }


/***
 *
 *  '''
    :param points:keypoints sequence
    :return:0:single click;1:double click;-1:no click
    '''
 */

int diff_click(std::vector<Point>& cli_seq)
{

        if (cli_seq.size()<1)
        {
            return -1;
        }
        std::vector<int> dy_seq;

        int length=cli_seq.size();
        for(int i=0;i<length;i++)
        {
            if (is_tip_in_box(cli_seq[i],tar_box))
            {
                dy_seq.push_back(1);
            } else
            {
                dy_seq.push_back(0);
            }
        }
//        for (int i = 0; i < length; i++) {
////            if((dy_seq[i]!=dy_seq[i+1])&&(dy_seq[i+1]!=dy_seq[i+2]))
////            {
////                dy_seq[i+1]=dy_seq[i];
////            }
//            std::cout<<dy_seq[i];
//        }

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

                if (min_cnt>=1)
                {
                    if (lst_dy==0)
                    {
                        str+="0";
                    }
                    else
                    {
                        str+="1";
                    }
                }
                lst_dy=cur_dy;
                min_cnt=1;
            }

            if(i==length-1)
            {
                if (min_cnt>=5&&lst_dy==0)
                {
                    str+="0";
                }
                if (min_cnt>1&&lst_dy==1)
                {
                    str+="1";
                }

            }

        }

//
//        std::cout<<std::endl<<"str="<<str<<std::endl;
        if (str=="10"||str=="010")
        {
//            std::cout<<"single click!!!"<<std::endl;
            return 0;
        }

        if (str=="1010"||str=="01010")
        {
//            std::cout<<"double click!!!"<<std::endl;
            return 1;
        }

        return -1;
}


int get_sto_tim(std::vector<Point>  sequence, cv::Rect box) {
    int time = 0;
    int point_num = sequence.size();
    for (int i=0;i<point_num;i++) {
        if (is_tip_in_box(sequence[i], box)) {
            time += 1;
        }
    }
    return time;
}


cv::Rect get_but_pos(std::vector<Point> sequence,cv::Rect boxes[])
{
    cv::Rect cur_box=cv::Rect(-1,-1,-1,-1);

    for (int i = 0; i < BUTTON_NUMBER; i++) {
        int sto_tim = get_sto_tim(sequence, boxes[i]);
        if (sto_tim>1)
        {
            cur_box = boxes[i];
        }
    }

    return cur_box;
}



//判断是否找到全剧变量tarbox,也就是是否有按钮被点击
bool is_tar_box_null(cv::Rect box)
{
    if (box.width==-1)
    {
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////////

//调用部分：
std::vector<Point> rightview_finger_series;

int main()
{


    if(is_tar_box_null(tar_box))
    {
        tar_box = get_but_pos(rightview_finger_series, rightview_click_area);
        if (!is_tar_box_null(tar_box))
        {
            int length=rightview_finger_series.size();
            if(length>6)
            {
                length-=6;
                std::vector<Point>::iterator it=rightview_finger_series.begin();
                while (it!=rightview_finger_series.end()-6)
                {
                    it=rightview_finger_series.erase(it);
                }
            }
        }

        if (!is_tar_box_null(tar_box))
        {
            int click_result=diff_click(rightview_finger_series);
            if(click_result==0)
            {
                std::cout<<'single click'<<endl;
                tar_box =cv::Rect(-1,-1,-1,-1);
                rightview_finger_series.clear();
            }
            if (click_result == 1)
            {
                std::cout<<'double click'<<endl;
                rightview_finger_series.clear();
                tar_box =cv::Rect(-1,-1,-1,-1);
            }

        }
    }


    return 0;
}

