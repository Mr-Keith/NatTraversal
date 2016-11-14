#include<winsock2.h>
#include<stdio.h>
#include<string.h>
#include<iostream>
using namespace std;
#pragma comment(lib,"ws2_32.lib")


#define buffize_size 1000
int main()
{
    int a;
    int port_server=3030,port_client1=3031,port_client2=3032;
WSADATA wsaData;
SOCKET sockServer;
struct sockaddr_in addrServer;
addrServer.sin_addr.S_un.S_addr=htonl(INADDR_ANY);//INADDR_ANY表示任何IP
addrServer.sin_family=AF_INET;
addrServer.sin_port=htons(port_server);//绑定端口
//检查协议栈
a=WSAStartup(MAKEWORD(2,2),&wsaData);
printf("1.%d %d\n",a,WSAGetLastError());
//建立监听socket
a=sockServer = socket(AF_INET,SOCK_STREAM,0);
printf("2.%d %d\n",a,WSAGetLastError());


//a=bind(serverListen,(SOCKADDR*)&addrServer,sizeof(SOCKADDR));
//printf("3.%d %d\n",a,WSAGetLastError());



//先接受Client1的请求和信息
struct sockaddr_in sockAddr1;
//sockAddr1.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
//sockAddr1.sin_family=AF_INET;
//sockAddr1.sin_port=htons(port_client1);//绑定端口

int sockAddrLen1 = sizeof(sockAddr1);
SOCKET sockClient1 = socket(AF_INET,SOCK_STREAM,0);

char flag=1;
setsockopt(sockClient1,SOL_SOCKET,SO_BROADCAST,(char*)&flag,sizeof(char));
setsockopt(sockClient1,SOL_SOCKET,SO_REUSEADDR,(char*)&flag,sizeof(char));

printf("3.%d %d\n",sockClient1,WSAGetLastError());
a=bind(sockServer,(SOCKADDR*)&addrServer,sizeof(SOCKADDR));
printf("4.%d %d\n",a,WSAGetLastError());

listen(sockServer,1);

char bufferRecive1[buffize_size];memset(bufferRecive1,'\0',sizeof(bufferRecive1));
char b[20];
sockClient1=accept(sockServer,(SOCKADDR*)&sockAddr1,&sockAddrLen1);
printf("5.%d %d\n",sockClient1,WSAGetLastError());
a=recv(sockClient1,bufferRecive1,sizeof(bufferRecive1),0);
printf("6.%d %d\n",a,WSAGetLastError());
printf("bufferReceive1:%s,",bufferRecive1);
printf(" Client1 IP:[%s]",inet_ntoa(sockAddr1.sin_addr));
printf(",Port:%d\n",sockAddr1.sin_port);
//printf("C1 IP:%s,PORT %s",inet_ntoa(sockAddr1.sin_addr),ntohs(sockAddr1.sin_port));

//向Client2发送C1的IP和PORT
struct sockaddr_in sockAddr2;
int sockAddrLen2 = sizeof(sockAddr2);
SOCKET sockClient2 = socket(AF_INET,SOCK_STREAM,0);

char bufferSend[buffize_size]; memset(bufferSend,'\0',sizeof(bufferSend));
//处理buffersend
char* IP1=inet_ntoa(sockAddr1.sin_addr);
char k[20];itoa(sockAddr1.sin_port,k,10);
sockClient2=accept(sockServer,(SOCKADDR*)&sockAddr2,&sockAddrLen2);
printf("7.%d %d\n",sockClient2,WSAGetLastError());//链接C2
strcpy(bufferSend,IP1);
strcat(bufferSend,"$");//分割标记
strcat(bufferSend,k);
printf("8.buffersend: %s\n",bufferSend);

int flag2;
flag2 = sendto(sockClient2,bufferSend,buffize_size,0,(SOCKADDR*)&sockAddr2,sockAddrLen2);

//接受C2的回复
char bufferRecive2[buffize_size];
a=recv(sockClient2,bufferRecive2,sizeof(bufferRecive2),0);
printf("9.%d %d\n",a,WSAGetLastError());
printf("10.bufferReceive2: %s\n",bufferRecive2);
if(strcmp(bufferRecive2,"OK")==0){
    //回复C1  C2的IP和PORT

char bufferSend2[buffize_size];memset(bufferSend2,'\0',sizeof(bufferSend));
char* IP2=inet_ntoa(sockAddr2.sin_addr);
itoa(sockAddr2.sin_port,k,10);
strcpy(bufferSend2,IP2);
strcat(bufferSend2,"$");//分割标记
strcat(bufferSend2,k);
printf("11.buuerSend2(Client2's IP PORT) to C1: %s\n",bufferSend2);
sendto(sockClient1,bufferSend2,buffize_size,0,(SOCKADDR*)&sockAddr1,sockAddrLen1);

}


closesocket(sockClient1);
closesocket(sockClient2);


WSACleanup();
getchar();
}
