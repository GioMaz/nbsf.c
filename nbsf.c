#include <stdlib.h>
#include "nbsf.h"

/*
 * Product of probabilities
 * params:
 *  - word_spam_prs: array of word probabilities
 *  - dim: size of the array
 */
double product(double *word_spam_prs, size_t dim)
{
    double res = 1;
    for (size_t i = 0; i < dim; ++i) {
        res *= word_spam_prs[i];
    }
    return res;
}

/*
 * Product of complementaries of probabilities
 * params:
 *  - word_spam_prs: array of word probabilities
 *  - dim: size of the array
 */
double comp_product(double *word_spam_prs, size_t dim)
{
    double res = 1;
    for (size_t i = 0; i < dim; ++i) {
        res *= 1 - word_spam_prs[i];
    }
    return res;
}

/*
 * Probability that a word is spam given the probability
 * that that word is in a spam message and in a ham message
 * params:
 *  - word_spam_pr: probability that a word is spam
 *  - word_ham_pr: probability that a word is ham
 */
double spam_word_pr(double word_spam_pr, double word_ham_pr)
{
    double num = word_spam_pr * INITIAL_SPAM_PR;
    double den = (word_spam_pr * INITIAL_SPAM_PR)
        + (word_ham_pr * INITIAL_HAM_PR);

    return num / den;
}

/*
 * Corrected version of spam_word_pr
 * params:
 *  - word_spam_pr: probability that a word is spam
 *  - word_ham_pr: probability that a word is ham
 *  - n: number of times the word is present in the training dataset
 */
double spam_word_pr_corrected(double word_spam_pr, double word_ham_pr, size_t n)
{
    if (n == 0)
        return INITIAL_SPAM_PR;

    double sw_pr = spam_word_pr(word_spam_pr, word_ham_pr);
    double num = (STRENGTH * INITIAL_SPAM_PR) + (n * sw_pr);
    double den = STRENGTH + n;

    return num / den;
}

/*
 * Probability that a message is spam given the
 * single word probabilities
 * params:
 *  - word_spam_pr: array of word probabilities in a message
 *  - dim: size of the array
 */
double msg_spam_pr(double *word_spam_prs, size_t dim)
{
    double num = product(word_spam_prs, dim);
    double den = comp_product(word_spam_prs, dim) + num;

    return num / den;
}
