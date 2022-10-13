#include "head.h"

#define PATHNAME "/mnt/hgfs/zong/c/flappy_bird/ioio1"

/*
    callback：用来针对命令内容进行回应
        @sockfd：   数据报套接字
*/
int callback(int sockfd);

/*
    lstos:向客户端发送ls命令的结果，发送服务器指定目录下的文件名
        @buf：      接收的请求包，包含filename
        @sockfd：   数据报套接字
        @caddr：    客户端的网络地址
        @len:       地址结构体长度
*/
int lstoc(char *buf, int sockfd, struct sockaddr_in caddr, socklen_t len);

/*
    getfile:向客户端执行get命令，发送指定filename的文件
        @buf：      接收的请求包，包含filename
        @sockfd：   数据报套接字
        @caddr：    客户端的网络地址
        @len:       地址结构体长度
*/
int getfile(char *buf, int sockfd, struct sockaddr_in caddr, socklen_t len);

/*
    putfile:向客户端put命令，接收指定filename的文件
        @buf：      接收的请求包，包含filename
        @sockfd：   数据报套接字
        @caddr：    客户端的网络地址
        @len:       地址结构体长度
*/
int putfile(char *buf, int sockfd, struct sockaddr_in caddr, socklen_t len);

/*
    reply：组成数据包，并向客户端发送指定内容数据包，数据包由包头+数据包长度+回应内容+包尾组成
        @buf：      回应的内容
        @sockfd：   数据报套接字
        @caddr：    客户端的网络地址
        @len：      地址结构体长度
*/
int reply(char *buf, int sockfd, struct sockaddr_in caddr, socklen_t len);

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

    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(atoi(argv[2]));
    saddr.sin_addr.s_addr = inet_addr(argv[1]);
    // 2.为套接字绑定一个地址
    int ret = bind(sockfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if (ret == -1)
    {
        perror("为服务器绑定地址失败");
        close(sockfd);
        return -1;
    }

    // 3.与客户端通信
    while (1)
    {
        struct pollfd fds[5]; //多路复用
        fds[0].fd = sockfd;
        fds[0].events = POLLIN;
        fds[0].revents = 0;

        int ret = poll(fds, sizeof(fds) / sizeof(struct pollfd), 3000);
        if (ret < 0)
        {
            perror("多路复用出错");
            continue;
        }
        else if (ret == 0)
        {
            printf("超时啦!\n");
            continue;
        }
        else if (ret > 0) //有文件描述符就绪啦
        {
            int i;
            for (i = 0; i < sizeof(fds) / sizeof(struct pollfd); i++)
            {
                if (fds[i].revents & POLLIN) //代表fds[i].fd可读啦
                {
                    int ret = callback(sockfd);
                }
            }
        }
    }
}

/*
    callback：用来针对命令内容进行回应
        @sockfd：   数据报套接字
*/
int callback(int sockfd)
{
    struct sockaddr_in caddr;
    socklen_t len = sizeof(caddr);

    unsigned char buf[1024] = {0};

    int ret = recvfrom(sockfd, buf, sizeof(buf) - 1, 0, (struct sockaddr *)&caddr, &len); //接收数据包

    if (ret > 0)
    {
        if (*(buf + 5) == 1) //命令ls
        {
            lstoc(buf, sockfd, caddr, len);
            return 1;
        }
        else if (*(buf + 5) == 2) //命令get <filename>
        {
            getfile(buf, sockfd, caddr, len);
            return 1;
        }
        else if (*(buf + 5) == 3) //命令put <filename>
        {
            putfile(buf, sockfd, caddr, len);
            return 1;
        }
        else if (*(buf + 5) == 4) //命令bye
        {
            printf("Recv From [ IP : %s ] [ PORT : %d ] Data : 连接已断开\n", inet_ntoa(caddr.sin_addr), ntohs(caddr.sin_port));
            return -2;
        }
    }
    else
    {
        printf("接收请求包失败！\n");
        return -1;
    }
}

/*
    reply：组成数据包，并向客户端发送指定内容数据包，数据包由包头+数据包长度+回应内容+包尾组成
        @buf：      回应的内容
        @sockfd：   数据报套接字
        @caddr：    客户端的网络地址
        @len：      地址结构体长度
*/
int reply(char *buf, int sockfd, struct sockaddr_in caddr, socklen_t len)
{
    unsigned char resp[1024] = {0};

    unsigned int pkglen = 4 + 4 + strlen(buf);
    unsigned int resplen = strlen(buf);

    int i = 0;

    resp[i++] = 0xC0; //包头

    resp[i++] = pkglen & 0xFF; //响应包的长度->小端模式存储
    resp[i++] = (pkglen >> 8) & 0xFF;
    resp[i++] = (pkglen >> 16) & 0xFF;
    resp[i++] = (pkglen >> 24) & 0xFF;

    resp[i++] = resplen & 0xFF; //回复内容的长度->小端模式存储
    resp[i++] = (resplen >> 8) & 0xFF;
    resp[i++] = (resplen >> 16) & 0xFF;
    resp[i++] = (resplen >> 24) & 0xFF;

    strncpy(resp + i, buf, resplen); //回复内容

    i = i + resplen;

    resp[i++] = 0xC0; //包尾
    int ret = sendto(sockfd, resp, sizeof(resp) - 1, 0, (struct sockaddr *)&caddr, len);
}

/*
    lstos:向客户端发送ls命令的结果，发送服务器指定目录下的文件名
        @buf：      接收的请求包，包含filename
        @sockfd：   数据报套接字
        @caddr：    客户端的网络地址
        @len:       地址结构体长度
*/
int lstoc(char *buf, int sockfd, struct sockaddr_in caddr, socklen_t len)
{
    printf("Recv From [ IP : %s ] [ PORT : %d ] Data : ls\n", inet_ntoa(caddr.sin_addr), ntohs(caddr.sin_port));
    reply("readyls", sockfd, caddr, len); //向客户端表示自己已经准备好了

    char ls[1000] = {0}; //回复内容

    DIR *dir = opendir(PATHNAME); //读取文件夹
    if (dir == NULL)
    {
        perror("打开目录失败");
        return 0;
    }

    struct dirent *dirp = NULL;
    while (dirp = readdir(dir)) //读取目录项
    {
        if (strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0) //排除文件名“.”，“..”
            sprintf(ls, "%s  %s", ls, dirp->d_name);
    }

    reply(ls, sockfd, caddr, len); //发送ls的内容

    closedir(dir); //关闭目录项
    return 0;
}

/*
    getfile:向客户端执行get命令，发送指定filename的文件
        @buf：      接收的请求包，包含filename
        @sockfd：   数据报套接字
        @caddr：    客户端的网络地址
        @len:       地址结构体长度
*/
int getfile(char *buf, int sockfd, struct sockaddr_in caddr, socklen_t len)
{
    reply("readyget", sockfd, caddr, len); //向客户端表示自己已经准备好了

    char path[1024] = {0};
    char filename[100] = {0};

    strncpy(filename, buf + 14, (int)*(buf + 6) - 4); //解析请求包内容
    sprintf(path, "%s/%s", PATHNAME, filename);
    printf("Recv From [ IP : %s ] [ PORT : %d ] Data : get %s\n", inet_ntoa(caddr.sin_addr), ntohs(caddr.sin_port), filename);

    int fd = open(path, O_RDONLY); //打开文件
    if (fd < 0)
    {
        perror("打开文件失败");
        return -1;
    }

    while (1) //循环发送文件内容
    {
        char filebuf[1024] = {0};
        int ret = read(fd, filebuf, 1023);
        if (ret < 0)
        {
            perror("读取文件失败");
            return -1;
        }

        sendto(sockfd, filebuf, ret, 0, (struct sockaddr *)&caddr, len);
        if (ret < 1023)
        {
            break;
        }

        recvfrom(sockfd, filebuf, 2, 0, (struct sockaddr *)&caddr, &len); //等待本次发送客户端接收完成
    }

    printf("发送完成\n");
    close(fd);
    return 0;
}

/*
    putfile:向客户端put命令，接收指定filename的文件
        @buf：      接收的请求包，包含filename
        @sockfd：   数据报套接字
        @caddr：    客户端的网络地址
        @len:       地址结构体长度
*/
int putfile(char *buf, int sockfd, struct sockaddr_in caddr, socklen_t len)
{
    reply("readyput", sockfd, caddr, len); //向客户端表示自己已经准备好了

    char path[1024] = {0};
    char filename[100] = {0};

    strncpy(filename, buf + 14, (int)*(buf + 6) - 4); //解析请求包
    sprintf(path, "%s/%s", PATHNAME, filename);
    printf("Recv From [ IP : %s ] [ PORT : %d ] Data : put %s\n", inet_ntoa(caddr.sin_addr), ntohs(caddr.sin_port), filename);

    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0777); //打开文件

    while (1) //循环写入
    {
        char filebuf[1024] = {0};
        int ret = recvfrom(sockfd, filebuf, sizeof(filebuf) - 1, 0, (struct sockaddr *)&caddr, &len);

        write(fd, (void *)filebuf, ret);
        if (ret < 1023)
        {
            break;
        }

        sendto(sockfd, "OK", 2, 0, (struct sockaddr *)&caddr, len); //向客户端表示本次接收已完成
    }

    printf("接受完成\n");

    close(fd);
    return 0;
}