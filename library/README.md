# Library Management System

## Overview
This C++ program implements a simple library management system. It reads information about libraries and books from a file, organizes the data into a map-based data structure, and allows users to perform various queries on the library collection.

## Commands
- **libraries**: Display a list of all libraries.
- **material [library_name]**: Display a list of books in the specified library, organized by author.
- **books [library_name] [author_name]**: Display books by a specific author in the given library, along with reservation information.
- **reservable [author_name] [book_name]**: Check the reservation status and availability of a specific book by an author across all libraries.
- **loanable**: List books that are immediately available for loan across all libraries.
- **quit**: Exit the program.

## Input File Format
The program reads library and book information from a file. Each line in the file should contain the following fields separated by semicolons:
1. Library Name
2. Author Name
3. Book Title
4. Reservations (either "on-the-shelf" or a numerical value)


The test input file is called (`input.csv`).

