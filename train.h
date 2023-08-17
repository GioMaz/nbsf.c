#include <stdlib.h>
#include <stdio.h>

#define DS_SIZE 2048
#define ROW_SIZE 512

typedef struct {
    char rows[DS_SIZE][ROW_SIZE];
    int size;
} Dataset;

#define DICT_CAPACITY 4096
#define KEY_SIZE 128

typedef struct {
    char keys[DICT_CAPACITY][KEY_SIZE];
    size_t spam_counts[DICT_CAPACITY];
    size_t ham_counts[DICT_CAPACITY];
    size_t size;
} Dict;

void read_csv(FILE *f, Dataset *ds_spam, Dataset *ds_ham);
void populate_dict(Dict *dict, Dataset *ds, const char *sep, bool spam);
void inc_dict(Dict *dict, char *key, bool spam);
void get_counts(Dict *dict, char *key, size_t *spam_count, size_t *ham_count);
