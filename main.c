#include <stdio.h>
#include <stdlib.h>
#include "random_number_generator.h"
#include "fisiere_binare.h"

int main() {
    printf("Introduceti numele imaginii de criptat :");
    char *encryptionSource = (char*) malloc(30);
    scanf("%s30", encryptionSource);
    printf("\nIntroduceti numele  fisierului in care va fi salvata imaginea criptata :");
    char *encryptionDestination = (char*) malloc(30);
    scanf("%s30", encryptionDestination);
    printf("\nIntroduceti numele  fisierului in care se afla cele 2 chei secrete:");
    char *secret_key = (char*) malloc(30);
    scanf("%s30", secret_key);
    int ok = encrypt(encryptionSource, encryptionDestination, secret_key);
    if (ok == 0) {
        printf("\nCriptarea s-a realizat cu succes");
    }
    else {
        printf("\nEroare la criptare!");
    }
    free(encryptionDestination);
    free(encryptionSource);
    free(secret_key);

    printf("\nIntroduceti numele imaginii de decriptat :");
    char *decryptionSource = (char*) malloc(30);
    scanf("%s30", decryptionSource);
    printf("\nIntroduceti numele  fisierului in care va fi salvata imaginea decriptata :");
    char *decryptionDestination = (char*) malloc(30);
    scanf("%s30", decryptionDestination);
    printf("\nIntroduceti numele  fisierului in care se afla cele 2 chei secrete, aceleasi chei folosite pentru criptarea initiala a imaginii :");
    secret_key = (char*) malloc(30);
    scanf("%s30", secret_key);
    ok = decrypt(decryptionSource, decryptionDestination, secret_key);
    if (ok == 0) {
        printf("\nDecriptarea s-a realizat cu succes");
    }
    else {
        printf("\nEroare la decriptare!");
    }
    free(decryptionSource);
    free(decryptionDestination);
    free(secret_key);
    return 0;
}