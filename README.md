# RainbowPhones

RainbowPhones is a utility to crack hashes of phone numbers by using Rainbow Tables.
It is based on the RainbowCrack program Version 1.2 which can be found here:
https://github.com/inAudible-NG/RainbowCrack-NG/tree/3b0af1628d79fdb65f3a4a6940ac012e2e20bea7

## Compiling
RainbowPhones depends on openMP and openSSL. We have seen signifcant performance uplifts with newer versions of openMP, so we recommend running the latest version of the library.

### Linux & MacOS
Simply run "make"
The binaries will be in the ./bin/ folder afterwards

### Windows
Run "nmake -f makefile.win"
The binaries will be in the ./bin/ folder afterwards

## Usage
### rtgen
This is the application for generating rainbow tables. It takes the following arguments in the order
they are listed in below:
1.  Hash Algorithm:
The hash algorithm to be used in the rainbow table.
Available algorithms are: lm, ntlm, md4, md5, sha1, ripemd160 and audible.
2.  Rainbow Table Index:
For generating a table that spans multiple files, this index assigns a number to each file. It will
also influence the index offset as described in Section 2.2.
3.  Rainbow Chain Length:
The length of each chain in the rainbow table
4.  Rainbow Chain Count:
The number of chains to produce for this file. To limit memory usage on lookup, the program
does not support more than 134.217.727 chains in one file (=2GB). To create larger tables,
simply create several files.
5.  Number Spec Name:
Name of the phone number specification. This expects to find a file called `./spec/<name>.csv`
in the current working directory.
6.  Output Folder:
A path to store the generated rainbow table in. If it does not exist, the path will be created.
The file name of the resulting table is determined by the program and includes all these parameters.
Do not change it, the parameters are necessary to perform lookups and are not stored in the file
itself.

A typical call might look like this:
````bash
./rtgen sha 1 1 1000 1000000 DE_mobile out
````
### rtsort
This program simply sorts a generated rainbow table. rcrack will reject unordered tables.
Rtsort only takes a single argument, the path to a generated rainbow table.
A typical call might look like this:
````bash
./rtsort ./out/sha1_1_1000x1000000_DE_mobile.rt
````
### rcrack
This is the executable for performing lookups in generated and sorted rainbow tables. It takes the
following arguments in the order they are presented in:
1.  Rainbow Table Files:
One or multiple files that contain a rainbow table. These can be listed, separated by a space
or specified using asterisks (e.g. `tables/*.rt`).
2.  Hashes to Search for:
These can be specified in one of these three ways:

`-h <single hash>` The single hash in hexadecimal format

`-l <hash list file>` A file containing a list of hashes in hexadecimal format. The file must only contain a single
hash per line.

`-f <pwdump file>` A file containing the output of the pwdump utility that dumps Windows password hashes.
The program will print all the plaintext numbers it finds to the console as it finds them.

A typical call might look like this:
````bash
./rcrack ./out/∗.rt −l ./hashes_DE
````
### hashgen
This is a small utility for testing purposes. It generates hashes of random phone numbers in a given
number specification and prints them to the console. It takes two arguments:
1.  Number of Hashes:
The number of hashes to generate.
2.  Number Spec Name:
Name of the phone number specification.

A typical call might look like this:
````bash
./hashgen 1000 DE_mobile > hashes_DE
````
### rtdump
This is also a utility program. It prints the contents of a specific chain in a given rainbow table.
For each iteration in that chain, it will calculate the plaintext number, its index, index offset and
hash. It takes two arguments:
1.  Rainbow Table File:
Path to a rainbow table file.
2.  Index within the File:
Index of the chain to print within the file.

A typical call might look like this:
````bash
./rtdump ./out/sha1_1_1000x1000000_DE_mobile.rt 5
````
