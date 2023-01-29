/*
 * json_parser.h
 * Library to convert raw string to key-value pair
*/

#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#define JSMN_STATIC
#include "jsmn/jsmn.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define KEYVALUELEN 128
#define TOKENS 128

typedef enum {
    ValueType_UNKNOWN = 0,
    ValueType_INT     = 1,
    ValueType_STRING  = 2,
    ValueType_BOOL    = 3,
} ValueType_t;

typedef struct KeyValue
{
    char key[KEYVALUELEN];
    char value[KEYVALUELEN];

    ValueType_t valueType;
    char valueLen;
} KeyValue_t;

typedef struct Jparser
{
    jsmntok_t tokens[TOKENS];
    int nextToken;
    jsmn_parser parser;
} Jparser_t;

void JParserInit(Jparser_t* jParser);
bool JParserGetTokens(Jparser_t* jParser, const char *line, size_t len);
bool JParserGetNext(Jparser_t* jParser, const char* line, KeyValue_t* keyValue);

#endif // JSON_PARSER_H
