/*
 * tvl.h
 * This library contain tvl routines
*/

#ifndef TVL_H
#define TVL_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <hashtable.h>
#include <json_parser.h>

typedef struct TVLRecord
{
    char outputBuff[128];
    char tag;
    char len;
} TVLRecord_t;

void TVLMakeHeader(TVLRecord_t *tvl, KeyValue_t* kv);
void TVLMakeRecord(TVLRecord_t* tvl, KeyValue_t* kv);
void TVLDictionaryRecord(char* buff, char tag, char* key, bool last, int* offset);
bool TVLDumpJstring(const char *line, size_t len, HashTable_t* tab, FILE *fp);
void TVLDumpDict(HashTable_t* tab, FILE *fp);

#endif // TVL_H
