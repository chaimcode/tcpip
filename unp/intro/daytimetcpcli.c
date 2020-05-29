#include "../unp.h"

int main(int argc, char **argv)
{
    int sockfd, n;
    char recvline[MAXLINE + 1];
    struct sockaddr_in servaddr;

    if (argc != 2)
        err_quit("usage: daytimetcpcli <IPaddress>");

    // 建立 socket 套接字
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_sys("socket error");

    // 清零 servaddr 结构体
    bzero(&servaddr, sizeof(servaddr));
    // 设置协议为网际协议簇 -> TCP/IP
    servaddr.sin_family = AF_INET;
    // 设置服务监听的端口, daytime_server -> 13
    servaddr.sin_port = htons(13);

    // argv[1] = ip, 将点分十进制 IP 的格式转换为套接字需要格式(二进制)
    // inet_addr 功能与 inet_pton 类似, 不过后者支持 ipv6
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
        err_quit("inet_pton error for %s", argv[1]);

    // 使用套接字建立 tcp 连接, SA -> struct socketaddr
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
        err_sys("connect error");

    // 从套接字中不断地读取数据
    while ((n = read(sockfd, recvline, MAXLINE)) > 0)
    {
        recvline[n] = 0; /* null terminate */
        if (fputs(recvline, stdout) == EOF)
            err_sys("fputs error");
    }
    if (n < 0)
        err_sys("read error");

    exit(0);
}
