#ifndef NBSF_H
#define NBSF_H

/*
 * TRAINING PARAMETERS
 */

// Initial probability of a word being spam
#define INITIAL_SPAM_PR 0.6

// Initial probability of a word being spam
#define INITIAL_HAM_PR (1 - INITIAL_SPAM_PR)

// If there are 3 spam messages with a specific word
// more confidence will be put in the spamicity 
// rather than in the INITIAL_SPAM_PR
#define STRENGTH 3

double product(double *word_spam_prs, int dim);
double comp_product(double *word_spam_prs, int dim);
double spam_word_pr(double word_spam_pr, double word_ham_pr);
double spam_word_pr_corrected(double word_spam_pr, double word_ham_pr, int n);
double msg_spam_pr(double *word_spam_prs, int dim);

#endif
