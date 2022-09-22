#ifndef UTILS_H_
#define UTILS_H_

#define MAX_SYMBOL_LENGHT 24

typedef struct {
    double money_bet, winnings;
    double overall_balance;
    unsigned int nrhands, handloss, handwin;
} stats;

typedef struct {
    char name[24];
    char passwd[24];
    float balance;
    stats user_stats;
} user;

typedef struct
{
	unsigned int nr;
	char sym[MAX_SYMBOL_LENGHT];
} card;

typedef struct node
{
    void* data;
    struct node *next;
} node;

typedef struct list
{
    node* head;
    unsigned int data_size;
    unsigned int size;
} list;

typedef struct hand
{
    list *cards;
    int score;
    float bet;
} hand;

typedef struct info {
	void *key;
	void *value;
} info;

typedef struct {
	list **buckets; /* Array de liste simplu-inlantuite. */
	/* Nr. total de noduri existente curent in toate bucket-urile. */
	unsigned int size;
	unsigned int hmax; /* Nr. de bucket-uri. */
	/* (Pointer la) Functie pentru a calcula valoarea hash asociata cheilor. */
	unsigned int (*hash_function)(void*);
	/* (Pointer la) Functie pentru a compara doua chei. */
	int (*compare_function)(void*, void*);
} hashtable;

void print_card(card);

void print_deck(list *);

#endif
