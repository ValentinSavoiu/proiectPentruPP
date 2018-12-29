#ifndef MODIFICARE_IMAGINE_H_INCLUDED
#define MODIFICARE_IMAGINE_H_INCLUDED
#define PRAG 0.5

typedef struct {
    unsigned int x, X, y, Y;
    double crossCorr;
    unsigned char nrTemp;

} detection;

int cmp (const void *A, const void *B) {  //comparatia pt quicksort
    detection D1 = *((detection*) A), D2 = *((detection*) B);
    if (D1.crossCorr == D2.crossCorr) return 0;
    return (D1.crossCorr - D2.crossCorr) > 0 ? -1 : 1;
}

double compute_overlap(detection A, detection B) {
    if (A.x > B.x) {detection aux = A; A = B; B = aux;} // rezolv individual cat se intersecteaza pe inaltime, stabilesc ca A incepe mai sus ca B
    if (A.X < B.x) return 0; // daca A se si termina inainte sa inceapa B , atunci nu am intersectie
    double xOverlap, yOverlap;
    xOverlap = A.X - B.x; // calculez intersectia pe inaltime
    if (B.X -  B.x < xOverlap) xOverlap = B.X - B.x; //inclusiv cazul in care B inclus in A, relevant doar daca sabloanele au dimensiuni diferite

    if (A.y > B.y) {detection aux = A; A = B; B = aux;}//rezolv individual cat se intersecteaza pe latime, la fel ca mai sus
    if (A.Y < B.y) return 0;
    yOverlap = A.Y - B.y;
    if (B.Y - B.y < yOverlap) yOverlap = B.y - B.y;

    return xOverlap * yOverlap;
}

double area (detection A) {
    return (A.X - A.x) * (A.Y - A.y);
}

void remove_non_maxims (detection *det, unsigned int size) {
    for(unsigned int i = 0; i < size; ++i) {
        if (det[i].x == det[i].X) // daca e deja sters
            continue;
        for(unsigned int j = i + 1; j < size; ++j) {
            if (det[j].x == det[j].X)
                continue;
            double overlap = compute_overlap(det[i], det[j]);
            overlap = (overlap) / (area(det[i]) + area(det[j]) - overlap);
            if (overlap >= 0.2) {
                det[j].x = det[j].X; // il consider sters
            }
        }

    }
}


double compute_average (pixel **image, unsigned int x, unsigned int y, unsigned int W, unsigned int H) {
    unsigned int sum = 0;
    for (unsigned int i = x; i < x + H; ++i)
        for (unsigned int j = y; j < y + W; ++j) {
            sum += image[i][j].grey;
        }
    return 1.0 * sum / (W * H);
}


double compute_standard_deviation (pixel **image, unsigned int x, unsigned int y, unsigned int W, unsigned int H, double avg) {
    double stdDev = 0;
    for (unsigned int i = x; i < x + H; ++i) {
        for (unsigned int j = y; j < y + W; ++j) {
            stdDev = stdDev + (image[i][j].grey - avg) * (image[i][j].grey - avg);
        }
    }
    return (double) sqrt( stdDev / (W * H - 1));
}


void draw (pixel **image, detection *det, unsigned int size, pixel *colors) {
    for (unsigned int cnt = 0; cnt < size; ++cnt) {
        unsigned int x = det[cnt].x, X = det[cnt].X, y = det[cnt].y, Y = det[cnt].Y;
        if (x == X)
            continue;
        for (unsigned int i = x; i < X; ++i)
            image[i][y] = image[i][Y - 1] = colors[det[cnt].nrTemp];
        for (unsigned int j = y; j < Y; ++j)
            image[x][j] = image[X - 1][j] = colors[det[cnt].nrTemp];
    }
}


detection* detections (pixel** image, unsigned int imageW, unsigned int imageH, pixel** temp, unsigned int tempW, unsigned int tempH, double tempAvg, double tempStdDev, unsigned int *size, unsigned char nrTemp) {
    double crossCorr = 0;
    *size = 0;
    detection *det = (detection*) malloc(2000 * sizeof(detection));
    if (!det) {
        printf ("\nMemorie insuficienta");
        *size = 1<<30;
        return NULL;
    }
    for (unsigned int i = 0; i < imageH - tempH; ++i)
        for (unsigned int j = 0; j < imageW - tempW; ++j) {

            double imageAvg = compute_average(image, i, j, tempW, tempH);
            double imageStdDev = compute_standard_deviation(image, i, j, tempW, tempH, imageAvg);

            for (unsigned int ii = 0; ii < tempH; ++ii)
                for(unsigned int jj = 0; jj < tempW; ++jj)
                    crossCorr = crossCorr + (image[i + ii][j + jj].grey - imageAvg) * (temp[ii][jj].grey - tempAvg);

            crossCorr = crossCorr / tempW / tempH / imageStdDev / tempStdDev;

            if (crossCorr > PRAG) {
                detection aux;
                aux.x = i;
                aux.X = i + tempH;
                aux.y = j;
                aux.Y = j + tempW;
                aux.nrTemp = nrTemp;
                aux.crossCorr = crossCorr;


                if ((*size) % 2000 || (*size) == 0)
                    det[*size] = aux;
                else {
                    detection *auxx = realloc (det, (*size) + 2000 * sizeof(detection));
                    if (auxx) {
                        det = auxx;
                        det[*size] = aux;
                    }
                    else {
                        printf("\nMemorie insuficienta");
                        free(det);
                        return NULL;
                    }

                }
                ++(*size);
            }
        }
    return det;
}

pixel* initialize_colors () {
    pixel* colors = (pixel *) malloc(10 * sizeof(pixel));
    colors[0].r = 255; colors[0].g = colors[0].b = 0;
    colors[1].r = colors[1].g = 255; colors[1].b = 0;
    colors[2].r = 0; colors[2].g = 255; colors[2].b = 0;
    colors[3].r = 0; colors[3].g = colors[3].b = 255;
    colors[4].r = 255; colors[4].g = 0; colors[4].b = 255;
    colors[5].r = colors[5].g = 0; colors[5].b = 255;
    colors[6].r = colors[6].g = colors[6].b = 192;
    colors[7].r = 255; colors[7].g = 140; colors[7].b = 0;
    colors[8].r = 128; colors[8].g = 0; colors[8].b = 128;
    colors[9].r = 128; colors[9].g = colors[9].b = 0;
    return colors;
}



int modify (char *detectionSource, char *detectionDestination) {
    unsigned int W, H, tempNumber;
    unsigned char *header, *aux;
    unsigned int *tempWidth, *tempHeight, detNumber = 0;
    detection *det = (detection*) malloc(10000 * sizeof(detection));
    if (!det) {
        printf("\nMemorie insuficienta");
        return 1;
    }
    char *templateFile;
    pixel **image = read_as_matrix(detectionSource, &header, &W, &H); //citesc imaginea
    if (image == NULL) {  //indiferent cum ies din program, ma asigur ca toata memoria este eliberata
        free(det);
        printf("\nImaginea nu a fost gasita");
        return 1;
    }



    printf("\nIntroduceti numarul de sabloane :");
    scanf("%u", &tempNumber);
    while (tempNumber > 10) {
        printf("\nCel mult 10 sabloane. Introduceti numarul de sabloane :");
        scanf("%u", &tempNumber);
    }
    pixel ***templates = (pixel ***) malloc(tempNumber * sizeof(pixel **));  //citesc sabloanele
    templateFile = (char *) malloc(30 * sizeof(char));
    tempWidth = (unsigned int *) malloc(tempNumber * sizeof(unsigned int));
    tempHeight = (unsigned int *) malloc(tempNumber * sizeof(unsigned int));

    if (!templates || !templateFile || !tempHeight || !tempWidth) {
        if(templates) free(templates);
        if(templateFile) free(templateFile);
        if(tempHeight) free(tempHeight);
        if(tempWidth) free(tempWidth);
        for (unsigned int i = 0; i < H; ++i)
            free(image[i]);
        free(image);
        printf("\nMemorie insuficienta");
        return 1;
    }

    for (unsigned int i = 0; i < tempNumber; ++i) {
        printf("\nIntroduceti calea sablonului numarul %d :", i + 1);
        scanf("%s30", templateFile);
        templates[i] = read_as_matrix(templateFile, &aux, &tempWidth[i], &tempHeight[i]);
        free(aux);


        if (templates[i] == NULL) {
            for(unsigned int j = 0; j < i; ++j) {
                for (unsigned int k = 0; k < tempHeight[j]; ++k)
                    free(templates[j][k]);
                free(templates[j]);
            }
            free(templates);
            free(tempHeight);
            free(tempWidth);
            free(det);
            free(templateFile);
            for(unsigned int i = 0; i < H; ++i)
	            free(image[i]);
            free(image);
            printf("\nSablonul numarul %d nu a fost gasit", i + 1);
            return 1;
        }

    }
    free(templateFile);

    for (unsigned int i = 0; i < tempNumber; ++i) {
        double tempAverage = compute_average(templates[i], 0, 0, tempWidth[i], tempHeight[i]);
        double tempStdDev = compute_standard_deviation(templates[i], 0, 0, tempWidth[i], tempHeight[i], tempAverage);
        unsigned int auxSize = 0;
        detection *detAux = detections(image, W, H, templates[i], tempWidth[i], tempHeight[i], tempAverage, tempStdDev, &auxSize, i);


        if (auxSize == 1<<30) {
            for(unsigned int j = 0; j < tempNumber; ++j) {
                for (unsigned int k = 0; k < tempHeight[j]; ++k)
                    free(templates[j][k]);
                free(templates[j]);
            }
            for(unsigned int i = 0; i < H; ++i)
	            free(image[i]);
            free(image);
            free(templates);
            free(tempHeight);
            free(tempWidth);
            free(det);
            return 1;
        }


        if( (detNumber + auxSize) / 10000 > detNumber / 10000) {
            detection *auxx = realloc(detAux, (detNumber + auxSize + 10000) * sizeof(detection));

            if (!auxx)
            {
                free (detAux);
                for(unsigned int j = 0; j < tempNumber; ++j) {
                     for (unsigned int k = 0; k < tempHeight[j]; ++k)
                         free(templates[j][k]);
                     free(templates[j]);
                }
                for(unsigned int i = 0; i < H; ++i)
	                free(image[i]);
                free(image);
                free(templates);
                free(tempHeight);
                free(tempWidth);
                free(det);
                printf("\nMemorie insuficienta");
                return 1;
            }

            det = auxx;
        }
        for(unsigned int j = detNumber; j < detNumber + auxSize; ++j)
            det[j] = detAux[j - detNumber];
        detNumber +=auxSize;
        free (detAux);
    }

    for(unsigned int j = 0; j < tempNumber; ++j) {
        for (unsigned int k = 0; k < tempHeight[j]; ++k)
            free(templates[j][k]);
        free(templates[j]);
    }
    free(templates);
    free(tempHeight);
    free(tempWidth);

    qsort(det, detNumber, sizeof(detection), cmp);
    remove_non_maxims(det, detNumber);
    pixel *colors = initialize_colors();


    draw(image, det, detNumber, colors);
    free(colors);
    free(det);
    save_image_matrix(detectionDestination, image, W, H, header);
    free(header);
    for(unsigned int i = 0; i < H; ++i)
	    free(image[i]);
    free(image);
    return 0;
}

#endif // MODIFICARE_IMAGINE_H_INCLUDED
