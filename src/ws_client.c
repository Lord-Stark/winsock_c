#include <stdio.h>
#include <winsock2.h>

int main()
{
    WSADATA wsaData;

    int WSAStartupRet;
    int connectRet;
    int recvRet;
    int sendRet;

    SOCKET Sock;
    struct sockaddr_in sockAddr;

    char recvBuffer[MAXBYTE];
    const char *message = "Hello, server! This is a message from client.\n";

    // initialization
    WSAStartupRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if(WSAStartupRet != 0)
    {
        fprintf(stderr, "WSAStartup Function Failed.\n");
        exit(-1);
    }

    // create socket
    Sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (Sock == INVALID_SOCKET)
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

    // connect
    connectRet = connect(Sock, (SOCKADDR *)&sockAddr, sizeof(SOCKADDR));
    if(connectRet != 0)
    {
        fprintf(stderr, "Connect Function Failed. Error Code: %d.\n", WSAGetLastError());
        closesocket(Sock);
        WSACleanup();
        exit(-1);
    }

    // receive messages from server
    memset(recvBuffer, 0, MAXBYTE);
    recvRet = recv(Sock, recvBuffer, MAXBYTE, 0);
    if(recvRet <= 0)
    {
        fprintf(stderr, "Recv Function Failed. Error Code: %d.\n", WSAGetLastError());
        closesocket(Sock);
        WSACleanup();
        exit(-1);
    }
    fprintf(stdout, "Message form server: %s\n", recvBuffer);

    // send messages to server
    sendRet = send(Sock, message, strlen(message), 0);
    if(sendRet <= 0)
    {
        fprintf(stderr, "Send Function Failed. Error Code: %d.\n", WSAGetLastError());
        closesocket(Sock);
        WSACleanup();
        exit(-1);
    }

    // close client socket
    closesocket(Sock);

    // WSA cleanup
    WSACleanup();

    return 0;
}