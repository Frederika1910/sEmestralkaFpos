#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>

#define MAX_HRACOV 5


int pary[10][10];
int pocetParov;
int otestovat[2][2];
int poradieKarticky = 0;
int skoreHracov[MAX_HRACOV];

bool otocilRovnaku = false;
bool jeKoniec = false;

bool odpojilSa = false;

int idHraca = 0;
int pocetUhadnuti = 0;

typedef struct da {
    int id;
    int * sockety;
    bool * koniec;
    bool * somNaRade;
    pthread_mutex_t * mutex;
    pthread_cond_t * necakam;
    int * hracNaRade;
    int pocetHracov;
} DATA;

void zistiSpravnost()
{
    int x1, x2, y1, y2;
    x1 = otestovat[0][0];
    y1 = otestovat[0][1];
    x2 = otestovat[1][0];
    y2 = otestovat[1][1];

    if (pary[x1][y1] == pary[x2][y2] && pary[x1][y1] != 0)
    {
        printf("ROVNAJU SA\n");
        pary[x1][y1] = 0;
        pary[x2][y2] = 0;
        skoreHracov[idHraca]++;
        pocetUhadnuti++;
        otocilRovnaku = true;
        if (pocetUhadnuti == pocetParov*2)
        {
            jeKoniec = true;
        }
    } else {
        printf("NEROVNAJU SA\n");
        otocilRovnaku = false;
    }
}


void hra(char * buffer, int newsockfd)
{
    int n;
    const char* msg = "$S: Zadajte suradnicu X:";
    //Pošleme odpoveď klientovi.
    sleep(0);                                   //s tymto naozaj neviem, ze co
    n = write(newsockfd, msg, strlen(msg)+1);
    if (n < 0)
    {
        perror("Error writing to socket");
        return;
    }

    //bzero(buffer,256);

    //Načítame správu od klienta cez socket do buffra.
    n = read(newsockfd, buffer, 255);
    if (n < 0)
    {
        perror("Error reading from socket");
        return;
    }

    if (strcmp(buffer, "Koncim\n")==0) {
        printf("Hrac %d sa odpojil.\n", newsockfd);
        odpojilSa = true;
        return;
    }

    //Vypíšeme prijatú správu na štandardný výstup.
    printf("$%d: Zadana suradnica x hracom: %s", newsockfd, buffer);
    otestovat[poradieKarticky][0] = atoi(buffer);
    const char* msg2 = "$S: Zadajte suradnicu Y:";
    //Pošleme odpoveď klientovi.
    n = write(newsockfd, msg2, strlen(msg2)+1);
    if (n < 0)
    {
        perror("Error writing to socket");
        return;
    }

    bzero(buffer,256);
    //Načítame správu od klienta cez socket do buffra.
    n = read(newsockfd, buffer, 255);
    if (n < 0)
    {
        perror("Error reading from socket");
        return;
    }
    //Vypíšeme prijatú správu na štandardný výstup.
    printf("$%d: Zadana suradnica y hracom: %s",newsockfd, buffer);
    otestovat[poradieKarticky][1] = atoi(buffer);
    if (poradieKarticky == 0)
    {
        poradieKarticky++;
    } else{
        poradieKarticky = 0;
    }
}

void otocKarticku1(int x, int y)
{
    for (int i = 0; i < pocetParov; i++) {
        for (int k = 0; k < pocetParov; k++) {
            if (pary[i][k] == 0)
            {
                printf("   ");
            } else {
                if (x==i && y==k)
                {
                    printf("%d  ",pary[i][k]);
                } else {
                    printf("X  ");
                }
            }
        }
        printf("\n");
    }
}

void otocKarticky2()
{
    for (int i = 0; i < pocetParov; i++) {
        for (int k = 0; k < pocetParov; k++) {
            if (pary[i][k] == 0)
            {
                printf("   ");
            } else {
                if ((otestovat[0][0]==i && otestovat[0][1]==k) || (otestovat[1][0]==i && otestovat[1][1]==k))
                {
                    printf("%d  ",pary[i][k]);
                } else {
                    printf("X  ");
                }
            }
        }
        printf("\n");
    }
}

void vypisHraciuPlochu()
{

    for (int i = 0; i < pocetParov; i++) {
        for (int k = 0; k < pocetParov; k++) {
            if (pary[i][k] != 0)
            {
                printf("X  ");
            } else {
                printf("   ");
            }
        }
        printf("\n");
    }
}

void vytvorPexeso()
{
    printf("Vytvaram pexeso.\n");
    //vynulovanie mapy pre pexeso
    for (int j = 0; j < pocetParov; ++j) {
        for (int k = 0; k < pocetParov; ++k) {
            pary[j][k] = 0;
        }
    }
    int x,y;
    //nahodne porozhadzovane cisla v mape
    for (int j = 0; j < (pocetParov*pocetParov)/2; j++) {
        do {
            x = rand()%pocetParov;
            y = rand()%pocetParov;
        } while (!(pary[x][y]==0));
        pary[x][y] = j+1;

        do {
            x = rand()%pocetParov;
            y = rand()%pocetParov;
        } while (!(pary[x][y]==0));
        pary[x][y] = j+1;
    }

    //printf("Vytvoril som dvojice\n");

    for (int i = 0; i < pocetParov; ++i) {
        for (int k = 0; k < pocetParov; ++k) {
            printf("%d  ",pary[i][k]);
        }
        printf("\n");
    }
    printf("\n");
    vypisHraciuPlochu();
}

void vypisHodnotenie(char* buffer, int newsockfd, int id)
{
    int n;
    sleep(0);
    bzero(buffer,256);
    sprintf(buffer, "%d", skoreHracov[id]);

    //Pošleme odpoveď klientovi.
    n = send(newsockfd, buffer, 256, 0);
    if (n < 0)
    {
        perror("Error writing to socket");
        return;
    }
}


void * hrajHru(void * data) {
    DATA * d = data;

    char buffer[256];
    char * buff = buffer;

    pthread_mutex_lock(d->mutex);
    //cyklus ide kym neuhadnu vsetky pary
    while (!(*d->koniec) && (d->pocetHracov) > 0) {
        //pokial hrac nie je na rade caka
        while (!(*d->somNaRade)) {
            pthread_cond_wait(d->necakam, d->mutex);
        }
        (*d->somNaRade) = false;
        (*d->koniec) = jeKoniec;
        printf("hrac na rade %d\n", (*d->hracNaRade));
        idHraca = (*d->hracNaRade);

        if (!(*d->koniec))
        {
            printf("hrac na rade %d\n", d->sockety[(*d->hracNaRade)]);

            const char * msg = "$S: Si na rade";
            send(d->sockety[(*d->hracNaRade)], msg, strlen(msg)+1, 0);

            hra(buff, d->sockety[(*d->hracNaRade)]);
            if (odpojilSa) {
                if ((*d->hracNaRade+1 < d->pocetHracov)) {
                    (*d->hracNaRade)++;
                } else {
                    (*d->hracNaRade) = 0;
                }
                d->pocetHracov--;
                (*d->somNaRade) = true;
                odpojilSa = false;
                continue;
            }
            otocKarticku1(otestovat[0][0],otestovat[0][1]);
            hra(buff, d->sockety[(*d->hracNaRade)]);
            //index hraca v poli, kde mame ulozene jednotlive sokety

            otocKarticky2();
            zistiSpravnost();

            if (!otocilRovnaku)
            {
                printf("akt pocet hracov %d\n", d->pocetHracov);
                if ((*d->hracNaRade+1 < d->pocetHracov)) {
                    (*d->hracNaRade)++;
                } else {
                    (*d->hracNaRade) = 0;
                }

            }
            //pthread_mutex_unlock(d->mutex);
            vypisHraciuPlochu();
        } else {
            const char * msg = "$S: Koniec\n";
            send(d->sockety[(*d->hracNaRade)], msg, strlen(msg)+1, 0);
            vypisHodnotenie(buff,d->sockety[(*d->hracNaRade)],(*d->hracNaRade));
            if ((*d->hracNaRade+1 < d->pocetHracov)) {
                (*d->hracNaRade)++;
            } else {
                (*d->hracNaRade) = 0;
            }
        }
        pthread_mutex_unlock(d->mutex);
            pthread_mutex_lock(d->mutex);
            pthread_cond_signal(d->necakam);
            (*d->somNaRade) = true;
            //(*d->koniec) = jeKoniec;
            pthread_mutex_unlock(d->mutex);
    }
    printf("?????????? %d\n", d->pocetHracov);
    return NULL;
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    int sockfd;

    socklen_t cli_len;
    struct sockaddr_in serv_addr, cli_addr;

    if (argc < 2) {
        fprintf(stderr, "usage %s port\n", argv[0]);
        return 1;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        return 1;
    }

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error binding socket address");
        return 2;
    }

    listen(sockfd, 5);

    printf("-----------HRA ZACINA--------------\n");

    int pocetHracov;
    printf("$S: Zadaj pocet hracov:");
    scanf("%d", &pocetHracov);
    //printf("Pocet hracov %d\n", pocetHracov);

    while (pocetHracov < 0) {
        printf("$S: Nemozes zadat negativny pocet hracov.\n");
        printf("$S: Zadaj pocet hracov:");
        scanf("%d", &pocetHracov);
    }

    while (pocetHracov > MAX_HRACOV) {
        printf("$S: Maximalny pocet hracov je %d.\n", MAX_HRACOV);
        printf("$S: Zadaj pocet hracov:");
        scanf("%d", &pocetHracov);
    }
    //max 10 riadkov
    pary[10][10];
    int velkostMatice;
    printf("$S: Zadaj velkost matice:");
    scanf("%d", &pocetParov);
    if (pocetParov%2 != 0)
    {
        pocetParov++;
    }

    for (int i = 0; i < MAX_HRACOV; ++i) {
        skoreHracov[i] = 0;
    }
    //int pocetUhadnuti;
    pthread_t vlaknoKlient[pocetHracov];

    int newsockfd[pocetHracov];
    bool koniec = false;
    bool somNaRade = true;
    int hracNaRade = 0;

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_t necakam;
    pthread_cond_init(&necakam, NULL);

    vytvorPexeso();

    //while(1) {
    int a = 2;

    DATA dat[a];
    for (int i = 0; i < a; ++i) {

        cli_len = sizeof(cli_addr);
        //pre kazdeho hraca sa vytvori nove spojenie, ktore sa ulozi do pila
        newsockfd[i] = accept(sockfd, (struct sockaddr *) &cli_addr, &cli_len);
        printf("$S: Hrac s portom %d sa pripojil.\n", newsockfd[i]);

    }

    int in = 0;
    for (int i = 0; i < a; ++i) {
        dat[i].id = i;
        dat[i].sockety = newsockfd;
        dat[i].koniec = &koniec;
        dat[i].somNaRade = &somNaRade;
        dat[i].mutex = &mutex;
        dat[i].necakam = &necakam;
        dat[i].hracNaRade = &hracNaRade;
        dat[i].pocetHracov = pocetHracov;
        //vytvorenie vlakien pre hraca
        pthread_create(&vlaknoKlient[i], NULL, &hrajHru, &dat[i]);

    }
    printf("/////\n");
    for (int i = 0; i < a; ++i) {
        printf("pocet hracov %d\n", pocetHracov);
        pthread_join(vlaknoKlient[i], NULL);
    }

    //}
    printf("-----------HRA KONCI--------------\n");

    int max = 0;
    int hrac = 0;
    for (int i = 0; i < a; ++i) {
        if (max < skoreHracov[i])
            max = skoreHracov[i];
        hrac = i;
    }
    for (int i = 0; i < a; ++i) {
        if (skoreHracov[i] == max)
        {
            printf("Hrac %d [index od 0] VYHRAL s poctom bodov %d.\n", i, max);
        }
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&necakam);
    //pojde toto tolko krat???
    for (int i = 0; i < a; ++i) {
        close(newsockfd[i]);
    }

    close(sockfd);

    return 0;
}
