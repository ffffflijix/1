#ifndef __BACK_H__
#define __BACK_H__
#include "head.h"

/*
	BMP_dianzhen:将bmp的各像素点读取到对应的文件中
*/
void BMP_dianzhen(const char *bmpname, BMP *bmp);

/*
	Lcd_Init:对LCD屏进行初始化
*/
int Lcd_Init(void);

/*
	将bmp文件在屏幕上打印出来
	x0，y0：打印的起点坐标
*/
void Draw_bmp(int x0, int y0, BMP *bmp);

/*
	将鸟在屏幕上打印出来，除了底色
	x0，y0：打印的起点坐标
*/
void Draw_bmpbird(int x0, int y0, BMP *bmp);

/*
	将( x0 , y0 )到（ x0 + width ， y0 + height ）处的背景打印
*/
void Draw_bird(int x0, int y0, int width, int height, BMP *bmp);

/*
	将( x0 , y0 )到（ x0 + width ， y0 + height ）处颜色不为底色的背景打印
*/
void Draw_birdback(int x0, int y0, int width, int height, BMP *bmp);

/*
	回收资源
*/
void Recover_Source(void);

#endif