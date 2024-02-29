#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

static const size_t FILE_SZ           = 174;
static const size_t FILE_HASH         = 7607535328156473761;
static const size_t PATCHED_FILE_HASH = 8921944274526814744;

static const size_t PATCH_POS = 53;

size_t djb2Hash(char *str, size_t str_len);

int main(int argc, char **argv)
{
    if(argc != 2) return EXIT_FAILURE;

    struct stat file_stat = {};
    int ex_stat = stat(argv[1], &file_stat);
    if(ex_stat == EOF) return EXIT_FAILURE;

    if(file_stat.st_size != FILE_SZ) return EXIT_FAILURE;
    char *buf = (char *)calloc(sizeof(char), FILE_SZ);

    FILE *patch_file = fopen(argv[1], "r+");
    fread(buf, sizeof(char), FILE_SZ, patch_file);
    fclose(patch_file);

    size_t hash = djb2Hash(buf, FILE_SZ);

    if(hash != FILE_HASH)
    {
        if(hash == PATCHED_FILE_HASH) printf("Already patched.\n");

        free(buf);
        return EXIT_FAILURE;
    }

    char jmp_code  = 0xEB;
    buf[PATCH_POS] = jmp_code;

    FILE *patched_file = fopen("PASSW_patched.COM", "wb");
    fwrite(buf, sizeof(char), FILE_SZ, patch_file);
    fclose(patched_file);

    free(buf);
    return EXIT_SUCCESS;
}

size_t djb2Hash(char *str, size_t str_len)
{
    size_t hash = 5381;
    for(size_t i = 0; i < str_len; i++)
    {
        hash = ((hash << 5) + hash) + str[i];
    }
    return hash;
}