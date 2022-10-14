#include "head.h"
#include "back.h"

unsigned char guanx[3300]; //管子点阵
int guan_color = 0x44FF2A; //管子颜色

/*
    将四根管子初始化
*/
void guancreat()
{
    //管子的点阵（ 330 * 80 ）
    for (int i = 0; i < 3300; i++)
    {
        guanx[i] = 0xFF;
    }

    // m：第一根管子从800的横坐标开始，随后的管子起点依次相差200
    int m = 800;

    //设置随机种子
    srandom(time(NULL));

    //依次随机管子宽度，有 32 ， 56 ， 80 三种
    for (int i = 0; i < 4; i++)
    {
        int flag = random() % 3;
        switch (flag)
        {
            case 0:
                guan[i].GUANWI = 32;
                break;
            case 1:
                guan[i].GUANWI = 56;
                break;
            case 2:
                guan[i].GUANWI = 80;
                break;
        }

        //管子的长度固定为300
        guan[i].GUANHI = 300;

        //管子的起始横坐标
        guan[i].GUAN_I = m;
        m = m - 200;

        //管子的起始纵坐标，当管子超出屏幕时，从上方打印
        guan[i].GUAN_J = 250 + random() % 180;
    }
}

/*
    当管子的整个部分从屏幕界面消失后，再次随机管子的宽度与起始纵坐标
*/
void guanch(int i)
{
    score++;
    srandom(time(NULL));
    int flag = random() % 3;
    switch (flag)
    {
        case 0:
            guan[i].GUANWI = 32;
            break;
        case 1:
            guan[i].GUANWI = 56;
            break;
        case 2:
            guan[i].GUANWI = 80;
            break;
    }
    guan[i].GUAN_J = 200 + random() % 200;
}

/*
    将给定坐标（ x , y ) 处的像素点以color着色
*/
void Lcd_DrawPoint(int x, int y, int color)
{
    if (x >= 0 && x < 800 && y >= 0 && y < 480)
    {
        *(Plcd + 800 * y + x) = color;
    }
}

/*
    Lcd_DrawWord:在位置为(x0,y0)处显示宽为w,高为h,颜色color的汉字
*/
void Lcd_DrawWord(int x0, int y0, int w, int h, char *data, int color)
{
    int i, k;

    for (i = 0; i < w * h / 8; i++)
    {
        for (k = 0; k < 8; k++)
        {
            if ((data[i] << k) & 0x80)
            {
                Lcd_DrawPoint(x0 + (i * 8 + k) % w, (y0 + i / (w / 8)) % 480, color);
            }
        }
    }
}

/*
    管子的控制线程
*/
void *guanzidong(void *arg)
{
    //在开始游戏后延时两秒出现，以免开局来不及控制，可以有效的控制游戏的难度
    usleep(2000000);

    int i = 0;

    //管子的初始化
    guancreat();

    //管子的出现是一个循环
    while (1)
    {
        for (i = 0; i < 4; i++)
        {
            //打印管子
            Lcd_DrawWord(guan[i].GUAN_I, guan[i].GUAN_J, guan[i].GUANWI, guan[i].GUANHI, guanx, guan_color);
        }
        // 0.1秒后将管子所在的地方用背景补充
        usleep(100000);

        for (i = 0; i < 4; i++)
        {
            Draw_bird(guan[i].GUAN_I, guan[i].GUAN_J, guan[i].GUANWI, guan[i].GUANHI, P);
        }

        //管子的横坐标控制，将gaunzidong起来
        for (i = 0; i < 4; i++)
        {
            // sudu是可以调节的参数，通过调节sudu控制nandu，设置了 10 ， 20 ， 50 三种难度
            guan[i].GUAN_I = guan[i].GUAN_I - sudu;
            if (guan[i].GUAN_I + 80 < 0)
            {
                guan[i].GUAN_I = 800;
                guanch(i);
            }
            //当鸟的中心点（ 0 ， j + 35 )处处于管子的内部时，生命live=0，即死
            if ((guan[i].GUAN_I < 80 && guan[i].GUAN_I + guan[i].GUANWI > 20) && (loac + 15 > guan[i].GUAN_J && loac + 15 < 480 || loac - 15 < (guan[i].GUAN_J + guan[i].GUANHI) % 480 && loac >= 0))
            {
                live = 0;
                //将guanzidong线程结束
                return NULL;
            }
        }
    }
}
