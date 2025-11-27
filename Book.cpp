// Book.cpp
#include "Book.h"
#include <iostream>
#include <fstream>
using namespace std;

void insertBook(Book*& head, int id, string title, string author, int totalCopies) {
    Book* newBook = new Book{id, title, author, totalCopies, totalCopies, Queue(), nullptr};

    if (!head || id < head->bookID) {
        newBook->next = head;
        head = newBook;
        return;
    }

    Book* current = head;
    while (current->next && current->next->bookID < id)
        current = current->next;

    newBook->next = current->next;
    current->next = newBook;
}

Book* searchByID(Book* head, int id) {
    while (head) {
        if (head->bookID == id) return head;
        head = head->next;
    }
    return nullptr;
}

void displayBooks(Book* head) {
    while (head) {
        cout << "ID: " << head->bookID
             << "\t Title: " << head->title
             << "\t Author: " << head->author
             << "\t Total: " << head->totalCopies
             << "\t Available: " << head->availableCopies;

        if (!isEmpty(head->waitingList))
            cout << "\t Waiting List: YES";
        cout << endl;

        head = head->next;
    }
}

void issueBook(Book* book, string studentID) {
    if (book->availableCopies > 0) {
        book->availableCopies--;
        cout << "Book issued.\n";
    } else {
        enqueue(book->waitingList, studentID);
        cout << "Added to waiting list.\n";
    }
}

void returnBook(Book* book) {
    if (isEmpty(book->waitingList)) {
        book->availableCopies++;
        cout << "Book returned.\n";
    } else {
        string next = dequeue(book->waitingList);
        cout << "Returned book issued to " << next << endl;
    }
}

void saveToFile(Book* head, const string &filename) {
    ofstream file(filename);
    while (head) {
        file << head->bookID << ","
             << head->title << ","
             << head->author << ","
             << head->totalCopies << ","
             << head->availableCopies << "\n";
        head = head->next;
    }
}

void loadFromFile(Book*& head, const string &filename) {
    ifstream file(filename);
    if (!file) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        int pos = 0;
        string vals[5];
        int i = 0;
        while ((pos = line.find(',')) != string::npos) {
            vals[i++] = line.substr(0, pos);
            line.erase(0, pos + 1);
        }
        vals[4] = line;

        int id = stoi(vals[0]);
        insertBook(head, id, vals[1], vals[2], stoi(vals[3]));
        Book* book = searchByID(head, id);
        if (book) book->availableCopies = stoi(vals[4]);
    }
}