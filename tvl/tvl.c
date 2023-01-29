#include "tvl.h"

#include <ctype.h>

#include <logger.h>

/*
 * Create header for TVL record
*/
void TVLMakeHeader(TVLRecord_t *tvl, KeyValue_t* kv)
{
    tvl->outputBuff[0] = tvl->tag;
    tvl->len = sizeof(tvl->tag) + sizeof(tvl->len);

    if (kv->valueType == ValueType_STRING)
    {
        tvl->len += kv->valueLen;
        tvl->outputBuff[1] = kv->valueLen;
    }
    else if (kv->valueType == ValueType_INT)
    {
        tvl->len += sizeof(char);
        tvl->outputBuff[1] = sizeof(char);
    }
    else if (kv->valueType == ValueType_BOOL)
    {
        tvl->len += 2;
        tvl->outputBuff[1] = 2;
    }
    else
    {
        tvl->len = 0;
    }
}

/*
 * Convert key-value to tvl. Bool values decode to (0xff + 't' or 'f')
*/
void TVLMakeRecord(TVLRecord_t *tvl, KeyValue_t* kv)
{
    TVLMakeHeader(tvl, kv);

    if (tvl->len)
    {
        if (kv->valueType == ValueType_STRING)
        {
            memcpy(&tvl->outputBuff[2], kv->value, kv->valueLen);
        }
        else if (kv->valueType == ValueType_INT)
        {
            tvl->outputBuff[2] = atoi(kv->value);
        }
        else if (kv->valueType == ValueType_BOOL)
        {
            //if first data byte == 0xff then data is boolean
            tvl->outputBuff[2] = 0xff;
            tvl->outputBuff[3] = tolower(kv->value[0]);
        }
        tvl->outputBuff[tvl->len] = '\0';
    }
}

/*
 * Make one TVL record to dictionary
*/
void TVLDictionaryRecord(char* buff, char tag, char* key, bool last, int* offset)
{
    *offset = 0;

    //copy tag
    memcpy(buff, &tag, sizeof(tag));
    *offset += sizeof(tag);

    //copy delimiter
    buff[*offset] = ':';
    *offset += 1;

    int keyLen = strlen(key);
    memcpy(&buff[*offset], key, keyLen);
    *offset += keyLen;

    //copy value
    if (!last)
    {
        const char* delimiter = ", ";
        int delimiterLen = strlen(delimiter);
        memcpy(&buff[*offset], delimiter, delimiterLen);
        *offset += delimiterLen;
    }
}

/*
 * 1.Split json sting to tokens,
 * 2.Write TVL value to file
 * 3.Add tag to dictionary
*/
bool TVLDumpJstring(const char *line, size_t len, HashTable_t* tab, FILE *fp)
{
    Jparser_t jParser;
    JParserInit(&jParser);

    //split json string to tokens
    if (!JParserGetTokens(&jParser, line, len))
    {
        LogInfo("Unable to split input string to tokens");
        return false;
    }

    for(int i = 0; i < jParser.tokens[0].size; i++)
    {
        //iterate over tokens to extract key/values
        KeyValue_t kv;
        if (!JParserGetNext(&jParser, line, &kv))
        {
            LogInfo("Wrong key/value format");
            return false;
        }

        int tag = SetNextTag(tab, kv.key);
        TVLRecord_t tvl = {
            .tag = tag,
            .len = 0,
        };

        TVLMakeRecord(&tvl, &kv);
        fwrite(&tvl.outputBuff, 1, tvl.len, fp);
    }
    return true;
}

/*
 * Dump tag records to end of the file
*/
void TVLDumpDict(HashTable_t* tab, FILE *fp)
{
    const apr_array_header_t *tarr = apr_table_elts(tab->tab);
    const apr_table_entry_t *telts = (const apr_table_entry_t*)tarr->elts;
    int i;

    char nl = '\n';
    fwrite(&nl, 1, sizeof(nl), fp);

    for (i = 0; i < tarr->nelts; i++) {
        int offset = 0;
        char output[128];

        bool lastValue = (i == tarr->nelts - 1);
        TVLDictionaryRecord(output, atoi(telts[i].val), telts[i].key, lastValue, &offset);
        fwrite(output, 1, offset, fp);
    }
}
