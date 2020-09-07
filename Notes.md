1.1 理解网络变编程和套接字
  一.构建接电话套接字
    1.调用socket函数（安装电话机）时进行的对话
      问：“接电话需要准备什么？”
      答：“当然是电话机”
      #include <sys/socket.h>
      int socket(int domain, int type, int protocol);
        --> 成功时返回文件描述符，失败时返回-1。
    2.调用bind函数（分配电话号码）时进行的对话
      问：“请问您的电话号码是多少？”
      答：“我的电话号码是123-1234。”
      #include <sys/socket.h>
      int bind(int sockfd, struct sockaddr *myaddr, socklen_t addrlen);
        --> 成功时返回0，失败时返回-1。
    3.调用lisen函数（连接电话线）时进行的对话
      问：“已架设完电话机后是否需连接电话线？”
      答：“对，只需连接就能接听电话。”
      #include <sys/socket.h>
      int listen(int sockfd, int backlog);
        --> 成功时返回0，失败时返回-1。
    4.调用accept函数（拿起话筒）时进行的对话
      问：“电话铃响了，我该怎么办？”
      答：“难道您真的不知道？接听啊！”
      #include <sys/socket.h>
      int accept(int sockfd, struct sockaddr*addr, socklen_t *addrlen);
        --> 成功时返回文件描述符，失败时返回-1。

  二.构建打电话套接字
    1.调用的时客户端套接字
      #include <sys/socket.h>
      int connect(int sockfd, struct sockaddr* serv_addr, socklen_t addrlen);
        --> 成功时返回0，失败时返回-1。

1.2 基于Linux的文件操作
    1.底层的文件访问(Low-level File Access)和文件描述符(File Descriptor)
      文件描述符           对象
        0             标准输入：Standard Input
        1             标准输出：Standard Output
        2             标准错误：Standard Error
    2.打开文件
      #include <sys/types.h>
      #include <sys/stat.h>
      #include <fcntl.h>

      int open(const char *path, int flag);   // path 文件名的字符串地址；  flag 文件打开模式信息
          --> 成功时返回文件描述符，失败时返回-1。
    3.关闭文件
      #include <unistd.h>
      int close(int fd);    // fd  需要关闭的文件或套接字的文件描述符。
          -->成功时返回0，失败时返回-1。
    4.将数据写入文件
      #include <unistd.h>
      ssize_t write(int fd, const void* buf, size_t nbytes);    // fd 显示数据传输对象的文件描述符；  buf 保存要传输数据的缓冲地址值。 nbytes 要传输数据的字节数
          -->成功时返回写入的字节数，失败时返回-1。
    5.读取文件中的数据
      #include <unistd.h>
      ssize_t read(int fd, void *buf, size_t nbytes);    // fd 显示数据接收对象的文件描述符；  buf 要保存接收数据的缓冲地址值。 nbytes 要接收数据的最大字节数
          --> 成功时返回接收字节数(但遇到文件结尾则返回0)， 失败时返回-1。
    