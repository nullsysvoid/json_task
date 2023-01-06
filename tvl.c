#include "tvl.h"

#include <ctype.h>

/*
 * Create header for TVL record
*/
static void MakeTVLHeader(TVLRecord_t *tvl, KeyValue_t* kv)
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
void MakeTVLRecord(TVLRecord_t *tvl, KeyValue_t* kv)
{
    MakeTVLHeader(tvl, kv);

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
void MakeTVLDictionaryRecord(char* buff, char tag, char* key, bool last, int* offset)
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
