#include <complex.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "nbsf.h"

#define ROW_SIZE 512
#define DS_SIZE 5600

typedef struct {
    char rows[DS_SIZE][ROW_SIZE];
    int size;
} Dataset;

#define KEY_SIZE 128
#define DICT_CAPACITY 16348

typedef struct {
    char keys[DICT_CAPACITY][KEY_SIZE];
    size_t spam_counts[DICT_CAPACITY];
    size_t ham_counts[DICT_CAPACITY];
    size_t size;
} Dict;

void inc_spam(Dict *dict, char *key)
{
    bool found = false;
    int i;
    for (i = 0; i < dict->size; i++) {
        if (strcmp(dict->keys[i], key) == 0) {
            dict->spam_counts[i]++;
            found = true;
        }
    }

    if (!found && i < DICT_CAPACITY) {
        strcpy(dict->keys[i], key);
        dict->spam_counts[i] = 1;
        dict->size++;
    }
}

void inc_ham(Dict *dict, char *key)
{
    bool found = false;
    int i;
    for (i = 0; i < dict->size; i++) {
        if (strcmp(dict->keys[i], key) == 0) {
            dict->ham_counts[i]++;
            found = true;
        }
    }

    if (!found && i < DICT_CAPACITY) {
        strcpy(dict->keys[i], key);
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

int main(int argc, char *argv[])
{
    Dataset ds_spam;
    Dataset ds_ham;

    FILE *f = fopen("spam.csv", "r");
    if (f == NULL) {
        printf("Opening of csv file failed!\n");
        return 1;
    }

    char buffer[ROW_SIZE];
    char *v1;
    char *v2;

    // ignore first row
    fgets(buffer, ROW_SIZE, f);

    int i = 0;
    while (!feof(f) && i < DS_SIZE) {
        fgets(buffer, ROW_SIZE, f);

        v1 = strtok(buffer, ",");
        v2 = strtok(NULL, ",,,");

        if (v2 != NULL) {
            if (strcmp(v1, "ham")) {
                strcpy(ds_spam.rows[ds_spam.size], v2);
                ds_spam.size++;
            } else {
                strcpy(ds_ham.rows[ds_ham.size], v2);
                ds_ham.size++;
            }
        }

        i++;
    }

    // Populate dictionary with spam words
    Dict dict;
    char *key;
    char *sep = " ,.-!?";
    for (int i = 0; i < ds_spam.size; i++) {
        key = strtok(ds_spam.rows[i], sep);
        while (key != NULL) {
            for (char *p = key; *p; p++)
                *p = tolower(*p);

            inc_spam(&dict, key);
            key = strtok(NULL, sep);
        }
    }

    // Populate dictionary with ham words
    for (int i = 0; i < ds_ham.size; i++) {
        key = strtok(ds_ham.rows[i], sep);
        while (key != NULL) {
            for (char *p = key; *p; p++)
                *p = tolower(*p);

            inc_ham(&dict, key);
            key = strtok(NULL, sep);
        }
    }

    // // Print results
    // for (int i = 0; i < dict.size; i++) {
    //     printf("%d %s %zu %zu\n", i, dict.keys[i],
    //             dict.spam_counts[i], dict.ham_counts[i]);
    // }

#define MAX_PHRASE_SIZE 128

    char phrase[] = "How are you my friend?";
    double word_spam_prs[MAX_PHRASE_SIZE];
    size_t phrase_size = 0;
    key = strtok(phrase, sep);
    size_t spam_count;
    size_t ham_count;
    double word_spam_pr;
    double word_ham_pr;
    size_t count;
    while (key != NULL && phrase_size < MAX_PHRASE_SIZE) {
        for (char *p = key; *p; p++)
            *p = tolower(*p);

        get_counts(&dict, key, &spam_count, &ham_count);
        word_spam_pr = (double) spam_count / (double) ds_spam.size;
        word_ham_pr = (double) ham_count / (double) ds_ham.size;
        count = spam_count + ham_count;
        if (word_spam_pr == 0) {
            word_spam_prs[phrase_size] = 0.5;
        } else {
            word_spam_prs[phrase_size] = 
                spam_word_pr_corrected(word_spam_pr, word_ham_pr, count);
        }

        phrase_size++;
        key = strtok(NULL, sep);
    }

    double res = phrase_spam_pr(word_spam_prs, phrase_size);
    printf("RES: %f\n", res);

    return 0;
}
