#ifndef HASHTABLE_FCT_H_
#define HASHTABLE_FCT_H_

#include "utils.h"

int compare_function_strings(void *, void *);

unsigned int hash_function_string(void *);

hashtable * ht_create(unsigned int, unsigned int (*)(void*), int (*)(void*, void*));

int ht_has_key(hashtable *, void *);

void *ht_get(hashtable *, void *);

void ht_put(hashtable *, void *, unsigned int, void *, unsigned int);

void ht_remove_entry(hashtable *, void *);

void ht_free(hashtable *);

#endif  //HASHTABLE_FCT_H_