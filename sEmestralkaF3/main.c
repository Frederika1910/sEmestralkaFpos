#include <stdio.h>
#include <stdlib.h>
#include "server.h"
#include "client.h"
#include "string.h"

int main(int argc, char *argv[]) {

    if (argc > 2) {
        startClient(argv[2], atoi(argv[1]));
    } else if (argc > 1) {
        startServer(atoi(argv[1]));
    }


    return 0;
}
