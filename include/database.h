#ifndef DATABASE_H_
#define DATABASE_H_

#include "utils.h"
#include "list_fct.h"
#include "hashtable_fct.h"

user *create_new_user(char *);

hashtable *register_new_user(hashtable *);

user *log_in(hashtable *);

user *log_out(hashtable *, user *);

hashtable *delete_user(hashtable *);

hashtable *load_database();

void exit_database(hashtable *);

void print_user_info(user *);

user *deposit(user *);

#endif  //DATABASE_H_