#ifndef RANDOM_NUMBER_GENERATOR_H_INCLUDED
#define RANDOM_NUMBER_GENERATOR_H_INCLUDED

unsigned int* xorshift32(unsigned int , unsigned int );

unsigned int* make_random_permutation(unsigned int* random_numbers, unsigned int length) {
    unsigned int *random_permutation = (unsigned int*) calloc(length, sizeof(unsigned int));
    for (unsigned int i = 0; i < length; ++i)
        random_permutation[i] = i;
    unsigned int i = 0;
    for (unsigned int k = length - 1; k > 0; k--) {
        unsigned int r = random_numbers[i] % (k + 1);
        ++i;
        unsigned int aux = random_permutation[r];
        random_permutation[r] = random_permutation[k];
        random_permutation[k] = aux;
    }
    return random_permutation;
}

unsigned int* xorshift32(unsigned int seed, unsigned int length) {
    unsigned int *v;
    v = (unsigned int*) calloc(length, sizeof(unsigned int));
    for(unsigned int i = 0; i < length; ++i) {
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;
        v[i] = seed;
    }
    return v;
}

#endif // RANDOM_NUMBER_GENERATOR_H_INCLUDED
