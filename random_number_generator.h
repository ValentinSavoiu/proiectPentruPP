#ifndef RANDOM_NUMBER_GENERATOR_H_INCLUDED
#define RANDOM_NUMBER_GENERATOR_H_INCLUDED

unsigned int* make_random_permutation(unsigned int* randomNumbers, unsigned int length) {
    unsigned int *randomPermutation = (unsigned int*) calloc(length, sizeof(unsigned int));
    if (!randomPermutation) {
        printf("\nMemorie insuficienta");
        return NULL;
    }
    for (unsigned int i = 0; i < length; ++i)
        randomPermutation[i] = i;
    unsigned int i = 0;
    for (unsigned int k = length - 1; k > 0; k--) {
        unsigned int r = randomNumbers[i] % (k + 1);
        ++i;
        unsigned int aux = randomPermutation[r];
        randomPermutation[r] = randomPermutation[k];
        randomPermutation[k] = aux;
    }
    return randomPermutation;
}

unsigned int* xorshift32(unsigned int seed, unsigned int length) {
    unsigned int *v;
    v = (unsigned int*) calloc(length, sizeof(unsigned int));
    if (!v) {
        printf("\nMemorie insuficienta");
        return NULL;
    }
    for(unsigned int i = 0; i < length; ++i) {
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;
        v[i] = seed;
    }
    return v;
}

#endif // RANDOM_NUMBER_GENERATOR_H_INCLUDED
