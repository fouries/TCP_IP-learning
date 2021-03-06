***Exercise 1.5***  
(1).套接字是网络数据传输用的软件设备。 我们把插头插到插座上就能从电网获得电力供给，同样，为了与远程计算机进行数据传输，需要连接互联网，而编程中的“套接字”就是用来连接该网络的工具。  
(2).listen函数将服务器端转换为可连接状态，等待客户端发送连接请求，而accept函数是处理客户端的连接请求。  
(3).Linux中的套接字也是文件，因而可以通过文件I/O函数read和write进行数据传输。Windows严格区分文件I/O函数和套接字I/O函数。  
(4).为其创建一个可供连接的接口。需要调用bind()函数。　　  
(5).Linux中的文件描述符是为了方便称呼操作系统创建的文件或套接字而赋予的数，Windows的句柄相当于Linux的文件描述符，分为文件句柄和套接字句柄，文件函数与套接字句柄相关函数是有区别的。  
(6).<1>.文件I/O 又称为低级磁盘I/O，遵循POSIX相关标准。任何兼容POSIX标准的操作系统上都支持文件I/O。标准I/O被称为高级磁盘I/O，遵循ANSI C相关标准。只要开发环境中有标准I/O库，标准I/O就可以使用。（Linux 中使用的是GLIBC，它是标准C库的超集。不仅包含ANSI C中定义的函数，还包括POSIX标准中定义的函数。因此，Linux下既可以使用标准I/O，也可以使用文件I/O）。    
<2>.通过文件I/O读写文件时，每次操作都会执行相关系统调用。这样处理的好处是直接读写实际文件，坏处是频繁的系统调用会增加系统开销，标准I/O可以看成是在文件I/O的基础上封装了缓冲机制。先读写缓冲区，必要时再访问实际文件，从而减少了系统调用的次数。    
<3>.文件I/O中用文件描述符表现一个打开的文件，可以访问不同类型的文件如普通文件、设备文件和管道文件等。而标准I/O中用FILE（流）表示一个打开的文件，通常只用来访问普通文件。  
(7).    

***Exercise 2.3***  
(1).协议是为了完成数据交换而定好的约定。 使数据按照约定的协议在服务器端和客户端之间传输。  
(2).1>.传输过程中数据不会丢失。 2>.按序传输数据  3>.传输数据不存在数据边界(Boundary)。  
(3).a、c、e。  
(4).a（SOCK_DGRAM） b（SOCK_DGRAM） c（SOCK_STREAM）  
(5).面向连接的套接字(SOCK_STREAM)不存在数据边界。  套接字必须一一对应。  
(6).让CPU执行多余任务以延迟代码运行的方式称为 “Busy Waiting”。  
      *for(i=0;i<3000;i++)*  
        *printf("Wait time %d \n",i);*  

***Exercise 3.6***
(1).IPv4和IPv6的差别主要是表示IP地址所用的字节数。IPv4标准的4字节IP地址分为网络地址和主机（指计算机）地址。IPv4的地址数大概为43亿个，会很快用尽(2019年11月26日用完)，所以开发了IPv6协议(2010年前后)。IPv6的个数号称可以为地球上的每一粒沙指定一个地址。  
(2).首先数据传输的第一个环节是向目标IP所属的网络传输数据。此时使用的是IP地址中的网络ID。传输的数据将被传到管理网络的路由器，接受数据的路由器将参照IP地址的主机号找自己保存的路由表，找到对应的主机发送数据  
(3).IP用于区分计算机，但仅凭这些无法传输给最终的应用程序。操作系统负责把传递到内部的数据适当分配给套接字，这时就需要端口号。IP地址是为了区分网络上的主机。端口号是区分同一主机下的不同的SOCKET，以确保软件准确收发数据。  
(4).分为A、B、C、D、E 等类型。 A:0-127 B:128-191 C:192-223  
    C A B  
(5).完成外网与本网主机之间的数据交换。  
(6).端口号由16位构成，可分配端口号范围0-65535。但0-1023是知名端口(Well-known PORT)。其中最知名的端口是HTTP:80端口和TCP:21  
(7).bind函数第二个参数类型是sockaddr结构体，很难分配IP地址和端口号，因此IP地址和PORT号的分配是通过sockaddr_in完成的。因为该结构体和sockaddr结构体的组成字节序和大小完全相同，所以可以强转。  
(8).大端序高位地址位保存高位字节数据；小端序高位地址位保存低位字节数据；因为保存栈的方式有差异，所以对网络传输数据的过程制定了标准，这就是“网路字节序”。而且在网络字节序中，数据传输的标准是“大端序”。  
(9).因为网络字节序为大端序，所以大端序计算机在传数据时不需要先转换字节顺序，直接传输。但是接受数据的是小端序计算机，因此要经过网络转本地字节序的过程，再保存到存储设备上。  
(10).回送地址表示计算机本身，为127.0.0.1。因此，如果将数据传送到IP地址127.0.0.1，数据不会通过传输到网络的其他设备上而直接返回。  

***Exercise 4.5***
(1).链路层 --> IP层 --> 传输层 --> 应用层
    TCP层级结构: 链路层 --> TCP --> 传输层 --> 应用层
    UDP层级结构: 链路层 --> UDP --> 传输层 --> 应用层
(2).链路层是LAN、WAN、MAN等网络标准相关的协议栈，是定义物理性质标准的层级。
    相反，IP层是定义网络传输数据标准的层级。
    IP层负责以链路层为基础的数据传输。(链路层是IP层数据传输的基础)
(3).将复杂的TCP/IP协议分层化的话，就可以将分层的层级标准发展成开放系统。实际上，TCP/IP是开放系统，各层级都被初始化，并以该标准为依据组成了互联网。
    因此，按照不同层级标准，硬件和软件可以相互替代，这种标准化是TCP/IP蓬勃发张的依据
(4).listen函数。
(5).listen函数的调用创建了连接请求队列。它是存储客户端连接请求信息的空间。accept函数调用后，将从笨的存储的连接请求信息取出，与客户端建立连接。
(6).客户端是连接请求程序，不是一个接受连接的程序。所以，指导服务器的地址信息是更重要的因素，没有必要通过bind函数明确地分配地址信息。但是，要想和服务器通信，必须将自己的地址
    信息分配到套接字上，因此，在connect函数调用时，自动把IP地址和端口号输出到套接字上。
(7).  

***Exercise 5.4*** 
(1).过程如 图E 5.4所示  
    ![图E 5.4 迭代服务器端的函数调用顺序](/Img/图E 5.4.jpg)    

        [SYN] seq:x, ack:-
        [SYN+ACK] seq:y, ack=x+1
        [ACK] seq:x+1, ack = y+1
(2).SEQ顺序标志符是给信息编号。ACK是用于回复带有编号的信息。也就是说，每次传输信息时，都同时发送SEQ标识，而收到信息的主机应以SEQ信息为基础回复发送信息的主机。通过这种机制，传输数据的主机就可以确认数据是否被正确接收。在传输失败时，可以重新传送。
(3).调用 write函数时，数据将移至输出缓冲，调用 read函数时，数据将会向输出缓冲移动。然后经过网络传输传输到对方主机的输入缓冲。这样输入缓冲中存储的数据通过read函数的响应来读取。
(4).对方会把输入缓冲中可存储的数据大小传送给传输数据方(本方)。因此，在剩余空间为50字节的情况下，不能传输50以上，剩余部分保存在传输方数据输出缓冲中，等待对方主机的输入缓冲出现空间。
    并且，这种交换缓冲多余空间信息的协议被称为滑动窗口协议。
(5).
        sendrecv_serv.c   recvsend_clnt.c
(6).    
        file_serv.c   recvsend_clnt.c

***Exercise 6.5*** 
(1).UDP没有流控制机制。由于该控制涉及到套接字的连接和结束，以及整个数据收发过程，因此，TCP传输的数据是可以信赖的,UDP传输是不可靠的，速度比TCP快。
(2).b c e
(3).IP负责链路选择， UDP负责端对端的传输
(4).TCP比UDP慢的原因有以下两点：１．收发数据前后进行的连接设置及清除过程　２．收发数据过程中为保证可靠性而添加的流控制。
    UDP与TCP不同，不经过连接以及断开SOCKET的过程，因此，在频繁的连接及断开的情况下，UDP的数据收发能力会凸显出更好的性能。
(5).第一次调用sendto函数时，若发现相应的套接字未分配IP和端口号，则给其相应的套接字自动分配IP和端口号。
(6).调用connect函数并不意味着要与对方UDP套接字连接，只是向UDP套接字注册目标IP和端口信息。
　　1.每次调用sendto函数只需传输数据。  2. 不仅可以使用sendto、recvfrom函数，还可以使用write、read函数进行通信。
(7).    uchar_server.c      uchar_client.c

***Exercise 7.3***
(1).把建立套接字连接后进入可交换数据的状态，看作一种流。 不能， 因为对于UDP来说，两个socket不能相互连接。
(2).单方面的断开连接意味着套接字无法再发送数据。一般在对方有剩余数据未发送完成时，断开己方连接，会造成问题。 
(3).半关闭是指只关闭输出流或输入流的一个。而且，如果对输出流进行半关闭，EOF无法被传送到对方主机，己方套接字无法传送数据，但可以接收对方主机传送的数据。

***Exercise 8.4***
(1).bd
(2).如果网络上没有特别的限制，可以将与本地网络相连的DNS服务器指定为其他完好的DNS服务器。因此，东秀提议的方法可能成为解决方法。也就是说静洙可以不去网吧。
(3). 
     1. 计算机向DNS服务器查询IP地址
     2. 默认DNS服务器没有IP地址信息，因此向DNS主机发出询问
     3. DNS查询服务器向更上级的DNS服务器接收IP地址信息
     4. DNS查询服务器将查到的IP地址逐级返回给主机
     5. 网络浏览器访问接收到的IP地址网络

***Exercise 9.5***
(1). acd
(2). 网络流量未受太大影响时，不使用Nagle算法要比使用它时传输速度快。最典型的是 “传输大文件数据”。将文件数据传入输出缓冲不会花太多时间，因此，即使不使用
    Nagle算法，也会在装满输出缓冲时传输数据包。这不仅不会增加数据包的数量，反而会在无需等待ACK的前提下连续传输，因此可以大大提高传输速度。

***Exercise 10.6***
(1).cd
(2).acd
(3). *测试代码如下：*
```
    #include <stdio.h>
    #include <unistd.h>
    #include <sys/socket.h>

    int main(void)
    {
        int pid;
        int sockfd = socket(PF_INET, SOCK_STREAM, 0);

        pid = fork();

        if(pid == 0)
            printf("Child process ID: %d\n", sockfd);
        else
            printf("Parent process ID: %d\n", sockfd);
        
        return 0;
    }

    Output: 
            Child process ID: 3
            Parent process ID: 3
```
(4).僵尸进程是子进程。当子进程结束时，其返回值会传到操作系统，直到返回值被父进程接收为止，该进程一直作为僵尸进程存在。所以为了防止这种情况
发生，父进程必须明确接收子进程结束时的返回值。
(5).

```
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <signal.h>

    void ask_exit(int sig)
    { 
        char a;
        puts("Exit? Press y to confirm ", stdout);
        scanf("%c", &a);
        if((a == 'y')||a == 'Y')
            exit(1);
    }

    int main()
    {

        struct sigaction act;

        act.sa_handler = ask_exit;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;
        sigaction(SIGINT, &act, 0);

        while(1)
        {
            printf("Output a simple string.\n");
            sleep(1);
        }

        return 0;
    }
```

***Exercise 11.3***
(1).从概念上来看，进程间通信意味着两个进程可以交换数据。从内存角度看，两个进程公用内存，因为共享的内存区域存在，可以进行数据交换。
(2).要想实现IPC机制，需要共享内存，但由于两个进程间不共享内存，因此需要操作系统的帮助，也就是说，两进程共享的内存空间必须由操作系统提供。
(3).
    a.管道是由pipe函数产生的，而实际产生的管道的主体是操作系统。
    b.pipe函数通过输入参数返回管道的输入输出文件描述符。这个文件描述符在fork函数中复制到其他子进程，因此，父进程和子进程可以同时访问同一管道。
    c.管道并不管理进程间的数据通信。因此，如果数据流入管道，任何进程都可以读取数据。因此，要合理安排共享空间的输入和读取。
(4). 参考pipe3通信模型

***Exercise 12.4***
(1).复用技术是指为了提高物理设备的效率，用最少的物理要素传递最多的数据时使用的技术。同样，I/O复用是指将需要I/O的套接字捆绑在一起，利用最少限度的资源
来收发数据的技术。
(2).多进程并发服务器的服务方式是，没当客户端提出连接请求时，就会追加生成进程。但构建新进程是一项非常有负担的工作（需要大量的运算和内存空间），因此，
向众多服务器端提供服务存在一定的局限性。而复用服务器端则是将套接字的文件描述符捆绑在一起管理的方式，因此可以一个进程管理所有的I/O操作。
(3).bc
(4).服务器套接字的作用是监听所有无连接请求，即判断接收的连接请求是否存在？应该将其包含到 “读”类监听对象的集合中。
(5).Linux的文件描述符从0开始递增，因此可以找出当前文件描述符数量和最后一生成的文件描述符之间的关系。但Windows的套接字句柄并非从0开始，并且句柄的整数
值之间并无规律可循，因此需要直接保存句柄的数组和记录句柄数的数量。

***Exercise 13.4***
(1).bc
(2).readv&writev函数可以将分散保存在多个缓冲中的数据一并接受和发送，是对数据进行整合传输及发送的函数，因此可以进行更有效的数据传输。
而且，输入输出函数的调用次数也相应减少，也会产生相应的优势。
(3).同时设置MSG_PEEK选项和MSG_DONTWAIT选项，以验证输入缓冲是否存在可接收的数据。设置MSG_PEEK选项并调用recv函数时，即使读取了输入缓冲数据也不会删除。
因此，该选项通常与MSG_DONTWAIT合作，用于调用以非阻塞方式验证待读数据存在与否的函数。
(4).MSG_OOB数据的接收，在select函数中属于异常数据，即在Windows中可以通过异常处理来接收Out-of-band数据。

***Exercise 1４.4***
(1).TTL是Time to live的缩写，是决定“信息传递距离”的主要因素。TTL表现为整数，每经过一个路由器就减１。如果该值为0，该数据报就会因无法再传递而消失。TTL设置
大了会对网络流量造成不良影响，设置太小了的话，就肯能无法到大目的地。
(2).多播和广播的相同点是，两者都是以UDP形式传输数据。一次传输数据，可以向两个以上主机传送数据。
但传送的范围是不同的：广播是对局域网的广播；而多播是对网络注册机器的多播。
(3).bc
(4).多播是在路由器中进行复制。因此，即使主机数量很多，如果各主机存在相同路径，也可以通过一次数据传输到多台主机上。但TCP无论路径如何，都要根据主机数量进行数据传输。
(5).以物理网络为基础，通过软件方法实现的多播通信必备虚拟网络。

***Exercise 15.4***
(1). 1.标准I/O函数具有良好的移植性（Portability）。 2.标准I/O函数可以利用缓冲提高性能。
(2).通过标准输出函数的传输的数据不直接通过套接字的输出缓冲区发送，而是保存在标准输出函数的缓冲中，然后再用fflush函数进行输出。因此，即使调用“fputs"函数，也不能立即发送数据。
如果想保障数据传输的时效性，必须经过fflush函数的调用过程

***Exercise 16.3***
(1).abcd
(2).全部正确

***Exercise 17.3***
(1). 1.调用select函数后常见的针对所有文件描述符的循环语句。 2.每次调用select函数时都需要向该函数传递监视对象信息。
(2). select和epoll是系统函数，准确地说，是要求观察套接字变化的方式的。套接字是受操作系统进行管理的。既，select和epoll是一个有操作系统执行的函数。因此，应该将监视对象的文件描述符传递给操作系统。
(3). epoll不同于select的地方是只要将监视对象文件描述符的信息传递一个给操作系统就可以了。因此epoll克服了select方式的缺点，体现在Linux内核上保存监视对象信息的方式。
(4). 1.服务器接入者少（不需要高性能）。  2.程序需要在多种操作系统上运行，需要兼容性的时候。
(5). 条件触发的区别在于发生事件的时间点。 条件触发是只要输入缓冲中还剩有数据，就将以事件的方式再次注册。边缘触发中输入缓冲收到数据时仅注册1次该事件。即使输入缓冲中还留有数据也不会再进行注册。
(6).如果使用边缘触发方式，在输入缓冲中接收数据时，只会发生一次事件通知，而且输入缓冲中仍有数据时，不会进行通知，因此可以在数据被接收后，在想要的时间内处理数据。而且，如果分离数据的接收和处理时间点，在服务器中会有更大的灵活性。
(7).

***Exercise 18.6***
(1).系统将CPU时间分成多个微小的块后分配给了多个进程。为了分时使用CPU，需要 “上下文切换”过程。“上下文切换”是指，在CPU改变运行对象的过程中，执行准备的过程中将之前执行的进程数据移出内存，并将待执行的数据移入内存。
(2).因为线程的上下文切换不需要切换数据区和堆区。同时，可以利用数据区和堆区交换数据。
(3).进程：在操作系统构成单独执行流的单元。 线程：在进程构成单独执行流的单元。
(4).bcd
(5).d
(6). 1.调用pthread_join函数，2.调用pthread_detach函数。
(7). echo_thrserv.c
(8).如果不同步，两个以上线程同时访问内存空间，从而引发问题。相反，如果同步，由于read函数中调用了临界区的参数，可能会发生无法读取其他客户端发送过来的字符串而必须等待的情况。

***Exercise 24.3***
(1).abe
(2).a
(3).IOCP和epoll都是可以管理两个以上socket的服务器模型。即在称为观察对象的端口中，感知与IO相关的事件发生的端口，并处理相关的I/O服务器模式。如实对网络服务器来说，并不要管理两个以上的socket。因为只要完成一次请求和回答的过程，连接就会结束。因此，用IOCP和epoll提高性能，有一定的局限性