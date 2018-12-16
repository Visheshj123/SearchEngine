#include "hashmap.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


//holds output of rank() for each word, will be used at the end to compare
double Ri_1;
double Ri_2;
double Ri_3;


//removes word if it appears in all three documents
void stop_word(struct hashmap* t, char* word){
  printf("Removing %s in stop_word()\n", word);
  //calls hm_remove() three times since each will have its own node for the same word
  hm_remove(t, word);
  return;



}

//initializes hashmap and creates a node for each bucket
struct hashmap* hm_create(int num_buckets){
  struct hashmap *t = (struct hashmap*)malloc(sizeof(struct hashmap));
  t->map = (struct llnode**)malloc(sizeof(struct llnode*) * num_buckets); //# of buckets
  t->num_buckets = num_buckets;
  t->num_elements = 0;

  int i = 0;
  while(i < num_buckets){
    t->map[i] = (struct llnode*)malloc(sizeof(struct llnode));
    t->map[i]->next = NULL;
    t->map[i]->word = "";
    t->map[i]->D1count = 0.0;
    t->map[i]->D2count = 0.0;
    t->map[i]->D3count = 0.0;
    i++;
  }
  //go through each bucket and create an empty node to point to
  return t;
}

//fetches node and returns number of occurences within that document, if not in there, returns -1
struct llnode* hm_get(struct hashmap* hm, char* word){
  int j = hash(hm, word); //get value assoicated with key
  struct llnode *listptr;
  //check if bucket is empty
  if (hm->map[j] == NULL){
    listptr = NULL;
    printf("Node not found\n");
    return listptr;
  }
  else{
    listptr = hm->map[j]; //if bucket is not empty, then we set a pointer to first node
  }
    struct llnode *temp = listptr;
  while (temp != NULL){
    //compare if word and document_id of node matches the arguments, if so we return num_occurrences
    if (strcmp(temp->word,word) == 0){
      return temp;
    }
    else{
    temp = temp->next; //if node doesn't match parameter, get next node
    }
  }
  printf("Node not found\n");
  temp = NULL;
  return temp;//if no node is present then we return 0
}

//places node in hashmap, if word already exists in document, then adds 1 to num_occurrences
void hm_put(struct hashmap* hm, char* word, double D1count, double D2count, double D3count){
  int j = hash(hm, word); //gets bucket number
  struct llnode *list;
  list = hm->map[j]; //list now points to appropriate buckets

  struct llnode *temp;
  temp = list; //temp and list point to same bucket
    while(temp){
      //compare strings to see if node has same contents as arguments
    if ((strcmp(temp->word,word) == 0)){
      temp->D1count += D1count; //if so we can iterate num_occurrences in that document_id
      temp->D2count += D2count;
      temp->D3count += D3count;
      return;
    }
    else{
      temp = temp->next;
    }
  }

  struct llnode *nNode = (struct llnode*)malloc(sizeof(struct llnode)); //get space for new node if value doesn't exist

  nNode->word = (char*)malloc(sizeof(char) * strlen(word));
  strcpy(nNode->word, word);

  nNode->D1count = D1count;
  nNode->D2count = D2count;
  nNode->D3count = D3count;

  hm->map[j] = nNode; //places node into appropriate bucket;
  hm->num_elements += 1;
  nNode->next = list; //list is a pointer to the first element in that bucket
  printf("Made node for %s", nNode->word);
  return;

}

//goes to each node and frees it
void hm_destroy(struct hashmap* hm){
  struct llnode *temp;
  //struct llnode *temp2;
  for (int i =0; i< hm->num_buckets; i++){ //go through and free each bucket list
    struct llnode *list = hm->map[i];
    while(list) {
      temp = list;
      list = list->next;
      free(temp);
    }
  }
  free(hm->map);
  free(hm);
  printf("Destroyed\n");
  return;
}

//performs hash operation by converted to ASCII and modding by number of buckets
int hash(struct hashmap* hm, char* word){
  int sum = 0;
  int hashValue = 0;
  int i = 0;

  char ch = word[i];

  while (ch != '\0'){
    sum += (int)word[i];
    i++;
    ch = word[i];
  }

  hashValue = sum % hm->num_buckets;
  printf("Hash value: %d\n", hashValue);
  return hashValue;
}

//removes the key value pair in the HashMap assoicated with given key
void hm_remove(struct hashmap* hm, char* word){
  int j = hash(hm, word); //gets bucket num
  struct llnode *listptr = hm->map[j]; //points to appropriate bucket
  struct llnode *temp;
  temp = listptr;
  struct llnode *temp2; //temp2 points to node behind temp;
  //printf("CRASH is here\n");

//if head is node to be removed
  if((strcmp(listptr->word,word) == 0)){
    hm->map[j] = temp->next;
    free(temp);
    printf("readjusted head node\n");
    return;
  }

  //printf("NOT IF STATEMENT\n");
// if node is not head and needs to be removed
  while(temp->next != NULL){
    if(strcmp(temp->next->word, word) == 0){
      //temp now points to node before one that should be deleted
      temp2 = temp->next; //temp2 points to node to be deleted
      temp->next = temp->next->next;
      temp2->next = NULL;
      free (temp2);
      return;
    }
    else{
    temp = temp->next;
    }
  }
  if (temp == NULL){ //if it doesn't exist
    printf("Node not found\n");
    return;
  }
return;
}

//creates hashmap and puts contents in their
struct hashmap* training(struct hashmap* t, int num_buckets, char* D1, char* D2, char* D3){
  //struct hashmap *hashpointer;
  t = hm_create(num_buckets);
  implementFiles(t, D1);
  implementFiles(t, D2);
  implementFiles(t, D3);
  //int g = hm_get(t, "computer", "D2.txt");
  //printf("value of hm_get g is %d\n", g);
  return t;
}

//gets search query from user, then parses phrase into seperate strings
//if user enter character '#' it immedately returns
int read_query(struct hashmap* t){
  char hashtag = '#';
  printf("in read_query\n");
//  char chbuff;
  char qSet[255];
  char *qPointer;
    //char qSet[255];
    printf("Enter Query Set: \n");
    scanf(" %[^\n]", qSet);
    printf("Query Set: %s\n", qSet);
    qPointer = strtok(qSet, " ");
    while(qPointer != NULL){
      printf("%s\n", qPointer );
      if((int)*qPointer == (int)hashtag){ //comparison for '#'
        printf("returning -1\n");
        return -1;
      }
      rank(qPointer, t); //calculates rank value for each document
      qPointer = strtok(NULL, " ");
    }
    //if all the Ri values are less than .001 then none of the query words match the search
    if (Ri_1 < .001 && Ri_2 < .001 && Ri_3 < .001){
      printf("None of the words you searched are in the documents\n");
      return 0;
    }

  return 1;


}

//will calculate rank value for each document
void rank(char* qPointer, struct hashmap* t){

  //will hold number of occurences within each document
  double D1c, D2c, D3c = 0.0;

  //will hold number of documents contianing the word
  double dfw = 0.0;

  printf("In rank() method\n");
  struct llnode *Dcount = hm_get(t, qPointer);

  //if node doesn't exist, go back and get next word
  if(Dcount == NULL){
    return;
  }

//will output how many documents contain the word of interest
  if(Dcount->D1count > 0){
    dfw++;
  }
  if(Dcount->D2count > 0){
    dfw++;
  }
  if(Dcount->D3count > 0){
    dfw++;
  }

//if they're all empty, word was not found in any document
  /*if ((int)Dcount->D1count == 0 && (int)Dcount->D2count == 0 && (int)Dcount->D3count == 0){
    printf("No documents match\n");
    return 0;
  }*/



    printf("D1count: %lf\n", Dcount->D1count);
    printf("D2count: %lf\n", Dcount->D2count);
    printf("D3count: %lf\n", Dcount->D3count);
    printf("dfw: %lf\n", dfw);
    printf("log10(3.0/dfw) is: %lf\n", log10(3.0/dfw));
    //double ZeroVal = 0.0;
    if(dfw == 3){
      printf("CALLING stop_word()\n");
      stop_word(t, qPointer); //calls stop_word() to get ride of node holding word;
    }
    D1c = Dcount->D1count * log10(3.0/dfw);
    D2c = Dcount->D2count * log10(3.0/dfw);
    D3c = Dcount->D3count * log10(3.0/dfw);

//D1count, D2count, or D3count will equal 0 if log(3.0/3.0) is done, this means the word counts as a stop word
  //conditional should be in log10() == 0, then stop_word()
    //if (D1count == 0 || D2count == 0 || D3count == 0){
      //stop_word(t, qPointer); //calls stop_word() to get ride of node holding word;
    //}
//puts equation into appropriate Ri value, will continually be added onto
    Ri_1 += D1c;
    Ri_2 += D2c;
    Ri_3 += D3c;

    //if all the Ri values are 0.0 then the search query hsa no words in document
  /*  if (Ri_1 < .001 && Ri_2 < .001 && Ri_3 < .001){
      printf("None of the terms were found in the documents\n");
      return 0;
    }*/
    //printf("Log of (3/2) is: %lf\n", log(3/dfw));
    printf("Ri_1: %lf\n", Ri_1);
    printf("Ri_2: %lf\n", Ri_2);
    printf("Ri_3: %lf\n", Ri_3);
  //  printf("Got a word placed in D1, D2, rank\n");
    return;


}


//sort double values of Ri_1, Ri_2, Ri_3 from largest to smallest
void compare(void){
  double R_values[3];

//stores all rank values into a double array
  R_values[0] = Ri_1;
  R_values[1] = Ri_2;
  R_values[2] = Ri_3;

  //make another array of strings and sort it along with for loop below
  char String_Array[3][10] = {
    "D1.txt",
    "D2.txt",
    "D3.txt"
  };


  double temp;
  char chartemp[10] ;

  for (int i =0; i<3; i++){
    for(int j= i+1; j<3; j++){
      if(R_values[i] < R_values[j]){
        temp = R_values[i];
        strcpy(chartemp, String_Array[i]);
      //  *chartemp = String_Array[i];

        R_values[i] = R_values[j];
        strcpy(String_Array[i], String_Array[j]);
        //String_Array[i] = String_Array[j];


        R_values[j] = temp;
        strcpy(String_Array[j], chartemp);
      }
    }
  }

  for(int k = 0; k < 3; k++){
    printf("Search Result Output: %s\n",  String_Array[k]);
  }
  Ri_1 = 0.0;
  Ri_2 = 0.0;
  Ri_3 = 0.0;
  return;
}
