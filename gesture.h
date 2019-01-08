//
// Created by narcissus on 18-5-19.
//

#ifndef GESTURE_RECOGNITION_GESTURE_H
#define GESTURE_RECOGNITION_GESTURE_H
#include <iostream>
#include <fstream>
#include <string>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <jsoncpp/json.h>
#define MIN_SCORE 0.1

//when drag a button,one's finger has to "stop" on the button in a continuos time.
// should be larger than the "stop time"of  one click operation or double click operation
#define MIN_DRAG_STOP_TIME 12
//the minimun "operation time" of   double click operation
#define MIN_DOUB_CLIC_STOP_TIME 15

//the minimun "stop time" of  one click operation
#define MIN_CLIC_STOP_TIME 3


int no_scale_count=0;
std::vector<int> scale_dis;
bool judge_scale=false;
int judge_scale_later = 0;
int scale_dis_least = 10000;
bool judge_scale_up = false;
bool judge_scale_down = false;
int scale_dis_most = 0;


namespace op

{

    class Point//点类
    {
    public:
        /**
         * the constructor
         * @param the x coordinate
         * @param the y coordinate
         */
        Point(double a = 0, double b = 0):x(a), y(b){}

        /***
         *
         * @return the x coordinate of a point
         */
        double getX();//得到x坐标
        /***
          *
          * @return the y coordinate of a point
          */
        double getY();//得到y坐标

        //重载<<实现点的坐标的输出
        friend std::ostream& operator<<(std::ostream &output, Point &p);

    protected:
        double x;//x坐标
        double y;//y坐标
    };
    class Gesture{

    public:

        std::vector<Point> get_cur_key(std::string point_file,bool hand_flag,bool is_click);


        cv::Rect get_but_pos(std::vector<Point> & sequence);
/***
 *
 * @param hand_flag 0:left hand; 1:right hand
 * @param is_click click or scale flag
 *@param sequence :finger tip coordinate sequence
 * @param key_point_dir :directory that get the hand keypoints files
 * @param cur_img_idx  the current frame
 * @return hand keypoint sequence
 */
        std::vector<Point>* get_key_seq(int hand_flag, bool is_click,std::string key_point_dir,std::vector<Point> sequence[]);
//        std::vector<Point> * get_sca_seq(std::string key_point_dir,std::vector<Point>* sequence,int cur_img_idx=200);
        int get_sto_tim(std::vector<Point> & sequence,cv::Rect  button);
        /***
         *
         * @param tar_box
         * @param point finger tip coordinate
         * @return if the keypont is in the box,return true;else return false
         */
        bool is_tip_on_but(Point point,cv::Rect tar_box);

        /***
         *
         * @param points
         * @return
         */
        int diff_click(std::vector<Point> & points);

        int judge_fingers(std::string point_file,int hand_flag);//score是当前帧大拇指的评分, 小于80可以直接认为是单指

        int JudgeScale(std::vector<Point> fingertip[]);


        bool get_is_dst_btn()
        {
            return is_dst_btn;
        }

        cv::Rect get_cur_but(){
            return cur_but;
        }

        void set_cur_but(cv::Rect button)
        {
            cur_but=button;
        }

        void set_is_dst_but(bool t)
        {
            is_dst_btn= t;
        }
    private:
        std::string img_dir;
        cv::Rect cur_but;
        bool is_dst_btn;
    };

}

namespace op
{
    std::vector<Point> Gesture::get_cur_key(std::string point_file,bool hand_flag,bool is_click) {

        std::vector<Point> sequence;
        std::ifstream in_file(point_file.c_str(),std::ios::in);

        if (!in_file)
        {
            std::cout<<"file"<<point_file<<" is not exist!"<<std::endl;
        } else
        {
            // 解析json用Json::Reader
            Json::Reader *readerinfo = new Json::Reader(Json::Features::strictMode());
//            Json::Reader reader;
            Json::Value root;
            readerinfo->parse(in_file,root);
            Json::Value hand_keys;
            if(hand_flag==0)
            {
                hand_keys=root["people"][0U]["hand_left_keypoints_2d"];

            } else
            {
                hand_keys= root["people"][0U]["hand_right_keypoints_2d"];
            }

//                std::cout<<hand_keys[26].asDouble()<<std::endl;
            if (is_click)
            {

                if (hand_keys[26].asDouble()>MIN_SCORE)// the score of a keypoint should be larger than MIN_SCORE
                {
                    sequence.push_back(Point(hand_keys[24].asInt(),hand_keys[25].asInt()));//get the forefinger tip

                }
            }
            else
            {
//                std::cout<<hand_keys[26].asDouble()<<" "<<hand_keys[14].asDouble()<<std::endl;
                if (hand_keys[26].asDouble()>MIN_SCORE&&hand_keys[14].asDouble()>MIN_SCORE)// the score of a keypoint should be larger than MIN_SCORE
                {
//                    std::cout<<sequence.back()<<std::endl;
                    sequence.push_back(Point(hand_keys[12].asInt(),hand_keys[15].asInt()));//get the thumb tip
                    sequence.push_back(Point(hand_keys[24].asInt(),hand_keys[25].asInt()));//get the forefinger tip
                }
            }
        }


        in_file.close();
        return sequence;
    }

    cv::Rect Gesture::get_but_pos(std::vector<Point> & sequence) {
        int button_num=11;
        cv::Rect boxes[button_num];
        boxes[0]= cv::Rect(158,317,38,23);//0
        boxes[1]= cv::Rect(196,320,39,22);//7
        boxes[2]= cv::Rect(235,323,40,23);//4
        boxes[3]= cv::Rect(275,323,40,23);//1
        boxes[4]= cv::Rect(142,340,44,23);//确定
        boxes[5]= cv::Rect(187,343,41,22);//8
        boxes[6]= cv::Rect(229,346,41,22);//5
        boxes[7]= cv::Rect(270,346,46,22);//2
        boxes[8]= cv::Rect(133,363,46,25);//取消
        boxes[9]= cv::Rect(180,365,43,25);//9
        boxes[10]= cv::Rect(224,368,42,25);//6
        boxes[11]= cv::Rect(268,368,46,25);//3


        for (int i = 0; i < button_num; ++i) {

            int sto_tim= get_sto_tim(sequence,boxes[i]);

            if (sto_tim>=MIN_CLIC_STOP_TIME)
            {
                set_is_dst_but(true);
                set_cur_but(boxes[i]);
//                std::cout<<"get the destination button:"<<boxes[i]<<std::endl;

            }
        }

        return get_cur_but();
    }

    std::vector<Point>* Gesture::get_key_seq(int hand_flag, bool is_click,std::string point_file, std::vector<Point> sequence[]) {

        std::vector<Point> temp=get_cur_key(point_file,hand_flag,is_click);
        if (temp.size()>0){

            if (is_click){
                sequence[1].push_back(temp[0]);
            } else
            {
                sequence[0].push_back(temp[0]);
                sequence[1].push_back(temp[1]);
            }
        }
        return sequence;
    }




//    std::vector<Point> * Gesture::get_sca_seq(std::string key_point_dir, std::vector<op::Point> *sequence,
//                                              int cur_img_idx)
//    {
//
//    }


    int Gesture::get_sto_tim(std::vector<Point> & sequence,cv::Rect  button)
    {

//        std::cout<<button<<std::endl;

        int time=0;
        int point_num=sequence.size();

//        std::cout<<"sequence.size()="<<sequence.size()<<std::endl;
        for (int i=point_num-1;i>=0;i--)
        {

            if(is_tip_on_but(sequence[i],button))
            {
                time++;
            }
        }

        return time;
    }
    bool Gesture::is_tip_on_but(Point point,cv::Rect tar_box) {

        int x=point.getX();
        int y=point.getY();
        if(((tar_box.x<=x)&&(x<=tar_box.x+tar_box.width)) &&((tar_box.y<=y)&&(y<=tar_box.y+tar_box.height)))

            return true;
        return false;
    }

    int Gesture::diff_click(std::vector<Point>& cli_seq) {

        if (cli_seq.size()<1)
        {
            return -1;
        }
        std::vector<int> dy_seq;

        int length=cli_seq.size();
        for(int i=0;i<length;i++)
        {
            if (is_tip_on_but(cli_seq[i],get_cur_but()))
            {
                dy_seq.push_back(1);
            } else
            {
                dy_seq.push_back(0);
            }
        }
        for (int i = 0; i < length; i++) {
//            if((dy_seq[i]!=dy_seq[i+1])&&(dy_seq[i+1]!=dy_seq[i+2]))
//            {
//                dy_seq[i+1]=dy_seq[i];
//            }
            std::cout<<dy_seq[i];
        }

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
                if (min_cnt>=1&&lst_dy==0)
                {
                    str+="0";
                }
                if (min_cnt>=1&&lst_dy==1)
                {
                    str+="1";
                }

            }

        }


        std::cout<<std::endl<<"str="<<str<<std::endl;
        if (str=="10"||str=="010")
        {
//            std::cout<<"single click!!!"<<std::endl;
            return 0;
        }

        if (str=="1010"||str=="0101")
        {
//            std::cout<<"double click!!!"<<std::endl;
            return 1;
        }

        return -1;
    }

    double Point::getX()
    {
        return x;
    }


    double Point::getY()
    {
        return y;
    }
/***
 * //override the operator"<<",in order to output the point coordinate of p:
 * @param output the ostream
 * @param p a point with coordinate(x,y)
 * @return the ostream
 */
    std::ostream& operator<<(std::ostream &output, Point &p)
    {
        output<<"("<<p.x<<","<<p.y<<")"<<std::endl;

        return output;
    }



//返回1是单指，返回2是双指
    int Gesture::judge_fingers(std::string point_file,int hand_flag)//score是当前帧大拇指的评分, 小于80可以直接认为是单指
    {


        std::vector<Point> fingertip[2];
        std::vector<Point> temp=get_cur_key(point_file,hand_flag, false);
//        std::cout<<temp[0]<<std::endl;
        if (temp.size()>0)
        {

            fingertip[0].push_back(temp[0]);
            fingertip[1].push_back(temp[1]);

            int len = fingertip[0].size();
            //x0 , y0是大拇指
            double x0 = fingertip[0][len-1].getX();
            double y0 = fingertip[0][len-1].getY();
            //x1, y1是食指
            double x1 = fingertip[1][len-1].getX();
            double y1 = fingertip[1][len-1].getY();

            //根据斜率加距离来判断
            double k = (y1-y0)/(x1-x0);
            double dis = sqrt(pow(x1-x0,2)+pow(y1-y0,2));
//            std::cout<<"point_file:"<<point_file<<" dis:"<<dis<<std::endl;
            if(dis<=30)
                return 2;
            if(dis>30 && k<6&& k>0)
                return 2;
        }
        else
        {
            return  0;
        }



        return 1;

    }



    //返回0是未检测到滑动，返回1是检测到scale_up，返回2是检测到scale_down
    int Gesture::JudgeScale(std::vector<Point> fingertip[])
    {
        if(no_scale_count>=4)
        {
            scale_dis.clear();
            no_scale_count=0;
        }

        if(judge_scale==true)
        {
            judge_scale_later+=1;
            if(judge_scale_later>=12)
            {
                judge_scale=false;
                judge_scale_later=0;
            }
        }

        int len = fingertip[0].size();

        if( 195<=fingertip[0][len-1].getX() && fingertip[0][len-1].getX()<=389 &&
            195<=fingertip[1][len-1].getX() && fingertip[1][len-1].getX()<=389 &&
            214<=fingertip[0][len-1].getY() && fingertip[0][len-1].getY()<=415 &&
            214<=fingertip[1][len-1].getY() && fingertip[1][len-1].getY()<=415 &&
            judge_scale==false)
        {
            int dis = (int)(sqrt(pow(fingertip[0][len-1].getX()-fingertip[1][len-1].getX(),2)+pow(fingertip[0][len-1].getY()-fingertip[1][len-1].getY(),2)));
            scale_dis.push_back(dis);

            int frame_now = scale_dis.size()-1;
            int i=0;

            if(judge_scale_up && dis>=scale_dis_most)
            {
                scale_dis_most=dis;
                scale_dis.clear();
                return 0;
            }

            if(judge_scale_up && dis<scale_dis_most){
                judge_scale=true;
                scale_dis.clear();
                judge_scale_later=0;
                judge_scale_up=false;
                scale_dis_most=0;
                return 0;
            }

            if(judge_scale_down && dis<=scale_dis_least)
            {
                scale_dis_least=dis;
                scale_dis.clear();
                return 0;
            }

            if(judge_scale_down && dis>scale_dis_least)
            {
                judge_scale=true;
                scale_dis.clear();
                judge_scale_later=0;
                judge_scale_down=false;
                scale_dis_least=10000;
                return 0;
            }

            //判断是scale_up
            while(i<scale_dis.size()-1)
            {
                if(scale_dis[frame_now] - scale_dis[i] > 40)
                {
                    if((frame_now - i > 3) && (scale_dis[i]<60))
                    {
                        int j=i;
                        while(j<frame_now)
                        {
                            if(scale_dis[j+1]>=scale_dis[j])
                                j+=1;
                            else
                                break;
                        }

                        if(j==frame_now)
                        {
                            //判断出是一次scale_up
                            judge_scale_up=true;
                            scale_dis_most = scale_dis[frame_now];
                            scale_dis.clear();
                            return 1;
                        }

                    }
                }

                i+=1;
            }

            i=0;
            //判断是scale_down
            while(i<scale_dis.size()-1)
            {
                if(scale_dis[frame_now] - scale_dis[i] < -40)
                {
                    if((frame_now - i > 3) && (scale_dis[frame_now]<60))
                    {
                        int j=i;
                        while(j<frame_now)
                        {
                            if(scale_dis[j+1]<=scale_dis[j])
                                j+=1;
                            else
                                break;
                        }

                        if(j==frame_now)
                        {
                            //判断出是一次scale_down
                            judge_scale_down=true;
                            scale_dis_least = scale_dis[frame_now];
                            scale_dis.clear();
                            return 2;
                        }

                    }
                }

                i+=1;
            }

        }


        else
        {
            no_scale_count+=1;
        }
        return 0;
    }



}


#endif //GESTURE_RECOGNITION_GESTURE_H
