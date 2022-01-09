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
#define VELKOST_MATICE 4

int pary[VELKOST_MATICE][VELKOST_MATICE];
int pocetParov;

int otestovat[2][2];
int poradieKarticky = 0;

int skoreHracov[MAX_HRACOV];

int x = 0;
int y = 0;


typedef struct da {
    int id;
    int * sockety;
    bool * koniec;
    bool * somNaRade;
    pthread_mutex_t * mutex;
    pthread_cond_t * necakam;
    int * hracNaRade;
    int * pocetHracov;
    bool odpojilSa;
    int hornaHranica;
    int * stavy;
    int * pom;
} DATA;

bool zistiSpravnost(int * stavy, int dlzka, int * pom)
{
    int prvaKarticka = -1;
    int druhaKarticka = -1;
    int indexPrvaKarticka = -1;
    int indexDruhaKarticka = -1;

    //ak je v poli stavy 1 znamena, ze je karticka otocena, tie karticky kontrolujeme
    for (int i = 0; i < dlzka*dlzka; ++i) {
        if (stavy[i] == 1) {
            if (prvaKarticka == -1) {
                prvaKarticka = pom[i];
                indexPrvaKarticka = i;
            } else {
                druhaKarticka = pom[i];
                indexDruhaKarticka = i;
            }
        }
    }

    //ak su karticky zhodne ich stav sa nastavi na -1 = uhadnute
    if (prvaKarticka == druhaKarticka && prvaKarticka != -1) {
        stavy[indexPrvaKarticka] = -1;
        stavy[indexDruhaKarticka] = -1;
        return true;
    }
    //inak sa stav karticok vrati na 0 = neotocene
    stavy[indexPrvaKarticka] = 0;
    stavy[indexDruhaKarticka] = 0;
    return false;
}

bool uzOtocenaKarta(int * stavy, int x, int y)
{
    int index = y + (x * pocetParov);
    if (stavy[index] == -1)
    {
        return true;
    } else{
        return false;
    }
}

int hra(char * buffer, int newsockfd, bool * odpojilSa)
{
    int n;
    const char* msg = "$S: Zadajte suradnicu X:";
    //Pošleme odpoveď klientovi.
    sleep(1);                                   //s tymto naozaj neviem, ze co
    n = write(newsockfd, msg, strlen(msg)+1);
    if (n < 0)
    {
        perror("Error writing to socket");
        return -1;
    }

    //bzero(buffer,256);

    //Načítame správu od klienta cez socket do buffra.
    n = read(newsockfd, buffer, 255);
    if (n < 0)
    {
        perror("Error reading from socket");
        return -1;
    }

    if (strcmp(buffer, "Koncim\n")==0) {
        printf("Hrac %d sa odpojil.\n", newsockfd);
        *odpojilSa = true;
        return -1;
    }

    //Vypíšeme prijatú správu na štandardný výstup.
    printf("$%d: Zadana suradnica x hracom: %s", newsockfd, buffer);
    //otestovat[poradieKarticky][0] = atoi(buffer);


    x = atoi(buffer);
    otestovat[poradieKarticky][0] = x;

    const char* msg2 = "$S: Zadajte suradnicu Y:";
    //Pošleme odpoveď klientovi.
    n = write(newsockfd, msg2, strlen(msg2)+1);
    if (n < 0)
    {
        perror("Error writing to socket");
        return -1;
    }

    bzero(buffer,256);
    //Načítame správu od klienta cez socket do buffra.
    n = read(newsockfd, buffer, 255);
    if (n < 0)
    {
        perror("Error reading from socket");
        return -1;
    }
    if (strcmp(buffer, "Koncim\n")==0) {
        printf("$S: Hrac %d sa odpojil.\n", newsockfd);
        *odpojilSa = true;
        return -1;
    }
    //Vypíšeme prijatú správu na štandardný výstup.
    printf("$%d: Zadana suradnica y hracom: %s",newsockfd, buffer);
    //otestovat[poradieKarticky][1] = atoi(buffer);

    y = atoi(buffer);
    otestovat[poradieKarticky][1] = y;

    if (y >= pocetParov || x >= pocetParov)
    {
        return -5;          //Y alebo X vacsie ako hranica
    } else {
        if (poradieKarticky == 1)
        {
            if (otestovat[0][0] == otestovat[1][0] && otestovat[0][1] == otestovat[1][1])
            {
                return -6;
            }
            poradieKarticky = 0;
        } else {
            poradieKarticky++;
        }

    }

    //preratavanie indexu z 2d pola do 1d
    int index = y + (x * pocetParov);

    return index;
}

void otocKarticku(int * stavy, int index) {
    stavy[index] = 1;
}

void vypisHraciuPlochu(int stavy[], int pary[], int dlzka) {


    for (int i = 0; i < dlzka; ++i) {
        for (int j = 0; j < dlzka; ++j) {
            int index = j + (i * dlzka);
            if (stavy[index] == 0) {            //este neotocena karta
                printf("X   ");
            } else if (stavy[index] == 1) {     //otocena karta
                printf("%d   ", pary[index]);
            } else if (stavy[index] == -1) {     //uhadnuta karta
                printf("    ");
            }
        }
        printf("\n");
    }
}

void vytvorPexeso()
{
    printf("$S: Vytvaram pexeso.\n");
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
}

void * hrajHru(void * data) {
    DATA *d = data;

    char buffer[256];
    char *buff = buffer;

    pthread_mutex_lock(d->mutex);
    //cyklus pokracuje kym hraci neuhadnu vsetky karticky alebo kym zostal aspon jeden hrac v hre
    while (!(*d->koniec) && d->pocetHracov > 0) {

        //pokial hrac nie je na rade caka
        while (!(*d->somNaRade)) {
            pthread_cond_wait(d->necakam, d->mutex);
        }
        (*d->somNaRade) = false;

        if (!(*d->koniec)) {
            sleep(2);
            const char *msg = "$S: Si na rade";
            send(d->sockety[(*d->hracNaRade)], msg, strlen(msg) + 1, 0);
            sleep(2);
            send(d->sockety[(*d->hracNaRade)], d->stavy, sizeof(int) * pocetParov * pocetParov, 0);

            //overovanie karticky
            int indexPrvaKarta = -2;

            while (indexPrvaKarta < -1) {
                if (indexPrvaKarta == -5) {
                    msg = "$S: Mas moznost opravit index vacsi ako pocetParov";
                    send(d->sockety[(*d->hracNaRade)], msg, strlen(msg) + 1, 0);
                }
                indexPrvaKarta = hra(buff, d->sockety[(*d->hracNaRade)], &d->odpojilSa);        //0
            }

            //pokial hrac zada Koncim
            if (d->odpojilSa == true) {
                //osetrenie v nasledujucom kole
                otestovat[0][0] = -1;
                otestovat[0][1]= -1;
                //close(d->sockety[(*d->hracNaRade)]);
                d->sockety[(*d->hracNaRade)] = -1;

                if ((*d->hracNaRade) + 1 < d->hornaHranica) {
                    (*d->hracNaRade)++;
                    while (d->sockety[*d->hracNaRade] == -1 && (*d->hracNaRade) < d->hornaHranica) {
                        (*d->hracNaRade)++;
                    }
                } else {
                    (*d->hracNaRade) = 0;
                    while (d->sockety[(*d->hracNaRade)] == -1 && (*d->hracNaRade) < d->hornaHranica) {
                        (*d->hracNaRade)++;
                    }
                }

                (*d->somNaRade) = true;
                pthread_cond_signal(d->necakam);
                (*d->pocetHracov)--;
                //if ((*d->pocetHracov) <= 1) {
                pthread_mutex_unlock(d->mutex);
                //}
                break;
            }

            while (uzOtocenaKarta(d->stavy, x, y)) {
                msg = "$S: Chces otocit uz otocenu karticku";
                send(d->sockety[(*d->hracNaRade)], msg, strlen(msg) + 1, 0);
                sleep(2);
                send(d->sockety[(*d->hracNaRade)], d->stavy, sizeof(int) * pocetParov * pocetParov, 0);
                msg = "$S: Skus este raz";
                sleep(2);
                send(d->sockety[(*d->hracNaRade)], msg, strlen(msg) + 1, 0);
                indexPrvaKarta = hra(buff, d->sockety[(*d->hracNaRade)], &d->odpojilSa);
            }

            //index vybranej karticky je validny
            if (indexPrvaKarta >= 0) {
                otocKarticku(d->stavy, indexPrvaKarta);
                vypisHraciuPlochu(d->stavy, d->pom, pocetParov);
                //posielame vsetkym hracom
                for (int i = 0; i < d->hornaHranica; ++i) {
                    msg = "$S: Zmena";
                    send(d->sockety[i], msg, strlen(msg) + 1, 0);
                    sleep(2);
                    send(d->sockety[i], d->stavy, sizeof(int) * pocetParov * pocetParov, 0);
                }

            }

            //zadavanie druhej karticky
            sleep(2);
            msg = "$S: Pokracuj";
            send(d->sockety[(*d->hracNaRade)], msg, strlen(msg) + 1, 0);

            int indexDruhaKarta = -2;

            while (indexDruhaKarta < -1) {
                if (indexDruhaKarta == -6) {
                    msg = "$S: Zadal si dve rovnake karty. Naprav svoj pokus";
                } else {
                    msg = "$S: Mas moznost opravit index vacsi ako pocetParov";
                }
                send(d->sockety[(*d->hracNaRade)], msg, strlen(msg) + 1, 0);

                //indexV1d - preratany index zo vstupu hraca
                indexDruhaKarta = hra(buff, d->sockety[(*d->hracNaRade)], &d->odpojilSa);   //0
            }

            if (d->odpojilSa == true) {
                d->sockety[(*d->hracNaRade)] = -1;
                otestovat[0][0] = -1;
                otestovat[0][1]= -1;
                d->stavy[indexPrvaKarta] = 0;
                if ((*d->hracNaRade) + 1 < d->hornaHranica) {
                    (*d->hracNaRade)++;
                    while (d->sockety[(*d->hracNaRade)] == -1 && (*d->hracNaRade) < d->hornaHranica) {
                        (*d->hracNaRade)++;
                    }
                } else {
                    (*d->hracNaRade) = 0;
                    while (d->sockety[(*d->hracNaRade)] == -1 && (*d->hracNaRade) < d->hornaHranica) {
                        (*d->hracNaRade)++;
                    }
                }

                (*d->somNaRade) = true;
                pthread_cond_signal(d->necakam);
                (*d->pocetHracov)--;
                //if ((*d->pocetHracov) <= 1) {
                pthread_mutex_unlock(d->mutex);
                //}
                break;
            }

            while (uzOtocenaKarta(d->stavy, x, y)) {
                msg = "$S: Chces otocit uz otocenu karticku";
                send(d->sockety[(*d->hracNaRade)], msg, strlen(msg) + 1, 0);
                sleep(2);
                send(d->sockety[(*d->hracNaRade)], d->stavy, sizeof(int) * pocetParov * pocetParov, 0);
                msg = "$S: Skus este raz";
                sleep(2);
                send(d->sockety[(*d->hracNaRade)], msg, strlen(msg) + 1, 0);
                indexPrvaKarta = hra(buff, d->sockety[(*d->hracNaRade)], &d->odpojilSa);
            }

            sleep(2);//2
            if (indexDruhaKarta >= 0) {
                otocKarticku(d->stavy, indexDruhaKarta);

                printf("\n");
                vypisHraciuPlochu(d->stavy, d->pom, pocetParov);
                for (int i = 0; i < d->hornaHranica; ++i) {
                    msg = "$S: Zmena";
                    send(d->sockety[i], msg, strlen(msg) + 1, 0);
                    sleep(2);
                    send(d->sockety[i], d->stavy, sizeof(int) * pocetParov * pocetParov, 0);
                }
            }

            //kontrola otocenych karticiek
            bool zhodne = zistiSpravnost(d->stavy, pocetParov, d->pom);
            if (zhodne)
            {
                skoreHracov[(*d->hracNaRade)]++;
            }

            //su vsetky karticky uz otocene?
            int index = 0;
            while (index < (pocetParov * pocetParov)) {
                if (d->stavy[index] != -1) {
                    break;
                }
                index++;
            }

            if (index >= (pocetParov * pocetParov)) {
                (*d->koniec) = true;
            }

            //zmena hraca, ktory je na tahu
            if (!(*d->koniec))
            {
                if (!zhodne) {
                    if ((*d->hracNaRade) + 1 < d->hornaHranica) {
                        if ((*d->hracNaRade) != 0 || d->sockety[(*d->hracNaRade)+1] != -1) {
                            (*d->hracNaRade)++;
                        }

                        while (d->sockety[(*d->hracNaRade)] == -1 && (*d->hracNaRade) < d->hornaHranica) {
                            (*d->hracNaRade)++;
                        }
                    } else {
                        (*d->hracNaRade) = 0;
                        while (d->sockety[(*d->hracNaRade)] == -1 && (*d->hracNaRade) < d->hornaHranica) {
                            (*d->hracNaRade)++;
                        }
                    }

                }
            }


            sleep(1);//2
            //posielame vsetkym hraocm, ktory su pripojeni
            for (int i = 0; i < d->hornaHranica; ++i) {
                if ((*d->koniec) && d->sockety[i] != -1)
                {
                    msg = "$S: Koniec";
                    send(d->sockety[i], msg, strlen(msg) + 1, 0);
                    sleep(2);
                    send(d->sockety[i], &skoreHracov[i], sizeof(int), 0);
                }else if (zhodne && d->sockety[i] != -1) {
                    msg = "$S: Karty su zhodne";
                    send(d->sockety[i], msg, strlen(msg) + 1, 0);
                    sleep(2);
                    send(d->sockety[i], d->stavy, sizeof(int) * pocetParov * pocetParov, 0);
                } else if (d->sockety[i] != -1) {
                    msg = "$S: Karty nie su zhodne";
                    send(d->sockety[i], msg, strlen(msg) + 1, 0);
                    sleep(2);
                    send(d->sockety[i], d->stavy, sizeof(int) * pocetParov * pocetParov, 0);
                }
            }
            vypisHraciuPlochu(d->stavy, d->pom, pocetParov);
        }

        pthread_cond_signal(d->necakam);
        (*d->somNaRade) = true;
        pthread_mutex_unlock(d->mutex);

    }

    return NULL;
}

int startServer(int port) {
    srand(time(NULL));
    int sockfd;

    socklen_t cli_len;
    struct sockaddr_in serv_addr, cli_addr;

    //if (argc < 2) {
    //    fprintf(stderr, "usage %s port\n", argv[0]);
    //    return 1;
    //}

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

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

    while (pocetHracov < 2) {
        printf("$S: Minimalny pocet hracov je 2.\n");
        printf("$S: Zadaj pocet hracov:");
        scanf("%d", &pocetHracov);
    }

    while (pocetHracov > MAX_HRACOV) {
        printf("$S: Maximalny pocet hracov je %d.\n", MAX_HRACOV);
        printf("$S: Zadaj pocet hracov:");
        scanf("%d", &pocetHracov);
    }
    //max 10 riadkov

    pocetParov = VELKOST_MATICE;

    for (int i = 0; i < MAX_HRACOV; ++i) {
        skoreHracov[i] = 0;
    }
    //int pocetUhadnuti;

    bool somNaRade = true;
    int hracNaRade = 0;

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_t necakam;
    pthread_cond_init(&necakam, NULL);

    vytvorPexeso();

    //jednorozmerne pole, kde su ulozene karticky
    int pom[pocetParov*pocetParov];
    //jednorozmerne pole so stavmi karticiek
    int stavy[pocetParov*pocetParov];
    for (int i = 0; i < pocetParov*pocetParov; ++i) {
        stavy[i] = 0;
    }
    int pomIndex = 0;
    for (int i = 0; i < pocetParov; ++i) {
        for (int j = 0; j < pocetParov; ++j) {
            pomIndex = (i*pocetParov) + j;
            pom[pomIndex] = pary[i][j];
        }
    }


    vypisHraciuPlochu(stavy, pom, pocetParov);

    //while(1) {
    int hornaHranica = pocetHracov;
    int newsockfd[hornaHranica];
    pthread_t vlaknoKlient[hornaHranica];
    DATA dat[hornaHranica];

    for (int i = 0; i < hornaHranica; ++i) {

        cli_len = sizeof(cli_addr);
        //pre kazdeho hraca sa vytvori nove spojenie, ktore sa ulozi do pila
        newsockfd[i] = accept(sockfd, (struct sockaddr *) &cli_addr, &cli_len);
        printf("$S: Hrac s portom %d sa pripojil.\n", newsockfd[i]);

    }

    bool jeKoniec = false;

    for (int i = 0; i < hornaHranica; ++i) {
        dat[i].id = i;
        send(newsockfd[i], &dat[i].id, sizeof(dat[i].id), 0);
        send(newsockfd[i], &pocetParov, sizeof(pocetParov), 0);
        send(newsockfd[i], pom, sizeof(pom)*pocetParov, 0);
        sleep(2);
        send(newsockfd[i], stavy, sizeof(stavy)*pocetParov, 0);

        dat[i].sockety = newsockfd;
        dat[i].koniec = &jeKoniec;
        dat[i].somNaRade = &somNaRade;
        dat[i].mutex = &mutex;
        dat[i].necakam = &necakam;
        dat[i].hracNaRade = &hracNaRade;
        dat[i].pocetHracov = &pocetHracov;
        dat[i].hornaHranica = hornaHranica;
        dat[i].odpojilSa = false;
        dat[i].stavy = stavy;
        dat[i].pom = pom;
        //vytvorenie vlakien pre hraca
        pthread_create(&vlaknoKlient[i], NULL, &hrajHru, &dat[i]);
    }

    for (int i = 0; i < hornaHranica; ++i) {
        pthread_join(vlaknoKlient[i], NULL);
    }

    //}
    printf("-----------HRA KONCI--------------\n");

    int max = 0;
    for (int i = 0; i < hornaHranica; ++i) {
        if (max < skoreHracov[i])
            max = skoreHracov[i];
    }
    for (int i = 0; i < hornaHranica; ++i) {
        if (skoreHracov[i] == max)
        {
            printf("Hrac %d [index od 0] VYHRAL s poctom bodov %d.\n", i, max);
        }
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&necakam);
    //pojde toto tolko krat???
    for (int i = 0; i < hornaHranica; ++i) {
        close(newsockfd[i]);
    }

    close(sockfd);

    return 0;
}