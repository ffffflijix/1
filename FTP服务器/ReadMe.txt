本项目文件夹包含三个文件
  1.head.h：本文件是FTP_server和FTP_client的头文件
  2.FTP_server是FTP服务器的服务器端
    使用socket套接字与UDP网络协议与客户端通信
    功能包括ls、get <filename>、put <filename>、bye
      ls：向客户端发送ls命令的结果，发送服务器指定目录下的文件名
      get <filename>：向客户端执行get命令，发送指定filename的文件
      put <filename>：向客户端put命令，接收指定filename的文件
      bye：表示自己已经断开连接
  3.FTP_client是FTP服务器的客户端
    使用socket套接字与UDP网络协议与客户端通信
    功能包括ls、get <filename>、put <filename>、bye
      ls：向服务器发送ls命令的结果，获取服务器指定目录下的文件名
      get <filename>：向服务器发送get命令，接收指定filename的文件
      put <filename>：向服务器发送put命令，发送指定filename的文件
      bye：表示自己已经断开连接
