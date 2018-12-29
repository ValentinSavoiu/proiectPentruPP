#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "random_number_generator.h"
#include "fisiere_binare.h"
#include "criptare.h"
#include "modificare_imagine.h"

int main() {

    char *encryptionSource = (char*) malloc(30 * sizeof(char));
    char *encryptionDestination = (char*) malloc(30 * sizeof(char));
    char *secretKey = (char*) malloc(30 * sizeof(char));;
    if (!encryptionDestination || !encryptionSource || !secretKey) {
        free(encryptionDestination);
        free(encryptionSource);
        free(secretKey);
        printf ("Nu am putut aloca 90 de octeti. Va rog sa nu rulati acesti program pe un prajitor de paine");
        return 0;
    }

    printf("Introduceti numele imaginii de criptat :"); //citiri
    scanf("%s30", encryptionSource);
    printf("\nIntroduceti numele  fisierului in care va fi salvata imaginea criptata :");
    scanf("%s30", encryptionDestination);
    printf("\nIntroduceti numele  fisierului in care se afla cele 2 chei secrete:");
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

    char *decryptionSource = (char*) malloc(30 * sizeof(char));
    char *decryptionDestination = (char*) malloc(30 * sizeof(char));
    secretKey = (char*) malloc(30 * sizeof(char));

    if (!decryptionDestination || !decryptionSource || !secretKey) {
        free(decryptionDestination);
        free(decryptionSource);
        free(secretKey);
        printf ("\nNu am putut aloca 90 de octeti. Va rog sa nu rulati acesti program pe un prajitor de paine");
        return 0;
    }

    printf("\nIntroduceti numele imaginii de decriptat :"); //citiri
    scanf("%s30", decryptionSource);
    printf("\nIntroduceti numele  fisierului in care va fi salvata imaginea decriptata :");
    scanf("%s30", decryptionDestination);
    printf("\nIntroduceti numele  fisierului in care se afla cele 2 chei secrete, aceleasi chei folosite pentru criptarea initiala a imaginii :");
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

    char *detectionSource = (char*) malloc(30 * sizeof(char));
    char *detectionDestination = (char*) malloc(30 * sizeof(char));
    if (!detectionDestination || !detectionSource) {
        free(detectionDestination);
        free(detectionSource);
         printf ("\nNu am putut aloca 90 de octeti. Va rog sa nu rulati acesti program pe un prajitor de paine");
    }

    printf("\nIntroduceti numele imaginii in care caut sabloane :"); //citiri
    scanf("%s30", detectionSource);
    printf("\nIntroduceti numele imaginii in care salvez imaginea modificata :");
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
