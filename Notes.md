Chapter 1  理解网络变成和套接字
===
1.1 理解网络变编程和套接字
---
 **一.构建接电话套接字** 
  
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

**二.构建打电话套接字**

    1.调用的时客户端套接字  
      #include <sys/socket.h>  
      int connect(int sockfd, struct sockaddr* serv_addr, socklen_t addrlen);  
        --> 成功时返回0，失败时返回-1。  

1.2 基于Linux的文件操作  
---
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
          --> 成功时返回0，失败时返回-1。
    4.将数据写入文件
      #include <unistd.h>
      ssize_t write(int fd, const void* buf, size_t nbytes);// fd 显示数据传输对象的文件描述符；buf 保存要传输数据的缓冲地址值。
      // nbytes 要传输数据的字节数  
          --> 成功时返回写入的字节数，失败时返回-1。
    5.读取文件中的数据
      #include <unistd.h>
      ssize_t read(int fd, void *buf, size_t nbytes); // fd 显示数据接收对象的文件描述符；buf 要保存接收数据的缓冲地址值；
      // nbytes 要接收数据的最大字节数  
          --> 成功时返回接收字节数(但遇到文件结尾则返回0)， 失败时返回-1。 

    * 数据类型_t 表示元数据类型，为操作系统定义的, 在 sys/types.h文件中一般由typedef声明定义
Chapter 2  套接字类型与协议设置
===
2.1 套接字协议及其数据传输特性
---
    1.创建套接字
      #include <sys/socket.h>
      int socket(int domain, int type, int protocol); // domain 套接字中的使用的协议族(Protocol Family)信息；
        --> 成功时返回文件描述符，失败时返回-1。  
2.2 协议族(Protocol Family)
---
    表２－１　 头文件sys/socket.h中声明的协议族
     名称                    　　　　　　协议族
    PF_INET                　　　　IPv4互联网协议族
    PF_INET6               　　　　IPv6互联网协议族
    PF_LOCAL               　　　　本地通信的UNIX协议族
    PF_PACKET              　　　　底层套接字的协议族
    PF_IPX                 　　　　IPX Novell协议族
 
2.3 套接字类型(Type)  
---
    1.套接字类型1：面向连接的套接字(SOCK_STREAM) 
      如果向socket函数的第二个参数传递SOCK_STREAM，则将创建面向连接的套接字。（2位工人通过传送带传递物品）  
        * 传输过程中数据不会消失。  
        * 按序传输数据。  
        * 传输的数据不存在数据边界(Boundary)。  
      特性：“传输数据的计算机通过３次调用write函数传递了100字节的数据，但接收数据的计算机仅通过１次read函数的调用就接收了全部100字节。”  
      收发数据的套接字内部有缓冲(buffer),简言之就是字节的数组。通过套接字传输的数据将保到该数组。因此，收到数据并不意味着马上调用read函数。  
      只要不超过数组容量，则有可能在数据填充满缓冲后通过一次read函数调用读取全部，也有可能分成多次read函数调用进行读取。也就是说，在面向连
      接的套接字中，read函数和write函数的调用次数并无太大意义。  
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
        1.“数据的传输不存在数据边界。” 让write函数调用次数不同于read函数的调用次数。因此，在客户端中分多次调用read函数以接收服务器端发送的
        全部数据。  

Chapter 3  地址族与数据序列
===
3.1 分配给套接字的IP地址与端口号
---
    IP是Internet Protocol(网络协议)的缩写，是为收发网络数据而分配给计算机的值。端口号并非赋予计算机的值，而是为区分程序中创建的
    套接字而分配给套接字的序号

    1.网络地址(Internet Address)
		为使计算机连接到网络并收发数据，必须向其分配IP地址。IP地址分为两类。
		* IPv4(Internet Protocol version 4)		  4字节地址族
		* IPv6(Internet Protocol version 6)     16字节地址族
		路由器和交换机:  
		若想构建网络，需要一种物理设备完成外网与本网主机之间的数据交换，这种设备便是路由器或者交换机。他们实际上也是一种计算机，只不过
    是为特殊目的而设计运行的，因此有了别名。所以，如果在我们使用的计算机上安装适当的软件，也可以将其用作交换机。另外，交换机比路由
    器功能要简单一些，而实际用途差别不大。	
	2.网络地址分类与主机地址边界
		只需通过IP地址的第一个字节即可判断网络地址占用的字节数，因为我们根据IP地址的边界区别网络地址，如下所示。
		* A类地址的首字节范围：0～127            以0开始
		* B类地址的首字节范围:128~191            前2位以10开始
		* C类地址的首字节范围：192~223           前3位以110开始
		正因如此，通过套接字收发数据时，数据传到网络后即可轻松找到正确的主机。
	3.用于区分套接字的端口号
		IP用于区分计算机，只要有IP地址就能向主机传输数据，但仅凭这些无法传输给最终的应用程序。
		计算机中一般配有NIC(Network Interface Card，网络接口卡(网络适配器、网卡))数据传输设备。通过NIC向计算机内部传输数据时会用到IP。
	操作系统负责把传递到内部的数据适当分配套接字，这时就要利用端口号。也就是说，通过NIC接收的数据内有端口号，操作系统正是参考此端口号把数据传输给相
        应端口的套接字。
        端口号就是在同一操作系统内为区分不同套接字而设置的，因此无法将1个端口号分配给不同的套接字。另外，端口号由16位构成，可分配端
        口号范围从0-65535。但0-1023是知名端口(Well-known PORT),一般分配给特定应用程序，所以应当分配范围之外的值。端口不能重复，
        但TCP套接字和UDP套接字不会公用端口号，所以允许重复。

3.2 地址信息的表示
---
    应用程序中使用的IP地址和端口号以结构体的形式给出定义。本书将以IPv4为中心，围绕此结构体讨论目标地址的表示方法。

    1.表示IPv4地址的结构体  
      struct sockaddr_in      
      {
          sa_family_t       sin_family;   //地址族（Address Family）  
          uint16_t          sin_port;     //16位TCP/UDP端口号  
          struct in_addr    sin_addr;     //32位IP地址  
          char              sin_zero[8];  //不使用  
      };  

      该结构体中提到的另一个结构体in_addr定义如下，它用来存放32位IP地址。
      struct in_addr
      {
        in_addr_t           s_addr;       //32位IPv4地址
      }；


                              表3-1 POSIX中定义的数据类型
        数据类型名称                  数据类型说明                                声明的头文件
          int8_t                  signed 8-bit int          
          uint8_t                 unsigned 8-bit int(unsigned char)
          int16_t                 signed 16-bit int
          uint16_t                unsigned 16-bit int(unsigned short)          sys/types.h
          int32_t                 signed 32-bit int
          uint32_t                unsigned 32-bit int(unsigned long)

          sa_family_t             地址族(address family)                        sys/socket.h
          socklen_t               长度（length of struct）                      

          in_addr_t               IP地址，声明为uint32_t                         netinet/in.h
          in_port_t               端口号，声明为uint16_t                     
    
    2.结构体 socket_in 的成元分析
      1).成员sin_family
          每种协议族适用的地址族均不同。比如，IPv4使用4字节地址族，IPv6使用16字节地址族。可以参考表3-2保存sin_family地址信息
                              表3-2 地址族
        地址族（Address Family）                             含义
            AF_INET                               IPv4网络协议中使用的地址族
            AF_INET6                              IPv6网络协议中使用的地址族
            AF_LOCAL                              本地通信中采用的UNIX协议的地址族
      2).成员sin_port
          该成员保存16位端口号，重点在于，它以网络字节序保存（关于这一点稍后将给出详细说明）。
      3).成员sin_addr
          该成员保存32位IP地址信息，且也以网络字节序保存。为理解好该成员，应同时观察结构体in_addr。但结构体in_addr声明为uint32_t，
          因此只需当作32位整数型即可。
      4).成员sin_zero
          无特殊含义。只是结构体sockaddr_in的大小与sockeaddr结构体保持一致而插入的成员。必须填充为0，否则无法得到想要的结果。

          struct sockaddr_in serv_addr;
          ....
          if(bind(serv_sock,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)
              error_handling("bind() error");
          ....
        此处重要的是第二个参数的传递。实际上，bind函数的第二个参数期望得到sockaddr结构体变量地址值，包括地址族、端口号、IP地址
        等。从下列代码也可以看出，直接向sockaddr结构体填充这些信息会带来麻烦。
        struct sockaddr_in
        {
          sa_family_t       sin_family;     // 地址族（Address Family）
          char              sa_data[14];    // 地址信息
        };
        此结构体成员sa_data保存的地址信息中需包含IP地址和端口号，剩余部分应填充0，这也是bind函数要求的。而这对于包含地址信息来讲非常麻烦，
        继而就有了新的结构体sockaddr_in。若按照之前的讲解填写sockaddr_in结构体，则将生成符合bind函数要求的字节流。最后转换为sockaddr
        型的结构体变量，再传递给bind函数即可。
3.3 网络字节序与地址变换
---
    不同的CPU中，4字节整数型值1在内存空间的保存方式是不同的。
    00000000 00000000 00000000 00000001       大端序
    有些CPU以这种顺序保存到内存，另外一些CPU则以倒序保存。
    00000001 00000000 00000000 00000000       小端序

    1.字节序（Order）与网络字节序
      CPU向内存保存数据的方式有2种，这意味着CPU解析数据的方式也有2种。
      × 大端序（Big Endian）: 高位字节存放到低位地址。
      × 小端序（Little Endian）: 高位字节存放到高位地址。
      示例进行说明。假设在0x20号开始的地址中保存4字节int类型数0x12345678。大端序保存方式如下所示。
                        
           0x20号      0x21号      0x22号      0x22号
            0x12        0x34        0x56       0x78 
                        图 3-4 大端序字节表示
      小端序保存方式如下所示。
                        
           0x20号      0x21号      0x22号      0x22号
            0x78        0x56       0x34        0x12 
                        图 3-5 小端序字节表示
        目前主流的Intel系列CPU以小端序方式保存数据。
        在通过网络传输数据时约定统一方式，这种约定称为网络字节序（Network Byte Order），非常简单———统一为大端序。
        即，先把数据数组转换为大端序格式再进行网络传输。因此，所有计算机接收数据时应识别该数据是网络字节序格式，小端序系统传输数据时应转化为
        大端序排列方式。

    2.字节序转换（Endian Conversions）
      转换字节序的函数。
      × unsigned short htons(unsigned short);
      × unsigned short ntohs(unsigned short);
      × unsigned long htonl(unsigned long);
      × unsigned long ntohl(unsigned long);
      通过函数名应该能掌握其功能，只需了解一下细节。
      × htons中的h代表主机(host)字节序，
      × htons中的n代表网络(network)字节序
      另外，s指的是short，l指的是long(Linux中long类型占用4个字节，这很关键)。因此，htons是 h、to、n、s的组合，也可以解释为“把short型
    数据从主机字节序转化为网络字节序”。
      ntohs,也可以解释为“把short型数据从网络字节序转化为主机字节序”。
      通常，以s作为后缀的函数中，s代表2个字节short,因此用于端口号转换；以l作为后缀的函数中，l代表4个字节，因此用于IP地址转换。  endian_conv.c
      * 除了向sockaddr_in结构体变量填充数据外，其他情况无需字节序问题。
3.4 网络地址的初始化与分配
---
    1.将字符串信息转换为网络字节序的整数型
      sockaddr_in中保存地址信息的成员为32位整数型。因此，为了分配IP地址，需要将其表示为32位整数型数据。 201.211.214.36转换为4字节整形数据。
      IP地址是点分十进制表示法(Dotted Decimal Notation)，而非整数型数据表示法。有个函数会帮我们将字符串形式的IP地址转换成32位整形数据。
      此函数在转换类型的同时进行网络字节序转换。
        #include <arpa/inet.h>
        in_addr_t inet_addr(const char * string);
            --> 成功时返回32位大端序整数型值，失败时返回 INADDR_NONE。
      向该函数传递类似 “211.214.107.99” 的分十进制格式字符串，它会将其转换为32位整数型数据并返回。   inet_addr.c
      从运行运行结果看出，inet_addr函数不仅可以把IP地址转成32位整数型，而且还可以检测无效的IP地址。另外从输出结果可以验证转换为网络字节序。
      inet_aton函数与inet_addr函数在功能上完全相同，也将字符串形式IP地址转换为32位网络字节序整数并返回。只不过该函数利用了in_addr结构体，
      且其使用频率更高。
        #include <arpa/inet.h>                                      // string 含需转换的IP地址信息的字符串地址值 
        int inet_aton(const char * string, struct in_addr * addr);  // addr 将保存转换结果的in_addr结构体变量的地址值。
            --> 成功时返回1（true）,失败时返回0（false）。
      实际编程中若要调用inet_addr函数，需将转换后的IP地址信息代入sockaddr_in结构体中声明的in_addr结构体变量。而inet_aton函数则不需要此过程。
      原因在于，若传递in_addr结构体变量地址值，函数会自动把结果填入该结构体变量。    inet_aton.c
      与inet_aton函数正好相反的函数，此函数可以把网络字节序整数型IP地址转换成我们熟悉的字符串形式。
        #include <arpa/inet.h>
        char * inet_ntoa(struct in_addr adr);
            --> 成功时返回转换的字符串地址值，失败时返回-1。
      该函数将通过参数传入的整数型IP地址转换为字符串格式返回。返回值类型为char指针。
      若需要长期保存，则应将字符串复制到其他内存空间。    inet_ntoa.c

    2.网络地址初始化
      结合前面所学的内容，现在介绍套接字创建过程中常见的网络地址信息初始化方法。

        struct sockaddr_in addr;
        char* ser_ip = "211.217.168.13";    // 声明 IP 地址字符串
        char* serv_port = "9190";           // 声明端口号字符串
        memset(&addr, 0, sizeof(addr));     // 结构体变量addr的所有成员初始化为 0
        addr.sin_family = AF_INET;          // 指定地址族
        addr.sin_addr.s_addr = inet_addr(serv_ip);    // 基于字符串的IP地址初始化
        addr.sin_port = htons(atoi(serv_port));       // 基于字符串的端口号初始化

      上述代码中，memset函数将每个字节初始化为同一值：第一个参数为结构体变量addr的地址值，即初始化对象为addr;第二个参数为0，因此初始化为0；
      最后一个参数中传入addr的长度，因此addr的所有字节均初始化为0。这么做是为了将sockaddr_in结构体的成员sin_zero初始化为0。
      总之，上述代码利用字符串格式的IP地址和端口号初始化了sockaddr_in结构体变量。

    3.客户端地址信息初始化
      上述网络地址信息初始化过程主要针对服务器端。给套接字分配IP地址和端口号主要时为了下面这件事做准备：
        “请把进入IP 211.217.168.13、9190端口的数据传给我！”
      反观客户端中连接请求如下：
        “请连接到IP 211.217.168.13、9190端口！”
      请求方法不同意味着调用的函数也不同。服务器端的准备工作通过bind函数完成，而客户端则通过connect函数完成。
      因此，函数调用前需准备的地址值类型也不同。服务器端声明sockaddr_in结构体变量，将其初始化为为赋予服务器端IP和套接字的端口号，然后调用bind
      函数；而客户端则声明sockaddr_in结构体，并初始化为要与之连接的服务器端套接字的IP和端口号，然后调用connect函数。

    4.INADDR_ANY
      每次创建服务器端套接字都要输入IP地址会有些繁琐，此时可如下初始化地址信息。

        struct sockaddr_in addr;
        char * serv_port = "9190";
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons(atoi(serv_port));

      与之前方式最大的区别在于，利用常数INADDR_ANY分配服务器端的IP地址。若采用这种方式，则可自动获取运行服务器端的计算机IP地址，不必亲自输入。
      若同一计算机中已分配多个IP地址，只要端口号一致，就可以从不同IP地址接收数据。

    ５.第1章的 hello_server.c、 hello_client.c运行过程
        ./hserver 9190  
      通过代码可知，向main函数传递9190为端口号。通过此端口号创建服务器端套接字并运行程序，但未传递IP地址，因为可以通过INADDR_ANY指定IP地址。
        ./hclient 127.0.0.1 9190
      127.0.0.1是会送地址（loopback address）,指的是计算机自身IP地址。
      如果服务器端和客户端分别在2台计算计中运行，则可以输入服务器端IP地址。

    6.向套接字分配网络地址
      前面讨论了sockaddr_in结构体的初始化方法，接下来就把初始化的地址信息分配给套接字。bind函数负责这项操作。
        #include<sys/socket.h>
        int bind(int sockfd, struct sockaddr_in * myaddr, socklen_t addrlen);  
            --> 成功时返回0， 失败时返回-1。                             
		// sockfd: 要分配地址信息（IP地址和端口号）的套接字文件描述符。  
		// myaddr: 存有地址信息的结构体变量地址值。
		// addrlen: 第二个结构体变量的长度。
	
      如果此函数调用成功，则将第二个参数指定的地址信息分配给第一个参数中的相应套接字。下面给出服务器端常见套接字初始化过程。

        int serv_sock;
        struct sockaddr_in serv_addr;
        char* serv_port = "9190";

        /* 创建服务器端套接字（监听套接字） */
        serv_sock = socket(PF_INET, SOCK_STREAM, 0);

        /* 地址信息初始化 */
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        serv_addr.sin_port = htons(atoi(serv_port));

        /* 分配地址信息 */
        bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        ......

        服务器端代码结构默认如上，当然还有未显示的异常处理代码。

3.5 基于Windows的实现
---
    Windows中同样存在sockaddr_in结构体及各种变换函数，而且名称、使用方法及含义都相同。

    1.函数 htons、htonl在 Windows中的使用
      source:   endian_conv_win.c

    2.函数 inet_addr、inet_ntoa在Windows中的使用
      Windows中不存在 inet_aton函数，故省略。

    3.在Windows环境下向套接字分配网络地址
      Windows中向套接字分配网络地址的过程与Linux中完全相同，因为bind函数的含义、参数及返回类型完全一致。
    
    4.WSAStringToAddress & WSAAddressToString

Chapter 4  基于TCP的服务器端/客户端(1)
===
4.1 理解 TCP和 UDP
---
    根据数据传输方式的不同，基于网络协议的套接字一般分为TCP套接字和UDP套接字。因为TCP套接字是面向连接的，因此又称基于流（stream）的套接字。
    TCP是Transmision Control Protocol（传输控制协议）的简写，意为“对数据传输过程的控制”。
    
    1.TCP/IP 协议栈
      讲解TCP前先介绍TCP所属的TCP/IP协议栈(Stack, 层)，如图 4-1所示。
   ![图4-1 TCP/IP协议栈](/Img/图4-1.jpg)  

    从图4-1可以看出，TCP/IP协议栈共分4层，可以理解为4个层次化过程。
    通过层次化方案————TCP/IP协议栈解决。通过TCP套接字收发数据时需要借助这4层，如图4-2所示  


   ![图4-2 TCP协议栈](/Img/图4-2.jpg)  
   
      反之，通过UDP套接字收发数据时，利用图4-3中的4层协议栈完成。  

   ![图4-3 UDP协议栈](/Img/图4-3.jpg)
   
    各层可能通过操作系统等软件实现，也可能通过类似NIC的硬件设备实现。

    Tips: 数据通信中使用的协议栈分为7层，而本书分了4层。对于程序员来说掌握4层协议栈就足够了。

    2.TCP/IP协议的诞生背景
         “通过英特网完成有效数据传输” 这个课题让许多专家聚集到了一起，这些人是硬件、系统、路由算法等各领域的顶级专家。
         计算机网络并非仅凭软件就能解决。编写软件之前要构建硬件系统，在此基础上需要通过软件实现各种算法。所以才需要众多领域的专家进行讨论，
      以形成各种规定。 把 “通过英特网完成有效数据传输” 问题按照不同领域划分成小问题后，出现多种协议，它们通过层级结构建立了紧密联系。

    3.链路层
         链路层是物理链接领域标准化的结果，也是最基本的领域，专门定义LAN、WAN、MAN等网络标准。若两台主机通过网络进行数据交换，则需要4-4所示的
      物理连接，链路层就负责这些标准。

   ![图4-4 网络连接结构](/Img/图4-4.jpg)

    4.IP层
         准备好物理连接后就要传输数据。为了复杂的网络传输中传输数据，首先需要考虑路径的选择。解决此问题就是IP层，该层使用的协议就是IP。
         IP本身是面向消息的、不可靠的协议。每次传输时会帮我们选择路径，但并不一致。如果传输中发生路径错误，则选择其他路径；但如果发生数据丢失
      或错误，则无法解决。换言之，IP协议无法应对数据错误。
    
    5.TCP/UDP 层
         IP层解决数据传输中的路径选择问题，只需照此路径传输数据即可。TCP和UDP层以IP层提供的路径信息为基础完成实际的数据传输，故该层又称传输层
      （Transport）。TCP可以保证可靠的数据传输，但它发送数据时以IP层为基础（这也是协议栈结构层次化的原因）。
         IP层只关注1个数据包（数据传输的基本单元）的传输过程。因此，即使传输多个数据包，每个数据包也是由IP层实际传输的，也就是说传输顺序及传输
      本身是不可靠的。若只利用IP层传输数据，则有可能导致后传数据包B比先传输的数据包A提前到达。
         TCP的作用就是，如果数据交换过程中可以确认对方已收到数据，并重传丢失数据，那么即便IP层不保证数据传输，这类通信也是可靠的，如图4-5所示。
  
   ![图4-5 传输控制协议](/Img/图4-5.jpg)

      图4-5简单描述了TCP的功能。总之，TCP和UDP存在IP层之上，决定主机之间的数据传输方式，TCP协议确认后向不可靠的IP协议赋予可靠性。

    6.应用层
          上述内容是套接字通信过程中自动处理的。选择数据传输路径、数据确认过程都被隐藏到套接字内部。程序员编程时无需考虑这些过程，但这并不意味
      着不用掌握这些知识。只有掌握了这些理论，才能编写出符合需求的网络程序。
          向各位提供的工具就是套接字，大家只需利用套接字编写出程序即可。编写软件过程中，需要根据程序特点决定服务器和客户端之间的数据传输规则
      （规定），这便是应用层协议。  网络编程的大部分内容就是设计并实现应用层协议。

4.2 实现基于 TCP 的服务器/客户端
---
    1.TCP 服务器端的默认函数调用顺序
          图4-6给出了TCP服务器端默认的函数调用顺序，绝大部分TCP服务器端都按照该顺序调用。
      
   ![图4-6 TCP服务器端函数调用顺序](/Img/图4-6.jpg)

      调用socket函数创建套接字，声明并初始化地址信息结构体变量，调用bind函数向套接字分配地址。

    2.进入等待连接请求状态
          我们已调用bind函数给套接字分配了地址，接下来就要通过调用listen函数进入等待连接请求状态。只有调用了listen函数，客户端才能进入可发出
      连接请求的状态。换言之，这时客户端才能调用connect函数（若提前调用将发生错误）。

      #include <sys/socket.h>
      int listen(int sock, int backlog);
          --> 成功时返回0, 失败时返回-1。    
      // sock: 希望进入等待连接请求状态的套接字文件描述符，传递的描述符套接字参数成为服务器端套接字（监听套接字）。
      // backlog: 连接请求等待队列（Queue）的长度，若为5，则队列长度为5，表示最多使5个连接请求进入队列。

      解释一下等待连接请求状态的含义和连接请求等待队列。“服务器端处于等待连接请求状态”是指，客户端请求连接时，受理连接前一直使请求处于等待状态，
      图4-7 给出了这个过程。

   ![图4-7 等待连接请求状态](/Img/图4-7.jpg) 

          由图4-7 可知作为listen函数的第一个参数传递的文件描述符套接字的用途。客户端连接请求本身也是从网络中接收到的一种数据，而要想接收就需要
      套接字。此任务就由服务器端套接字完成。服务器端套接字是接收连接请求的一名门卫或一扇门。
          服务器端调用listen函数即可生成这种门卫（服务器端套接字），listen函数的第二个参数决定了等候室的大小。等候室称为连接请求等待队列，准备
      好服务器端套接字和连接请求等待队列后，这种课接收请求的状态称为等待连接请求状态。
          listen函数的第二个参数值与服务器端的特性有关，像频繁接收请求的Web服务器端至少应为15。另外，连接请求队列的大小始终根据实验结果而定。
    
    3.受理客户端连接请求
        调用listen函数后，若有新的连接请求，则应按序受理。受理请求意味着进入可接受数据的状态。进入这种状态所需的部件--套接字！ 下面这个函数将
    自动创建套接字，并连接到发起请求的客户端。

      #include <sys/socket.h>
      int accept(int sock, struct sockaddr * addr, socklen_t * addrlen);
          --> 成功时返回创建的套接字文件描述符，失败时返回-1。
      // sock: 服务器套接字的文件描述符
      // addr: 保存发起连接请求的客户端地址信息的变量地址值，调用函数后向传递来的地址变量参数填充客户端地址信息。
      // addrlen: 第二个参数addr结构体的长度，但是存有长度的变量地址。函数调用完成后，该变量即被填入客户端地址长度。

        accept函数受理连接请求等待队列中待处理的客户端连接请求。函数调用成功时，accept函数内部将产生用于数据I/O的套接字，并返回其文件描述符。
    需强调的是，套接字是自动创建的，并自动与发起连接请求的客户端建立连接。 图4-8展示了accept函数的调用过程。

   ![图4-8 受理连接请求状态](/Img/图4-8.jpg) 

    图4-8展示了 “从等待队列中取出1个连接请求，创建套接字并完成连接请求” 的过程。服务器端单独创建的套接字与客户端建立连接后进行数据交换。
    
    4.回顾 Hello world服务器端
                                hello_server.c    

        我们按照服务器端实现顺序把看起来很复杂的第一章代码进行了重新整理。可以看出，服务器端的基本实现过程实际上非常简单。

    5.TCP 客户端的默认函数调用顺序
        客户端的实现顺序。比服务器端简单许多。 因为创建套接字和请求连接就是客户端的全部内容，如图 4-9所示。
   
   ![图4-9 TCP 客户端的默认函数调用顺序](/Img/图4-9.jpg) 

    与服务器端相比，区别就在于 “请求连接”，它是创建客户端套接字后向服务器端发起的连接请求。服务器端调用 listen函数后
    创建连接请求等待队列，之后客户端即可请求连接。 发起连接请求，通过调用如下函数完成。

        #include <sys/socket.h>
        int connect(int sock, struct sockaddr* servaddr, socklen_t addrlen);
            --> 成功时返回 0，失败时返回-1。
        // sock: 客户端套接字文件描述符。
        // servaddr: 保存目标服务器端地址信息的变量地址值。
        // addrlen: 以字节为单元传递已传递给第二个结构体参数servaddr的地址变量长度。

        客户端调用 connect函数后，发生以下情况之一会返回（返回函数调用）
          * 服务器端接收连接请求。
          * 发生断网等异常情况而中断连接请求。
        需要注意，所谓的 “接收连接” 并不意味着服务器端调用 accept函数，其实是服务器端把连接请求信息记录到等待队列。因此
    connect函数返回后并不立即进行数据交换。
    
    Tips: 服务器端给套接字分配了IP和端口号。但客户端实现过程中并未出现套接字地址分配，而是创建套接字后立即调用了connect函数。
          调用connect函数时，在操作系统中，更准确的说是内核中分配地址。 IP用计算机（主机）的IP，端口随机。客户端的IP地址和
          端口号在调用connect函数时自动分配，无需调用标记的 bind函数进行分配。
    
    6.回顾 Hello world客户端
                            hello_client.c
    
    7.基于 TCP的服务器端/客户端函数调用关系
        勾勒出TCP服务器/客户端之间的交互过程，如图4-10所示。

   ![图4-10 函数调用关系](/Img/图4-10.jpg) 

    图4-10的总体流程如下：服务器端创建套接字后连续调用bind、listen函数进入等待状态，客户端通过调用connect函数发起连接请求。
    客户端通过调用connect函数发起连接请求。需要注意的是，客户端智能等到服务器端调用listen函数后才能调connect函数。同时要清楚，
    客户端调用connnect函数前，服务器端才有可能率先调用accept函数。此时服务器端在调用accept函数时进入阻塞（blocking）状态，
    直到客户端调用connect函数为止。

4.3 实现迭代服务器端/客户端
---
        本节编写回声（echo）服务器端/客户端。即服务器端将客户端传输的字符串数据原封不动地传回客户端，就像回声一样。在此之前先要解释
    一下迭代服务器端。
    
    1.实现迭代服务器端
        之前讨论的 Hello world服务器处理完1个客户端连接请求即退出，连接请求等待队列实际没有太大的意义。但这并非我们想象的服务器端。
    设置好等待队列大小后，应向所有客户端提供服务。如果想继续受理后续的客户端连接请求，最简单的办法就是插入循环语句反复调用accept函数
    如图 4-11所示。

   ![图4-11 迭代服务器端的函数调用顺序](/Img/图4-11.jpg) 

        从图4-11可以看出，调用accept函数后，紧接着调用I/O相关的read、write函数，然后调用close函数。这并非针对服务器端套接字，而是针对
    accept函数调用时创建的套接字。
        调用close函数就意味着结束了针对某一客户端的服务。此时如果还想服务于其他客户端，就要重新调用accept函数。 
        同一个时刻确实只能服务于一个客户端。将来学完进程和线程之后，就可以编写同时服务多个客户端的服务器了。
    
    2.迭代回声服务器端/客户端
        前面讲的就是迭代服务器端。即使服务器端以迭代方式运转，客户端代码已无太大区别。接下来创建迭代回声服务器端及与其配套的回声客户端。
    基本运行方式：
        * 服务器端在同一时刻只与一个客户端相连，并提供回声服务。
        * 服务器端依次向5个客户端提供服务并退出。
        * 客户端接收用户输入的字符串并发送到服务器端。
        * 服务器端将接收到的字符串数据传回客户端，即“回声”。
        * 服务器端与客户端之间的字符串回声一直执行到客户端输入Q为止。
    
    回声服务器端代码。 echo_server.c

    3.回声客户端存在的问题
      下列是 echo_client.c的第45～48行代码。

        write(sock, message, strlen(message));
        str_len = read(sock, message, strlen(message));
        message[str_len] =0;
        printf("Message from server: %s", message);

      以上代码有个错误假设： “每次调用read、write函数时都会以字符串为单元执行实际的I/O操作。”
      “字符串太长了，需要分2个数据包发送！”
      服务器端希望通过调用1次write函数传输数据，但如果数据太大，操作系统就有可能把数据分成多个数据包发送到客户端。另外在此过程中，
    客户端有可能在尚未收到全部数据包时就调用read函数。

4.4 基于 Windows的实现
---
    1.基于 Windows的回声服务器端
      为了将Linux平台下的示例转化成Windows平台示例，需要记住下4点。
      × 通过 WSAStartup、WSACleanup函数初始化并清除套接字相关库。
      × 把数据类型和变量名切换成Windows风格。
      × 数据传输中用recv、send函数而非read、write函数。
      × 关闭套接字时用closesocket函数而非close函数。
                echo_server_win.c

    2.基于 Windows的回声客户端
                echo_client_win.c

Chapter 5  基于TCP的服务器端/客户端(2)
===
5.1 回声客户端的完美实现
---
    1.回声服务器端没有问题，只有回声客户端有问题？
        回声客户端传输的是字符串，而且是通过调用write函数一次性发送的。之后还调用一次read函数，期待着接收自己传输的字符串。这就是问题所在。

    2.回声客户端问题的解决方法
        其实很容易解决，因为可以提前确定接收数据的大小。若之前传输了20字节长的字符串，则在接收时循环调用read函数读取20个字节即可。
                echo_client2.c

    3.如果问题不在于回声客户端：定义应用层协议
        之前回声服务端/客户端中定义了如下协议
             “收到Ｑ就立即终止连接”
            同样，收发数据过程中也需要定好规则（协议）以表示数据的边界，或提前告知收发数据的大小。服务器/客户端实现过程中逐步定义的这些规则集合就是
        应用层协议。
        × 客户端连接到服务器端后以1字节整数形式传递待算字个数
        × 客户端向服务器端传递的每个整数型数据占用4字节
        × 传递整数型数据后接着传递运算符。运算符信息占用1字节
        × 选择字符 +、-、*之一传递
        × 服务器端以4字节整数型向客户端传回运算结果
        × 客户端得到运算结果后终止与服务器端的连接 

    4.计算器服务器端/客户端示例
            op_server.c  op_client.c

5.2 TCP 原理
---
    1.TCP 套接字中的 I/O缓冲
      × I/O缓冲在每个TCP套接字中单独存在
      × I/O缓冲在创建套接字时自动生成
      × 即使关闭套接字也会继续输出缓冲中遗留的数据
      × 关闭套接字将丢失输入缓冲中的数据
    TCP 会控制数据流。TCP 中有滑动窗口（Sliding Window）协议。数据收发也是如此，因此 TCP中不会因为缓冲溢出而丢失数据。

    ２.TCP 内部工作原理1： 与对方套接字的连接
    TCP 套接字从创建到消失所经过程分为如下3步。
      × 与对方套接字建立连接
      × 与对方套接字进行数据交换
      × 断开与对方套接字的连接

    3.TCP 内部的工作原理2： 与对方主机的数据交换
      通过第一步三次握手过程完成了数据交换准备，下面就正式开始收发数据，其默认方式如图 4-5所示
      按照如下方式传递 ACK消息：
            ACK号 --> SEQ号 + 传递的字节数 + 1
      与三次握手协议相同，最后加1是为了告知对方下次要传递的SEQ号。

    4.TCP 的内部工作原理3： 断开与套接字的连接
      TCP套接字的结束过程非常优雅。
      先由套接字Ａ向套接字Ｂ传递断开连接的消息，套接字Ｂ发出确认收到的消息，然后套接字Ａ传递可以断开连接的消息，套接字Ａ同样发出确认消息。如图 5-6

5.3 基于 Windows的实现
---
        op_server_win.c   op_client_win.c

Chapter 6  基于UDP的服务器端/客户端
===
6.1 理解UDP
---
    1.UDP套接字的特点
      类比寄信，UDP 是一种不可靠的数据传输服务。 
      TCP 在不可靠的 IP层进行流控制，而 UDP就缺少这种流控制机制。流控制是区分 UDP和 TCP最重要的标志。
    
    2.UDP内部的原理
      UDP 最重要的作用就是根据端口号将传到主机的数据包交付给最终的 UDP套接字。
    
    3.UDP的高效使用
      UDP 也具有一定的可靠性。 如果收发的数据量小但需要频繁连接时，UDP比TCP高效。
6.2 实现基于UDP的服务器端/客户端
---
    1.UDP中的服务器端和客户端没有连接
      与TCP不同，无需经过连接过程。也就是说无需调用TCP连接过程中调用的listen函数和accept函数。
      UDP中只有创建套接字的过程和数据交换过程。
    
    2.UDP服务器端和客户端均需1个套接字
      收发信件时使用的邮筒可以比喻为UDP套接字。只要附近有一个邮筒，就可以通过它向任意地址寄出信件。
      1个UDP套接字就能和多台主机通信。
    
    3.基于UDP的数据I/O函数
      TCP套接字将保持与对方套接字的连接。但UDP不会保持连接状态（UDP套接字只有简单的邮筒功能），因此每次传输数据都要添加目标地址信息。
    
    #include <sys/socket.h>
    ssize_t sendto(int sock, void *buff, size_t nbytes, int flags, struct sockaddr *to, socklen_t addrlen);
          --> 成功时返回传输的字节数，失败时返回 -1
          sock    用于传输数据的UDP套接字文件描述符
          buff    保存待传输数据的缓冲地址值
          nbytes  待传输的数据长度，以字节为单位
          flags   可选项参数，若没有则传递0
          to      存有目标地址信息的sockaddr结构体变量的地址值
          addrlen 传递给参数to的地址结构体变量长度
    上述函数与之前的TCP输出函数的最大区别在于，此函数需要向它传递目标地址信息。

    #include <sys/socket.h>
    ssize_t recvfrom(int sock, void *buff, size_t nbytes, int flags, struct sockaddr *from, socklen_t * addrlen);
          --> 成功时返回接收的字节数，失败时返回 -1
          sock    用于接收数据的UDP套接字文件描述符
          buff    保存接收数据的缓冲地址值
          nbytes  可接收的最大字节数，故无法超过参数buff所指的缓冲大小
          flags   可选项参数，若没有则传入0
          from    存有发送端地址信息的sockaddr结构体变量的地址值
          addrlen 保存参数from的结构体变量长度的变量地址值
    
    4.基于UDP的回声服务器端/客户端
            uecho_server.c    uecho_client.c
    
    5.UDP 客户端套接字的地址分配
      TCP客户端调用connnect函数自动完成IP和端口号的分配。首次调用sendto函数时自动分配IP和端口号，分配地址一直保持到程序结束。
    
6.3 UDP的数据传输特性和调用connect函数
---    
    1.存在数据边界的UDP套接字
      TCP数据传输中国不存在数据边界，这表示“数据传输过程中调用Ｉ/O函数的次数不具有任何意义”
      UDP是具有数据边界的协议，输入函数的调用次数和输出函数的调用次数完全一致。
          bound_host1.c     bound_host2.c
    
    2.已连接（connected）UDP套接字与未连接（unconnected）UDP套接字
      TCP套接字中需注册待传数据的目标IP和端口号，而UDP中则无需注册。因此通过sendto函数传递数据的过程大致分为3个阶段。
      × 第1阶段：向UDP套接字注册目标IP和端口号
      × 第2阶段：传输数据
      × 第3阶段：删除UDP套接字中注册的目标地址信息
      上述三个阶段中，第一个和第三个阶段占整个通信过程近1/3的时间，缩短这部分时间将大大提高整体性能。
      每次调用sendto函数时重复上述过程。每次都变更目标地址，因此可以重复利用同一UDP套接字向不同目标传输数据。
    
    3.创建已连接的UDP套接字
      创建已连接UDP套接字的过程格外简单，只需针对UDP套接字调用connect函数。
      sock = socket(PF_INT, SOCK_DGRAM, 0);
      memset(&addr, 0, sizeof(addr));
      addr.sin_family = AF_INET;
      addr.sin_addr.s_addr = ...;
      addr.sin_port = ...;
      connect(sock, (struct sockaddr*) &addr, sizeof(addr));
      UDP套接字调用connect函数并不意味着要与对方UDP套接字连接，这只是向UDP套接字注册目标IP和端口信息。
      之后就与TCP套接字一样，每次调用sendto函数时只需要传输数据。因此已经指定了收发对象，所以不仅可以使用sendto、recvfrom函数，还可以
      使用write、read函数进行通信。
            uecho_con_client.c  uecho_server.c
    
Chapter 7  优雅地断开套接字连接
===    
7.1 基于TCP的半关闭
---
    1.单方面断开连接带来的问题
      Linux的close函数和windows的closesocket函数意味着完全断开连接。
      实际上是无法调用与接收数据相关的函数。最终，由主机B传输的、主机Ａ必须接收的数据也销毁了。
      为了解决这类问题，“只关闭一部分数据交换中使用流”(Ｈalf—close)的方法应运而生。只关闭流的一半。
    
    2.套接字和流（Stream）
      两台主机通过套接字建立连接后进入可交换数据的状态，又称“流形成的状态”。也就是把建立套接字后可交换数据的状态看作一种流。
    
    3.针对优雅断开的 shutdown函数
      #include <sys/socket.h>
      int shutdown(int sock, int howto);
          --> 成功时返回 0， 失败时返回 -1。
          sock   需要断开的套接字文件描述符。
          howto  传递断开方式信息。
      第二个参数决定断开连接的方式
      × SHUT_RD: 断开输入流         套接字无法接收数据，即使输入缓冲收到数据也会抹去。且无法调用输入相关函数。
      × SHUT_WR: 断开输出流         中断输出流，输出缓冲中留有未传输的数据，则将传至目标主机。
      × SHUT_RDWR: 同时断开I/O流    同时关闭I/O流。相当于分两次调用shutdown，一次以SHUT_RD为参数，一次以SHUT_WR为参数。

    4.为何需要半关闭
      调用close函数的同时关闭I/O流，这样也会向对方发送EOF。但此时无法再接收对方传输的数据。这时需要调用shutdown函数，只关闭服务器端
      的输出流（半关闭）。这样既可以发送EOF,同时又保留了输入流，可以接收对方的数据。
    
    5.基于半关闭的文件传输程序
      上述文件传输服务器端和客户端的数据流可整理如图7-3
  ![图7-3 文件传输数据流程图](/Img/图7-3.jpg)
          file_server.c   file_client.c

7.2 基于Windows的实现
---
      file_server_win.c     file_client_win.c

Chapter 8  域名及网络地址
===    
8.1 域名系统
---
    DNS是对IP地址和域名进行互相转换的系统，其核心是DNS服务器。

    1.什么是域名
      提供网络服务的服务器端也是通过IP地址区分的，但几乎不可能以非常难记的ＩP地址形式交换服务端地址信息。
      因此，将容易记、易表述的域名分配并取代IP地址。
    
    2.DNS服务器
      域名是赋予服务器端的虚拟地址，而非实际地址。因此，需要将虚拟地址转化为实际地址。DNS服务器担此重任，可以向DNS服务器请求转换地址。

8.2 IP地址和域名之间的转换
---
    1.程序中有必要使用域名吗？
      IP地址比域名发声变更的概率要高，所以利用IP地址编写程序并非上策，域名可能永久不变，利用域名编写程序时会根据域名获取IP地址，再接入服务器，
      这样程序就不会依赖于服务器IP地址了。
    
    2.利用域名获取IP地址
      利用以下函数可以通过传递字符串格式的域名获取IP地址
      ＃include <netdb.h>
      struct hostent *gethostbyname(const char *hostname);
          -->　成功时返回　hostent结构体地址，　失败时返回 NULL指针。

      struct hostent
      {
        char *h_name;        //official name
        char **h_aliases;    //alias list
        int h_addrtype;      //host address type
        int h_length;        //address length
        char **h_addr_list;  //address list
      }

      ×  h_name: 该变量中存有官方域名（Official domain name）。官方域名各代表某一主页，但一些著名公司并未用官方域名注册。
      ×  h_aliases: 可以通过多个域名访问同一主页。同一IP可以绑定多个域名，因此，除官方域名外还可以指定其他域名。这些信息可以通过h_aliases获得。
      ×  h_addrtype: gethostbyname函数不仅支持IPv4，还支持IPv6。因此可以通过此变量获取保存在h_addr_list的IP地址的地址族信息。若是IPv4，此变量存有AF_INET。
      ×  h_length: 保存IP地址长度。若是IPv4地址，因为是4字节，则保存4；IPv6时，因为是16字节，故保存16。
      ×  h_addr_list: 这是最重要的成员。通过此变量以整数形式保存域名对应的IP地址。另外，用户较多的网站可能分配多个IP给同一域名，利用多个服务器进行负载均衡。
         此时同样可以通过此变量获取IP地址信息。

              gethostbyname.c

    3．利用IP地址获取域名
              gethostbyaddr.c

Chapter 9  套接字的多种可选项
===
9.1 IP地址和域名之间的转换
---
    1.套接字可选项和I/O缓冲大小
      我们之前写的程序都是创建好套接字后（未经特别操作）直接使用的，此时通过默认的套接字特性进行数据通信。
                        表9.1  可设置套接字的多种可选项
      --------------------------------------------------------------------------------
        协议层                    选项名                  读取                    设置
      --------------------------------------------------------------------------------
                                SO_SNDBUF                 O                       O
                                SO_RCVBUF                 O                       O       
                                SO_REUSEADDR              O                       O
                                SO_KEEPALIVE              O                       O
        SOL_SOCKET              SO_BROADCAST              O                       O
                                SO_DONTRROUTE             O                       O
                                SO_OOBINLINE              O                       O
                                SO_ERROR                  O                       X
                                SO_TYPE                   O                       X
      --------------------------------------------------------------------------------
                                IP_TOS                    O                       O
                                IP_TTL                    O                       O
        IPPROTO_IP              IP_MULTICAST_TTL          O                       O
                                IP_MULTICAST_LOOP         O                       O
                                IP_MULTICAST_IF           O                       O
      --------------------------------------------------------------------------------
                                TCP_KEEPALIVE             O                       O
        IPPROTO_TCP             TCP_NODELAY               O                       O
                                TCP_MAXSSEG               O                       O
      --------------------------------------------------------------------------------
    
      从表9-1可以看出，套接字可选项是分层的。IPPROTO_IP层可选项是IP协议相关事项，IPPROTO_TCP层可选项是TCP协议相关的事项，
      SOL_SOCKET层是套接字相关的通用可选项。
    
    2.getsockopt & setsockopt
      我们几乎可以针对表9-1中的所有可选项进行读取（Get）和设置（Set）。（当然，有些可选项只能进行一种操作）。
   
      #include <sys/socket.h>
      int getsockopt(int sock, int level, int optname, void* optval, socklen_t *optlen);
        --> 成功时返回 0，失败时返回 -1。
          sock     用于查看套接字文件描述符
          level    要查看的可选项的协议层
          optname  要查看的可选项名
          optval   保存查看结果的缓冲地址值
          optlen   向第四个参数optval传递的缓冲大小。调用函数后，该变量中保存通过第四个参数返回的可选项信息的字节数
      
      上述函数用于读取套接字可选项，并不难。 下面是更改可选项时调用的函数

      #include <sys/socket.h>
      int setsockopt(int sock, int level, int optname, const void* optval, socklen_t optlen);
        --> 成功时返回 0，失败时返回 -1。
          sock     用于更改可选项的套接字文件描述符
          level    要更改的可选项协议层
          optname  要更改的可选项名
          optval   保存要更改的选项信息的缓冲地址值
          optlen   向第四个参数optval传递的可选项信息的字节数
      
            sock_type.c     setsockopt在其他函数中给出
      用于验证套接字类型的 SO_TYPE是典型的只读可选项， 套接字类型只能在创建时决定，以后不能再更改。

    3.SO_SNDBUF & SO_RCVBUF
      前面介绍过，创建套接字将同时生成I/O缓冲。（自动创建）
      SO_RCVBUF是输入缓冲大小相关的可选项，SO_SNDBUF是输出缓冲大小的相关可选项。用这2个可选项既可以读取当前I/O缓冲大，
    也可以进行更改。通过下列实例读取创建套接字时默认的I/O缓冲大小。
            get_buf.c       set_buf.c
    
9.2 SO_REUSEADDR
---
    可选项 SO_REUSEADDR及其相关的 Time-wait状态很重要，务必理解并掌握。

    1.发生地址分配错误（Binding Error）
      学习 SO_REUSEADDR可选项之前，应理解好 Time_wait状态。
            reuseadr_eserver.c
      客户端和服务器端连接，先断开服务器端会出现问题。如果用同一端口号重新运行服务器端，将输出 "bind() error"消息，并且无法再次运行。

    2.Time-wait状态
      只有先断开连接的（先发送FIN消息的）主机才会经过 Time-wait状态。因此，若服务器端先断开连接，则无法立即重新运行。套接字处在 Time-wait过程时，
    相应端口是正在使用状态。因此，就像之前验证过得，bind函数调用过程中当然会发生错误。
    
      × 先断开连接的套接字必然会经过 Time-wait过程。但无需考虑客户端 Time-wait状态。因为客户端套接字是任意指定的。
    
    3.地址再分配
      在主机A如果四次握手的过程中，如果最后数据丢失，则主机B会认为主机A未能收到自己发送的FIN消息，因此重传。这时，收到FIN消息的主机A将重启
    Time-wait计时器。因此，如果网络状况不理想，Time-wait状态将持续。
      解决方案就是在套接字的可选项中更改 SO_REUSEADDR的状态。SO_REUSEADDR的默认值为0（假）。
    
9.3 TCP_NODELAY
---
    1.Nagle算法
      为防止因数据包过多而发生网络过载，Nagle算法在1984年诞生。
      × TCP套接字默认使用Nagle算法交换数据，因此最大限度地进行缓冲，直到收到ACK。
      × 共需传递4个数据包以传输1个字符串。
      × 一般情况下，不使用Nagle算法可以提高传输速度。但如果无条件放弃使用Nagle算法，就会增加过多的网络流量，反而会影响传输。
      × 因此，未准确判断数据特性时不应禁用Nagle算法。 

    2.禁用Nagle算法
      “大文件数据” 应禁用 Nagle算法。换言之，如果有必要，就应禁用Nagle算法。
      × “Nagle算法使用与否在网络流量上差别不大，使用Nagle算法的传输速度更慢”

      禁用方法: 只需将套接字可选项 TCP_NODELAY改为1（真）即可。

      int opt_val = 1;
      setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void*) &optval, sizeof(opt_val));
      
      可以通过TCP_NODELAY的值查看Nagle算法的设置状态。

      int opt_val;
      socklen_t opt_len;
      opt_len = sizeof(opt_val);
      getsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void*) &optval, &opt_len);

9.4 基于Windows的实现
---
          buf_win.c

Chapter 10  多进程服务器端
===
10.1 进程概念及应用
---
    1.两种类型的服务器端
      “第一个连接请求的受理时间为0秒，第50个连接请求的受理时间为50秒，第100个连接请求的受理时间为100秒！但只要受理，服务只需1秒钟。”
      “所有连接请求的受理时间不超过1秒，但平均服务时间为2-3秒。”
    
    2.并发服务器端的实现方法
      即使有可能延长服务时间，也有必要改进服务器端，使其同时向所有发起请求的客户端提供服务，以提高平均满意度。
      具有代表性的并发服务器端实现模型和方法：

      × 多进程服务器： 通过创建多个进程提供服务。（不太适合在Windows平台下（Windows不支持））
      × 多路复用服务器：通过捆绑并统一管理I/O对象提供服务。
      × 多线程服务器：通过生成与客户端等量的线程提供服务。

    3.理解进程（Process）
      “占用内存空间的正在运行的程序”
    
    4.进程ID 
      无论进程是如何创建的，所有进程都会从操作系统分配到ID。此ID称为 “进程ID”，其值为大于2的整数。1要分配给操作系统启动后的（用于协助操作系统）
    首个进程，因此用户进程无法得到ID值为1.
    
    5.通过调用fork函数创建进程
      #include <unistd.h>
      pid_t fork(void);
          --> 成功时返回进程ID, 失败时返回 -1。

      fork函数将创建调用的进程副本（概念上略难）。也就是说，并非根据完全不同的程序创建进程，而是复制正在运行的、调用fork函数的进程。另外，两个
    进程都将执行fork函数调用后的语句(准确的说是在fork函数返回后)。但因为通过同一个进程、复制相同的内存空间，之后的程序流要根据fork函数的返回值
    加以区分。即利用fork函数的如下特点区分程序执行流程。
     × 父进程：fork函数返回子进程ID。
     × 子进程：fork函数返回0。
    此处 “父进程”（Parent Process）原指进程，即调用fork函数的主体，而 “子进程”（Child Process）是通过父进程调用fork函数复制出的进程。
    fork函数调用后分成了两个完全不同的进程，只是二者共享同一代码而已。
            fork.c
    
10.2 进程和僵尸进程
---
    1.僵尸（Zombie）进程
      进程完成工作后（执行完main函数中的程序后）应被销毁。但有时这些进程将变成僵尸进程，占用系统中的重要资源。是给系统带来负担的原因之一。
    
    2.产生僵尸进程的原因
      调用fork函数产生子进程的终止方式：
      × 传递参数并调用exit函数。
      × main函数中执行return语句并返回值。
    
    僵尸进程何时被销毁呢？
      “应该向创建子进程的父进程传递子进程的exit参数值或return语句的返回值。”
      
      × 只有父进程主动发起请求（函数调用）时，操作系统才会传递该值。换言之，如果父进程未主动要求获得子进程的结束状态值，操作系统将一直保存，
      并让子进程长时间处于僵尸进程状态。
              zombie.c

    3.销毁僵尸进程1: 利用 wait函数
      为了摧毁子进程，父进程应主动请求获取子进程的返回值。
      
      ＃include <sys/wait.h>
      pid_t wait(int * statloc);
          --> 成功时返回终止的子进程ID, 失败时返回 -1。
      
      × WIFEXITED 子进程正常终止时返回　“真”（true）。
      × WEXITSTATUS 返回子进程的返回值。
    
      也就是说，向 wait函数传递变量status的地址时，调用wait函数后应编写如下代码。
      if(WIFEXITED(status))     // 是正常终止的吗？
      {
        puts("Normal termination!");
        printf("Child pass num: %d", WEXITSTATUS(status));  //那么返回值是多少？
      }

              wait.c
    
    4.销毁僵尸进程2：使用 waitpid函数
      wait函数会引起程序阻塞，还可以考虑 waitpid函数。这是防止僵尸进程的第二种方法，也是防止阻塞的方法。

      #include <sys/wait.h>
      pid_t waitpid(pid_t pid, int* statloc, int options);
          --> 成功时返回终止的子进程ID（或0）， 失败时返回 -1。
          
      pid       等待终止的目标进程的ID,若传递-1，则与 wait函数相同，可以等待任意子进程终止。
      statloc   与wait函数的 statloc参数具有相同含义。
      options   传递头文件sys/wait.h中声明的常量ＷNOHANG，即使没有终止的子进程也不会进入阻塞状态，而是返回0并退出函数。

              waitpid.c
    
10.3 信号处理
---    
    1.向操作系统求助
      子进程终止的识别主体是操作系统。 信号处理（Signal Handling）机制。此处的“信号”是在特定事件发生时由操作系统向进程发送的消息。
    为了响应该消息，执行与消息相关的自定义操作的过程称为“处理”或“信号处理”。

    2.关于JAVA的题外话：保持开放思维
      JAVA为了保持平台的移植性，以独立于操作系统的方式提供进程和线程的创建方法。因此JAVA在语言层面支持进程和线程的创建。
      如果有机会，可以尝试JAVA网络编程，而不仅仅局限于Linux或Windows。
    
    3.信号和signal函数
    
      #include <signal.h>
      void (*signal(int signo, void (*func)(int)))(int);
          --> 为了在产生信号时调用，返回之前注册的函数指针。

      × 函数名：signal
      × 参数：int signo, void (*func)(int)
      × 返回类型：参数类型为int型，返回void型函数指针
    
      第一个参数为特殊情况信息，第二个参数为特殊情况下将要调用的函数的地址值（指针）。发生第一个参数代表的情况时，调用第二个参数所指的函数。
      × SIGALRM:  已到通过调用alarm函数注册的时间。
      × SIGINT:   输入 ctrl+C。
      × SIGCHLD:  子进程终止。

      1>.“子进程终止则调用mychild函数。”    此时mychild函数参数应为int，返回值应为void。 常数SIGCHLD定义了子进程终止的情况，signal的第一参数。
            signal(SIGCHLD, mychild);
      2>.“已到通过alarm函数注册的时间，请调用timeout函数。” “输入CTRL+C时调用keycontol函数”
            signal(SIGALRM, timeout);
            signal(SIGINT, keycontol);  
      
      以上就是信号注册过程。注册好信号后，发生注册信号时（注册的情况发生时），操作系统将调用该信号对应的函数。下面先介绍alarm函数。

      #include<unistd.h>
      unsigned int alarm(unsigned int seconds);
            --> 返回 0或以秒为单位的距 SIGALRM信号发生所剩时间。
    
      × 如果调用该函数的同时向它传递一个正整数型参数，响应时间后（以秒为单位）将产生 SIGALRM信号。若向该函数传递0,则之前对SIGALRM信号的预约将取消。
      如果通过该函数预约信号后未指定该信号对应的处理函数，则（通过调用signal函数）终止进程，不做任何处理。
              signal.c
              
    4.

    