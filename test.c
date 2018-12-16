#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"
#include <math.h>
#include <string.h>

extern double Ri_1;
extern double Ri_2;
extern double Ri_3;

char* inputfile1 = "D1.txt";
char* inputfile2 = "D2.txt";
char* inputfile3 = "D3.txt";

//takes files and sets D1count, D2count, D3count depending on which .txt file is being read
void implementFiles (struct hashmap* hm, char* file){
  char chbuff[255];
  FILE* inputfile;
  inputfile = fopen(file, "r");
  while (fscanf(inputfile, "%s", chbuff) != EOF){
    if (strcmp(file, inputfile1) == 0){
        hm_put(hm, chbuff, 1.0,0.0,0.0);
    }
    if (strcmp(file, inputfile2) == 0){
        hm_put(hm, chbuff, 0.0,1.0,0.0);
    }
    if (strcmp(file, inputfile3) == 0){
        hm_put(hm, chbuff, 0.0,0.0,1.0);
    }
    printf("putting %s into the put method\n", chbuff);
      //hm_put(hm, chbuff, file, 1);
    }
    return;
  }


int main(void){


   int num_buckets = 0;
    printf("Enter Number of buckets\n");
    scanf("%d", &num_buckets);
    char chbuff;
  //creates hashmap with right number of buckets
    char* inputfile1 = "D1.txt";
    char* inputfile2 = "D2.txt";
    char* inputfile3 = "D3.txt";
    struct hashmap *t = NULL;
    //struct hashmap *q = NULL;
    t = training(t, num_buckets,inputfile1, inputfile2, inputfile3);

    printf("Enter a S or X\n");
    scanf(" %c", &chbuff);
    while(chbuff == 'S'){
      printf("GOT S\n");


      int c = read_query(t); //depending on return value, may shut down, compare, or reprompt user
      if (c == -1){
        hm_destroy(t);
        printf("Exiting Program\n");
        return 0;
      }
      if(c == 1){
      compare();
    }

      printf("Enter a S or X\n");
      scanf(" %c", &chbuff);
    }
    if(chbuff == 'X'){
      hm_destroy(t);
      printf("Shutting Down Program\n");
      return 0;
    }
    else{
      printf("invalid option...shutting program down\n");
      return 0;
    }
      return 0;
    }
