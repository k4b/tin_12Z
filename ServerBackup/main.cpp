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

using namespace std;


void obsluz(int);
void* serveConnection(void* clientSocket);

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int port_number = 11111;

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, pid;
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

    listen(sockfd,2);
    clilen = sizeof(cli_addr);
    while (true)
    {
        int socket = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

        if(fork() == 0)
        {
            close(sockfd);
            serveConnection((void *)&socket);
            exit(0);
        }
        close(socket);
    }
    close(sockfd);

    cout<<"wyszedłem z głównej pętli"<<endl;

    return 0;
}


void* serveConnection(void* arg)
{
    int* clientSocket = (int*)arg;
    if(*clientSocket < 0)
        error("ERROR on accept");

    while(true)
        obsluz(*clientSocket);
}



/******** DOSTUFF() *********************
 There is a separate instance of this function
 for each connection.  It handles all communication
 once a connnection has been established.
 *****************************************/





void obsluz (int sock)
{
    int n;
    char buffer[256];
    bzero(buffer,256);

    n = read(sock,buffer,255);
    if (n < 0) error("ERROR reading from socket");
    printf("Odebrana wiadomość: %d\n",buffer[0]);


    n = write(sock,"Odebrałem\n",18);
    if (n < 0)
        error("ERROR writing to socket");
}
