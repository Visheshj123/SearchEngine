#ifndef HASHMAP_H
#define HASHMAP_H

struct llnode {
        char* word;
        char* document_id;
        double D1count;
        double D2count;
        double D3count;
        struct llnode* next;
};

struct hashmap {
        struct llnode** map;
        int num_buckets;
        int num_elements;
};

struct hashmap* hm_create(int num_buckets);
struct llnode* hm_get(struct hashmap* hm, char* word);
void hm_put(struct hashmap* hm, char* word, double D1count, double D2count, double D3count);
void hm_remove(struct hashmap* hm, char* word);
void hm_destroy(struct hashmap* hm);
int hash(struct hashmap* hm, char* word);
void implementFiles(struct hashmap* hm, char* file);
int read_query(struct hashmap* t);
struct hashmap* training(struct hashmap* t, int num_buckets, char* D1, char* D2, char* D3);
void rank(char* qPointer, struct hashmap* t);
int main(void);
void stop_word(struct hashmap* t, char* word);
void compare(void);


#endif
