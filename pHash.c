#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include "specific.h"


typedef enum bool{false, true} bool;

/*
   Initialise the Associative array
   keysize : number of bytes (or 0 => string)
   This is important when comparing keys since
   we'll need to use either memcmp() or strcmp()
*/
assoc* assoc_init(int keysize);

/*
   Insert key/data pair
   - may cause resize, therefore 'a' might
   be changed due to a realloc() etc.
*/
void assoc_insert(assoc** a, void* key, void* data);

/*
   Returns the number of key/data pairs 
   currently stored in the table
*/
unsigned int assoc_count(assoc* a);

/*
   Returns a pointer to the data, given a key
   NULL => not found
*/
void* assoc_lookup(assoc* a, void* key);

/* Free up all allocated space from 'a' */
void assoc_free(assoc* a);

assoc* _new_assoc(int keysize, int tablesize);
assoc_item _new_item(void* key, void* data);
bool _is_prime(int x);
int _next_prime(int x);
int _hash_index(void* key, int tablesize);
int _hash_probe(void* key, int tablesize);
void _resize(assoc** a);
bool _is_same_key(void* key1, void* key2, int keysize);
void _hash_in(assoc** a, void* key, void* data);

/*
   Initialise the Associative array
   keysize : number of bytes (or 0 => string)
   This is important when comparing keys since
   we'll need to use either memcmp() or strcmp()
*/
assoc* assoc_init(int keysize)
{
    return _new_assoc(keysize, INITSIZE);
}

/*
   Insert key/data pair
   - may cause resize, therefore 'a' might
   be changed due to a realloc() etc.
*/
void assoc_insert(assoc** a, void* key, void* data)
{
    if( ((*a)->itemcount) >= (unsigned int)((*a)->tablesize*LOADFACTOR) ){
        _resize(a);
    }
    _hash_in(a, key, data);
}

/*
   Returns the number of key/data pairs 
   currently stored in the table
*/
unsigned int assoc_count(assoc* a)
{
    return a->itemcount;
}

/*
   Returns a pointer to the data, given a key
   NULL => not found
*/
void* assoc_lookup(assoc* a, void* key)
{
    int i;
    int hash = _hash_index(key, a->tablesize);        
    int probe = _hash_probe(key, a->tablesize);
    for(i = 0; i < a->tablesize; i++){
        if(a->table[hash].key == NULL){
            return NULL;
        }else{
            if(_is_same_key(a->table[hash].key, key, a->keysize)){
                return a->table[hash].data;
            }
        }
        hash += probe;
        hash = hash%a->tablesize;
    }
    return NULL;
}

/* Free up all allocated space from 'a' */
void assoc_free(assoc* a)
{
    free(a->table);
    free(a);
}

/************************************************************************/
/*                       Auxiliary Functions                            */
/************************************************************************/

/* Creat a new assoc and give some initial conditions */
assoc* _new_assoc(int keysize, int tablesize)
{
    assoc* a = (assoc *)ncalloc(1,sizeof(assoc));
    a->keysize = keysize;
    a->tablesize = tablesize;
    a->itemcount = 0;
    a->table = (assoc_item *)ncalloc(tablesize, sizeof(assoc_item));
    return a;
}

/* Creat a new assoc_item 
for storing the pointers to key and data */
assoc_item _new_item(void* key, void* data)
{
    assoc_item item;
    item.key = key;
    item.data = data;
    return item;
}

/* Check input is prime or not */
bool _is_prime(int x)
{
    int i;
    for(i = 2; i <= x/2; i++)
    {
        if( x%i == 0 )
        {
            return false; 
        }
    }
    return true;
}

/* Find the next prime number after the input */
int _next_prime(int x)
{
    while(_is_prime(x) == false){
        x++;
    }
    return x;
}

/* Creat a hash number for the initial index 
while getting into the hash table */
int _hash_index(void* key, int tablesize)
{
    /* Modified Bernstein hashing from 
    Dr. Neill Campbell's Data Struc teaching pdf (University of Bristol)
    both 5381 and 33 are magic numbers required in the function */
    char* k;
    int c;
    unsigned long hash = 5381;
    k = (char *)key;
    while(  (c = *k++) ){
        hash = 33 * hash ^ c;
    }
    return (int)(hash%tablesize);
}

/* Creat a hash number for the probing steps 
while collisions and lookup  */
int _hash_probe(void* key, int tablesize)
{
    /* Modified hashing function inspired by
    Dr. Neill Campbell's Data Struc teaching pdf (University of Bristol)
    both 5381 and 33 are magic numbers required in the function */
    char* k;
    int c;
    unsigned long hash = 5381;
    k = (char *)key;
    while( (c = *k++) ){
        hash = 33 * hash ^ c;
    }
    if((int)((hash/tablesize)%tablesize) < 1){
        return 1;
    }else{
        return (int)((hash/tablesize)%tablesize);
    }
}

/* Check two of the input key is the same or not */
bool _is_same_key(void* key1, void* key2, int keysize)
{
    switch (keysize){
    case 0:
        if(strcmp((char *)key1, (char *)key2) == 0){
            return true;
        };
        break;
    default:
        if(memcmp(key1, key2, keysize) == 0 ){
            return true;
        };
        break;
    }
    return false;
}

/* Makes the size of the hash table bigger 
according to the SCALEFACTOR */
void _resize(assoc** a)
{
    int i;
    assoc* old;
    int next_tablesize;
    old = (*a);
    next_tablesize = _next_prime(old->tablesize*SCALEFACTOR);
    (*a) = _new_assoc(old->keysize, next_tablesize);
    for(i = 0; i < old->tablesize; i++){
        if(old->table[i].key != NULL){
            _hash_in(a, old->table[i].key, old->table[i].data);
        } 
    }
    assoc_free(old);
}

/* Save the input pointers to key and data into the hashtable in a */
void _hash_in(assoc** a, void* key, void* data)
{
    int i;
    int hash;
    int probe;
    hash = _hash_index(key, (*a)->tablesize);
    probe = _hash_probe(key, (*a)->tablesize);
    for(i = 0; i < (*a)->tablesize; i++){
        if( (*a)->table[hash].key != NULL ){
            if( _is_same_key((*a)->table[hash].key, key, (*a)->keysize) == true ){
                return;
            }
        }else{
            (*a)->table[hash] = _new_item(key, data);
            (*a)->itemcount ++;
            return;
        }
        hash += probe;
        hash = hash%(*a)->tablesize; 
    }
}
