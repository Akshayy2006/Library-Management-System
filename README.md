This Library Management System has been developed as a part of C++ Assignment for course **CS253**, semester 2024/25-2 at IIT Kanpur.  
Submitted by **Pratul Koolwal**, Roll No. **230782**, B.Tech CSE.  
Submitted to Prof. Indranil Saha, course instructor CS253.  

## Overview  
The program is designed for users to interact with it using a command-line interface. The system is designed to make certain prompts regarding the steps to navigate through the system or listing down menu options when needed. In such cases, appropriate inputs are expected from the user.  

## Compiling and Executing Program
The source code for the program is in the `lms.cpp` file, which can be compiled using a C++ compiler. Data of users and books are stored in `users.txt` and `books.txt` files respectively. These files **must be** in the **same folder** at the time of execution.

### Dependencies  
The program needs several Standard Template Libraries, which includes (but is not limited to) libraries like `iostream.h`, `fstream.h`, `vector.h`, as well as `ctime.h`, and others. Many of these libraries are necessary for the compilation of the code.  

## Initializations 
As per the email by the professor, the initialization like adding books and users (as given in the question) is done directly in the `user.txt` and `books.txt` files, instead of using the `main()` function of `lms.cpp` to do the same.
- The sample books have book IDs ranging form 1001 to 1010.
- Sample users with Student role have IDs 101, 102 etc., are named Student1, Student2 etc and have passwords stud1, stud2 etc.
- Sample accounts of Faculties have IDs 201, 202 etc., are named Faculty1, Faculty2 etc and have passwords fac1, fac2 etc.
- Sample accounts of Librarians have IDs 301, 302 etc., are named Librarian1, Librarian2 etc and have passwords lib1, lib2 etc.
- New users can be added by Librarians and they can have any User ID that is 3-digit and not previously used, and can have any Username that does not contains spaces, and can set any custom password. (There are some additional conditions assumed for these fields, which are mentioned in subsequent sections.) 

## Points to Note  
1. The data stored in files is converted to a custom database format every time the program restarts. All the changes that are made are saved after the program execution ends, and not during execution. (Hence, if a session is abruptly terminated, then the changes made in the current session might not be saved in the `.txt` files. However, the `.txt` files are expected to retain the same state as before that session of program call was executed.)  
2. The data is stored in `.txt` files by appropriately converting data to strings and parsing those strings back again in subsequent program calls. However, if unexpected changes are made to the `.txt` database files, the system might behave erroneously. This scenario will not arise if the database is changed **ONLY** by the program execution calls, keeping in mind the appropriate constraints (as listed here).  
3. The system does not generate a new User ID when new users are added, and user IDs have to be manually inserted by the librarian. But the system certainly checks the uniqueness of that User ID and provides necessary error messages in case that user ID is already assigned to some other user (irrespective of their role).  
4. In this program, we assume that while a librarian can add/remove any other user (including other librarians), they *cannot remove their own account*, and the system warns the user and returns back to main menu if they try to do so. Since the system does not restrict the number of librarians, the librarians should get their accounts removed from another librarian's account if needed.  
5. The system often provides confirmation messages after the requested actions are done, but that is not the case for every action.
6. Validity and uniqueness of ISBN numbers of books are not checked by the program.  

## Operating Conditions and Constraints  
While efforts have been made to handle edge cases, it is advisable for users to follow these constraints:  

1. While storing the database in `.txt` files, the character `|` is used as a separator, and users **MUST NOT** use this in any strings, including passwords. Although the system would not throw an immediate error upon encountering this character from user input, it can lead to string parsing errors in subsequent sessions.
2. The system **automatically fetches the current date at the beginning of each session**. Hence it assumes that one session will not remain open for more than one day. If that happens, then system might still issue and return books considering current date as the date on which the current session was opened.   
3. The **User ID** and **Book ID**, whenever needed, are assumed to be integers, and non-integer inputs may give erroneous outputs.  
4. **User ID** is a 3-digit number and is assumed to have a non-zero digit in the Hundred's place (the Most Significant Digit).  
5. **Book ID** is a 4-digit number and is assumed to have a non-zero digit in the Thousand's place (the Most Significant Digit).  
6. After login, in the menu options, the system may behave unexpectedly if prompts other than those listed at respective places are given.
7. Menu commands are case sensitive and users are required to use the exact case (Uppercase or lowercase) as is indicated in the menu options list.  
8. In data fields such as **Book Title, Username, etc.,** with string values, the data must not have spaces in between. If spaces are present, those fields automatically truncate the string at the space, and the next word may be interpreted as input to the next field, which may give unexpected results. Hence, it is advised that users enter an underscore `_` instead of a space if needed.  

## Testing and Stability  
The system has been extensively tested on my system for a variety of test cases and has been found to work well in test conditions under the listed constraints.
