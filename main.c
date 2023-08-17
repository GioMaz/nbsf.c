#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "train.h"
#include "nbsf.h"

#define DEBUG 0

#define MAX_MSG_SIZE 128

void get_spam_word_prs(Dict *dict, Dataset *ds_spam, Dataset *ds_ham,
        double *spam_word_prs, size_t *msg_size, char *msg, const char *sep)
{
    char *key = strtok(msg, sep);

    while (key != NULL && *msg_size < MAX_MSG_SIZE) {
        for (char *p = key; *p; p++)
            *p = tolower(*p);

        size_t spam_count;
        size_t ham_count;
        get_counts(dict, key, &spam_count, &ham_count);

        double word_spam_pr = (double) spam_count / (double) ds_spam->size;
        double word_ham_pr = (double) ham_count / (double) ds_ham->size;
        size_t n = spam_count + ham_count;

        spam_word_prs[*msg_size] = 
            spam_word_pr_corrected(word_spam_pr, word_ham_pr, n);

#if DEBUG
        printf("key: %s\twsp: %f whp: %f swp: %f\n", key, word_spam_pr, word_ham_pr, spam_word_prs[*msg_size]);
#endif

        (*msg_size)++;
        key = strtok(NULL, sep);
    }
}

int main(int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr,
                "usage: %s [training data] [evaluation data]\n",
                argv[0]);
        return 1;
    }

    FILE *f_train = fopen(argv[1], "r");
    if (f_train == NULL) {
        fprintf(stderr,
                "%s: %s: No such file or directory\n",
                argv[0],
                argv[1]);
        return 1;
    }

    FILE *f_eval = fopen(argv[2], "r");
    if (f_train == NULL) {
        fprintf(stderr,
                "%s: %s: No such file or directory\n",
                argv[0],
                argv[2]);
        return 1;
    }

    // Populate datasets with csv rows
    Dataset ds_spam, ds_ham;
    read_csv(f_train, &ds_spam, &ds_ham);

    // Populate dictionary with spam words
    Dict dict;
    const char *sep = " ,.-!?";
    populate_dict(&dict, &ds_spam, sep, true);
    populate_dict(&dict, &ds_ham, sep, false);

#if DEBUG
    for (int i = 0; i < dict.size; i++) {
        printf("%d\t%s\t\t\t%zu %zu\n", i, dict.keys[i], dict.spam_counts[i], dict.ham_counts[i]);
    }
#endif

    Dataset ds_spam_eval, ds_ham_eval;
    read_csv(f_eval, &ds_spam_eval, &ds_ham_eval);

    size_t tp = 0, fp = 0, tn = 0, fn = 0;

    double spam_word_prs[MAX_MSG_SIZE];
    size_t msg_size = 0;
    for (int i = 0; i < ds_spam_eval.size; i++) {
        get_spam_word_prs(&dict, &ds_spam, &ds_ham,
                spam_word_prs, &msg_size, ds_spam_eval.rows[i], sep);

        double res = msg_spam_pr(spam_word_prs, msg_size);

        msg_size = 0;

        res > INITIAL_HAM_PR ? tp++ : fp++;
    }

    for (int i = 0; i < ds_ham_eval.size; i++) {
        get_spam_word_prs(&dict, &ds_spam, &ds_ham,
                spam_word_prs, &msg_size, ds_ham_eval.rows[i], sep);

        double res = msg_spam_pr(spam_word_prs, msg_size);

        msg_size = 0;

        res < INITIAL_HAM_PR ? tn++ : fn++;
    }

    // Percentage of predicted positives that are actually positives
    double precision = ((double) tp) / ((double) (tp + fp));

    // Percentage of actual positives predicted correctly (also called sensitivity)
    double recall = ((double) tp) / ((double) (tp + fn));

    // Percentage of times the model is correctly
    // this is a bad index because the dataset is imbalanced
    double accuracy = ((double) (tp + tn)) / ((double) (tp + fp + tn + fn));

    printf("True positive: %zu\n", tp);
    printf("False positive: %zu\n", fp);
    printf("True negative: %zu\n", tn);
    printf("False negative: %zu\n", fn);
    printf("Precision: %f\n", precision);
    printf("Recall: %f\n", recall);
    printf("Accuracy: %f\n", accuracy);

    return 0;
}
