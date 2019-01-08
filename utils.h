//
// Created by narcissus on 18-5-19.
//

#ifndef GESTURE_RECOGNITION_UTILS_H
#define GESTURE_RECOGNITION_UTILS_H

#include<iostream>
#include<string>
#include<dirent.h>

int get_file_count(std::string dirname)
{
    int count=0;
    DIR *dp;
    struct dirent *dirp;

    if((dp = opendir(dirname.c_str())) == NULL)
        std::cout << "Can't open " << dirname << std::endl;

    while((dirp = readdir(dp)) != NULL) {
        if (dirp->d_type == 8)  // 只输出文件名，不输出目录名
        {
            count++;
//            std::cout << dirp->d_name << std::endl;
        }
    }
    closedir(dp);
    return count;
}
#endif //GESTURE_RECOGNITION_UTILS_H

