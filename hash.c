#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

/* Creates a new performance structure */
struct Performance *newPerformance(){
    struct Performance *newperf = malloc(sizeof(struct Performance));
    if (newperf == NULL){
        printf("Error: Memory could not be allocated for then new performance\n");
        exit(0);
    } // Memory check
    newperf->frees = 0;
    newperf->mallocs = 0;
    newperf->reads = 0;
    newperf->writes = 0;

    return(newperf); 
}

/* Creates a new hashtable */
struct HashTable *createTable(struct Performance *performance, unsigned int capacity,
    int (*hash)(void *, int), int (*compar)(const void *, const void *)){

    struct HashTable *newHash = malloc(sizeof(struct HashTable));
    if (newHash == NULL){
        printf("Error: Memory could not be allocated for the new hashtable\n");
        exit(1); // Memory check
    }    
    newHash->capacity = capacity;
    newHash->hash = hash;
    newHash->compar = compar;
    newHash->nel = 0;
    newHash->data = malloc(sizeof(void *) * capacity);
    if (newHash->data == NULL){
        printf("Error: Memory could not be allocated for the hashtable data\n");
        exit(2); // Memory check
    }
    performance->mallocs++;

    for (int i = 0; i < capacity; i++){
        newHash->data[i] = NULL;
    }
    performance->reads += capacity;

    return(newHash);
}

/* Adding an element to a hash table */             
void addElement(struct Performance *performance, struct HashTable *table, void *src){
    if (table->nel == table->capacity){
        printf("Error: Capacity is full\n");
        exit(3);
    } // Error check
    int dataIndex = table->hash(src, table->capacity);
    while (table->data[dataIndex] != NULL){
        if (table->data[dataIndex] == NULL){
            break;
        }
        dataIndex++;
        performance->reads++;
        if (dataIndex == table->capacity){
            dataIndex = 0;
        }
    }
    table->data[dataIndex] = src;
    table->nel++;
    performance->writes++;
}

/* Getting the index of given data pointer */
int getIdx(struct Performance *performance, struct HashTable *table, void *src){
    int dataIndex = table->hash(src, table->capacity);
    int comparResult = -1;

    /* using compar result to compare all values of table with src */
    while (comparResult != 0) {
        if (table->data[dataIndex] != NULL){
            comparResult = table->compar(src, table->data[dataIndex]);
            performance->reads++;
        }
        /* returning appropriate values based on comparResult */
        if (comparResult == 0){
            return(dataIndex);
        }
        dataIndex++;
        if (dataIndex == table->hash(src, table->capacity)){
            return(-1);
        }
        if (dataIndex == table->capacity){
            dataIndex = 0;
        }
    } 
   return(dataIndex);
}

/* Freeing table */
void freeTable(struct Performance *performance, struct HashTable *table){
    struct Performance *tempPerf = newPerformance();
    if (table->data == NULL){
        exit(4);
    }
    performance->frees++;
    /* removing each data index, then freeing */
    for (int i = 0; i < table->capacity; i++){
        removeElement(tempPerf, table, table->data[i]);
        free(table->data[i]);
    }
    free(table->data);
    free(tempPerf);
    free(table);
}

/* Get the element using getIdx */
void *getElement(struct Performance *performance, struct HashTable *table, void *src){
    int dataIndex = getIdx(performance, table, src);
    if (dataIndex == -1){
        return(NULL);
    } else {
        return(table->data[dataIndex]);
    }
}

/* remove a target element from the hashtable */
void removeElement(struct Performance *performance, struct HashTable *table, void *target){
    int index = -1;
    /* getting index if target is not null */
    if (target != NULL){
        index = getIdx(performance, table, target);
    }
    /* setting target to null at index */
    if (table->data[index] != NULL && index != -1){
        table->data[index] = NULL;
        performance->writes++;
    }
    table->nel--;
} 

/* find the accuracy of the hashtable */
int hashAccuracy(struct HashTable *table){
    int difference = 0;
    int hashIndex, index;
    /* traverse through table and add to difference when hash Index is 
            different from the table index */
    for (index = 0; index < table->capacity; index++){
        if (table->data[index] != NULL){
            hashIndex = table->hash(table->data[index], table->capacity);
            if (index > hashIndex){
                difference += index - hashIndex;
            }
            if (index < hashIndex){
                difference += index + (hashIndex - table->capacity);
            } 
        } 
    }
    return(difference);   
}

/* Rehash to improve the hashAccuracy */
void rehash(struct HashTable *table){
    int index, hashIndex;
    /* Traversing through all indexes */
    for (index = 0; index < table->capacity; index++){
        if (table->data[index] != NULL){
            /* checking the correct hash index */
            hashIndex = table->hash(table->data[index], table->capacity);
            /* move value closer to the hash index */
            if (index != hashIndex){
                if (table->data[hashIndex] == NULL){
                    table->data[hashIndex] = table->data[index];
                    table->data[index] = NULL;
                } else {
                    int i = 0;
                    while (table->data[hashIndex + i] != NULL){
                        i++;
                    }
                    table->data[hashIndex + i] = table->data[index];
                    table->data[index] = NULL;
                }
            }          
        }
        
    }
}