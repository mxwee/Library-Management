// Book.h
#ifndef BOOK_H
#define BOOK_H
#include <string>
#include "Queue.h"
using namespace std;

struct Book {
    int bookID;
    string title;
    string author;
    int totalCopies;
    int availableCopies;

    Queue waitingList;
    Book* next;
};

void insertBook(Book*& head, int id, string title, string author, int totalCopies);
Book* searchByID(Book* head, int id);
void displayBooks(Book* head);

// Issue and Return
void issueBook(Book* book, string studentID);
void returnBook(Book* book);

// File
void saveToFile(Book* head, const string &filename);
void loadFromFile(Book*& head, const string &filename);

#endif
