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

    listen(sockfd,20);
    clilen = sizeof(cli_addr);
    while (true)
    {
        int socket = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        //threadsList.push_back(0);
        pthread_create(&threads[numThread++], NULL, serveConnection, &socket);
    }
    return 0;
}


void* serveConnection(void* arg)
{
    int* clientSocket = (int*)arg;
    printf("Przydzielony socket: %d\n", *clientSocket);

    if(*clientSocket < 0)
        error("ERROR on accept");

    while(true)
        if(obsluz(*clientSocket))
            break;
    printf("Zamknąłem gniazdo\n");
}



/******** DOSTUFF() *********************
 There is a separate instance of this function
 for each connection.  It handles all communication
 once a connnection has been established.
 *****************************************/





bool obsluz (int sock)
{
    int n;
    char buffer[256];
    bzero(buffer,256);



    //n = read(sock,buffer,255);
    n = recv(sock, buffer, 256, 0);

    if (n < 0)
        error("ERROR reading from socket");


    if(buffer[0] == 'e' && buffer[1] == 'x' && buffer[2] == 'i' && buffer[3] == 't')
    {
        printf("buffert[0] = %d", buffer[0]);

        return true;
    }



    if(buffer[0] != 0)
    {
        printf("Odebrana wiadomość: %s\n",buffer);

        //n = write(sock,"Odebrałem\n",18);
        n = send(sock, "Odebrałem\n", 11, 0);

        if (n < 0)
            error("ERROR writing to socket");
    }
    else(buffer[0] == 0 && (buffer[1] == 0))
    {
//        printf("kod :");
//        for(int i=0; i<n; i++)
//        {
//            printf("%d", buffer[i]);
//        }
        close(sock);
        return true;
    }

    return false;
}
