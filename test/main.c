#include <assert.h>
#include <ctype.h>

#include <hashtable.h>
#include <json_parser.h>
#include <tvl.h>

#include <logger.h>


void HashTableTests()
{
    LogInfo("Starting test: %s", __func__);
    {
        HashTable_t ht1;
        assert(CreateHashTable(&ht1) == true);

        const char* key1 = "key1";
        const char* val1 = "val1";
        SetKeyValue(&ht1, key1, val1);

        const char* testValue = GetValue(&ht1, key1);
        assert(strcmp(testValue, val1) == 0);
        LogInfo("[%s]: hashtable base test - OK", __func__);
    }
    {
        HashTable_t ht1;
        int tag = 1;
        const char* key1 = "key1";
        const char* key2 = "key2";
        assert(CreateHashTable(&ht1) == true);

        assert(SetNextTag(&ht1, key1) == 1);
        const char* testValue1 = GetValue(&ht1, key1);
        assert(strcmp(testValue1, "1") == 0);

        assert(SetNextTag(&ht1, key2) == 2);
        const char* testValue2 = GetValue(&ht1, key2);
        assert(strcmp(testValue2, "2") == 0);
        LogInfo("[%s]: tag increase value - OK", __func__);
    }
    LogInfo("Test: %s - OK", __func__);
}

void JParserTests()
{
    LogInfo("Starting test: %s", __func__);
    {
        Jparser_t jParser;
        JParserInit(&jParser);

        const char* testString = "Hello";
        assert(JParserGetTokens(&jParser, testString, strlen(testString)) == false);

        LogInfo("[%s]: Not json object test - OK", __func__);
    }

    LogInfo("Starting test: %s", __func__);
    {
        Jparser_t jParser;
        JParserInit(&jParser);

        const char* testString = "{ {} }";
        assert(JParserGetTokens(&jParser, testString, strlen(testString)) == true);

        KeyValue_t kv;
        assert(JParserGetNext(&jParser, testString, &kv) == false);

        LogInfo("[%s]: Not allowed type - OK", __func__);
    }

    LogInfo("Starting test: %s", __func__);
    {
        Jparser_t jParser;
        JParserInit(&jParser);

        const char* testString = "{ \"key1\" }";
        assert(JParserGetTokens(&jParser, testString, strlen(testString)) == true);

        KeyValue_t kv;
        assert(JParserGetNext(&jParser, testString, &kv) == false);

        LogInfo("[%s]: Missing value - OK", __func__);
    }

    {
        Jparser_t jParser;
        JParserInit(&jParser);

        const char* testString = "{ \"key1\":\"value\", \"key2\":42, \"key3\":TRUE}";
        assert(JParserGetTokens(&jParser, testString, strlen(testString)) == true);

        KeyValue_t kv;
        assert(JParserGetNext(&jParser, testString, &kv) == true);
        assert(strcmp("key1", kv.key) == 0);
        assert(strcmp("value", kv.value) == 0);

        assert(JParserGetNext(&jParser, testString, &kv) == true);
        assert(strcmp("key2", kv.key) == 0);
        assert(strcmp("42", kv.value) == 0);

        assert(JParserGetNext(&jParser, testString, &kv) == true);
        assert(strcmp("key3", kv.key) == 0);
        assert(strcmp("TRUE", kv.value) == 0);

        LogInfo("[%s]: testString %s test - OK", __func__, testString);
    }

    LogInfo("Starting test: %s", __func__);
    {
        Jparser_t jParser;
        JParserInit(&jParser);

        const char* testString = "{\"sadsf\":\"dsewtew\", \"dsre\":3221, \"sdfds\":\"dsfewew\"}";
        assert(JParserGetTokens(&jParser, testString, strlen(testString)) == true);

        KeyValue_t kv;
        assert(JParserGetNext(&jParser, testString, &kv) == true);
        assert(strcmp("sadsf", kv.key) == 0);
        assert(strcmp("dsewtew", kv.value) == 0);

        assert(JParserGetNext(&jParser, testString, &kv) == true);
        assert(strcmp("dsre", kv.key) == 0);
        assert(strcmp("3221", kv.value) == 0);

        assert(JParserGetNext(&jParser, testString, &kv) == true);
        assert(strcmp("sdfds", kv.key) == 0);
        assert(strcmp("dsfewew", kv.value) == 0);

        LogInfo("[%s]: testString %s test - OK", __func__, testString);
    }

    LogInfo("Test: %s - OK", __func__);
}

void TVLTests()
{
    LogInfo("Starting test: %s", __func__);
    {
        char testData[] = { 1, 5, 'v', 'a', 'l', 'u', 'e', 0 };
        TVLRecord_t tvl = {.tag = 1};
        KeyValue_t kv = {
            .valueType = ValueType_STRING,
            .key = "key",
            .value = "value",
            .valueLen = strlen("value"),
        };

        TVLMakeRecord(&tvl, &kv);

        assert(strcmp(testData, tvl.outputBuff) == 0);
        LogInfo("[%s]: Make TVL header test - OK", __func__);
    }
    {
        char testData[] = { 1, 5, 'v', 'a', 'l', 'u', 'e', 2, 1, 42, 3, 2, 0xff, 't' };
        HashTable_t ht1;
        CreateHashTable(&ht1);
        const char* testString = "{ \"key1\":\"value\", \"key2\":42, \"key3\":TRUE}";

        FILE *fp;
        assert((fp = fopen("test.txt", "wb")) != NULL);
        TVLDumpJstring(testString, strlen(testString), &ht1, fp);
        fflush(fp);
        fclose(fp);

        char buf[100];

        assert((fp=fopen("test.txt", "rb")) != NULL);
        fread(buf, sizeof(testData), 1, fp);
        assert(memcmp(testData, buf, sizeof(testData)) == 0);
    }
    LogInfo("Test: %s - OK", __func__);
}

void TestAll()
{
    HashTableTests();
    JParserTests();
    TVLTests();
}

int main(int argc, char *argv[])
{
    TestAll();
    return 0;
}
