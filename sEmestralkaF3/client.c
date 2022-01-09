#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void zadajTvojTip(char* buffer, int sockfd)
{
    int n;
    n = read(sockfd, buffer, 255);
    if (n < 0)
    {
        perror("Error reading from socket");
        return;
    }
    //Vypíšeme odpoveď z buffra na štandardný výstup.
    printf("%s",buffer);
    //Načítame správu od používateľa zo štandardného vstupu do buffra.
    fgets(buffer, 255, stdin);
    //Pošleme správu cez socket servru.

    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
    {
        perror("Error writing to socket");
        return;
    }

    if (strcmp(buffer, "Koncim\n")==0) {
        close(sockfd);
        printf("Odpojeny zo servera.\n");
        exit(0);
    }

    bzero(buffer,256);
    //Načítame odpoveď od servra do buffra.
    n = read(sockfd, buffer, 255);
    if (n < 0)
    {
        perror("Error reading from socket");
        return;
    }
    //Vypíšeme odpoveď z buffra na štandardný výstup.
    printf("%s ",buffer);

    bzero(buffer,256);
    //Načítame správu od používateľa zo štandardného vstupu do buffra.
    fgets(buffer, 255, stdin);
    //Pošleme správu cez socket servru.
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
    {
        perror("Error writing to socket");
        return;
    }
    if (strcmp(buffer, "Koncim\n")==0) {
        close(sockfd);
        printf("Odpojeny zo servera.\n");
        exit(0);
    }
    bzero(buffer, 256);
}

void cakajVysledok(char* buffer, int sockfd)
{
    int n;

    bzero(buffer,256);
    //Načítame odpoveď od servra do buffra.
    n = recv(sockfd, buffer, 256, 0);
    if (n < 0)
    {
        perror("Error reading from socket");
        return;
    }
    //Vypíšeme odpoveď z buffra na štandardný výstup.

    printf("Moje skore: %s", buffer);

}

int main(int argc, char *argv[])
{
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent* server;

    char buffer[256];

    if (argc < 3)
    {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        return 1;
    }

    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "Error, no such host\n");
        return 2;
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy(
            (char*)server->h_addr,
            (char*)&serv_addr.sin_addr.s_addr,
            server->h_length
    );
    serv_addr.sin_port = htons(atoi(argv[2]));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        return 3;
    }

    if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error connecting to socket");
        return 4;
    }

    printf("Vitaj, a drzim palce.\n");


    while(1) {
        recv(sockfd, buffer, 255, 0);
        printf("%s\n", buffer);

        if (strcmp(buffer, "$S: Si na rade")==0) {
            zadajTvojTip(buffer,sockfd);
            zadajTvojTip(buffer,sockfd);
            //recv(sockfd, buffer, 255,0);
            //printf("-- %s\n", buffer);
        }
        if (strcmp(buffer, "$S: Koniec\n")==0) {
            cakajVysledok(buffer,sockfd);
            break;
        }
    }

    //cakajVysledok(buffer,sockfd);
    close(sockfd);
    return 0;
}



