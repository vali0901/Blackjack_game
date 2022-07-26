#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "include/list_fct.h"
#include "include/utils.h"

int score(list *player_cards)
{
	int sum = 0;
	int has_ace = 0;
	node *ptr = player_cards->head;

	while (ptr)
	{	if((*((card *)ptr->data)).nr == 1)
			has_ace = 1;
		if((*((card *)ptr->data)).nr < 10)
			sum += (*((card *)ptr->data)).nr;
		else
			sum += 10;

		ptr = ptr->next;
	}

	if(has_ace && sum + 10 <= 21)
		sum += 10;
	
	return sum;
}

int is_split_card(card dealed_card)
{
	return (dealed_card.nr == 0);
}

void print_players_hand(hand some_hand, int number)
{
		printf("\t%d hand:\n", number);
		sleep(1);
		node *ptr = some_hand.cards->head;
		while(ptr) {
			printf("\t\t");
			print_card(*((card *)ptr->data));
			ptr = ptr->next;
			sleep(1);
		}
		printf("\tscore: %d\n", score(some_hand.cards));

}

void print_dealers_hand(hand some_hand, int number)
{
	if(number == 0) {
		printf("\t\t");
		print_card(*((card*)some_hand.cards->head->data));
		sleep(1);
		printf("\t\tunkown\n");
	} else {
		node *ptr = some_hand.cards->head;
		while(ptr) {
			printf("\t\t");
			print_card(*((card *)ptr->data));
			ptr = ptr->next;
			sleep(1);
		}
		printf("\tscore: %d\n", score(some_hand.cards));
	}
}

int check_split(list *cards, int *aces_pair) {
	if(((card *)cards->head->data)->nr == ((card *)cards->head->next->data)->nr && cards->size == 2) {
		if(((card *)cards->head->data)->nr == 1)
			*aces_pair = 1;
		else
			*aces_pair = 0;
		return 1;
	}
	*aces_pair = 0;
	return 0;
}

hand *resize_hand(hand *player_hand, int i, int nr_seats)
{
	player_hand = realloc(player_hand, sizeof(hand) * (nr_seats + 1));
	for(int j = nr_seats; j > i; j--)
	{	
		player_hand[j + 1] = player_hand[j];
	}
	player_hand[i + 1].cards = create_list(sizeof(card));
	node *second_card = remove_nth_node(player_hand[i].cards, 1);
	add_nth_node(player_hand[i + 1].cards, 0, second_card);
	free(second_card);

	return player_hand;
}

list* deal_cards(list* decks)
{
	list* dealed_cards = create_list(sizeof(card));

	int nr_seats;

	int split_card_dealed = 0;
	int breaker = 0;
	

	do
	{
		printf("Insert the number of seats (bets): ");
		scanf("%d", &nr_seats);
		
		hand *player_hands, dealer_hand;
		player_hands = malloc(sizeof(hand) * nr_seats);
		for(int i = 0 ; i < nr_seats; i++) {
			node* dealed_card = remove_nth_node(decks, 0);
			split_card_dealed = is_split_card(*((card*)dealed_card->data));

			if(split_card_dealed) {
				breaker = 1;
				free(dealed_card->data);
				free(dealed_card);
				--i;
				continue;
			}

			//add_nth_node(dealed_cards, dealed_cards->size, dealed_card->data);
			player_hands[i].cards = create_list(sizeof(card));
			add_nth_node(player_hands[i].cards, player_hands[i].cards->size, dealed_card->data);
			
			//player_cards[i] = *((card *)(dealed_card->data));
			free(dealed_card->data);
			free(dealed_card);
		}	

		node* dealed_card = remove_nth_node(decks, 0);
		split_card_dealed = is_split_card(*((card*)dealed_card->data));

		if(split_card_dealed) {
			breaker = 1;
			free(dealed_card->data);
			free(dealed_card);
			node* dealed_card = remove_nth_node(decks, 0);
		}

		dealer_hand.cards = create_list(sizeof(card));
		//add_nth_node(dealed_cards, dealed_cards->size, dealed_card->data);
		
		add_nth_node(dealer_hand.cards, dealer_hand.cards->size, dealed_card->data);
		free(dealed_card->data);
		free(dealed_card);
		
		for(int i = 0 ; i < nr_seats; i++) {
			node* dealed_card = remove_nth_node(decks, 0);
			split_card_dealed = is_split_card(*((card*)dealed_card->data));
			if(split_card_dealed) {
				breaker = 1;
				free(dealed_card->data);
				free(dealed_card);
				--i;
				continue;
			}
			
			add_nth_node(player_hands[i].cards, player_hands[i].cards->size, dealed_card->data);
			//player_cards[i * 2 + 1] = *((card *)(dealed_card->data));

			free(dealed_card->data);
			free(dealed_card);
		}	

		dealed_card = remove_nth_node(decks, 0);
		split_card_dealed = is_split_card(*((card*)dealed_card->data));
		if(split_card_dealed) {
			breaker = 1;
			free(dealed_card->data);
			free(dealed_card);
			node* dealed_card = remove_nth_node(decks, 0);
		}
		add_nth_node(dealer_hand.cards, dealer_hand.cards->size, dealed_card->data);
		free(dealed_card->data);
		free(dealed_card);
		
		printf("Dealer's hand:\n");
		sleep(1);
		print_dealers_hand(dealer_hand, 0);

		printf("Your cards:\n");

		for(int i = 0; i < nr_seats; i++) {
			//print_card(*((card *)player_cards[i]->head->data));
			sleep(1);
			print_players_hand(player_hands[i], i + 1);
		}
		sleep(1);
		//players_decisions
		printf("\nTime for decisions:\n");
		for(int i = 0; i < nr_seats; i++)
		{		
			player_hands[i].score = score(player_hands[i].cards);
			int bust = 0;
			if(player_hands[i].score == 21) {
				sleep(1);
				printf("Congratulations! Blackjack on the %d hand!\n", i + 1);
				continue;
			}
			int aces_pair;
			int can_split = check_split(player_hands[i].cards, &aces_pair);
			
			if(can_split) {
				printf("Decisions for %d hand:\n", i+1);
				sleep(1);
				while(player_hands[i].score < 21){
				printf("Dealer's hand:\n");
				print_dealers_hand(dealer_hand, 0);

				printf("Your cards:\n");
				print_players_hand(player_hands[i], i + 1);


				char decision[8];
				printf("Please take a decision for %d hand:(stay/hit/double/split)\n", i + 1);
				scanf("%s", decision);
				if(!strcmp(decision, "stay")) {
					break;
				} else if(!strcmp(decision, "hit")) {
					node* dealed_card = remove_nth_node(decks, 0);
					split_card_dealed = is_split_card(*((card*)dealed_card->data));

					if(split_card_dealed) {
						breaker = 1;
						free(dealed_card->data);
						free(dealed_card);
						node* dealed_card = remove_nth_node(decks, 0);
					}

					add_nth_node(player_hands[i].cards, player_hands[i].cards->size, dealed_card->data);

					player_hands[i].score = score(player_hands[i].cards);
					sleep(1);
					if (player_hands[i].score >= 21) {
						printf("Dealer's hand:\n");
						print_dealers_hand(dealer_hand, 0);

						printf("Your cards:\n");
						print_players_hand(player_hands[i], i + 1);

						if(player_hands[i].score > 21)
							printf("Damn! You busted\n");
					}
						
				} else if(!strcmp(decision, "double")) {
					node* dealed_card = remove_nth_node(decks, 0);
					split_card_dealed = is_split_card(*((card*)dealed_card->data));

					if(split_card_dealed) {
						breaker = 1;
						free(dealed_card->data);
						free(dealed_card);
						node* dealed_card = remove_nth_node(decks, 0);
					}

					add_nth_node(player_hands[i].cards, player_hands[i].cards->size, dealed_card->data);
					sleep(1);
					printf("Dealer's hand:\n");
					print_dealers_hand(dealer_hand, 0);
					
					printf("Your cards:\n");
					print_players_hand(player_hands[i], i + 1);

					player_hands[i].score = score(player_hands[i].cards);
				
					if (player_hands[i].score > 21)
						printf("Damn! You busted\n");

					break;
				}else if(!strcmp(decision, "split")) {
					player_hands = resize_hand(player_hands, i, nr_seats);

					node* dealed_card = remove_nth_node(decks, 0);
					split_card_dealed = is_split_card(*((card*)dealed_card->data));

					if(split_card_dealed) {
						breaker = 1;
						free(dealed_card->data);
						free(dealed_card);
						node* dealed_card = remove_nth_node(decks, 0);
					}
					
					add_nth_node(player_hands[i].cards, player_hands[i].cards->size, dealed_card);

					free(dealed_card);

					dealed_card = remove_nth_node(decks, 0);
					split_card_dealed = is_split_card(*((card*)dealed_card->data));

					if(split_card_dealed) {
						breaker = 1;
						free(dealed_card->data);
						free(dealed_card);
						node* dealed_card = remove_nth_node(decks, 0);
					}

					add_nth_node(player_hands[i + 1].cards, player_hands[i + 1].cards->size, dealed_card);

					free(dealed_card) ;

					if(aces_pair == 1) {
						i++;
						continue;
					} else {
						--i;
						continue;
					}
					
				} else {
					printf("Please use one of the following words: stay, hit, double, split\n");
				}
			}
			}

			printf("Decisions for %d hand:\n", i+1);
			sleep(1);
			while(player_hands[i].score < 21){
				printf("Dealer's hand:\n");
				print_dealers_hand(dealer_hand, 0);

				printf("Your cards:\n");
				print_players_hand(player_hands[i], i + 1);


				char decision[8];
				printf("Please take a decision for %d hand:(stay/hit/double)\n", i + 1);
				scanf("%s", decision);
				if(!strcmp(decision, "stay")) {
					break;
				} else if(!strcmp(decision, "hit")) {
					node* dealed_card = remove_nth_node(decks, 0);
					split_card_dealed = is_split_card(*((card*)dealed_card->data));

					if(split_card_dealed) {
						breaker = 1;
						free(dealed_card->data);
						free(dealed_card);
						node* dealed_card = remove_nth_node(decks, 0);
					}

					add_nth_node(player_hands[i].cards, player_hands[i].cards->size, dealed_card->data);

					player_hands[i].score = score(player_hands[i].cards);
					sleep(1);
					if (player_hands[i].score >= 21) {
						printf("Dealer's hand:\n");
						print_dealers_hand(dealer_hand, 0);

						printf("Your cards:\n");
						print_players_hand(player_hands[i], i + 1);

						if(player_hands[i].score > 21)
							printf("Damn! You busted\n");
					}
						
				} else if(!strcmp(decision, "double")) {
					node* dealed_card = remove_nth_node(decks, 0);
					split_card_dealed = is_split_card(*((card*)dealed_card->data));

					if(split_card_dealed) {
						breaker = 1;
						free(dealed_card->data);
						free(dealed_card);
						node* dealed_card = remove_nth_node(decks, 0);
					}

					add_nth_node(player_hands[i].cards, player_hands[i].cards->size, dealed_card->data);
					sleep(1);
					printf("Dealer's hand:\n");
					print_dealers_hand(dealer_hand, 0);
					
					printf("Your cards:\n");
					print_players_hand(player_hands[i], i + 1);

					player_hands[i].score = score(player_hands[i].cards);
				
					if (player_hands[i].score > 21)
						printf("Damn! You busted\n");

					break;
				} else {
					printf("Please use one of the following words: stay, hit, double\n");
				}
			}
		}

		sleep(1);
		printf("Dealer's card reveal:\n");
		print_dealers_hand(dealer_hand, 1);

		printf("Your cards:\n");

		for(int i = 0; i < nr_seats; i++) {
		//print_card(*((card *)player_cards[i]->head->data));
			sleep(1);
			print_players_hand(player_hands[i], i + 1);
		}

		//dealer's showdown

		sleep(1);
		dealer_hand.score = score(dealer_hand.cards);
		while(dealer_hand.score < 17) {
			//hit
			node* dealed_card = remove_nth_node(decks, 0);
			split_card_dealed = is_split_card(*((card*)dealed_card->data));

			if(split_card_dealed) {
				breaker = 1;
				free(dealed_card->data);
				free(dealed_card);
				node* dealed_card = remove_nth_node(decks, 0);
			}

			add_nth_node(dealer_hand.cards, dealer_hand.cards->size, dealed_card->data);
			dealer_hand.score = score(dealer_hand.cards);
			
			printf("Dealer is hitting:\n");
			sleep(1);
			print_dealers_hand(dealer_hand, 1);

			if(dealer_hand.score > 21)
				printf("Congratulations! Dealer busted\n");
		}

		//win conclusion

		for(int i = 0; i < nr_seats; i++) {
			if(dealer_hand.score == 21 && dealer_hand.cards->size == 2 && player_hands[i].score == 21 && player_hands[i].cards->size == 2)
				printf("Push on %d hand, money back.\n", i + 1);
			else if(dealer_hand.score == 21 && dealer_hand.cards->size == 2 && !(player_hands[i].score == 21 && player_hands[i].cards->size == 2))
				printf("Dealer has blackjack! No winnings on %d hand.\n", i + 1);
			else if(!(dealer_hand.score == 21 && dealer_hand.cards->size == 2) && player_hands[i].score == 21 && player_hands[i].cards->size == 2)
				printf("Congratulations! Blackjack on %d hand, payed 3 to 2!\n", i + 1);
			else if(player_hands[i].score > 21)
				printf("Bust on %d hand, no winnings.\n", i + 1);
			else if(dealer_hand.score > 21 && player_hands[i].score <= 21)
				printf("Dealer busted! Win on %d hand.\n", i + 1);
			else if(dealer_hand.score <= 21 && player_hands[i].score <= 21) {
				if(dealer_hand.score > player_hands[i].score)
					printf("Dealer wins on %d hand, no winnings.\n", i + 1);
				else if (dealer_hand.score < player_hands[i].score)
					printf("Win on %d hand, payed 2 to 1!\n", i + 1);
				else
					printf("Push on %d hand, money back.\n", i + 1);
			}
		}
	} while(!breaker);

	

	return decks;
}

list* shuffle(list* decks)
{
	node** list_array = list_to_array(decks);
	
	int size = decks->size;
	
	time_t t;
	//printf("%ld", t);
	srand(t);
	int index = (int)random() % size;
  
	node* curr_node = list_array[index];
	list_array[index] = NULL;
	node* next_node;

	decks->head = curr_node;

	for(int i = 0; i < decks->size - 1; i++) {
		do {
			index = (int)random() % size;
			//printf("%d\n", index);
		} while(!list_array[index]);

		next_node = list_array[index];

		curr_node->next = next_node;
		curr_node = next_node;
		list_array[index] = NULL;
	}
	
	curr_node->next = NULL;
	
	int split_index = (int)random() % (size / 10) - size / 20;
	split_index += size / 2;
	//printf("%d\n", split_index);

	int i = 0;
	node *ptr = decks->head;
	while(i < split_index)
	{   
		ptr = ptr->next;
		i++;
	}

	card split_card;
	split_card.nr = 0;
	strcpy(split_card.sym, "RED\n");

	add_nth_node(decks, split_index, &split_card);

	free(list_array);
	return decks;
}

list* start(list *decks)
{
	FILE *f = fopen("deck", "rt");

	decks = create_list(sizeof(card));

	char line[64];
	card new_card;

	while(fgets(line, 64, f)) {
		int nr = atoi(strtok(line, " "));
		char *sym = strtok(NULL, " ");
		sym[strlen(sym) - 1] = '\0';
		new_card.nr = nr;
		strcpy(new_card.sym, sym);
		
		add_nth_node(decks, decks->size, &new_card);
	}

	fclose(f);

	return decks;
}

int main()
{
	//printf("BAw\n");
	list *decks = NULL;
	decks = start(decks);
	decks = shuffle(decks);
	decks = deal_cards(decks);
	//print_deck(decks);
	//decks = free_list(decks);
	return 0;
}