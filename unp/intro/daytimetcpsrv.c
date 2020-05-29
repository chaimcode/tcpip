#include "../unp.h"
#include <time.h>

int main(int argc, char **argv)
{
    int listenfd, connfd;
    struct sockaddr_in servaddr;
    char buff[MAXLINE];
    time_t ticks;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    // INADDR_ANY -> 任意的 ip 地址
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(13); /* daytime server */

    // 绑定到指定的套接字
    Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));

    // 监听指定的套接字, LISTENQ -> 最大连接数
    Listen(listenfd, LISTENQ);

    for (;;)
    {
        connfd = Accept(listenfd, (SA *)NULL, NULL);

        // utc 的时间戳
        ticks = time(NULL);
        // 时间戳 -> 可读时间格式
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        Write(connfd, buff, strlen(buff));

        Close(connfd);
    }
}
