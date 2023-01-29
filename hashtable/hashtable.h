/*
 * hashtable.h
 * APR based dictionary
*/

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <apr_general.h>
#include <apr_tables.h>
#include <stdbool.h>

typedef struct HashTable
{
    char tagCounter;
    apr_pool_t *mp;
    apr_table_t *tab;
} HashTable_t;

bool CreateHashTable(HashTable_t* ht);
void SetKeyValue(HashTable_t* ht, const char *key, const char *val);
const char* GetValue(HashTable_t* ht, const char *key);
int SetNextTag(HashTable_t* ht, const char *key);

#endif // HASHTABLE_H
