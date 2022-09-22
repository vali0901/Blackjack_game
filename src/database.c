#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/list_fct.h"
#include "../include/utils.h"
#include "../include/hashtable_fct.h"

user *create_new_user(char *name) 
{
    user *tmp = calloc(1, sizeof(user));
    strcpy(tmp->name, name);
    
    char pass1[24], pass2[24];
    do{
        printf("Create a password:\n");
        scanf("%s", pass1);
        printf("Retype your password:\n");
        scanf("%s", pass2);
    } while(strcmp(pass1, pass2));

    strcpy(tmp->passwd, pass1);

    return tmp;
}

hashtable *register_new_user(hashtable *ht)
{

start_fct:
    char name[24];
    printf("Please enter your user_name:\n");
    scanf("%s", name);
    
    if(ht_has_key(ht, name)) {
        printf("This user_name is not available\n");
        
        char aux;
        do{
            printf("Do you want to try another one?(y/n)");
            
            do{
                scanf("%c", &aux);
            } while(aux == '\0' || aux == '\n');
        
        } while(aux != 'y' && aux != 'n');
        
        if(aux == 'n')
            return ht;
        else
            goto start_fct; 
    }

    user *new_user = create_new_user(name);
    
    if(ht)
        ht_put(ht, name, strlen(name) + 1, new_user, sizeof(user));
    else
        printf("Database not working... please try again later\n");

    free(new_user);
    return ht;
}

user *log_in(hashtable *ht)
{
    char name[24];
    printf("Please enter your user_name:\n");
    scanf("%s", name);
    
    user *curr_user = (user *)ht_get(ht, name);
    
    if(!curr_user) {
        printf("User does not exist, register it first\n");
        return NULL;
    }
    
    char pass[24];
    for(int i = 0; i < 3; i++) {
        printf("Enter your password:\n");
        scanf("%s", pass);
        
        if(!strcmp(pass, curr_user->passwd)) {
            printf("Log in succesful. Have fun, %s!\n", name);
            return curr_user;
        } else {
            printf("Wrong password! You have %d tries left\n", 2 - i);
        }
    }
    
    printf("Exiting...\n");
    return NULL;
}

user *log_out(hashtable *ht, user *curr_user)
{
    // update the user infos intto the hashtable
    ht_put(ht, curr_user->name, strlen(curr_user->name) + 1, curr_user, sizeof(user));
    printf("Log out done!\n");
    return NULL;
}

hashtable *delete_user(hashtable *ht)
{
    char name[24];
    printf("Please enter the user_name you want to delete:\n");
    scanf("%s", name);
    
    user *curr_user = (user *)ht_get(ht, name);
    
    if(!curr_user) {
        printf("User does not exist, exiting...\n");
        return ht;
    }
    
    char pass[24];
    for(int i = 0; i < 3; i++) {
        printf("Enter user's password\n");
        scanf("%s", pass);
        
        if(!strcmp(pass, curr_user->passwd)) {
            printf("Log in succesful. Deleting user %s...\n", name);
            ht_remove_entry(ht, name);
            return ht;
        } else {
            printf("Wrong password! You have %d tries left\n", 2 - i);
        }
    }
    
    printf("Exiting...\n");
    return ht;
}

hashtable *load_database()
{
    FILE *f = fopen(".database", "rb");
    
    hashtable *tmp = ht_create(100, hash_function_string, compare_function_strings);
    user *aux = malloc(sizeof(user));
   
    while(fread(aux, sizeof(user), 1, f)) {
        ht_put(tmp, aux->name, strlen(aux->name) + 1, aux, sizeof(user));
    }

    fclose(f);
    
    free(aux);
    return tmp;
}

void exit_database(hashtable *ht)
{
    FILE *f = fopen(".database", "wb");

    for(int i = 0, j = 0; i < ht->hmax && j < ht->size; i++) {
        node *ptr = ht->buckets[i]->head;
        while(ptr) {
            user *curr_user = (user *)(((info *)ptr->data)->value);
            fwrite(curr_user, sizeof(user), 1, f);
            ptr = ptr->next;
            j++;
        }
    }

    ht_free(ht);
    ht = NULL;
    
    fclose(f);
}

void print_user_info(user *curr_user)
{
    printf("User_name: %s\n", curr_user->name);
    printf("Available balance: %f\n", curr_user->balance);
    printf("User stats:\n");
    printf("\tMoney placed on bets: %lf\n", curr_user->user_stats.money_bet);
    printf("\tWinnings: %lf\n", curr_user->user_stats.winnings);
    printf("\tOverall balance: %lf\n", curr_user->user_stats.winnings - curr_user->user_stats.money_bet);
    printf("\tTotal number of bets: %d\n", curr_user->user_stats.nrhands);
    printf("\tTotal number of wins: %d\n", curr_user->user_stats.handwin);
    printf("\tTotal number of losses: %d\n", curr_user->user_stats.handloss);
}

user *deposit(user *curr_user)
{
    printf("To confirm your identity, you have to enter your password.\n");

    char pass[24];
    for(int i = 0; i < 3; i++) {
        printf("Enter your password:\n");
        scanf("%s", pass);
        
        if(!strcmp(pass, curr_user->passwd)) {
            printf("Identity confirmed!\n");
            break;
        } else {
            printf("Wrong password! You have %d tries left\n", 2 - i);
        }
    }

    printf("Please enter the amount of money you want to deposit:(min 5)\n");

    float sum;
    scanf("%f", &sum);

    if(sum < 5) {
        printf("Deposit must be at least 5!\nExiting...\n");
        return curr_user;
    }
    
    printf("Money added to your account. Have fun, %s!\n", curr_user->name);
    
    curr_user->balance += sum;
    return curr_user;
}