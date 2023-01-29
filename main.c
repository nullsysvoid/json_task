#include <ctype.h>

#include <hashtable.h>
#include <json_parser.h>
#include <tvl.h>

#include <logger/logger.h>

#include "config.h"

void OnError(FILE* fpi, FILE* fpo)
{
    if (fpo != NULL)
        fclose(fpo);

    if (fpi != NULL)
        fclose(fpi);

    LogInfo("Task ended with error");
    exit(EXIT_FAILURE);
}


int main(int argc, char *argv[])
{
    //input file
    FILE* fpi;

    //output file
    FILE* fpo;

    char buf[INPUT_STRLEN];
    char* line = buf;

    LogInfo("Start entity");

    if ((fpo = fopen(OUTPUT_FILE, "wb")) == NULL)
    {
        LogInfo("Can't create output file %s", OUTPUT_FILE);
        OnError(fpo, fpi);
    }

    if ((fpi = fopen(INPUT_FILE, "r")) == NULL)
    {
        LogInfo("Can't open input file: %s", INPUT_FILE);
        OnError(fpo, fpi);
    }

    //hashtable for tags
    HashTable_t tab;
    if (!CreateHashTable(&tab))
    {
        LogInfo("Can't init hashtable");
        OnError(fpo, fpi);
    }

    size_t len = 0;
    ssize_t read;

    bool result = true;
    while ((read = getline(&line, &len, fpi)) != -1)
    {
        if (len >= INPUT_STRLEN)
        {
            LogInfo("Input string is too long");
            OnError(fpo, fpi);
        }

        LogInfo("Parsing input string: %s", line);
        if (!TVLDumpJstring(line, read, &tab, fpo))
        {
            OnError(fpo, fpi);
        }
    }

    TVLDumpDict(&tab, fpo);
    LogInfo("Task successfully ended");

    fclose(fpi);
    fclose(fpo);

    return 0;
}


