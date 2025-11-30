#include <iostream>
#include "Book.h"
using namespace std;

int main() {
    Book* head = nullptr;                           // Startf of linked list
    loadFromFile(head, "books.txt");   // Load saved books into linked list

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
            // Add a new book
            int id, total;
            string title, author;
            cout << "ID: "; cin >> id;
            cout << "Title: "; cin.ignore(); getline(cin, title);
            cout << "Author: "; getline(cin, author);
            cout << "Total: "; cin >> total;

            // Insert into linked list, sorted by ID
            insertBook(head, id, title, author, total);
        }
        else if (choice == 2) {
            // Search for book by ID, Title, Author
            int searchType;
            cout << "Search by:\n1. ID\n2. Title\n3. Author\nChoice: ";
            cin >> searchType;
            Book* b = nullptr;
            
            if (searchType == 1) {
                int id;     // ny ID
                cout << "Enter ID: ";
                cin >> id;
                b = searchByID(head, id);
            } else if (searchType == 2) {
                string title;   // by Title
                cout << "Enter Title: ";
                cin.ignore();
                getline(cin, title);
                b = searchByTitle(head, title);
            } else if (searchType == 3) {
                string author;      // by Author
                cout << "Enter Author: ";
                cin.ignore();
                getline(cin, author);
                b = searchByAuthor(head, author);
            } else {
                cout << "Invalid choice.\n";
                continue;
            }

            // Display result
            if (b) {
                cout << "Found:\n"
                     << "ID: " << b->bookID
                     << "\t Title: " << b->title
                     << "\t Author: " << b->author
                     << "\t Total: " << b->totalCopies
                     << "\t Available: " << b->availableCopies;

                // Check if waiting list is not empty
                if (!isEmpty(b->waitingList)) {
                    cout << "\t Waiting List: YES\n";
                }
            } else {
                cout << "Not found.\n";
            }
        }
        else if (choice == 3) {
            // Loan a book
            int id;
            string studentID;
            cout << "Book ID: "; cin >> id;
            cout << "Student ID: "; cin >> studentID;
            Book* b = searchByID(head, id);
            if (b) issueBook(b, studentID);
            else cout << "Not found.\n";
        }
        else if (choice == 4) {
            // Return a book
            int id;
            cout << "Book ID: "; cin >> id;
            Book* b = searchByID(head, id);
            if (b) returnBook(b);
        }
        else if (choice == 5) {
            // Display full list
            displayBooks(head);
        }
        else if (choice == 6) {
            // Save before exit
            saveToFile(head, "books.txt");
            cout << "Saved to books.txt\n";
        }

    } while (choice != 6);

    // Free allocated memory
    Book* temp;
    while (head) {
        temp = head;
        head = head->next;
        cleanupQueue(temp->waitingList);    // Free queue nodes
        delete temp;                           // free book nodes
    }

    return 0;
}
