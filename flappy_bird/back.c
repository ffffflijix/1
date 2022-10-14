#include "head.h"

//屏幕文件的文件描述符
int Fd_Lcd = -1;

//映射区域的地址
//*Plcd = NULL ;

//屏幕文件的路径
#define LCD_PATH "/dev/fb0"

//屏幕文件的大小
#define LCD_WIDTH 800
#define LCD_HEIGHT 480

/*
	BMP_dianzhen:将bmp的各像素点读取到对应的文件中
*/
void BMP_dianzhen(const char *bmpname, BMP *bmp)
{
	// 1.打开BMP文件
	int fd_bmp = open(bmpname, O_RDONLY);

	// 2.判断bmpname是不是BMP文件
	unsigned char buf[2] = {0};
	int ret = read(fd_bmp, buf, 2);

	if (buf[0] != 0x42 || buf[1] != 0x4D)
	{
		printf("%s is not bmp file,bye bye!\n", bmpname);
		close(fd_bmp);
		return;
	}

	// 3.读取当前bmp文件的宽度
	//将光标偏移至宽度数据之前
	lseek(fd_bmp, 0x12, SEEK_SET);
	int width;
	ret = read(fd_bmp, &width, 4);

	// 4.读取当前bmp文件的高度
	//将光标偏移至高度数据之前
	lseek(fd_bmp, 0x16, SEEK_SET);
	int height;
	ret = read(fd_bmp, &height, 4);

	// 5.读取当前bmp文件的色深
	//将光标偏移至色深数据之前
	lseek(fd_bmp, 0x1C, SEEK_SET);
	short depth;
	read(fd_bmp, &depth, 2);

	if (ret == -1)
	{
		perror("读取BMP文件色深失败");
		return;
	}

	printf("%s is bmp file : width %d height %d depth %d\n", bmpname, width, height, depth);

	//求出BMP文件的像素数组一行的大小
	int hangsize = width * (depth / 8) + (4 - (width * depth / 8) % 4);

	int laizi = 0;
	//求出赖子的个数
	if ((width * depth / 8) % 4)
	{
		laizi = 4 - (width * depth / 8) % 4;
	}
	//求出用来保存像素数组的空间一行的字节数
	int xiangwidth = width * depth / 8;

	//求出用来保存像素数组的空间大小
	int zong = xiangwidth * height;
	//把空间开辟出来
	unsigned char *a = malloc(zong);

	//将像素数组读取到开辟出来的空间中去
	lseek(fd_bmp, 54, SEEK_SET);
	int i, j, k = 0, l = 0;
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			read(fd_bmp, &a[l], 3);
			l = l + 3;
			k = k + 3;
			if (depth == 32)
			{
				read(fd_bmp, &a[l], 1);
				l = l + 1;
				k = k + 1;
			}
			if (k == xiangwidth)
			{
				lseek(fd_bmp, laizi, SEEK_CUR);
				k = 0;
			}
		}
	}

	// if depth == 24 每次从空间中取三/四个字节整合形成一个像素点的颜色值
	int bb[800 * 480 * 2] = {0};
	for (i = 0; i < width * height; i++)
	{
		int flag = 0;
		int touming = 0;
		if (depth == 32)
		{
			flag = 1;
			touming = a[(3 + flag) * i + 3] * 0x1000000;
		}
		bb[i] = a[(3 + flag) * i + 2] * 0x10000 + a[(3 + flag) * i + 1] * 0x100 + a[(3 + flag) * i] + touming;
	}

	bmp->BMPHI = height; //将宽与长的信息装到对应bmp文件的结构体中
	bmp->BMPWI = width;
	k = 0;
	for (i = height - 1; i >= 0; i--) //将各像素点信息装入bmp图片结构体
	{
		for (j = 0; j < width; j++)
		{
			bmp->color[i][j] = bb[k++];
		}
	}

	close(fd_bmp);
}

/*
	Lcd_Init:对LCD屏进行初始化
*/
int Lcd_Init(void)
{
	//打开屏幕文件
	Fd_Lcd = open(LCD_PATH, O_RDWR);
	if (Fd_Lcd == -1)
	{
		perror("打开屏幕文件失败");
		return -1;
	}

	//将屏幕文件映射到内存中去
	Plcd = mmap(NULL, LCD_WIDTH * LCD_HEIGHT * 4, PROT_READ | PROT_WRITE, MAP_SHARED, Fd_Lcd, 0);
	if (Plcd == MAP_FAILED)
	{
		perror("映射屏幕文件失败");
		return -1;
	}
	return 0;
}

/*
	将bmp文件在屏幕上打印出来
	x0，y0：打印的起点坐标
*/
void Draw_bmp(int x0, int y0, BMP *bmp)
{
	int i, j;
	for (i = 0; i < bmp->BMPHI; i++)
	{
		for (j = 0; j < bmp->BMPWI; j++)
		{
			*(Plcd + 800 * ((i + y0) % 480) + (j + x0)) = bmp->color[i][j];
		}
	}
}

/*
	将鸟在屏幕上打印出来，除了底色
	x0，y0：打印的起点坐标
*/
void Draw_bmpbird(int x0, int y0, BMP *bmp)
{
	int i, j;
	for (i = 0; i < bmp->BMPHI; i++)
	{
		for (j = 0; j < bmp->BMPWI; j++)
		{
			if (bmp->color[i][j] != 0x99D9EA)
			{
				*(Plcd + 800 * ((i + y0) % 480) + (j + x0)) = bmp->color[i][j];
			}
		}
	}
}

/*
	将( x0 , y0 )到（ x0 + width ， y0 + height ）处的背景打印
*/
void Draw_bird(int x0, int y0, int width, int height, BMP *bmp)
{
	int i, j;
	for (i = y0 + 0; i < height + y0; i++)
	{
		for (j = x0 + 0; j < width + x0; j++)
		{
			*(Plcd + 800 * (i % 480) + (j % 800)) = bmp->color[i % 480][j % 800];
		}
	}
}

/*
	将( x0 , y0 )到（ x0 + width ， y0 + height ）处颜色不为底色的背景打印
*/
void Draw_birdback(int x0, int y0, int width, int height, BMP *bmp)
{
	int i, j;
	for (i = y0 + 0; i < height + y0; i++)
	{
		for (j = x0 + 0; j < width + x0; j++)
		{
			if (bmp->color[i - y0][j - x0] != 0x99D9EA)
			{
				*(Plcd + 800 * (i % 480) + (j % 800)) = P->color[i % 480][j % 800];
			}
		}
	}
}

/*
	回收资源
*/
void Recover_Source(void)
{
	close(Fd_Lcd);
	munmap(Plcd, LCD_WIDTH * LCD_HEIGHT * 4);
}
