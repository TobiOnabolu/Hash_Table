/*
 -------------------------------------
 File:    quiz8.c
 Project: onab4340_quiz08
 file description
 -------------------------------------
 Author:  Alvin Onabolu
 ID:      190994340
 Email:   onab4340@mylaurier.ca
 Version  2020-11-22
 -------------------------------------
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Constants
int HASH_SIZE = 10;

///node for chained hashing
typedef struct node {
	char user[30];
	struct node *next;

} node;

///create a new node
struct node* newNode(char info[]) {
	struct node *new_node = (struct node*) malloc(sizeof(struct node));
	if (new_node != NULL) {
		new_node->next = NULL;
		strcpy(new_node->user, info);

	}
	return new_node;
}

///make a new string with the format user/pass
void clean_string(char *user, char *new_user) {
	//user and new user are pointer to first element in array

	int i = 0;
	int j = 0;
	int space_count = 0;

	//traverse user string
	//while all these conditions are true, run the while
	while ((*(user) != '\0') && (*(user) != '\r') && (*(user) != '\n')) {
		//if char in user is not a space copy it to new string
		if (*user != ' ') {
			//use strncpy, since we just want to copy a single char at a time
			strncpy(new_user, user, 1);
			//increase newuser to point to next array spot
			new_user += 1;
		}
		//if char is space and there has only been one space fill new user with backslash
		else if ((*user == ' ') && (space_count == 0)) {
			strncpy(new_user, "/", 1);
			space_count += 1;
			new_user += 1;
		}
		//traverse next spot in string
		user += 1;
	}
	//ensure the last element after string is null char
	(*new_user) = '\0';
	return;
}

///hash fx using division method
int hash_fx(int key) {
	int hash_val;
	hash_val = key % HASH_SIZE;
	return hash_val;
}

///get ascii sum
int sum_string(char user[]) {
	int i = 0;
	int sum = 0;
	//get ascii sum
	while (user[i] != '\0') {
		sum += user[i];
		i += 1;
	}
	return (sum);
}

int main() {
	setbuf(stdout, NULL);
	char user[30], new_user[30], pass[15];
	struct node *curr = NULL, *check = NULL;
	int key, hash_val;

	//create a file pointer
	FILE *fptr;

	//Initialize an array of null pointer
	int *hash_table[10] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL };

	//open file for reading
	fptr = fopen("password.txt", "r");
	//Make sure file opened
	if (fptr == NULL) {
		printf("File did not open properly");
		exit(1);
	}

	//traverse file
	while (!feof(fptr)) {
		//read a line in file, and store it in user
		fgets(user, 30, fptr);

		//format the string to user/pass
		clean_string(user, new_user);

		//get the key and then the hash
		key = sum_string(new_user);
		hash_val = hash_fx(key);

		//store regularly if spot is empty
		if (hash_table[hash_val] == NULL)
			hash_table[hash_val] = newNode(new_user);

		//chain hash on collision
		else {
			//make a curr node and it give it smae address as the shared key value
			//change the next pointer to new_node of user
			curr = (hash_table[hash_val]);
			curr->next = newNode(new_user);
		}

	}
	fclose(fptr);

	///test
	printf("\nLogin: ");
	scanf(" %s", &user);
	printf("Password: ");
	scanf(" %s", &pass);

	//make info user/pass format
	strcat(user, "/");
	strcat(user, pass);

	//get key and hash val
	key = sum_string(user);
	hash_val = hash_fx(key);

	//if key not even in table, login failed
	if (hash_table[hash_val] == NULL) {
		printf("Authentication: Failed");
	}

	//if key in table
	else {
		//have check node, that loops thru the array spot to see, if that user/combo is mapped to the key
		check = hash_table[hash_val];
		//loop thru to find user/combo
		while ((check != NULL) && (strcmp(check->user, user) != 0)) {
			check = check->next;
		}
		//if check did not reach null, then we found correct combo
		if (check != NULL)
			printf("Authentication: Successful ");
		//if it didnt then we didnt find any combo for that key
		else
			printf("Authentication: Failed");
	}

}
