#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap.h"

int main(void) {
    FILE *in = fopen("in.txt", "r");
    FILE *out = fopen("out.txt", "w");
    if (in == NULL || out == NULL) {
        exit(-1);
    }
    int i = 0;
    int m;
    bitmap_init();
    while (!feof(in)) {
        fscanf(in, "%d", &m);
        printf("%d\n", m);
        bitmap_set(m);
    }
    printf("abnother\n");
    for (i = 0; i < BITMAP_N; i++) {
        if (bitmap_test(i)) {
            printf("%d\n", i);
            fprintf(out, "%d\n", i);
        }
    }
    fclose(in);
    fclose(out);
    return 0;
} 