#include "head.h"

#define PATHNAME "/mnt/hgfs/zong/c/flappy_bird/ioio2"

/*
    lstos:向服务器发送ls命令，获取服务器指定目录下的文件名
        @buf：      输入的命令
        @sockfd：   数据报套接字
        @saddr：    服务器的网络地址
        @len:       地址结构体长度
*/
int lstos(char *buf, int sockfd, struct sockaddr_in saddr, socklen_t len); // ls的结果给客户端

/*
    getfile:向服务器发送get命令，获取指定filename的文件
        @buf：      输入的命令，包含filename
        @sockfd：   数据报套接字
        @saddr：    服务器的网络地址
        @len:       地址结构体长度
*/
int getfile(char *buf, int sockfd, struct sockaddr_in saddr, socklen_t len);

/*
    putfile:向服务器发送put命令，发送指定filename的文件
        @buf：      输入的命令，包含filename
        @sockfd：   数据报套接字
        @saddr：    服务器的网络地址
        @len:       地址结构体长度
*/
int putfile(char *buf, int sockfd, struct sockaddr_in saddr, socklen_t len);

/*
    reply：组成数据包，并向服务器发送指定内容数据包，数据包由包头+数据包长度+命令代码+命令长度+命令内容+包尾组成
        @buf：      输入的命令，包含filename
        @sockfd：   数据报套接字
        @saddr：    服务器的网络地址
        @len：      地址结构体长度
        @cmd：      命令代号（1.ls  2.get <filename>  3.put <filename>  4.bye
*/
int reply(char *buf, int sockfd, struct sockaddr_in saddr, socklen_t len, int cmd);

/*
    @argv[1]:服务器IP地址
    @argv[2]:服务器端口
*/
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "%s <SERVER_IP> <SERVER_PORT>!\n", argv[0]);
        return 0;
    }

    // 1.创建一个数据报套接字
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        perror("创建普通套接字失败");
        return -1;
    }

    // 2.保存服务器的网络地址
    struct sockaddr_in saddr;                   //服务器的网络地址结构体实例化
    socklen_t len = sizeof(saddr);              //网络地址长度
    saddr.sin_family = AF_INET;                 //协议
    saddr.sin_port = htons(atoi(argv[2]));      //端口
    saddr.sin_addr.s_addr = inet_addr(argv[1]); // IP地址

    // 3.与客户端通信
    while (1)
    {
        char buf[1024] = {0};                       //用来装客户端输入的命令+filename

        fgets(buf, sizeof(buf), stdin);

        buf[strlen(buf) - 1] = '\0';                //将fgets获取的'\n'去除

        if (strncmp("ls", buf, 2) == 0)             //发送ls指令
        {
            lstos(buf, sockfd, saddr, len);
        }
        else if (strncmp("get", buf, 3) == 0)       // get
        {
            getfile(buf, sockfd, saddr, len);
        }
        else if (strncmp("put", buf, 3) == 0)       // put
        {
            putfile(buf, sockfd, saddr, len);
        }
        else if (strncmp("bye", buf, 3) == 0)       // bye
        {
            reply("bye", sockfd, saddr, len, 4);
            printf("Recv From [ IP : %s ] [ PORT : %d ] Data : 连接已断开\n", inet_ntoa(saddr.sin_addr), ntohs(saddr.sin_port));
            return -2;
        }
        else
        {
            printf("无此命令\n");
        }
    }
}


/*
    reply：组成数据包，并向服务器发送指定内容数据包，数据包由包头+数据包长度+命令代码+命令长度+命令内容+包尾组成
        @buf：      输入的命令，包含filename
        @sockfd：   数据报套接字
        @saddr：    服务器的网络地址
        @len：      地址结构体长度
        @cmd：      命令代号（1.ls  2.get <filename>  3.put <filename>  4.bye
*/
int reply(char *buf, int sockfd, struct sockaddr_in saddr, socklen_t len, int cmd)
{
    unsigned char resp[1024] = {0};

    unsigned int pkglen = 4 + 1 + 4 + strlen(buf);
    unsigned int resplen = strlen(buf);

    int i = 0;

    resp[i++] = 0xC0;                       //包头

    resp[i++] = pkglen & 0xFF;              //响应包的长度->小端模式存储
    resp[i++] = (pkglen >> 8) & 0xFF;
    resp[i++] = (pkglen >> 16) & 0xFF;
    resp[i++] = (pkglen >> 24) & 0xFF;

    resp[i++] = (unsigned char)cmd;

    resp[i++] = resplen & 0xFF;             //回复内容的长度->小端模式存储
    resp[i++] = (resplen >> 8) & 0xFF;
    resp[i++] = (resplen >> 16) & 0xFF;
    resp[i++] = (resplen >> 24) & 0xFF;

    strncpy(resp + i, buf, resplen);        //具体数据

    i = i + resplen;

    resp[i++] = 0xC0;                       //包尾

    int ret = sendto(sockfd, resp, sizeof(resp) - 1, 0, (struct sockaddr *)&saddr, len);
    if( ret < 0 )
    {
        perror("发送数据包失败");
        return -1;
    }
}

/*
    lstos:向服务器发送ls命令，获取服务器指定目录下的文件名
        @buf：      输入的命令
        @sockfd：   数据报套接字
        @saddr：    服务器的网络地址
        @len:       地址结构体长度
*/
int lstos(char *buf, int sockfd, struct sockaddr_in saddr, socklen_t len)
{
    printf("Send to [ IP : %s ] [ PORT : %d ] Data : ls\n", inet_ntoa(saddr.sin_addr), ntohs(saddr.sin_port));
    
    reply(buf, sockfd, saddr, len, 1);      //先向服务器发送命令，使服务器进入相对应的状态

    char getre[1024] = {0};                 //用来接收服务器的回应
    int ret = recvfrom(sockfd, getre, sizeof(getre) - 1, 0, NULL, NULL);
    if( ret == -1 )
    {
        perror("接收服务器回应失败");
        return -1;
    }
    char answer[1024] = {0};                //用来解析回应包
    strncpy(answer, getre + 9, *(int *)(getre + 5));
    answer[*(int *)(getre + 5)] = '\0';

    if (strcmp(answer, "readyls") != 0)     //服务器回应并非已经进入对应程序
    {
        printf("服务器回应错误：%s\n", answer);
        return -1;
    }

    ret = recvfrom(sockfd, getre, sizeof(getre) - 1, 0, NULL, NULL);
    if (ret == -1)
    {
        printf("接受文件夹内容失败！\n");
    }
    strncpy(answer, getre + 9, *(int *)(getre + 5));
    answer[*(int *)(getre + 5)] = '\0';
    printf("%s\n", answer);                 //文件夹里的文件名
    printf("接受完成\n");
    return 0;
}


/*
    getfile:向服务器发送get命令，获取指定filename的文件
        @buf：      输入的命令，包含filename
        @sockfd：   数据报套接字
        @saddr：    服务器的网络地址
        @len:       地址结构体长度
*/
int getfile(char *buf, int sockfd, struct sockaddr_in saddr, socklen_t len)
{
    reply(buf, sockfd, saddr, len, 2);

    char getre[1024] = {0};                 //用来接收服务器的回应
    int ret = recvfrom(sockfd, getre, sizeof(getre) - 1, 0, NULL, NULL);
    if( ret == -1 )
    {
        perror("接收服务器回应失败");
        return -1;
    }
    char answer[1024] = {0};                //用来解析回应包
    strncpy(answer, getre + 9, *(int *)(getre + 5));
    answer[*(int *)(getre + 5)] = '\0';

    if (strcmp(answer, "readyget") != 0)     //服务器回应并非已经进入对应程序
    {
        printf("服务器回应错误：%s\n", answer);
        return -1;
    }

    char path[1024] = {0};                  //文件路径
    char filename[100] = {0};               //文件名

    strncpy(filename, buf + 4, strlen(buf) - 4);
    sprintf(path, "%s/%s", PATHNAME, filename);
    printf("Send to [ IP : %s ] [ PORT : %d ] Data : get %s\n", inet_ntoa(saddr.sin_addr), ntohs(saddr.sin_port), filename);
    
    int fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0777);
    if (fd < 0)
    {
        perror("打开文件失败");
        return -1;
    }
    while (1)                               //重复获取内容并写入文件
    {
        char filebuf[1024] = {0};
        int ret = recvfrom(sockfd, filebuf, sizeof(filebuf) - 1, 0, NULL, NULL);
        if (ret < 0)
        {
            perror("读取文件失败");
            return -1;
        }
        write(fd, (void *)filebuf, ret);
        if (ret < 1023)
        {
            break;
        }
        sendto(sockfd, "OK", 2, 0, (struct sockaddr *)&saddr, len);     //表示本次已接收
    }
    printf("接收完成\n");
    close(fd);
    return 0;
}


/*
    putfile:向服务器发送put命令，发送指定filename的文件
        @buf：      输入的命令，包含filename
        @sockfd：   数据报套接字
        @saddr：    服务器的网络地址
        @len:       地址结构体长度
*/
int putfile(char *buf, int sockfd, struct sockaddr_in saddr, socklen_t len)
{
    reply(buf, sockfd, saddr, len, 3);

    char getre[1024] = {0};                     //用来接收服务器的回应
    int ret = recvfrom(sockfd, getre, sizeof(getre) - 1, 0, NULL, NULL);
    if( ret == -1 )
    {
        perror("接收服务器回应失败");
        return -1;
    }
    char answer[1024] = {0};                    //用来解析回应包
    strncpy(answer, getre + 9, *(int *)(getre + 5));
    answer[*(int *)(getre + 5)] = '\0';

    if (strcmp(answer, "readyput") != 0)        //服务器回应并非已经进入对应程序
    {
        printf("服务器回应错误：%s\n", answer);
        return -1;
    }

    char path[1024] = {0};                      //文件路径
    char filename[100] = {0};                   //文件名
    strncpy(filename, buf + 4, strlen(buf) - 4);
    sprintf(path, "%s/%s", PATHNAME, filename);

    int fd = open(path, O_RDONLY);
    printf("Send to [ IP : %s ] [ PORT : %d ] Data : put %s\n", inet_ntoa(saddr.sin_addr), ntohs(saddr.sin_port), filename);
    
    while (1)                                   //重复发送文件内容
    {
        char filebuf[1024] = {0};
        int ret = read(fd, filebuf, 1023);
        sendto(sockfd, filebuf, ret, 0, (struct sockaddr *)&saddr, len);
        if (ret < 1023)
        {
            break;
        }
        recvfrom(sockfd, filebuf, 2, 0, NULL, NULL);    //向服务器确定本次发送被接收
    }
    printf("发送完成\n");
    close(fd);
    return 0;
}
