#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "random_number_generator.h"
#include "fisiere_binare.h"
#include "criptare.h"
#include "modificare_imagine.h"

int main() {
    printf("Introduceti numele imaginii de criptat :"); //citiri
    char *encryptionSource = (char*) malloc(30 * sizeof(char));
    scanf("%s30", encryptionSource);
    printf("\nIntroduceti numele  fisierului in care va fi salvata imaginea criptata :");
    char *encryptionDestination = (char*) malloc(30 * sizeof(char));
    scanf("%s30", encryptionDestination);
    printf("\nIntroduceti numele  fisierului in care se afla cele 2 chei secrete:");
    char *secretKey = (char*) malloc(30 * sizeof(char));;
    scanf("%s30", secretKey);


    int ok = encrypt(encryptionSource, encryptionDestination, secretKey);
    if (ok == 0) {
        printf("\nCriptarea s-a realizat cu succes");
    }
    else {
        printf("\nEroare la criptare!");
    }


    free(encryptionDestination);
    free(encryptionSource);
    free(secretKey);

    printf("\nIntroduceti numele imaginii de decriptat :"); //citiri
    char *decryptionSource = (char*) malloc(30 * sizeof(char));
    scanf("%s30", decryptionSource);
    printf("\nIntroduceti numele  fisierului in care va fi salvata imaginea decriptata :");
    char *decryptionDestination = (char*) malloc(30 * sizeof(char));
    scanf("%s30", decryptionDestination);
    printf("\nIntroduceti numele  fisierului in care se afla cele 2 chei secrete, aceleasi chei folosite pentru criptarea initiala a imaginii :");
    secretKey = (char*) malloc(30 * sizeof(char));
    scanf("%s30", secretKey);


    ok = decrypt(decryptionSource, decryptionDestination, secretKey);
    if (ok == 0) {
        printf("\nDecriptarea s-a realizat cu succes");
    }
    else {
        printf("\nEroare la decriptare!");
    }


    free(decryptionSource);
    free(decryptionDestination);
    free(secretKey);


    printf("Introduceti numele imaginii in care caut sabloane :"); //citiri
    char *detectionSource = (char*) malloc(30 * sizeof(char));
    scanf("%s30", detectionSource);
    printf("Introduceti numele imaginii in care salvez imaginea modificata :");
    char *detectionDestination = (char*) malloc(30 * sizeof(char));
    scanf("%s30", detectionDestination);

    ok = modify(detectionSource, detectionDestination);
    if (ok == 0) {
        printf("\nModificarea s-a realizat cu succes");
    }
    else {
        printf("\nEroare la modificare!");
    }


    free(detectionDestination);
    free(detectionSource);
    return 0;
}
