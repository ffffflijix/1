#ifndef __GUAN_H__
#define __GUAN_H__

/*
    将四根管子初始化
*/
void guancreat();

/*
    当管子的整个部分从屏幕界面消失后，再次随机管子的宽度与起始纵坐标
*/
void guanch(int i);

/*
    将给定坐标（ x , y ) 处的像素点以color着色
*/
void Lcd_DrawPoint(int x, int y, int color);

/*
    Lcd_DrawWord:在位置为(x0,y0)处显示宽为w,高为h,颜色color的汉字
*/
void Lcd_DrawWord(int x0, int y0, int w, int h, char *data, int color);

/*
    管子的控制线程
*/
void *guanzidong(void *arg);

#endif