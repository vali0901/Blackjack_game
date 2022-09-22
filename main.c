#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "include/list_fct.h"
#include "include/utils.h"
#include "include/hashtable_fct.h"
#include "include/database.h"
#include "include/game.h"

int main()
{
	// user database
	hashtable *ht = NULL;
	ht = load_database();

	user *curr_user = NULL;
	char buff[24];
	
	do {
		printf("What do you want to do? (register_user, log_in, log_out, play, delete_user, exit, user_info, deposit)\n");
		scanf("%s", buff);
		
		if(!strcmp(buff, "play")) {
			if(!curr_user) {
				printf("You have to log in first!\n");
				continue;
			}
			if(curr_user->balance < 5) {
				printf("You dont have enough money for playing!\nDeposit first!\n");
				continue;
			}
			play(curr_user);
		} else if(!strcmp(buff, "register_user")) {
			if(curr_user) {
				printf("Log_out first!\n");
				continue;
			}
			register_new_user(ht);
		} else if(!strcmp(buff, "log_in")) {
			if(curr_user) {
				printf("Log_out first!\n");
				continue;
			}
			curr_user = log_in(ht);
		} else if(!strcmp(buff, "log_out")) {
			if(!curr_user) {
				printf("Log_in first!\n");
				continue;
			}
			curr_user = log_out(ht, curr_user);
		} else if(!strcmp(buff, "delete_user")) {
			if(curr_user) {
				printf("Log_out first!\n");
				continue;
			}
			ht = delete_user(ht);
		} else if(!strcmp(buff, "user_info")) {
			if(!curr_user) {
				printf("Log_in first!\n");
				continue;
			}
			print_user_info(curr_user);
		} else if(!strcmp(buff, "deposit")) {
			if(!curr_user) {
				printf("Log_in first!\n");
				continue;
			}
			curr_user = deposit(curr_user);
		}
	} while(strcmp(buff, "exit"));

	exit_database(ht);
	
	return 0;
}