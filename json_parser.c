#include "json_parser.h"

#include <ctype.h>

#define TOKENS 20
#define WRONGTOKEN -1
#define DATATOKEN 1

/*
 * Init JSMN json parser
*/
void JparserInit(Jparser_t* jParser)
{
    jParser->nextToken = DATATOKEN;
    jsmn_init(&jParser->parser);
}

/*
 * Slit input string to json tokens
*/
bool GetTokens(Jparser_t* jParser, const char *line, size_t len)
{
    jParser->nextToken = DATATOKEN;
    jsmn_parse(&jParser->parser, line, len, jParser->tokens, TOKENS);

    if (jParser->tokens[0].type != JSMN_OBJECT)
    {
        return false;
    }

    return true;
}

/*
 * Get key from token
*/
static bool GetKey(Jparser_t* jParser, const char* line, KeyValue_t* keyValue)
{
    int keyIndex = jParser->tokens[jParser->nextToken].start;
    int keyLen = jParser->tokens[jParser->nextToken].end - jParser->tokens[jParser->nextToken].start;

    if (keyLen < 1)
    {
        jParser->nextToken = WRONGTOKEN;
        return false;
    }

    memcpy(keyValue->key, &line[keyIndex], keyLen);
    keyValue->key[keyLen] = 0;

    jParser->nextToken++;

    return true;
}

/*
 * Get value from token
*/
static bool GetValue(Jparser_t* jParser, const char* line, KeyValue_t* keyValue)
{
    int valueIndex = jParser->tokens[jParser->nextToken].start;
    int valueLen = jParser->tokens[jParser->nextToken].end - jParser->tokens[jParser->nextToken].start;

    if (valueLen < 1)
    {
        jParser->nextToken = WRONGTOKEN;
        return false;
    }

    memcpy(keyValue->value, &line[valueIndex], valueLen);
    keyValue->valueLen = valueLen;

    if (jParser->tokens[jParser->nextToken].type == JSMN_STRING)
    {
        keyValue->valueType = ValueType_STRING;
        jParser->nextToken++;
    }
    else if (jParser->tokens[jParser->nextToken].type == JSMN_PRIMITIVE)
    {
        if(isdigit(keyValue->value[0]))
        {
            keyValue->valueType = ValueType_INT;
        }
        else
        {
            keyValue->valueType = ValueType_BOOL;
        }
        jParser->nextToken++;
    }

    return true;
}

/*
 * Extract key-value pair from token
*/
bool GetNextKeyValue(Jparser_t* jParser, const char* line, KeyValue_t* keyValue)
{
    //in case of last read was unsuccessful or key is not string we don't parse other values
    if (jParser->nextToken == WRONGTOKEN || jParser->tokens[jParser->nextToken].type != JSMN_STRING)
    {
        jParser->nextToken = WRONGTOKEN;
        return false;
    }

    if (!GetKey(jParser, line, keyValue))
        return false;

    if (!GetValue(jParser, line, keyValue))
        return false;

    return true;
}
