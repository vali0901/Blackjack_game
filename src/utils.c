#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/utils.h"

void print_card(card curr_card)
{
    printf("%d %s\n", curr_card.nr, curr_card.sym);
}

void print_deck(list *deck)
{
    node *ptr = deck->head;
    int i = 0;
    while(ptr)
    {
        print_card(*((card *)ptr->data));
        ptr = ptr->next;
        i++;
    }
    printf("%d\n", deck->size);
}