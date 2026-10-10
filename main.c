#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char uchar;

int main(int argc, char** argv)
{
    uchar* buf = NULL;

    if (argc < 2) {
        fprintf(stderr, "Usage: cc <path to file>\n");
        return 1;
    }

    FILE* fp = fopen(argv[1], "rb");
    if (NULL == fp) {
        perror("fopen");
        return 1;
    }

    if (fseek(fp, 0, SEEK_END)) {
        perror("fseek");
        goto cleanup;
    }

    const long file_size = ftell(fp);
    rewind(fp);

    if (file_size < 1) {
        perror("ftell");
        goto cleanup;
    }

    buf = calloc((size_t)file_size, sizeof(*buf));
    if (NULL == buf) {
        perror("calloc");
        goto cleanup;
    }

    if ((size_t)file_size != fread(buf, 1, (size_t)file_size, fp)) {
        perror("fread");
        goto cleanup;
    }

    for (int i = 0; i < file_size; ++i) {
        printf("%c", buf[i]);
    }

    fclose(fp);
    free(buf);
    return 0;
cleanup:
    fclose(fp);
    free(buf);
    return 1;
}
