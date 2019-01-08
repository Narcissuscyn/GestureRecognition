#include "gesture.h"
#include "utils.h"
#include <iostream>
#include <string>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace std;
using namespace op;
using namespace cv;


/***
 * according to operation sequences,determine which operation is conducted:
 * one click: get the Lateral camera operation
 *
 * double click:
 */

Gesture gesture;
std::vector<op::Point>  key_seq[2];
int finger_count=0;
int occa_count=0;
int lst_fin_num=0;
bool num_flag= false;
int frame_duration=-1;
int temp_i=0;

void variable_clear()
{
    //////////////////////////////////////////////////////////////////////////////////////////
    //variable clear
    temp_i=0;
    finger_count=0;
    occa_count=0;
    lst_fin_num=0;
    num_flag= false;
    gesture.set_is_dst_but(false);
    key_seq[0].clear();
    key_seq[1].clear();
    frame_duration=-1;

    //////////////////////////////////////////////////////////////////////////////////////////
}

int hand_flag=0;
int  main()
{
    std::string key_point_dir="/home/narcissus/file/dataset/ges_rec/test12/hand_results/";

    std::string key_point_dir_side="/home/narcissus/file/dataset/ges_rec/test12/hand_results1/";

    int file_count=get_file_count(key_point_dir);

    //    cout<<"file_count="<<file_count<<endl;

//    file_count=347;
    int i=0;
   while (i<file_count-1)
    {
        i++;
        if(frame_duration>=0)
        {
            frame_duration++;
        }
        //use the keypoints to judge the gestures
        int idx=i+1;
        std::stringstream ss;
        ss << idx;
        std::string str = ss.str();
        std::string point_file=key_point_dir+"img_"+str+"_l_keypoints.json";

        if (finger_count<5)
        {
            int fin_num=gesture.judge_fingers(point_file,hand_flag);
//            cout<<fin_num<<endl;
            if(fin_num!=0)
            {
                if(!num_flag)
                {
                    lst_fin_num=fin_num;
                    num_flag= true;
                }
//                cout<<"fin_num"<<fin_num<<endl;
                if (fin_num==lst_fin_num)
                {

                    finger_count++;
                    continue;
                }
                else
                {
                    occa_count++;
                    if (occa_count>1)
                    {
                        occa_count=0;
                        finger_count=0;
                        lst_fin_num=fin_num;
                    }
                    continue;
                }
            }
            else
            {
                occa_count=0;
                finger_count=0;
                num_flag= false;
                lst_fin_num=0;
                continue;
            }
        }

        if(num_flag) {
            i=i-5;
            temp_i=i;
            cout <<"frame "<<i<<" get finger number="<< lst_fin_num << endl;
            num_flag= false;
            frame_duration=0;
        }
        if (lst_fin_num!=0)// the hand keypoints are detected
        {

            if (lst_fin_num==1)//one click

            {

                point_file=key_point_dir_side+"img_"+str+"_l_keypoints.json";
                gesture.get_key_seq(hand_flag, true,point_file, key_seq);
                if (!gesture.get_is_dst_btn())
                {
                    gesture.get_but_pos(key_seq[1]);
                    if (gesture.get_is_dst_btn())
                    {
                        i=temp_i;
                        cout<<"frame: "<<i<<" get the destination button:"<<gesture.get_cur_but()<<endl;
                        key_seq[1].clear();
                        continue;
                    }
                }
                else
                {
//                    cout<<"click "<<endl;

                    int sto_tim= gesture.get_sto_tim(key_seq[1],gesture.get_cur_but());

                    if (sto_tim > MIN_DRAG_STOP_TIME) {
                        //Drag and drop
                        std::cout<<"Drag and drop!!!"<<std::endl;
                        i+=50;//拖拽至少50帧之后才会有第二个操作
                        variable_clear();
                    }
                    else
                    {
                        if (frame_duration>25)//单独判断是否为单击；本来20帧就足够了，但是有时候检测不准，所以得得要20帧
//                            if (key_seq[1].size()>MIN_DOUB_CLIC_STOP_TIME)//单独判断是否为单击；本来20帧就足够了，但是有时候检测不准，所以得得要20帧
                            {
//                            cout<<"frame_duration="<<frame_duration<<endl;
                            int flag=gesture.diff_click(key_seq[1]);
                            if(flag==1)//这里加不加都无所谓，暂时判断下
                            {
                                std::cout<<"frame "<<i<<" double click!!!"<<std::endl;
                            } else if(flag==0)
                            {
                                std::cout<<"frame "<<i<<" single click!!!"<<std::endl;
                            }
                            else
                            {
                                std::cout<<"frame "<<i<<" NO click!!!"<<std::endl;

                            }
                            i+=5;//点击至少5帧之后才会有第二个操作
                            variable_clear();

                        }
                    }
                }
            }
            else//scale
            {
                gesture.get_key_seq(hand_flag, false,point_file, key_seq);

                if (key_seq[0].size()>0)
                {

                   int scale_flag= gesture.JudgeScale(key_seq);

                   if(scale_flag==1)
                   {
                        cout<<"frame "<<i<<" scale up"<<endl;

//                       i+=50;//拖拽至少50帧之后才会有第二个操作
                       variable_clear();

                   } else if (scale_flag==2)
                   {
                       cout<<"frame "<<i<<"scale down"<<endl;
//                       i+=50;//拖拽至少50帧之后才会有第二个操作
                       variable_clear();
                   }
                }
            }
        }
        //###############################################################################################################################


    }

    return 0;
}


//#include <iostream>
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/opencv.hpp"
//
//
//using namespace cv;
//int main()
//{
//    Mat srcImage=imread("/home/narcissus/file/recent_job/ges_rec/mycode/test_data/1.png");
//    imshow("原图",srcImage);
//    Mat element=getStructuringElement(MORPH_RECT,Size(15,15));
//    Mat dstImg;
//    erode(srcImage,dstImg,element);
//    imshow("腐蚀后",dstImg);
//    waitKey(0);
//}