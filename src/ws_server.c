#include <stdio.h>
#include <winsock2.h>

int main()
{
    WSADATA wsaData;

    int WSAStartupRet;
    int bindRet;
    int listenRet;
    int sendRet;
    int recvRet;

    int nSize;

    SOCKET servSock;
    SOCKET clntSock;
    SOCKADDR clntAddr;
    struct sockaddr_in sockAddr;

    char recvBuffer[MAXBYTE];
    const char *str = "Hello, client! This is a message from server.\n";

    // initialization
    WSAStartupRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if(WSAStartupRet != 0)
    {
        fprintf(stderr, "WSAStartup Function Failed.\n");
        exit(-1);
    }

    // create socket
    servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (servSock == INVALID_SOCKET)
    {
        fprintf(stderr, "Socket Function Failed. Error Code: %d.\n", WSAGetLastError());
        WSACleanup();
        exit(-1);
    }

    // set IP address and port
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockAddr.sin_port = htons(1234);

    // bind
    bindRet = bind(servSock, (SOCKADDR *)&sockAddr, sizeof(SOCKADDR));
    if (bindRet == -1)
    {
        fprintf(stderr, "Bind Function Failed. Error Code: %d.\n", WSAGetLastError());
        closesocket(servSock);
        WSACleanup();
        exit(-1);
    }

    // listen
    listenRet = listen(servSock, 1);
    if (listenRet == -1)
    {
        fprintf(stderr, "Listen Function Failed. Error Code: %d.\n", WSAGetLastError());
        closesocket(servSock);
        WSACleanup();
        exit(-1);
    }

    // accept
    nSize = sizeof(SOCKADDR);
    clntSock = accept(servSock, (SOCKADDR *)&clntAddr, &nSize);
    if (clntSock == -1)
    {
        closesocket(servSock);
        WSACleanup();
        exit(-1);
    }

    // close server socket
    closesocket(servSock);

    // send messages to client
    sendRet = send(clntSock, str, strlen(str), 0);
    if(sendRet <= 0)
    {
        fprintf(stderr, "Send Function Failed. Error Code: %d.\n", WSAGetLastError());
        closesocket(clntSock);
        WSACleanup();
        exit(-1);
    }

    // receive messages from client
    memset(recvBuffer, 0, MAXBYTE);
    recvRet = recv(clntSock, recvBuffer, MAXBYTE, 0);
    if(recvRet <= 0)
    {
        fprintf(stderr, "Recv Function Failed. Error Code: %d.\n", WSAGetLastError());
        closesocket(clntSock);
        WSACleanup();
        exit(-1);
    }

    fprintf(stdout, "Message form client: %s\n", recvBuffer);

    // close client socket
    closesocket(clntSock);

    // WSA cleanup
    WSACleanup();

    return 0;
}