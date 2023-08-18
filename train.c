#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "train.h"

void read_csv(FILE *f, Dataset *ds_spam, Dataset *ds_ham)
{
    char buffer[ROW_SIZE];
    char *v1;
    char *v2;
    // Ignore first row
    fgets(buffer, ROW_SIZE, f);

    int i = 0;
    while (!feof(f) && i < TS_SIZE) {
        fgets(buffer, ROW_SIZE, f);

        v1 = strtok(buffer, ",");
        v2 = strtok(NULL, ",,,");

        if (v2 != NULL) {
            if (strcmp(v1, "ham") == 0) {
                strcpy(ds_spam->rows[ds_spam->size], v2);
                ds_spam->size++;
            } else {
                strcpy(ds_ham->rows[ds_ham->size], v2);
                ds_ham->size++;
            }
        }

        i++;
    }
}

void populate_dict(Dict *dict, Dataset *ds, const char *sep, bool spam)
{
    char *key;
    for (int i = 0; i < ds->size; i++) {
        key = strtok(ds->rows[i], sep);
        while (key != NULL) {
            for (char *p = key; *p; p++)
                *p = tolower(*p);

            inc_dict(dict, key, spam);

            key = strtok(NULL, sep);
        }
    }
}

void inc_dict(Dict *dict, char *key, bool spam)
{
    bool found = false;
    int i;
    for (i = 0; i < dict->size; i++) {
        if (strcmp(dict->keys[i], key) == 0) {
            if (spam)
                dict->spam_counts[i]++;
            else
                dict->ham_counts[i]++;

            found = true;
        }
    }

    if (!found && i < DICT_CAPACITY) {
        strcpy(dict->keys[i], key);

        if (spam)
            dict->spam_counts[i] = 1;
        else
            dict->ham_counts[i] = 1;

        dict->size++;
    }
}

void get_counts(Dict *dict, char *key, size_t *spam_count, size_t *ham_count)
{
    *spam_count = 0;
    *ham_count = 0;
    bool found = false;
    for (int i = 0; i < dict->size; i++) {
        if (strcmp(dict->keys[i], key) == 0) {
            *spam_count = dict->spam_counts[i];
            *ham_count = dict->ham_counts[i];
        }
    }
}
