***Chapter 1***
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
"""      文件描述符           对象  
            0             标准输入：Standard Input  
            1             标准输出：Standard Output  
            2             标准错误：Standard Error"""  
    2.打开文件
      #include <sys/types.h>
      #include <sys/stat.h>
      #include <fcntl.h>

      int open(const char *path, int flag);   // path 文件名的字符串地址；  flag 文件打开模式信息
          --> 成功时返回文件描述符，失败时返回-1。
    3.关闭文件
      #include <unistd.h>
      int close(int fd);    // fd  需要关闭的文件或套接字的文件描述符。
          --> 成功时返回0，失败时返回-1。
    4.将数据写入文件
      #include <unistd.h>
      ssize_t write(int fd, const void* buf, size_t nbytes);// fd 显示数据传输对象的文件描述符；buf 保存要传输数据的缓冲地址值。nbytes 要传输数据的字节数  
          --> 成功时返回写入的字节数，失败时返回-1。
    5.读取文件中的数据
      #include <unistd.h>
      ssize_t read(int fd, void *buf, size_t nbytes); // fd 显示数据接收对象的文件描述符；buf 要保存接收数据的缓冲地址值；nbytes 要接收数据的最大字节数  
          --> 成功时返回接收字节数(但遇到文件结尾则返回0)， 失败时返回-1。  

***Chapter 2***  
2.1 套接字协议及其数据传输特性
    1.创建套接字
      #include <sys/socket.h>
      int socket(int domain, int type, int protocol); // domain 套接字中的使用的协议族(Protocol Family)信息；
        --> 成功时返回文件描述符，失败时返回-1。  
2.2 协议族(Protocol Family)
"""  　　　　　表２－１　头文件sys/socket.h中声明的协议族
   名称                    　　　　　　协议族
   PF_INET                　　　　IPv4互联网协议族
   PF_INET6               　　　　IPv6互联网协议族
   PF_LOCAL               　　　　本地通信的UNIX协议族
   PF_PACKET              　　　　底层套接字的协议族
   PF_IPX                 　　　　IPX Novell协议族 """
2.3 套接字类型(Type)  
    1.套接字类型1：面向连接的套接字(SOCK_STREAM) 
      如果向socket函数的第二个参数传递SOCK_STREAM，则将创建面向连接的套接字。（2位工人通过传送带传递物品）  
        * 传输过程中数据不会消失。  
        * 按序传输数据。  
        * 传输的数据不存在数据边界(Boundary)。  
      特性：“传输数据的计算机通过３次调用write函数传递了100字节的数据，但接收数据的计算机仅通过１次read函数的调用就接收了全部100字节。”  
      收发数据的套接字内部有缓冲(buffer),简言之就是字节的数组。通过套接字传输的数据将保到该数组。因此，收到数据并不意味着马上调用read函数。  
      只要不超过数组容量，则有可能在数据填充满缓冲后通过一次read函数调用读取全部，也有可能分成多次read函数调用进行读取。也就是说，在面向连接
      的套接字中，read函数和write函数的调用次数并无太大意义。  
      “接字连接必须是一一对应”  
      “可靠的、按需传递的、基于子节的面向连接的数据传输方式的套接字”  
    2.套接字类型2：面向消息的套接字(SOCK_DGRAM) 
      如果向socket函数的第二个参数传递SOCK_DGRAM，则将创建面向消息的套接字。（摩托车快递的包裹(数据)传输方式）
        * 强调快速传输而非传输顺序。  
        * 传输的数据可能丢失也可能损毁。  
        * 传输的数据有数据边界。  
        * 限制每次传输的数据大小。  
      特性：“不可靠、不按顺序传递的、以数据的高速传输为目的的套接字”
    3.协议的最终选择
      socket函数的第三个参数，该参数决定最终采用的协议。  
      “IPv4协议族中面向连接的套接字”  
      IPv4与网络地址系统相关，关于这点将给出单独说明，目前只需记住：本书是基于IPv4展开的。参数PF_INRT指IPv4网络协议族， SOCK_STREAM是面
      向连接的数据传输。满足这2个条件的协议只有IPPROTO_TCP，因此可以如下调用socket函数创建套接字，这种套接字称为TCP套接字。
          int tcp_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
      “IPv4协议中面向消息的套接字”
      SOCK_DGRAM指的是面向消息的数据传输方式，满足上述条件的协议只有IPPROTO_UDP。因此可以如下调用socket函数创建套接字，这种套接字称为UDP
      套接字。  
          int udp_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);  
    4.面向连接的套接字：TCP套接字示例
        * hello_server.c --> tcp_server.c：无变化！
        * hello_client.c --> tcp_client.c：更改read函数调用方式！
      验证TCP套接字的如下特性：
        1.“数据的传输不存在数据边界。” 让write函数调用次数不同于read函数的调用次数。因此，在客户端中分多次调用read函数以接收服务器端发送的全部数据。  
        
    　
      
