#include "hashtable.h"
#include <stdio.h>
#include <logger.h>

/*
 * Init hashtable for APR
*/
bool CreateHashTable(HashTable_t* ht)
{
    LogDebug("Init new table");
    ht->tagCounter = INITTAG;
    apr_initialize();
    apr_pool_create(&ht->mp, NULL);
    ht->tab = apr_table_make(ht->mp, TABLE_INIT_SZ);
    return ht->tab != NULL;
}

/*
 * Put data to hashtable
*/
void SetKeyValue(HashTable_t* ht, const char *key, const char *val)
{
    apr_table_set(ht->tab, key, val);
}

/*
 * Get data from hashtable
*/
const char* GetValue(HashTable_t* ht, const char *key)
{
    return apr_table_get(ht->tab, key);
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
