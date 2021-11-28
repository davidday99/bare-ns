#include "test_ip.h"
#include "test_enet.h"
#include "test_enet_ip.h"
#include <stdio.h>

int main() {



    printf("**** test_enet ****\n\n");
    test_enet();
    printf("*******************\n\n");

    printf("**** test_ip ****\n\n");
    test_ip();
    printf("*******************\n\n");

    printf("**** test_enet_ip ****\n\n");
    test_enet_ip();
    printf("*******************\n\n");

    return 0;
}