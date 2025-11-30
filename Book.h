#pragma once
#include <string>
#include "Queue.h"
using namespace std;

// Linked list node to represent book
struct Book {
    int bookID;
    string title;
    string author;
    int totalCopies;
    int availableCopies;

    Queue waitingList;      // Queue for students waiting
    Book* next;             // Points to next book
};

// Function declarations
void insertBook(Book*& head, int id, string title, string author, int totalCopies);
Book* searchByID(Book* head, int id);
Book* searchByTitle(Book* head, const string &title);
Book* searchByAuthor(Book* head, const string &author);
void displayBooks(Book* head);

// Issue and Return
void issueBook(Book* book, string studentID);
void returnBook(Book* book);

// File
void saveToFile(Book* head, const string &filename);
void loadFromFile(Book*& head, const string &filename);
