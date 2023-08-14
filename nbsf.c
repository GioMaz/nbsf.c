#include <stdio.h>
#include "nbsf.h"

// Initial probability of a word being spam
#define INITIAL_SPAM_PR 0.45

// Initial probability of a word being spam
#define INITIAL_HAM_PR (1 - INITIAL_SPAM_PR)

#define STRENGTH 3

double product(double *word_spam_prs, int dim)
{
    double res = 1;
    for (int i = 0; i < dim; ++i) {
        res *= word_spam_prs[i];
    }
    return res;
}

double comp_product(double *word_spam_prs, int dim)
{
    double res = 1;
    for (int i = 0; i < dim; ++i) {
        res *= 1 - word_spam_prs[i];
    }
    return res;
}

double spam_word_pr(double word_spam_pr, double word_ham_pr)
{
    double num = word_spam_pr * INITIAL_SPAM_PR;
    double den = (word_spam_pr * INITIAL_SPAM_PR)
        + (word_ham_pr * INITIAL_HAM_PR);

    // printf("ISP: %f IHP: %f\n", INITIAL_SPAM_PR, INITIAL_HAM_PR);
    // printf("wsp: %f whp: %f\n", word_spam_pr, word_ham_pr);
    // printf("num: %f den: %f\n\n", num, den);
    return num / den;
}

double spam_word_pr_corrected(double word_spam_pr, double word_ham_pr, int n)
{
    double sw_pr = spam_word_pr(word_spam_pr, word_ham_pr);
    double num = (STRENGTH * INITIAL_SPAM_PR) + (n * sw_pr);
    double den = STRENGTH + n;

    return num / den;
}

double phrase_spam_pr(double *word_spam_prs, int dim)
{
    double num = product(word_spam_prs, dim);
    double den = comp_product(word_spam_prs, dim) + num;

    return num / den;
}
