#include "Book.h"
#include <iostream>
#include <fstream>
using namespace std;

void insertBook(Book*& head, int id, string title, string author, int totalCopies) {
    // Dont allow 2 same IDs
    if (searchByID(head, id)) {
        cout << "Book ID already exists.\n";
        return;
    }

    // Cannot add negative copies
    if (totalCopies < 0) {
        cout << "Total copies cannot be negative.\n";
        return;
    }

    // Create new linked list book
    Book* newBook = new Book{id, title, author, totalCopies, totalCopies, Queue(), nullptr};

    // Insert book at front if list is empty or ID is smaller
    if (!head || id < head->bookID) {
        newBook->next = head;
        head = newBook;
        return;
    }

    // Insert into sorted list
    Book* current = head;
    while (current->next && current->next->bookID < id)
        current = current->next;

    newBook->next = current->next;
    current->next = newBook;
}

Book* searchByID(Book* head, int id) {
    // linear search in linked list
    while (head) {
        if (head->bookID == id) return head;
        head = head->next;
    }
    return nullptr;
}

Book* searchByTitle(Book* head, const string &title) {
    while (head) {
        if (head->title == title) return head;
        head = head->next;
    }
    return nullptr;
}

Book* searchByAuthor(Book* head, const string &author) {
    while (head) {
        if (head->author == author) return head;
        head = head->next;
    }
    return nullptr;
}

void displayBooks(Book* head) {
    // Print all books
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
    // If copies are available -1 count
    if (book->availableCopies > 0) {
        book->availableCopies--;
        cout << "Book issued.\n";
    } else {
        // if not add to waiting list
        int posBefore = getQueueSize(book->waitingList);
        enqueue(book->waitingList, studentID);
        int posAfter = getQueueSize(book->waitingList);
        if (posAfter > posBefore) {
            cout << "Added to waiting list. Position: " << posAfter << endl;
        } else {
            cout << "Student already in waiting list.\n";
        }
    }
}

void returnBook(Book* book) {
    // To not let availableCopies from getting bigger then totalCopies
    if (book->availableCopies >= book->totalCopies) {
        cout << "All copies are already available.\n";
        return;
    }
    // If no waiting list return the book
    if (isEmpty(book->waitingList)) {
        book->availableCopies++;
        cout << "Book returned.\n";
    } else {
        // If queue exists -> give the book to the next student
        string next = dequeue(book->waitingList);
        cout << "Returned book issued to " << next << endl;
    }
}

void saveToFile(Book* head, const string &filename) {

    // Save books to file
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
    // Load books from file if it exists
    ifstream file(filename);
    if (!file) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        // parce format
        int pos = 0;
        string vals[5];
        int i = 0;

        // split by ","s
        while ((pos = line.find(',')) != string::npos) {
            vals[i++] = line.substr(0, pos);
            line.erase(0, pos + 1);
        }
        vals[4] = line;

        // Convert to correct types
        int id = stoi(vals[0]);

        // Insert into linked list
        insertBook(head, id, vals[1], vals[2], stoi(vals[3]));

        // Overwrite copies
        Book* book = searchByID(head, id);
        if (book) book->availableCopies = stoi(vals[4]);
    }
}