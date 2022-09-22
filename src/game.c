#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "../include/list_fct.h"
#include "../include/utils.h"

// this function takes the cards from file and creates a list with them without changing their order
list* start(list *decks)
{
	printf("How many decks do you want to play with?\n");
	
	int nr;
	do{
		printf("Please enter one of the following options: 1, 2, 4, 8.\n");
		scanf("%d", &nr);
	} while(nr != 1 && nr != 2 && nr != 4 && nr != 8);
	char filename[12];

	if(nr == 1)
		strcpy(filename, "Decks/deck");
	else if(nr == 2)
		strcpy(filename, "Decks/deck2");
	else if(nr == 4)
		strcpy(filename, "Decks/deck4");
	else if(nr == 8)
		strcpy(filename, "Decks/deck8");
	
	FILE *f = fopen(filename, "rt");

	decks = create_list(sizeof(card));

	char line[32];
	card new_card;

	while(fgets(line, 32, f)) {
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

// here the cards are shuffled and the list with the randomized order of cards is returned
list* shuffle(list* decks)
{
	// creating an array because it is easier to acces random nodes from an array comparing to a list
	node** list_array = list_to_array(decks);
	
	int size = decks->size;
	
	// generating the first random index
	time_t t;
	srand(t);
	int index = (int)random() % size;

	// and setting him as the head of the decks of cards
	node* curr_node = list_array[index];
	list_array[index] = NULL;
	decks->head = curr_node;

	node* next_node;

	// doing the same steps until there are no more nodes in the array
	for(int i = 0; i < size - 1; i++) {
		do {
			index = (int)random() % size;
		} while(!list_array[index]);

		next_node = list_array[index];

		curr_node->next = next_node;
		curr_node = next_node;
		list_array[index] = NULL;
	}
	
	curr_node->next = NULL;
	
	// creating the index for the "red card" so that he is approximately in the middle
	int split_index = (int)random() % (size / 10) - size / 20;
	split_index += size / 2;

	card split_card;
	split_card.nr = 0;
	strcpy(split_card.sym, "RED\n");

	add_nth_node(decks, split_index, &split_card);

	free(list_array);
	return decks;
}

// returns the score of a blackjack hand
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

// checks if the card is the "red card"
int is_split_card(card dealed_card)
{
	return (dealed_card.nr == 0);
}

// for the print functions i am using sleep function for a better display
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

// checks if a hand can be splitted and also if that is a pair of aces
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

int useful_scanf() {
	char buff[5];
	scanf("%s", buff);
	if(!strcmp(buff, "stop")) {
		return 0;
	}
	return atoi(buff);
}

// draw a card from the decks and put it in the hand
// returns 0 if the "red card" was not drawn and 1 otherwise
int deal_a_card(list *decks, list **hand, int *breaker)
{
	if(!(*hand)) {
		*hand = create_list(sizeof(card));
	}

	node* dealed_card = remove_nth_node(decks, 0);
	int split_card_dealed = is_split_card(*((card*)dealed_card->data));

	if(split_card_dealed) {
		free(dealed_card->data);
		free(dealed_card);
		*breaker = 1;
		return 1;
	}

	add_nth_node(*hand, (*hand)->size, dealed_card->data);

	free(dealed_card->data);
	free(dealed_card);

	return 0;
}

list* deal_cards(list* decks, user *curr_user)
{
	int nr_seats;

	int split_card_dealed = 0;
	int breaker = 0;
	
	// the list where all of the used cards are stored and when the player stops playing
	// all the remaining cards are merged to this and this list is returned
	list* dealed_cards = create_list(sizeof(card));

	do {
		if(curr_user->balance < 5) {
			printf("Your balance is too low! Deposit first.\nExiting...\n");
			break;
		}
		
		do {
			printf("Insert the number of seats (bets, less or equal than 7):\nIf you want to stop playing type stop\n");
			nr_seats = useful_scanf();
			if(nr_seats > 7) {
				printf("Max 7 seats allowed!\n");
			}
			if(nr_seats * 5 > curr_user->balance && nr_seats)
				printf("You dont have enough balance for all the seats (min bet per seat: 5)\n");
		} while((nr_seats * 5 > curr_user->balance && nr_seats) || (nr_seats > 7 && nr_seats));
		
		if(nr_seats == 0) {
			dealed_cards = merge_and_free_lists(dealed_cards, decks);

			return dealed_cards;
		}

		hand *player_hands, *dealer_hand;
		player_hands = calloc(nr_seats, sizeof(hand));
		dealer_hand = calloc(1, sizeof(hand));

		float tmp[nr_seats], ok = 1;
		do {
			ok = 1;
			float sum = 0;
			printf("Please enter the bet for each hand. (5 -> 5000)\nAvailable balance: %f\n", curr_user->balance);
			for(int i = 0; i < nr_seats; i++) {
				float aux;
				scanf("%f", &aux);
				if(aux < 5 || aux > 5000) {
					printf("The bet must be between 5 and 5000\n");
					ok = 0;
					break;
				}
				tmp[i] = aux;
				sum += aux;
			}

			if(sum > curr_user->balance) {
				ok = 0;
				printf("Not enough money in your balance!\n");
			} else {
				curr_user->balance -= sum;
				curr_user->user_stats.money_bet += sum;
				curr_user->user_stats.nrhands += nr_seats;
			}

		} while(!ok);

		for(int i = 0; i < nr_seats; i++)
			player_hands[i].bet = tmp[i];

		printf("Bets placed succesfully!\n");

		//dealing the first row of cards for the player
		for(int i = 0 ; i < nr_seats; i++) {
			if(deal_a_card(decks, &(player_hands[i].cards), &breaker)) {
				deal_a_card(decks, &(player_hands[i].cards), &breaker);
			}
		}

		// then dealing the first card for the dealer
		if(deal_a_card(decks, &(dealer_hand->cards), &breaker)) {
			deal_a_card(decks, &(dealer_hand->cards), &breaker);
		}	
		
		// dealing the second row of cards for the player
		for(int i = 0 ; i < nr_seats; i++) {
			if(deal_a_card(decks, &(player_hands[i].cards), &breaker)) {
				deal_a_card(decks, &(player_hands[i].cards), &breaker);
			}
		}	

		// and the second and unknown card for the dealer
		if(deal_a_card(decks, &(dealer_hand->cards), &breaker)) {
			deal_a_card(decks, &(dealer_hand->cards), &breaker);
		}
		
		// showing the cards
		printf("Dealer's hand:\n");
		sleep(1);
		print_dealers_hand(dealer_hand[0], 0);

		printf("Your cards:\n");

		for(int i = 0; i < nr_seats; i++) {
			sleep(1);
			print_players_hand(player_hands[i], i + 1);
		}
		sleep(1);

		// player's decisions
		printf("\nTime for decisions:\n");
		for(int i = 0; i < nr_seats; i++)
		{
			// first checks if the hand is a blackjack
			player_hands[i].score = score(player_hands[i].cards);
			if(player_hands[i].score == 21) {
				sleep(1);
				printf("Congratulations! Blackjack on the %d hand!\n", i + 1);
				continue;
			}

			// if not, the player must decide what he will do based on the available_decisions
			printf("Decisions for %d hand:\n", i + 1);
			sleep(1);

			int aces_pair;
			int can_split = check_split(player_hands[i].cards, &aces_pair);
			int can_double = (player_hands[i].cards->size == 2 ? 1 : 0);

			char available_decisions[48] = "stay/hit";
			
			if(can_double && player_hands[i].bet <= curr_user->balance)
				strcat(available_decisions, "/double");
			if(can_split && player_hands[i].bet <= curr_user->balance)
				strcat(available_decisions, "/split");

			while(player_hands[i].score < 21){
				printf("Dealer's hand:\n");
				print_dealers_hand(*dealer_hand, 0);

				printf("Your cards:\n");
				print_players_hand(player_hands[i], i + 1);

				printf("Please take a decision for %d hand:(%s)\nCurrent balance: %f\n", i + 1, available_decisions, curr_user->balance);

				char decision[8];
				scanf("%s", decision);
				if(!strcmp(decision, "stay")) {
					break;
				} else if(!strcmp(decision, "hit")) {
					if(deal_a_card(decks, &(player_hands[i].cards), &breaker)) {
						deal_a_card(decks, &(player_hands[i].cards), &breaker);
					}

					player_hands[i].score = score(player_hands[i].cards);
					sleep(1);
					if (player_hands[i].score >= 21) {
						printf("Dealer's hand:\n");
						print_dealers_hand(*dealer_hand, 0);

						printf("Your cards:\n");
						print_players_hand(player_hands[i], i + 1);

						if(player_hands[i].score > 21)
							printf("Damn! You busted\n");
					}
					strcpy(available_decisions, "stay/hit");
				} else if(!strcmp(decision, "double") && strstr(available_decisions, "double")) {
					
					curr_user->balance -= player_hands[i].bet;
					curr_user->user_stats.money_bet += player_hands[i].bet;

					player_hands[i].bet *= 2;
					
					if(deal_a_card(decks, &(player_hands[i].cards), &breaker)) {
						deal_a_card(decks, &(player_hands[i].cards), &breaker);
					}

					sleep(1);
					printf("Dealer's hand:\n");
					print_dealers_hand(*dealer_hand, 0);
				
					printf("Your cards:\n");
					print_players_hand(player_hands[i], i + 1);

					player_hands[i].score = score(player_hands[i].cards);

					if (player_hands[i].score > 21)
						printf("Damn! You busted\n");

					break;
				} else if(!strcmp(decision, "split") && strstr(available_decisions, "split")) {

					curr_user->balance -= player_hands[i].bet;
					curr_user->user_stats.money_bet += player_hands[i].bet;

					// when a hand is splitted, there is allocated one more place for the new hand
					player_hands = realloc(player_hands, sizeof(hand) * (nr_seats + 1));
					for(int j = nr_seats; j > i + 1; j--) {	
						player_hands[j] = player_hands[j - 1];
					}
					
					player_hands[i + 1].cards = create_list(sizeof(card));
					player_hands[i + 1].bet = player_hands[i].bet;

					node *second_card = remove_nth_node(player_hands[i].cards, 1);
					add_nth_node(player_hands[i + 1].cards, 0, second_card->data);
					free(second_card->data);
					free(second_card);
					nr_seats++;

					if(deal_a_card(decks, &(player_hands[i].cards), &breaker)) {
						deal_a_card(decks, &(player_hands[i].cards), &breaker);
					}

					if(deal_a_card(decks, &(player_hands[i + 1].cards), &breaker)) {
						deal_a_card(decks, &(player_hands[i + 1].cards), &breaker);
					}

					// if the pair was of aces, the new hands are printed and then skipped
					if(aces_pair == 1) {
						printf("Your cards:\n");
						print_players_hand(player_hands[i], i + 1);
						i++;
						print_players_hand(player_hands[i], i + 1);
						break;
					} else {
						i--;
						break;
					}
					
				} else {
					printf("Please use one of the following options: %s\n", available_decisions);
				}
			}
		}

		//dealer's reveal
		sleep(1);
		printf("Dealer's card reveal:\n");
		print_dealers_hand(*dealer_hand, 1);

		printf("Your cards:\n");

		int all_hands_are_bj = 1, all_hands_are_bust = 1;
		for(int i = 0; i < nr_seats; i++) {
			sleep(1);
			print_players_hand(player_hands[i], i + 1);
			if(player_hands[i].cards->size != 2 || player_hands[i].score != 21)
				all_hands_are_bj = 0;
			if(player_hands[i].score <= 21)
				all_hands_are_bust = 0;
		}

		sleep(1);
		dealer_hand->score = score(dealer_hand->cards);
		while(dealer_hand->score < 17 && !all_hands_are_bj && !all_hands_are_bust) {
			if(deal_a_card(decks, &(dealer_hand->cards), &breaker)) {
				deal_a_card(decks, &(dealer_hand->cards), &breaker);
			}

			dealer_hand->score = score(dealer_hand->cards);
			
			printf("Dealer is hitting:\n");
			sleep(1);
			print_dealers_hand(*dealer_hand, 1);

			if(dealer_hand->score > 21)
				printf("Congratulations! Dealer busted\n");
		}

		// win conclusion
		for(int i = 0; i < nr_seats; i++) {
			if(dealer_hand->score == 21 && dealer_hand->cards->size == 2 && player_hands[i].score == 21 && player_hands[i].cards->size == 2) {
				printf("Push on %d hand, money back.\n", i + 1);
				curr_user->balance += player_hands[i].bet;
				curr_user->user_stats.winnings += player_hands[i].bet;
			} else if(dealer_hand->score == 21 && dealer_hand->cards->size == 2 && !(player_hands[i].score == 21 && player_hands[i].cards->size == 2)) {
				printf("Dealer has blackjack! No winnings on %d hand.\n", i + 1);
				curr_user->user_stats.handloss++;
			} else if(!(dealer_hand->score == 21 && dealer_hand->cards->size == 2) && player_hands[i].score == 21 && player_hands[i].cards->size == 2) {
				printf("Congratulations! Blackjack on %d hand, payed 3 to 2!\n", i + 1);
				curr_user->balance += player_hands[i].bet * 3 / 2;
				curr_user->user_stats.winnings += player_hands[i].bet * 3 / 2;
				curr_user->user_stats.handwin++;
			} else if(player_hands[i].score > 21) {
				printf("Bust on %d hand, no winnings.\n", i + 1);
				curr_user->user_stats.handloss++;
			} else if(dealer_hand->score > 21 && player_hands[i].score <= 21) {
				printf("Dealer busted! Win on %d hand.\n", i + 1);
				curr_user->balance += player_hands[i].bet * 2;
				curr_user->user_stats.winnings += player_hands[i].bet * 2;
				curr_user->user_stats.handwin++;
			} else if(dealer_hand->score <= 21 && player_hands[i].score <= 21) {
				if(dealer_hand->score > player_hands[i].score) {
					printf("Dealer wins on %d hand, no winnings.\n", i + 1);
					curr_user->user_stats.handloss++;
				} else if (dealer_hand->score < player_hands[i].score) {
					printf("Win on %d hand, payed 2 to 1!\n", i + 1);
					curr_user->balance += player_hands[i].bet * 2;
					curr_user->user_stats.winnings += player_hands[i].bet * 2;
					curr_user->user_stats.handwin++;
				} else {
					printf("Push on %d hand, money back.\n", i + 1);
					curr_user->balance += player_hands[i].bet;
					curr_user->user_stats.winnings += player_hands[i].bet;
				}
			}
		}

		printf("Current balance: %f\n", curr_user->balance);

		for(int i = 0; i < nr_seats; i++) {
			dealed_cards = merge_and_free_lists(dealed_cards, player_hands[i].cards);
		}

		free(player_hands);

		dealed_cards = merge_and_free_lists(dealed_cards, dealer_hand->cards);

		free(dealer_hand);

	} while(!breaker);

	if(breaker)
		printf("The shoe needs to be changed, exiting...\n");

	dealed_cards = merge_and_free_lists(dealed_cards, decks);

	return dealed_cards;
}

// saving the cards in their randomized order
void save_decks(list *decks)
{
	char filename[12];
	if(decks->size == 52 || decks->size == 53)
		strcpy(filename, "Decks/deck");
	else if(decks->size == 104 || decks->size == 105)
		strcpy(filename, "Decks/deck2");
	else if(decks->size == 208 || decks->size == 209)
		strcpy(filename, "Decks/deck4");
	else if(decks->size == 416 || decks->size == 417)
		strcpy(filename, "Decks/deck8");

	FILE *f = fopen(filename, "wt");

	node *ptr = decks->head;

	while(ptr) {
		if(!((card *)ptr->data)->nr) {
			ptr = ptr->next;
			continue;
		}

		fprintf(f, "%u %s\n", ((card *)ptr->data)->nr, ((card *)ptr->data)->sym);

		ptr = ptr->next;
	}

	fclose(f);
}

void play(user *curr_user) 
{
	list *decks = NULL;
	decks = start(decks);
	decks = shuffle(decks);
	decks = deal_cards(decks, curr_user);
	save_decks(decks);
	decks = free_list(decks);
}