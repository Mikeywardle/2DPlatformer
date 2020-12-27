#include "FileIO.h"
#include <stdio.h>
#include <stdlib.h>

const char* ReadTextFile(const char* file)
{
    FILE* fp;
    long size;
    char* buffer;

    errno_t err = fopen_s(&fp, file, "rb");

    fseek(fp, 0L, SEEK_END);
    size = ftell(fp);
    rewind(fp);

    buffer = (char*)calloc(1, size + 1);
    if (!buffer) fclose(fp), fputs("", stderr), exit(1);

    if (1 != fread(buffer, size, 1, fp))
        fclose(fp), free(buffer), fputs("entire read fails", stderr), exit(1);

    return buffer;
}
