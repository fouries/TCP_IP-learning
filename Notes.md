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
    
    [![](/Img/图4-1.png "图4-1 TCP/IP协议栈")][TCP/IP]