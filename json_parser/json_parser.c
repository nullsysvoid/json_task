#include "json_parser.h"

#include <ctype.h>

#include <logger.h>

#define WRONGTOKEN -1
#define DATATOKEN 1

/*
 * Init JSMN json parser
*/
void JParserInit(Jparser_t* jParser)
{
    jParser->nextToken = DATATOKEN;
    jsmn_init(&jParser->parser);
}

/*
 * Slit input string to json tokens
*/
bool JParserGetTokens(Jparser_t* jParser, const char *line, size_t len)
{
    jParser->nextToken = DATATOKEN;
    jsmn_parse(&jParser->parser, line, len, jParser->tokens, TOKENS);

    if (jParser->tokens[0].type != JSMN_OBJECT)
    {
        LogInfo("Wrong input string format: root is not object");
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
        LogInfo("Can't parse key token from input string: wrong length");
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
    if ((jParser->tokens[jParser->nextToken].type != JSMN_PRIMITIVE &&
        jParser->tokens[jParser->nextToken].type != JSMN_STRING))
    {
        jParser->nextToken = WRONGTOKEN;
        LogInfo("Can't parse value token: not allowed type");
        return false;
    }

    int valueIndex = jParser->tokens[jParser->nextToken].start;
    int valueLen = jParser->tokens[jParser->nextToken].end - jParser->tokens[jParser->nextToken].start;

    if (valueLen < 1 || jParser->tokens[jParser->nextToken].start == -1)
    {
        LogInfo("Can't parse value token: wrong length");
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

    keyValue->value[valueLen] = 0;
    return true;
}

/*
 * Extract key-value pair from token
*/
bool JParserGetNext(Jparser_t* jParser, const char* line, KeyValue_t* keyValue)
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
