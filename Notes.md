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
