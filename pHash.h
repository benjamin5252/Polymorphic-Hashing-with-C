/* specific.h for realloc */
/* INITSIZE is the initial size of the hash tables.
SCALEFACTOR is the growing speed of the hash tables performed when tables is not big enough.
LOADFACTOR is the maximun ratio of the number of entries occupied in the hash table
to the number of buckets. */
#define INITSIZE 17
#define SCALEFACTOR 2
#define LOADFACTOR 0.75

/* Define type of struct assoc_item 
to store the pointers to the key and data */
typedef struct assoc_item{
    void* key;
    void* data;
} assoc_item;

/* Define type of struct assoc
to store the hash table(array of assoc_item), keysize,
tablesize and the count of the item in the table */
typedef struct assoc{
    assoc_item *table;
    int keysize;
    int tablesize;
    unsigned int itemcount;
} assoc;
