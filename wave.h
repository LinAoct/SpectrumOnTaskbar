#pragma once

#include <fstream>
#include <math.h>
#include <cmath>

using namespace std;

struct Wave
{
	unsigned long file_size;        //文件大小
	unsigned short channel;         //通道数
	unsigned long frequency;        //采样频率
	unsigned long Bps;              //Byte率
	unsigned short sample_num_bit;  //一个样本的位数
	unsigned long data_size;        //数据大小
	double wav_time;				//音频的时间
};