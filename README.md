# Search for files containing the specified MAC address value in text form
## Overview
This is a command line program written in C to search for files containing the specified MAC address value in text. The program can search for a file containing the MAC address value represented in hexadecimal format, display that file and files with a length equal to the input argument of the --leng parameter
## Task Description
1. Implementing a Makefile
2. lab1cntN3250.c
   Program for calling the file check function from a plugin. Additionally displays information about the plugin.
   Example of launch:
   ./main -P ../path_to_fileso --mac-addr aabbccddeeff --leng 691 test1
3. libcntN3250.so
   Search for files containing the specified MAC address in text form (in the formats aa-bb-cc-dd-ee-ff, aa:bb:cc:dd:ee:ff, aabbccddeeff , or aa bb cc dd ee ff).
   Returns 0 on success and 1 on failure.
4. lib_add.so
   Search for files whose length is equal to the input argument of the --leng parameter
   Returns 0 on success and 1 on failure.
## How to run
1. Clone the Repository: git clone https://github.com/CNT14032000/OCP_2022.git
2. Compile the program by running the following command in the directory containing the source code: Makefile
3. Running</br>
Use the following command to run the program:</br>
./lab1cntN3250 [directory] [target] [Options] </br>
Where:</br>
* [directory] is the path to the directory from which the search begins.</br>
* [target] is the MAC address value represented in hexadecimal format.</br>
* Options</br>
The program supports the following options:</br>
--mac-addr: Target id of mac and check mac-addr</br>
--leng: Requiered file size</br>
--entropy: Target value of entropy</br>
--offset-from: Start offset</br>
--offset-to: End offset</br>
