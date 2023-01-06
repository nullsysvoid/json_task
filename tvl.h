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

#include "json_parser.h"

typedef struct TVLRecord
{
    char outputBuff[128];
    char tag;
    char len;
} TVLRecord_t;

void MakeTVLRecord(TVLRecord_t* tvl, KeyValue_t* kv);
void MakeTVLDictionaryRecord(char* buff, char tag, char* key, bool last, int* offset);

#endif // TVL_H
