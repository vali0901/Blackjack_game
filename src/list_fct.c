#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/utils.h"

list* create_list(unsigned int data_size)
{
    list *tmp = malloc(sizeof(list));
    tmp->data_size = data_size;
    tmp->head = NULL;
    tmp->size = 0;
    return tmp;
}

node* get_nth_node(list* list, int n)
{
    node *ptr = list->head;
    int i = 0;
    while(i < n)
    {
        ptr = ptr->next;
        i++;
    }
    return ptr;
}

list *add_nth_node(list* list, unsigned int n, const void* data)
{
    node *new_nod = malloc(sizeof(node));
    new_nod->data = malloc(list->data_size);
    memcpy(new_nod->data, data, list->data_size);

    if(n == 0) {
        list->head = new_nod;
        new_nod->next = NULL;
        list->size += 1;
        return list;
    }
    node *ptr = get_nth_node(list, n - 1);

    new_nod->next = ptr->next;
    ptr->next = new_nod;

    list->size += 1;

    return list;
}

node* remove_nth_node(list* list, unsigned int n)
{
    if(n == 0)
    {
        node *ptr = list->head;
        
        list->head = ptr->next;

        ptr->next = NULL;
        
        list->size -= 1;
        return ptr;
    }
    node *ptr = get_nth_node(list, n - 1);

    node *q = ptr->next;

    ptr->next = ptr->next->next;

    q->next = NULL;

    list->size -= 1;
    return q;
}

list *free_list(list* pp_list)
{
    node *ptr = pp_list->head;
    int i = 0;
    while(i < pp_list->size) {
        node *q = ptr->next;
        free(ptr->data);
        free(ptr);
        ptr = q;
        i++;
    }
    free(pp_list);
    return NULL;
}

list *list_copy(list *pp_list)
{
    list *tmp = malloc(sizeof(list));
    tmp->head = NULL;
    tmp->size = pp_list->size;
    tmp->data_size = pp_list->data_size;
    node *ptr = pp_list->head;
    int k = 0;
    while(ptr) {
        add_nth_node(tmp, k++, ptr->data);
        ptr = ptr->next;
    }
    return tmp;
}

node** list_to_array(list *pp_list)
{
    node** array = malloc(pp_list->size * sizeof(node *));

    int i = 0;
    node* ptr = pp_list->head;

    while(i < pp_list->size)
    {
        //printf("%d\n", i);
        array[i] = ptr;
        ptr = ptr->next;
        i++;
    }

    return array;
}

list *merge_and_free_lists(list *final_list, list *added_list)
{
    if(!final_list) {
        return added_list;
    }

    if(!added_list) {
        return final_list;
    }

    node *final_node = get_nth_node(final_list, final_list->size - 1);
    if(final_node)
        final_node->next = added_list->head;
    else
        final_list->head = added_list->head;
    
    final_list->size += added_list->size;
    free(added_list);

    
    return final_list;
}