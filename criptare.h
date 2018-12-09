

unsigned char* permute_image_by_permutation (unsigned char *image, unsigned int *permutation, unsigned int size) {
    unsigned char* newImage = (unsigned char *) malloc (3 * size * sizeof(unsigned char));
   for (unsigned int i = 0; i < size ; ++i) {
        unsigned int poz1 = 3 * i, poz2 = 3 * permutation[i];
        for (int k = 0; k < 3; ++k)
            newImage[poz2 + k] = image[poz1 + k];
   }
   return newImage;
}

double* chi_squared (char* fileName) {
    unsigned int W, H;
    unsigned char *header;
    unsigned char *image = read_linearize(fileName, &header, &W, &H);
    unsigned int size = W * H;

    unsigned int **frecv;
    frecv = (unsigned int **) malloc (3 * sizeof(unsigned int *));

    double *chiSquared = calloc(3 , sizeof(double)), fmedie = size / 256;
    chiSquared[0] = chiSquared[1] = chiSquared[2] = 0;
    for (int k = 0; k < 3; ++k)
        frecv[k] = (unsigned int*) calloc(256 , sizeof(unsigned int));
    for (unsigned int i = 0; i < 3 * size; ++i) {
        frecv[i % 3][image[i]]++;
    }

    for (unsigned int i = 0; i <= 255; ++i) {
        for (unsigned int k = 0; k < 3; ++k) {
            double f = fmedie - frecv[k][i];
            chiSquared[k] += ( (f * f) / fmedie);
        }
    }
    free (image);
    free (frecv);
    return chiSquared;

}

void xor_encryption (unsigned char* image, unsigned int size, unsigned int* randomNumbers, unsigned int SV) {
    unsigned int k, i;
    for (k = 0; k < 3; ++k) {
        unsigned char mask2 = ( (randomNumbers[0] >> ( k* 8) ) & 255 );
        unsigned char mask1 = ( ( SV >> (k * 8) ) & 255 );
        image[k] = image[k] ^ mask1 ^ mask2;
    }
    for (i = 1; i < size; ++i) {
        for (k = 0; k < 3; ++k) {
            unsigned char mask1 = image[(3 * (i - 1) + k)];
            unsigned char mask2 = ( (randomNumbers[i] >> (k * 8) ) & 255 );
            image[3 * i + k] = mask1 ^ mask2 ^ image[3 * i  + k];
        }
    }
}

void xor_decryption (unsigned char* image, unsigned int size, unsigned int* randomNumbers, unsigned int SV) {
    unsigned int k, i;

    for (i = size - 1; i > 0; --i) {
        for (k = 0; k < 3; ++k) {
            unsigned char mask1 = image[(3 * (i - 1) + k)];
            unsigned char mask2 = ( (randomNumbers[i] >> (k * 8) ) & 255 );
            image[3 * i + k] = mask1 ^ mask2 ^ image[3 * i  + k];
        }
    }

    for (k = 0; k < 3; ++k) {
        unsigned char mask2 = ( (randomNumbers[0] >> (  k * 8) ) & 255 );
        unsigned char mask1 = ( ( SV >>  (k * 8) ) & 255 );
        image[k] = image[k] ^ mask1 ^ mask2;
    }
}


unsigned int* invert_permutation (unsigned int *permutation, unsigned int size) {
    unsigned int* newPermutation = (unsigned int *) malloc(sizeof(unsigned int) * size);
    for (unsigned int i = 0; i < size; ++i)
        newPermutation[permutation[i]] = i;
    return newPermutation;
}


int encrypt (char *sourceFileName, char *destinationFileName, char *keyFileName) {
    unsigned char *header, *image;
    unsigned int W, H, seed, SV;
    image = read_linearize(sourceFileName, &header, &W, &H);
    if (image == NULL) {
        printf("\nImaginea de criptat nu a fost gasita");
        return -1;
    }

    double *chiSquared = chi_squared(sourceFileName);
    printf("%f %f %f \n", chiSquared[2], chiSquared[1], chiSquared[0]);
    FILE *fin = fopen(keyFileName, "r");
    if (fin == NULL) {
        printf("\nCheia secreta nu a fost gasita");
        return -1;
    }
    fscanf(fin, "%u %u", &seed, &SV);
    fclose(fin);
    unsigned int *randomNumbers;
    randomNumbers = xorshift32(seed, 2 * W * H - 1);
    unsigned int *randomPermutation;
    randomPermutation = make_random_permutation(randomNumbers, W * H);
    unsigned char *newImage = permute_image_by_permutation(image, randomPermutation, W * H);
    free(randomPermutation);
    free(image);
    image = newImage;
    xor_encryption(image, W * H, randomNumbers + W * H - 1, SV);


    save_image(destinationFileName, image, W, H, header);
    chiSquared = chi_squared(destinationFileName);
    printf("%f %f %f \n", chiSquared[2], chiSquared[1], chiSquared[0]);
    free(header);
    free(image);
    free(chiSquared);
    free(randomNumbers);
    return 0;
}



int decrypt (char *sourceFileName, char *destinationFileName, char *keyFileName) {
    unsigned char *header, *image;
    unsigned int W, H, seed, SV;
    image = read_linearize(sourceFileName, &header, &W, &H); // BGR
    if (image == NULL) {
        printf("\nImaginea de criptat nu a fost gasita");
        return 1;
    }
    FILE *fin = fopen(keyFileName, "r");
    if (fin == NULL) {
        printf("\nCheia secreta nu a fost gasita");
        return 1;
    }
    fscanf(fin, "%u %u", &seed, &SV);
    unsigned int *randomNumbers;
    randomNumbers = xorshift32(seed, 2 * W * H - 1);
    xor_decryption(image, W * H, randomNumbers + W * H - 1, SV);
    unsigned int *randomPermutation;
    randomPermutation = make_random_permutation(randomNumbers, W * H);
    unsigned int *inversePermutation = invert_permutation(randomPermutation, W * H);
    unsigned char *newImage = permute_image_by_permutation(image, inversePermutation, W * H);
    free(randomPermutation);
    free(inversePermutation);
    free(image);
    image = newImage;
    save_image(destinationFileName, image, W, H, header);
    free(header);
    free(randomNumbers);
    free(image);
    return 0;

}
