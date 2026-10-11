#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "lexer.h"

uchar* file_load(const char* path, size_t* size);

int main(int argc, char** argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: cc <path to file>\n");
        return 1;
    }

    size_t size = 0;
    uchar* buf = file_load(argv[1], &size);
    if (NULL == buf) goto cleanup;
    if (size > ((size_t)UINT32_MAX)) {
        fprintf(stderr, "Error: input file too large\n");
        goto cleanup;
    }

    lexer_clean(buf, (uint32_t*)&size);
    for (int i = 0; i < size; ++i) {
        printf("%c", buf[i]);
    }
    printf("\n");

    free(buf);
    return 0;
cleanup:
    free(buf);
    return 1;
}

uchar* file_load(const char* restrict path, size_t* restrict size)
{
    uchar* buf = NULL;
    FILE* fp = fopen(path, "rb");
    *size = 0;

    if (NULL == fp) {
        perror("fopen");
        return NULL;
    }

    if (fseek(fp, 0, SEEK_END)) {
        perror("fseek");
        goto cleanup;
    }

    const long file_size = ftell(fp);

    if (file_size < 1) {
        perror("ftell");
        goto cleanup;
    }

    rewind(fp);
    *size = (size_t)file_size;
    buf = calloc(*size, sizeof(*buf));
    if (NULL == buf) {
        perror("calloc");
        goto cleanup;
    }

    if ((*size) != fread(buf, 1, *size, fp)) {
        perror("fread");
        goto cleanup;
    }

    fclose(fp);
    return buf;
cleanup:
    *size = 0;
    fclose(fp);
    free(buf);
    return NULL;
}
