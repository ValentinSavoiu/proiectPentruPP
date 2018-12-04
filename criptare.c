#include <stdio.h>
#include <stdlib.h>

unsigned char* permute_image_by_permutation (unsigned char *image, unsigned int *permutation, unsigned int size) {
    unsigned char* new_image = (unsigned char *) malloc (3 * size * sizeof(unsigned char));
   for (unsigned int i = 0; i < size ; ++i) {
        unsigned int poz1 = 3 * i, poz2 = 3 * permutation[i];
        for (int k = 0; k < 3; ++k)
            new_image[poz2 + k] = image[poz1 + k];
   }
   return new_image;
}

double *chi_squared (unsigned char* image, unsigned int size) {
    unsigned int **frecv;
    frecv = (unsigned int **) malloc (3 * sizeof(unsigned int *));

    double *chisquared = calloc(3 , sizeof(double)), fmedie = size / 256;
    chisquared[0] = chisquared[1] = chisquared[2] = 0;
    for (int k = 0; k < 3; ++k)
        frecv[k] = (unsigned int*) calloc(256 , sizeof(unsigned int));
    for (unsigned int i = 0; i < 3 * size; ++i) {
        frecv[i % 3][image[i]]++;
    }

    for (unsigned int i = 0; i <= 255; ++i) {
        for (unsigned int k = 0; k < 3; ++k) {
            double f = fmedie - frecv[k][i];
            chisquared[k] += ( (f * f) / fmedie);
        }
    }
    free (frecv);
    return chisquared;

}

void xor_encryption (unsigned char* image, unsigned int size, unsigned int* random_numbers, unsigned int SV) {
    unsigned int k, i;
    for (k = 0; k < 3; ++k) {
        unsigned char mask2 = ( (random_numbers[0] >> ( k* 8) ) & 255 );
        unsigned char mask1 = ( ( SV >> (k * 8) ) & 255 ); // De ce 2 - k ? Pentru ca BGR
        image[k] = image[k] ^ mask1 ^ mask2;
    }
    for (i = 1; i < size; ++i) {
        for (k = 0; k < 3; ++k) {
            unsigned char mask1 = image[(3 * (i - 1) + k)];
            unsigned char mask2 = ( (random_numbers[i] >> (k * 8) ) & 255 );
            image[3 * i + k] = mask1 ^ mask2 ^ image[3 * i  + k];
        }
    }
}

void xor_decryption (unsigned char* image, unsigned int size, unsigned int* random_numbers, unsigned int SV) {
    unsigned int k, i;

    for (i = size - 1; i > 0; --i) {
        for (k = 0; k < 3; ++k) {
            unsigned char mask1 = image[(3 * (i - 1) + k)];
            unsigned char mask2 = ( (random_numbers[i] >> (k * 8) ) & 255 );
            image[3 * i + k] = mask1 ^ mask2 ^ image[3 * i  + k];
        }
    }

    for (k = 0; k < 3; ++k) {
        unsigned char mask2 = ( (random_numbers[0] >> (  k * 8) ) & 255 );
        unsigned char mask1 = ( ( SV >>  (k * 8) ) & 255 ); // De ce 2 - k ? Pentru ca BGR
        image[k] = image[k] ^ mask1 ^ mask2;
    }
}
unsigned int* invert_permutation (unsigned int *permutation, unsigned int size) {
    unsigned int* new_permutation = (unsigned int *) malloc(sizeof(unsigned int) * size);
    for (unsigned int i = 0; i < size; ++i)
        new_permutation[permutation[i]] = i;
    return new_permutation;
}

int encrypt(char *sourceFileName, char *destinationFileName, char *keyFileName) {
    unsigned char *header, *image;
    unsigned int W, H, seed, SV;
    image = read_linearize(sourceFileName, &header, &W, &H); // BGR
    if (image == NULL) {
        printf("\nImaginea de criptat nu a fost gasita");
        return -1;
    }
    double *chisquared = chi_squared(image, W * H);
    printf("%f %f %f \n", chisquared[2], chisquared[1], chisquared[0]);
    FILE *fin = fopen(keyFileName, "r");
    if (fin == NULL) {
        printf("\nCheia secreta nu a fost gasita");
        return -1;
    }
    fscanf(fin, "%u %u", &seed, &SV);
    unsigned int *random_numbers;
    random_numbers = xorshift32(seed, 2 * W * H - 1);
    unsigned int *random_permutation;
    random_permutation = make_random_permutation(random_numbers, W * H);
    unsigned char *new_image = permute_image_by_permutation(image, random_permutation, W * H);
    free(random_permutation);
    free(image);
    image = new_image;
    xor_encryption(image, W * H, random_numbers + W * H - 1, SV);
    chisquared = chi_squared(image, W * H);
    printf("%f %f %f \n", chisquared[2], chisquared[1], chisquared[0]);
    save_image(destinationFileName, image, W, H, header);
    free(image);
    free(chisquared);
    free(random_numbers);
    return 0;
}



int decrypt(char *sourceFileName, char *destinationFileName, char *keyFileName) {
    unsigned char *header, *image;
    unsigned int W, H, seed, SV;
    int ok;
    image = read_linearize(sourceFileName, &header, &W, &H); // BGR
    if (image == NULL) {
        printf("\nImaginea de criptat nu a fost gasita");
        return -1;
    }
    FILE *fin = fopen(keyFileName, "r");
    if (fin == NULL) {
        printf("\nCheia secreta nu a fost gasita");
        return -1;
    }
    fscanf(fin, "%u %u", &seed, &SV);
    unsigned int *random_numbers;
    random_numbers = xorshift32(seed, 2 * W * H - 1);
    xor_decryption(image, W * H, random_numbers + W * H - 1, SV);
    unsigned int *random_permutation;
    random_permutation = make_random_permutation(random_numbers, W * H);
    unsigned int *inverse_permutation = invert_permutation(random_permutation, W * H);
    unsigned char *new_image = permute_image_by_permutation(image, inverse_permutation, W * H);
    free(random_permutation);
    free(image);
    image = new_image;
    save_image(destinationFileName, image, W, H, header);
    free(random_numbers);
    free(image);
    return 0;

}
