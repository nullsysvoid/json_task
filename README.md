C assignment: json packer

Before checking code, please read below:
As task doesn't contain some details so there are several assumptions:
  1. Multithreading? - not supported
  2. Input string len? - limited by 128 bytes
  3. How many key-value records json string contain? - limited to 128
  4. Should we check same keys for different Json strings in dictionary? - not checked
  5. Error handling? - not checked
  6. Output file limitations? - not checked
  7. Tag counts? - 1-255
  8. TVL Data len? - 255 bytes

From task description we it's allowed to use:
1. Some json parser https://github.com/zserge/jsmn - as it provide necessary functionality and speed
2. Apache Portable Runtime library - for dictionary
3. Cmake

How to use:
There are 3 main units:
1. json_parser - allow to get tokens from raw string and extract key-value pair
2. hashtable - store key-tag pair
3. tvl - converts key-value to tvl record

Business logic located in main function:
Parse string, convert to tvl, put to dictionary and dump to file.

