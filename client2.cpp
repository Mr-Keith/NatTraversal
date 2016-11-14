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
SOCKET sockClient2;
a=sockClient2=socket(AF_INET,SOCK_STREAM,0);
printf("2.%d %d\n",a,WSAGetLastError());

SOCKADDR_IN addrClient2;
addrClient2.sin_addr.S_un.S_addr=htonl(INADDR_ANY);//Ŀ��IP(127.0.0.1�ǻ��͵�ַ)
addrClient2.sin_family=AF_INET;
addrClient2.sin_port=htons(port_client2);

int flag=1;
setsockopt(sockClient2,SOL_SOCKET,SO_BROADCAST,(char*)&flag,sizeof(int));
setsockopt(sockClient2,SOL_SOCKET,SO_REUSEADDR,(char*)&flag,sizeof(int));
//���Ӷ˿�
a=bind(sockClient2,(SOCKADDR*)&addrClient2,sizeof(SOCKADDR));
printf("3.%d %d\n",a,WSAGetLastError());

a=connect(sockClient2,(SOCKADDR*)&addrServer,sizeof(SOCKADDR));
printf("4.%d %d\n",a,WSAGetLastError());


//���ܲ��������ģ��ҵ�C1��IP��PORT
char bufRecv[buffize_size];
a=recv(sockClient2,bufRecv,sizeof(bufRecv),0);
printf("5.%d %d\n",a,WSAGetLastError());
printf("6.bufferrecrive: %s\n",bufRecv);
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
    printf("7.Client1's ip: %s, port: %s\n",ip,port);
int port1=atoi(port);

//��C1�������ݰ�����ԽNAT_B
SOCKET sockClient1=socket(AF_INET,SOCK_STREAM,0);
SOCKADDR_IN addrClient1;
addrClient1.sin_addr.S_un.S_addr=inet_addr(ip);
addrClient1.sin_family=AF_INET;
addrClient1.sin_port=htons(port1);//���Ӷ˿�
char bufSend[buffize_size]; memset(bufSend,'\0',sizeof(bufSend));
strcpy(bufSend,"I'M CLIENT2");
printf("8.%s\n",bufSend);
int flag2;
flag2 = sendto(sockClient1,bufSend,buffize_size,0,(SOCKADDR*)&addrClient1,sizeof(addrClient1));

//�ظ�Server׼�����
memset(bufSend,'\0',sizeof(bufSend));
strcpy(bufSend,"OK");
flag2 = sendto(sockClient2,bufSend,buffize_size,0,(SOCKADDR*)&addrServer,sizeof(addrServer));
closesocket(sockClient2);
//NAT_B��Խ���
closesocket(sockClient2);

SOCKADDR_IN addrC1;
SOCKADDR_IN addrC2;
SOCKET sockC1;
SOCKET sockC2;
a=sockC1=socket(AF_INET,SOCK_STREAM,0);
a=sockC2=socket(AF_INET,SOCK_STREAM,0);
printf("9.%d %d\n",a,WSAGetLastError());

addrC2.sin_addr.S_un.S_addr=addrClient2.sin_addr.S_un.S_addr;//Ŀ��IP(127.0.0.1�ǻ��͵�ַ)
addrC2.sin_family=AF_INET;
addrC2.sin_port=addrClient2.sin_port;



setsockopt(sockC2,SOL_SOCKET,SO_BROADCAST,(char*)&flag,sizeof(char));
setsockopt(sockC2,SOL_SOCKET,SO_REUSEADDR,(char*)&flag,sizeof(char));

a=bind(sockC2,(SOCKADDR*)&addrC2,sizeof(SOCKADDR));
printf("10.%d %d\n",a,WSAGetLastError());


a=listen(sockC2,1);
printf("11.%d %d\n",a,WSAGetLastError());
int len=sizeof(addrC1);
sockC1=accept(sockC2,(SOCKADDR*)&addrC1,&len);
printf("12.%d %d\n",sockC1,WSAGetLastError());

char message[buffize_size];
a=recv(sockC1,message,sizeof(message),0);
printf("13.%d %d\n",a,WSAGetLastError());
if(strcmp(message,"")!=0){
    fstream fileRcv("FileRcv.txt",ios::out);
    fileRcv<<message<<endl;
    cout<<"file has received";
    fileRcv.close();
};
closesocket(sockC1);
closesocket(sockC2);
WSACleanup();

getchar();


}
