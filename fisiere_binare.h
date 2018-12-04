#ifndef FISIERE_BINARE_H_INCLUDED
#define FISIERE_BINARE_H_INCLUDED
#define header_size 54
/*unsigned char* read_linearize(char *, unsigned char **, unsigned int, unsigned int);
void save_image (char *, unsigned char *, unsigned int , unsigned int , unsigned char* );*/

unsigned char* read_linearize(char *fileName, unsigned char **header, unsigned int *W, unsigned int *H) {
    FILE *fin = fopen(fileName, "rb");
    *header = (unsigned char *) malloc(header_size * sizeof(unsigned char));
    fread(*header, sizeof(unsigned char), header_size, fin); //asa da
    fseek(fin, 18, SEEK_SET);
    fread(W, sizeof(unsigned int), 1, fin);
    fread(H, sizeof(unsigned int), 1, fin);
    unsigned char padding;
    if ( (*W) % 4)
        padding = 4 - (3 * (*W)) % 4;
    else
        padding = 0;
    unsigned char *v = (unsigned char *) calloc( (*W) * (*H) * 3, sizeof(unsigned char));
    fseek (fin, 54, SEEK_SET);
    for (unsigned int i = 0; i < (*H); ++i) {
        fread(v + (*H - i - 1)* 3 * (*W), sizeof(unsigned char), 3 * (*W), fin);
        //fread(v + 3 * i * (*W), sizeof(unsigned char), 3 * (*W), fin);
        fseek(fin, padding * sizeof(unsigned char), SEEK_CUR);
    }
    return v;
}

void save_image (char *fileName, unsigned char *linearization, unsigned int W, unsigned int H, unsigned char *header) {
    FILE *fout = fopen(fileName, "wb");
    fwrite(header, sizeof(unsigned char), header_size, fout);
    unsigned char padding;
    if (W % 4)
        padding = 4 - (3 * W) % 4;
    else
        padding = 0;
    for (unsigned int i = 0; i < H; ++i) {
        fwrite(linearization + (H - i - 1) * 3 * W, sizeof(unsigned char), 3 * W, fout);
        fseek(fout, padding * sizeof(unsigned char), SEEK_CUR);
    }
}
#endif // FISIERE_BINARE_H_INCLUDED
