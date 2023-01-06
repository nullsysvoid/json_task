#include <assert.h>

#include <ctype.h>

#include "../hashtable.h"
#include "../json_parser.h"
#include "../tvl.h"

void TestValue(const char *line, size_t len, KeyValue_t* kv)
{
    Jparser_t jParser;
    JparserInit(&jParser);

    //split json string to tokens
    GetTokens(&jParser, line, len);


    GetNextKeyValue(&jParser, line, kv);
}

int main(int argc, char *argv[])
{
    const char *JSON_STRING1 = "{ \"key1\":\"value\", \"key2\":42, \"key3\":TRUE}";
    const char *JSON_STRING2 = "{\"sadsf\":\"dsewtew\", \"dsre\":3221, \"sdfds\":\"dsfewew\"}";

    {
        KeyValue_t kv;
        TestValue(JSON_STRING1, strlen(JSON_STRING1), &kv);
        assert(strcmp(kv.key, "key1") == 0);
    }

    {
        KeyValue_t kv;
        TestValue(JSON_STRING2, strlen(JSON_STRING2), &kv);
        assert(strcmp(kv.key, "sadsf") == 0);
    }

    {
        KeyValue_t kv = {
            .key = "key1",
            .value = "value1",
            .valueType = ValueType_STRING,
            .valueLen = 6,
        };
        TVLRecord_t tvl = {
            .tag = 1,
            .len = 0,
        };

        MakeTVLRecord(&tvl, &kv);
        assert(tvl.outputBuff[0] == 1);
        assert(tvl.outputBuff[1] == 6);
        assert(memcmp(&tvl.outputBuff[2], "value1", 6) == 0);
    }

    return 0;
}
