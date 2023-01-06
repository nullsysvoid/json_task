#include <ctype.h>

#include "hashtable.h"
#include "json_parser.h"
#include "tvl.h"

FILE *fp;

/*
 * 1.Split json sting to tokens,
 * 2.Write TVL value to file
 * 3.Add tag to dictionary
*/
void PutJstring(const char *line, size_t len, HashTable_t* tab)
{
    Jparser_t jParser;
    JparserInit(&jParser);

    //split json string to tokens
    GetTokens(&jParser, line, len);

    bool res = true;
    while (res)
    {
        //iterate over tokens to extract key/values
        KeyValue_t kv;
        res = GetNextKeyValue(&jParser, line, &kv);

        if (res)
        {
            int tag = SetNextTag(tab, kv.key);
            TVLRecord_t tvl = {
                .tag = tag,
                .len = 0,
            };

            MakeTVLRecord(&tvl, &kv);
            fwrite(&tvl.outputBuff, 1, tvl.len, fp);
        }
    }
}

/*
 * Dump tag records to end of the file
*/
void DumpDictToFile(HashTable_t* tab)
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
        MakeTVLDictionaryRecord(output, atoi(telts[i].val), telts[i].key, lastValue, &offset);
        fwrite(output, 1, offset, fp);
    }
}

int main(int argc, char *argv[])
{
    //output file
    if ((fp = fopen("output.txt", "wb")) == NULL) {
        return 0;
    }

    //hashtable for tags
    HashTable_t tab;
    CreateHashTable(&tab);

    FILE * fpi;
    fpi = fopen("input.txt", "r");
    if (fpi == NULL)
        exit(EXIT_FAILURE);

    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, fpi)) != -1) {
        PutJstring(line, read, &tab);
    }

    DumpDictToFile(&tab);
    fclose(fpi);
    if (line)
        free(line);

    fclose(fp);
    return 0;
}


