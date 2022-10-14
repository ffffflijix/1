#include "head.h"

#define TOUCH_PATH "/dev/input/event0"

void *Get_Touch(void *arg)
{
	//打开触摸屏文件
	int fd_touch = open(TOUCH_PATH, O_RDONLY);
	if (fd_touch == -1)
	{
		perror("打开触摸屏文件失败");
		return NULL;
	}

	struct input_event ev;
	//读取触摸屏文件的内容
	while (1)
	{
		int ret = read(fd_touch, &ev, sizeof(ev));
		if (ret != sizeof(ev))
		{
			continue;
		}

		if (ev.type == EV_ABS && ev.code == ABS_X)
		{
			//意味这是x轴的坐标
			p.x = ev.value;
			printf("X:%d ", p.x * 800 / 1024);
		}
		else if (ev.type == EV_ABS && ev.code == ABS_Y)
		{
			//意味这是y轴的坐标
			p.y = ev.value;
			printf("Y:%d\n", p.y * 480 / 600);
		}
		else if (ev.type == EV_KEY && ev.code == BTN_TOUCH && ev.value == 0)
		{
			//意味着手指离开屏幕
			p.x = -2;
			p.y = -2;
		}
	}
}
