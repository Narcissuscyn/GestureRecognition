//
// Created by narcissus on 18-5-22.
//

#ifndef GESTURE_RECOGNITION_POINT_H
#define GESTURE_RECOGNITION_POINT_H

#include <iostream>
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

#endif //GESTURE_RECOGNITION_POINT_H
