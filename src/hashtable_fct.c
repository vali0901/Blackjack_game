#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/list_fct.h"
#include "../include/utils.h"

int compare_function_strings(void *a, void *b)
{
	char *str_a = (char *)a;
	char *str_b = (char *)b;

	return strcmp(str_a, str_b);
}

unsigned int hash_function_string(void *a)
{
	/*
	 * Credits: http://www.cse.yorku.ca/~oz/hash.html
	 */
	unsigned char *puchar_a = (unsigned char*) a;
	unsigned long hash = 5381;
	int c;

	while ((c = *puchar_a++))
		hash = ((hash << 5u) + hash) + c; /* hash * 33 + c */

	return hash;
}

hashtable * ht_create(unsigned int hmax, unsigned int (*hash_function)(void*), int (*compare_function)(void*, void*))
{
    hashtable *h = malloc(sizeof(hashtable));
    h->size = 0;
    h->hmax = hmax;
    h->hash_function = hash_function;
    h->compare_function = compare_function;
    h->buckets = malloc(hmax * sizeof(list));
    
    for(int i = 0; i < hmax; i++)
        h->buckets[i] = create_list(sizeof(info));
    
    return h;
}

int ht_has_key(hashtable *ht, void *key)
{
	unsigned int hash = ht->hash_function(key) % ht->hmax;
    node *ptr = ht->buckets[hash]->head;
    while(ptr) {
        if(ht->compare_function(key, (*(info *)(ptr->data)).key) == 0)
            return 1;
        ptr = ptr->next;
    }

	return 0;
}

void *ht_get(hashtable *ht, void *key)
{
	
    if(ht_has_key(ht, key) == 1) {
        unsigned int hash = ht->hash_function(key) % ht->hmax;
        node *ptr = ht->buckets[hash]->head;
        while(ptr)
        {
            if(ht->compare_function(key, (*(info *)(ptr->data)).key) == 0)
                return (*(info *)(ptr->data)).value;
            ptr = ptr->next;
        }
    }

	return NULL;
}

void ht_put(hashtable *ht, void *key, unsigned int key_size, void *value, unsigned int value_size)
{
    unsigned int hash = ht->hash_function(key) % ht->hmax;
    if(ht_has_key(ht, key) == 1) {
        node *ptr = ht->buckets[hash]->head;
        while(ptr) {
            if(ht->compare_function(key, (*(info *)(ptr->data)).key) == 0)
                break;
            ptr = ptr->next;
        }
        
        memcpy((*(info *)(ptr->data)).value, value, value_size);
    } else if(ht_has_key(ht, key) == 0) {
        info *data;
        data = malloc(sizeof(info));
        
        data->key = malloc(key_size);
        memcpy(data->key, key, key_size);
        
        data->value = malloc(value_size);
        memcpy(data->value, value, value_size);
        
        add_nth_node(ht->buckets[hash], ht->buckets[hash]->size, data);
        
        free(data);
        ht->size++;
    }
}

void ht_remove_entry(hashtable *ht, void *key)
{
	if(ht_has_key(ht, key) == 1)
    {
        unsigned int hash = ht->hash_function(key) % ht->hmax;
        node *ptr = ht->buckets[hash]->head;
        node *rm;
        int i = 0;
        while(ptr) {
            if(ht->compare_function(key, (*(info *)(ptr->data)).key) == 0) {
                rm = remove_nth_node(ht->buckets[hash], i);
                break;
            }
            ptr = ptr->next;
            i++;
        }
        if(rm) {
            free((*((info *)rm->data)).key);
            free((*((info *)rm->data)).value);
            free(rm->data);
            free(rm);
        }
    }
}

void ht_free(hashtable *ht)
{	
    for(int i = 0; i < ht->hmax; i++) {
        node *ptr = ht->buckets[i]->head;
        while(ptr) {
            node *rm = ptr;
            ptr = ptr->next;
            free((*((info *)rm->data)).key);
            free((*((info *)rm->data)).value);
            free(rm->data);
            free(rm);
        }
        free(ht->buckets[i]);
    }
    free(ht->buckets);
    free(ht);
}