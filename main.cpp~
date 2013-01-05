/* A simple server in the internet domain using TCP
   The port number is passed as an argument
   This version runs forever, forking off a separate
   process for each connection
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <iostream>
#include <limits.h>
#include <list>

using namespace std;

pthread_t threads[100];
int numThread = 0;
//list<pthread_t> threadsList;

bool obsluz(int);
void* serveConnection(void* clientSocket);
void* serverIPv4Start(void* arg);
void* serverIPv6Start(void* arg);

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int port_number = 11111;

int main(int argc, char *argv[])
{
    //pthread_create(&threads[numThread++], NULL, serverIPv4Start, NULL);       //todo przekurwi przez zakres numThread++;
    //pthread_create(&threads[numThread++], NULL, serverIPv6Start, NULL);       //todo przekurwi przez zakres numThread++;


    //sleep(1000000);

    int pid;
    pid = fork();

    if(pid != 0)
        serverIPv6Start(NULL);
    else
        serverIPv4Start(NULL);

    //serverIPv4Start();
    //serverIPv6Start(NULL);
    return 0;
}


void* serverIPv6Start(void* arg)
{
    int n;
    struct sockaddr_in6 from;
   	struct sockaddr_in6 sin;

   	memset((char *)&sin, 0, sizeof(sin));
   	//sin.sin6_len = sizeof(sin);
   	sin.sin6_port = htons(22222);
   	sin.sin6_addr = in6addr_any;
   	sin.sin6_family = AF_INET6;

   	n = socket(AF_INET6, SOCK_STREAM, 0);
   	if (bind(n, (struct sockaddr *)&sin, sizeof(sin)) < 0)
   	{
   		error("ERROR on binding");
   	}

   	listen(n, 5);
   	for (;;)
   	{
   	    cout<<"oczekuje na złączenie"<<endl;
        int g;
        socklen_t len = sizeof(from);

        g = accept(n, (struct sockaddr *) &from, &len);

        //cout<<"cos tam złączyłem"<<endl;

        //threadsList.push_back(0);
        int idWatku;
        idWatku = pthread_create(&threads[numThread++], NULL, serveConnection, &g);
        printf("idWatku: %d \n", idWatku);
   	}

   	return NULL;
}






/*
void* serverIPv6Start()
{
    int sockfd, newsockfd, pid;     //todo pid nie potrzebny
    socklen_t clilen;
    struct sockaddr_in6 serv_addr, cli_addr;

    sockfd = socket(AF_INET6, SOCK_STREAM, 0);

    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin6_family = AF_INET6;
    serv_addr.sin6_addr = in6addr_any;
    serv_addr.sin6_port = htons(22222);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)    //todo czy dobra struktura sockaddr?
        error("ERROR on binding");

    listen(sockfd, 20);
    clilen = sizeof(cli_addr);

    while (true)
    {
        cout<<"oczekuje na złączenie"<<endl;

        int socket = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

        cout<<"cos tam złączyłem"<<endl;

        //threadsList.push_back(0);
        int idWatku;
        idWatku = pthread_create(&threads[numThread++], NULL, serveConnection, &socket);
        printf("idWatku: %d \n", idWatku);
    }
}
*/

void* serverIPv4Start(void* arg)
{
    int sockfd;     //todo pid nie potrzebny
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
        error("ERROR opening socket");

    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port_number);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    listen(sockfd, 20);
    clilen = sizeof(cli_addr);
    while (true)
    {
        int socket = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        //threadsList.push_back(0);
        int idWatku;
        idWatku = pthread_create(&threads[numThread++], NULL, serveConnection, &socket);
        printf("idWatku: %d \n", idWatku);
    }
    return NULL;
}

void* serveConnection(void* arg)
{
    int* clientSocket = (int*)arg;
    printf("Przydzielony socket: %d\n", *clientSocket);

    if(*clientSocket < 0)
        error("ERROR on accept");

    while(true)
        if(obsluz(*clientSocket))
        {
            close(*clientSocket);
            break;
        }
    printf("Zamknąłem gniazdo\n");

    return NULL;
}



bool obsluz (int sock)
{
    int n;
    char buffer[256];
    bzero(buffer,256);

    n = read(sock,buffer,255);

    if(buffer[0] == 0)
    {
        return true;
    }

    printf("Odebrana wiadomość: %s\n",buffer);

    n = write(sock,"Odebrałem\n",18);

    return false;
}
