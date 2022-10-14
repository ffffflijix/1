#ifndef __HEAD_H__
#define __HEAD_H__

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/mman.h>
#include <time.h>
#include <linux/input.h>
#include <pthread.h>

#define kuan 480
#define gao 800

typedef struct point
{
	int x; //用于在触摸时获取触摸的坐标
	int y; // x为横向坐标，y为纵向坐标
} Point;

Point p; //定义坐标

typedef struct bmp
{
	int BMPHI;			 //用于图片的结构体
	int BMPWI;			 // BIRDHI是图片xxx.bmp的高
						 // BIRDWI是图片xxx.bmp的宽
	int color[480][800]; // color是xxx.bmp的像素颜色
} BMP;

BMP flappy; // flappy
BMP flappy1;
BMP back; //背景
BMP back1;
BMP kaishi;
BMP end;
BMP nandu;
BMP gougou;
BMP *P;
typedef struct guan
{
	int GUANHI; //管子的高度，固定为330
	int GUANWI; //管子的宽度，有32，48，80三种宽度，随机选择
	int GUAN_I; //管子的起始点，各管子间x相距150 , y在250-380间随机
	int GUAN_J;
	/*
		color:管子的像素点，是否显示颜色
		管子的显示，在i，j处显示高为350，宽为32/48/80的绿色(0x44FF2A)管子
	*/
} GUAN;

GUAN guan[4]; //管子固定为5个，当某管子超出屏幕后，管子的x坐标与宽度重新随机
			  //并重新从屏幕右端输出
int *Plcd;
int loac;
int live;
int score;
int sudu;
#endif
