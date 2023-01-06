#include "hashtable.h"

#include <stdio.h>

#define TABLE_INIT_SZ           32
#define INITTAG                 1


/*
 * Init hashtable for APR
*/
void CreateHashTable(HashTable_t* ht)
{
    ht->tagCounter = INITTAG;
    apr_initialize();
    apr_pool_create(&ht->mp, NULL);
    ht->tab = apr_table_make(ht->mp, TABLE_INIT_SZ);
}

/*
 * Put data to hashtable
*/
void SetKeyValue(HashTable_t* ht, const char *key, const char *val)
{
    apr_table_set(ht->tab, key, val);
}

/*
 * Put key-tag to hashtable
*/
int SetNextTag(HashTable_t* ht, const char *key)
{
    int val = ht->tagCounter;
    ht->tagCounter++;

    //tag stored as string for debug purpose only
    char value[8];
    sprintf(value,"%d", val);
    SetKeyValue(ht, key, value);

    return val;
}
