# Polymorphic-Hashing-with-C

# Description
An abstract data type with hash funtion is built with c.  <br />
The abstract data type assoc provide functions to quick check the existance of the data stored with double hash. <br />

# Features
dynamic memory management(malloc) for size changing hash table. <br />
Implementation of double hash. <br />
Pointer to the input data for Polymorphic support. <br />
Abstract data type design with struct and various functions <br />

# Instructions
A sample test.c code is provided to show how users can use this abstract data type. <br />

# Function provided

### assoc* assoc_init(int keysize);
   Initialise the Associative array <br />
   keysize : number of bytes (or 0 => string) <br />
   This is important when comparing keys since <br />
   we'll need to use either memcmp() or strcmp() <br />



### void assoc_insert(assoc** a, void* key, void* data); 
   Insert key/data pair <br />
   - may cause resize, therefore 'a' might <br />
   be changed due to a realloc() etc. <br />

### unsigned int assoc_count(assoc* a); 
   Returns the number of key/data pairs  <br />
   currently stored in the table <br />



### void* assoc_lookup(assoc* a, void* key); 
   Returns a pointer to the data, given a key <br />
   NULL => not found <br />

### void assoc_free(assoc* a); 

Free up all allocated space from 'a'  <br />


