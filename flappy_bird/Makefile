#目标文件
TARGET := main

#指定编译器
CC := arm-linux-gcc

#CSRCS指定工程中所有的.c源文件列表
CSRCS := $(wildcard *.c)

#OBJS指定的是工程中所有的.o文件列表
OBJS := $(patsubst %.c,%.o,$(CSRCS))

#INCS指定头文件的目录
INCS += -I ./ -I /home/china/Tools/arm-jpeg/include -I ./inc

#LIBS指定所需要用到的库的搜索路径
LIBS += -L ./ -L /home/china/Tools/arm-jpeg/lib -l jpeg

$(TARGET):$(OBJS)
	$(CC) $^ $(LIBS) -o $@ -pthread

%.o:%.c
	$(CC) -c $< $(INCS) -o $@ -pthread

#清除：所有的中间文件和最终的可执行文件
clean:
	rm -rf $(OBJS)
	rm -rf $(TARGET)


















