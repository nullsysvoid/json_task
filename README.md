C assignment: json packer

The task is to read data in JSON format and using dictionary encoding write a compressed stream to disk.

It's allowed to use:
1. Some json parser https://github.com/zserge/jsmn - as it provide necessary functionality and speed
2. Apache Portable Runtime library - for dictionary
3. Cmake

How to use:
There are 3 main units:
1. json_parser - allow to get tokens from raw string and extract key-value pair
2. hashtable - store key-tag pair
3. tvl - converts key-value to tvl record

Ver1 to ver2 upgrades:

INPUT_FILE - file with raw json strings to convert
OUTPUT_FILE - created if conversion was successful

1. Added config.h file to easy setup input/output files and max input string length
2. Added tests
3. Added Logging library
4. All libraries compile separatly for reusable
5. Code cleanup
6. Error handling
