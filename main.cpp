// main.cpp
#include <iostream>
#include "Book.h"
using namespace std;

int main() {
    Book* head = nullptr;
    loadFromFile(head, "books.txt");

    int choice;
    do {
        cout << "\n1. Add Book\n"
                  "2. Search\n"
                  "3. Issue\n"
                  "4. Return\n"
                  "5. Display\n"
                  "6. Exit\n"
                  "Choice: ";
        cin >> choice;

        if (choice == 1) {
            int id, total;
            string title, author;
            cout << "ID: "; cin >> id;
            cout << "Title: "; cin.ignore(); getline(cin, title);
            cout << "Author: "; getline(cin, author);
            cout << "Total copies: "; cin >> total;
            insertBook(head, id, title, author, total);
        }
        else if (choice == 2) {
            int searchType;
            cout << "Search by:\n1. ID\n2. Title\n3. Author\nChoice: ";
            cin >> searchType;
            Book* b = nullptr;
            
            if (searchType == 1) {
                int id;
                cout << "Enter ID: ";
                cin >> id;
                b = searchByID(head, id);
            } else if (searchType == 2) {
                string title;
                cout << "Enter Title: ";
                cin.ignore();
                getline(cin, title);
                b = searchByTitle(head, title);
            } else if (searchType == 3) {
                string author;
                cout << "Enter Author: ";
                cin.ignore();
                getline(cin, author);
                b = searchByAuthor(head, author);
            } else {
                cout << "Invalid choice.\n";
                continue;
            }
            
            if (b) {
                cout << "Found:\n"
                     << "ID: " << b->bookID
                     << "\t Title: " << b->title
                     << "\t Author: " << b->author
                     << "\t Total Copies: " << b->totalCopies
                     << "\t Available Copies: " << b->availableCopies;
                if (!isEmpty(b->waitingList)) {
                    cout << "\t Waiting List: YES\n";
                }
            } else {
                cout << "Not found.\n";
            }
        }
        else if (choice == 3) {
            int id;
            string studentID;
            cout << "Book ID: "; cin >> id;
            cout << "Student ID: "; cin >> studentID;
            Book* b = searchByID(head, id);
            if (b) issueBook(b, studentID);
            else cout << "Not found.\n";
        }
        else if (choice == 4) {
            int id;
            cout << "Book ID: "; cin >> id;
            Book* b = searchByID(head, id);
            if (b) returnBook(b);
        }
        else if (choice == 5) {
            displayBooks(head);
        }
        else if (choice == 6) {
            saveToFile(head, "books.txt");
            cout << "Saved to books.txt\n";
        }

    } while (choice != 6);

    Book* temp;
    while (head) {
        temp = head;
        head = head->next;
        cleanupQueue(temp->waitingList);
        delete temp;
    }

    return 0;
}
