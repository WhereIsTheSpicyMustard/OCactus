#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "lexer.h"

char* file_load(const char* const restrict path, uint32_t* restrict size)
{
    char* buf = NULL;
    FILE* fp = fopen(path, "rb");
    *size = 0;

    if (NULL == fp) {
        report_error("fopen");
        return NULL;
    }

    if (fseek(fp, 0, SEEK_END)) {
        report_error("fseek");
        goto cleanup;
    }

    const long file_size = ftell(fp);

    if (file_size < 1) {
        report_error("ftell");
        goto cleanup;
    }

    if (fseek(fp, 0, SEEK_SET)) {
        report_error("fseek");
        goto cleanup;
    }

    if ((unsigned long)file_size > UINT32_MAX) {
        report_error("File too large");
        goto cleanup;
    }

    *size = (uint32_t)file_size;
    buf = calloc((size_t)(*size), sizeof(*buf));
    if (NULL == buf) {
        report_error("Failed to allocate buf");
        goto cleanup;
    }

    if ((size_t)(*size) != fread(buf, 1, (size_t)(*size), fp)) {
        report_error("fread");
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

int main(int argc, char** argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: cc <path to file>\n");
        return 1;
    }

    char* buf = NULL;
    uint32_t size;
    buf = file_load(argv[1], &size);
    if (NULL == buf) goto cleanup;

    lexer_clean(buf, &size);

    for (uint32_t i = 0; i < size; ++i)
        printf("%c", buf[i]);
    printf("\n");

    free(buf);
    return 0;
cleanup:
    free(buf);
    return 1;
}

