#include<winsock2.h>
#include<stdio.h>
#include<string.h>
#include<fstream.h>
#include<iostream.h>
#pragma comment(lib,"ws2_32.lib")

#define buffize_size 1000
int main()
{
    int a;
    int port_server=3030,port_client1=3031,port_client2=3032;
WSADATA wsaData;
//�ͻ���Socket
a=WSAStartup(MAKEWORD(2,2),&wsaData);
printf("1.%d %d\n",a,WSAGetLastError());
//�½��ͻ���socket

SOCKADDR_IN addrServer;//����˵�ַ,����Ҫ���ӵķ���˵�ַ
addrServer.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");//Ŀ��IP(127.0.0.1�ǻ��͵�ַ)
addrServer.sin_family=AF_INET;
addrServer.sin_port=htons(port_server);//���Ӷ˿�

//���ӵ������

SOCKET sockClient1=socket(AF_INET,SOCK_STREAM,0);
int flag=1;
setsockopt(sockClient1,SOL_SOCKET,SO_BROADCAST,(char*)&flag,sizeof(int));
setsockopt(sockClient1,SOL_SOCKET,SO_REUSEADDR,(char*)&flag,sizeof(int));
printf("2.%d %d\n",a,WSAGetLastError());

SOCKADDR_IN addrClient1;
addrClient1.sin_addr.S_un.S_addr=htonl(INADDR_ANY);//Ŀ��IP(127.0.0.1�ǻ��͵�ַ)
addrClient1.sin_family=AF_INET;
addrClient1.sin_port=htons(port_client1);


//���Ӷ˿�
a=bind(sockClient1,(SOCKADDR*)&addrClient1,sizeof(SOCKADDR));
printf("3.%d %d\n",a,WSAGetLastError());
a=connect(sockClient1,(SOCKADDR*)&addrServer,sizeof(SOCKADDR));
printf("4.%d %d\n",a,WSAGetLastError());
//������������
char bufferSend[]="hello,I'M Client1";
a=sendto(sockClient1,bufferSend,sizeof(bufferSend),0,(SOCKADDR*)&addrServer,sizeof(addrServer));
printf("5.%d %d\n",a,WSAGetLastError());
char b;
printf("6.%s\n",itoa(addrClient1.sin_port,&b,10));

//����Server��Ӧ
char bufRecv[buffize_size];
a=recv(sockClient1,bufRecv,sizeof(bufRecv),0);
printf("7.%d %d\n",a,WSAGetLastError());
printf("8.%s\n",bufRecv);
char ip[20],port[10];
int i;
    for (i=0;i<strlen(bufRecv);i++)
        if (bufRecv[i] != '$')
            ip[i] = bufRecv[i];
        else break;
    ip[i] = '\0';
    int j;
    for (j=i+1;j<strlen(bufRecv)||(bufRecv[j]!=NULL);j++)
        port[j - i - 1] = bufRecv[j];
    port[j - i - 1] = '\0';
    printf("9.Client2's ip:%s , port: %s\n",ip,port);
int port2=atoi(port);


//NAT ��Խ��ϣ���ʼ��CLIENT2ͨ��


closesocket(sockClient1);

SOCKADDR_IN addrClient2;
addrClient2.sin_addr.S_un.S_addr=inet_addr(ip);
addrClient2.sin_family=AF_INET;
addrClient2.sin_port=port2;
//���Ӷ˿�

SOCKADDR_IN addrC1;
SOCKET sockC1;

a=sockC1=socket(AF_INET,SOCK_STREAM,0);

printf("10.%d %d\n",a,WSAGetLastError());
a=bind(sockC1,(SOCKADDR*)&addrClient1,sizeof(SOCKADDR));
printf("11.%d %d\n",a,WSAGetLastError());
a=connect(sockC1,(SOCKADDR*)&addrClient2,sizeof(SOCKADDR));
printf("12.%d %d\n",a,WSAGetLastError());


fstream fileout;
char filename[20];
char filesend[buffize_size],ch;
memset(filesend,'\0',20);
cout<<"enter file name:";
cin>>filename;
fileout.open(filename,ios::in);
if(!fileout) printf("error:file does not exist\n");
while(fileout&&!fileout.eof())  fileout.getline(filesend,buffize_size,'\n');
fileout.close();
a=sendto(sockC1,filesend,sizeof(filesend),0,(SOCKADDR*)&addrClient2,sizeof(addrClient2));
printf("13.%d %d\n",a,WSAGetLastError());
closesocket(sockC1);
WSACleanup();

//�ر�socket
getchar();
getchar();

}
