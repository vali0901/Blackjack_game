#ifndef LIST_FCT_H_
#define LIST_FCT_H_

#include "utils.h"

list* create_list(unsigned int);

node* get_nth_node(list*, unsigned int);

list *add_nth_node(list*, unsigned int, const void*);

node* remove_nth_node(list*, unsigned int);

list *free_list(list*);

list *list_copy(list *);

node** list_to_array(list *);

list *merge_and_free_lists(list *, list *); 

#endif  //LIST_FCT_H_