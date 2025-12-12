# CY-STUDENT Management System

## Description
A C program for managing student records at CY Tech. This system allows for storing, retrieving, and manipulating student information efficiently. Libraries are provided in both static and dynamic formats.

## Features
A modular design with static and dynamic libraries with the following functionalities :
- Loading promotion from a specific data format (see data/data.txt)
- Saving to binary file / loading from binary file
- Finding best students in specific courses or overall,
- Display promotion information, including courses and student grades,
- Display all student result per field,
- Sorting students by average grades, name, or ID (after setting sort criteria)
- Ciphering and deciphering binary files using XOR method, with user-defined keys (prompted at runtime)


## Building
use 
```bash
make
``` 
for more information on compilation.


If you encounter an issue, please try before executing :
```bash
make clean
```

## Warning :
Binary files generated are NOT portable between systems with different endianness or different sizes for data types.

## Author
Martin CRISSOT
CY Tech Student

## License
This project is part of CY Tech programming coursework 