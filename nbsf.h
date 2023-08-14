#ifndef NBSF_H
#define NBSF_H

// Initial probability of a word being spam
#define INITIAL_SPAM_PR 0.45

// Initial probability of a word being spam
#define INITIAL_HAM_PR (1 - INITIAL_SPAM_PR)

// If there are 3 spam messages with a specific word
// more confidence in the spamicity than in INITIAL_SPAM_PR
#define STRENGTH 3

double product(double *, int);
double comp_product(double *, int);
double spam_word_pr(double, double);
double spam_word_pr_corrected(double, double, int);
double phrase_spam_pr(double *, int);

#endif
