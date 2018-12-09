#ifndef FISIERE_BINARE_H_INCLUDED
#define FISIERE_BINARE_H_INCLUDED
#define HEADER_SIZE 54
/*unsigned char* read_linearize(char *, unsigned char **, unsigned int, unsigned int);
void save_image (char *, unsigned char *, unsigned int , unsigned int , unsigned char* );*/

typedef struct {
    unsigned char r, b, g, grey;
}pixel;

unsigned char* read_linearize(char *fileName, unsigned char **header, unsigned int *W, unsigned int *H) {
    FILE *fin = fopen(fileName, "rb");
    if (fin == NULL) {
        return NULL;
    }


    *header = (unsigned char *) malloc(HEADER_SIZE * sizeof(unsigned char));
    fread(*header, sizeof(unsigned char), HEADER_SIZE, fin);
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
    fclose(fin);
    return v;
}


pixel** read_as_matrix(char *fileName, unsigned char **header, unsigned int *W, unsigned int *H) {
	FILE *fin = fopen(fileName, "rb");
    if (fin == NULL) {
        return NULL;
    }
    *header = (unsigned char *) malloc(HEADER_SIZE * sizeof(unsigned char));
    fread(*header, sizeof(unsigned char), HEADER_SIZE, fin);
    fseek(fin, 18, SEEK_SET);
    fread(W, sizeof(unsigned int), 1, fin);
    fread(H, sizeof(unsigned int), 1, fin);
    unsigned char padding;
    if ( (*W) % 4)
        padding = 4 - (3 * (*W)) % 4;
    else
        padding = 0;
    pixel **v;
    v = (pixel **) malloc ((*H) * sizeof(pixel*));
    for(unsigned int i = 0; i < *H; ++i) {
        v[i] = (pixel *) malloc ((*W) * sizeof(pixel));
    }
    fseek (fin, 54, SEEK_SET);
    for (unsigned int i = 0; i < (*H); ++i) {
        for (unsigned int j = 0; j < (*W); ++j) {
            fread(&v[*H - i - 1][(int) j].b, sizeof(unsigned char), 1, fin);
            fread(&v[*H - i - 1][j].g, sizeof(unsigned char), 1, fin);
            fread(&v[*H - i - 1][j].r, sizeof(unsigned char), 1, fin);
            v[*H - i - 1][j].grey = ( 0.299 * v[*H - i - 1][j].r + 0.587 * v[*H - i - 1][j].g + 0.114 * v[*H - i - 1][j].b);
        }
        fseek(fin, padding * sizeof(unsigned char), SEEK_CUR);
    }
    fclose(fin);
    return v;
}

void save_image_matrix (char *fileName, pixel **A, unsigned int W, unsigned int H, unsigned char *header) {
    FILE *fout = fopen(fileName, "wb");
    fwrite(header, sizeof(unsigned char), HEADER_SIZE, fout);
    unsigned char padding;
    if (W % 4)
        padding = 4 - (3 * W) % 4;
    else
        padding = 0;
    for (unsigned int i = 0; i < H; ++i) {
        for (unsigned int j = 0; j < W; ++j) {
            fwrite(&A[H - i - 1][j].b, sizeof(unsigned char), 1, fout);
            fwrite(&A[H - i - 1][j].g, sizeof(unsigned char), 1, fout);
            fwrite(&A[H - i - 1][j].r, sizeof(unsigned char), 1, fout);
        }
        fseek(fout, padding * sizeof(unsigned char), SEEK_CUR);
    }
    fclose(fout);
}

void save_image (char *fileName, unsigned char *linearization, unsigned int W, unsigned int H, unsigned char *header) {
    FILE *fout = fopen(fileName, "wb");
    fwrite(header, sizeof(unsigned char), HEADER_SIZE, fout);
    unsigned char padding;
    if (W % 4)
        padding = 4 - (3 * W) % 4;
    else
        padding = 0;
    for (unsigned int i = 0; i < H; ++i) {
        fwrite(linearization + (H - i - 1) * 3 * W, sizeof(unsigned char), 3 * W, fout);
        fseek(fout, padding * sizeof(unsigned char), SEEK_CUR);
    }
    fclose(fout);
}
#endif // FISIERE_BINARE_H_INCLUDED
