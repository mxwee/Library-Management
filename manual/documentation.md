# Library Management System - Code Documentation

## Table of Contents
1. [Overview](#overview)
2. [Data Structures](#data-structures)
3. [Queue Module (Queue.h & Queue.cpp)](#queue-module)
4. [Book Module (Book.h & Book.cpp)](#book-module)
5. [Main Program (main.cpp)](#main-program)
6. [How Everything Works Together](#how-everything-works-together)

---

## Overview

This Library Management System is a C++ program that manages book records using:
- **Linked List**: Stores all books sorted by Book ID
- **Queue**: Manages waiting lists when books are unavailable
- **File Handling**: Persists data to disk (CSV format)

### File Structure
- `Queue.h` / `Queue.cpp` - Queue data structure for waiting lists
- `Book.h` / `Book.cpp` - Book linked list and operations
- `main.cpp` - User interface and program flow

---

## Data Structures

### Queue Structure (FIFO - First In, First Out)
Used to store student IDs waiting for a book when all copies are checked out.

### Linked List Structure
Used to store all books in sorted order by Book ID.

---

## Queue Module

### Queue.h - Header File

#### `struct QueueNode`
**Purpose**: Represents one student waiting in the queue.

**Fields**:
- `string studentID` - The ID of the student waiting for the book
- `QueueNode* next` - Pointer to the next student in line (or `nullptr` if this is the last one)

**How it works**: Each `QueueNode` points to the next one, creating a chain. This is a **singly linked list**.

#### `struct Queue`
**Purpose**: Represents the entire waiting list for a book.

**Fields**:
- `QueueNode* front` - Points to the **first** student (next to receive the book)
- `QueueNode* rear` - Points to the **last** student (most recently added)

**Constructor**:
- `Queue() : front(nullptr), rear(nullptr) {}` - Initializes an empty queue

**Why two pointers?**
- `front` allows quick removal (dequeue) from the beginning
- `rear` allows quick addition (enqueue) at the end
- Without `rear`, adding would require traversing the entire list

#### Function Declarations
- `void enqueue(Queue &q, string studentID)` - Add student to end of queue
- `string dequeue(Queue &q)` - Remove and return first student
- `bool isEmpty(const Queue &q)` - Check if queue is empty
- `int getQueueSize(const Queue &q)` - Count students in queue
- `void cleanupQueue(Queue &q)` - Free all memory

---

### Queue.cpp - Implementation

#### `enqueue(Queue &q, string studentID)`
**Purpose**: Add a new student to the waiting list (if not already present).

**Step-by-step**:
1. **Check for duplicates**: 
   - Start from `q.front` and walk through all nodes
   - If `temp->studentID == studentID`, the student is already in queue → return early
   - This prevents the same student from being added twice

2. **Create new node**:
   - `new QueueNode{studentID, nullptr}` creates a new node with the student ID
   - `next` is `nullptr` because this will be the last node

3. **If queue is empty** (`q.rear == nullptr`):
   - Set both `q.front` and `q.rear` to point to the new node
   - This is the first and only element

4. **If queue is not empty**:
   - Link the new node: `q.rear->next = newNode`
   - Update `rear`: `q.rear = newNode`
   - The new student is now at the end of the line

**Example**: If queue has [Alice, Bob] and we add "Charlie":
- Before: `front → Alice → Bob → nullptr`, `rear → Bob`
- After: `front → Alice → Bob → Charlie → nullptr`, `rear → Charlie`

#### `dequeue(Queue &q)`
**Purpose**: Remove the first student from the queue (FIFO - first come, first served).

**Step-by-step**:
1. **Check if empty**:
   - If `q.front == nullptr`, return empty string `""`
   - Nothing to remove

2. **Remove front node**:
   - Save `q.front` in `temp` (the node we're deleting)
   - Save `temp->studentID` in `id` (to return it)
   - Move `q.front` forward: `q.front = q.front->next`

3. **Handle empty queue**:
   - If `q.front` becomes `nullptr`, the queue is now empty
   - Set `q.rear = nullptr` as well (both pointers should be null)

4. **Clean up**:
   - `delete temp` frees the memory
   - Return `id`

**Example**: If queue has [Alice, Bob, Charlie]:
- Before: `front → Alice → Bob → Charlie → nullptr`
- After: `front → Bob → Charlie → nullptr`, return "Alice"

#### `isEmpty(const Queue &q)`
**Purpose**: Check if the queue has any students.

**Implementation**:
- Returns `true` if `q.front == nullptr`
- Returns `false` otherwise

**Why check `front`?**: If `front` is null, there are no nodes. `rear` alone doesn't tell us if queue is empty (it could be null even if front exists, but that's an invalid state).

#### `getQueueSize(const Queue &q)`
**Purpose**: Count how many students are waiting.

**Step-by-step**:
1. Start from `q.front`
2. Initialize `count = 0`
3. Walk through the list: `temp = temp->next`
4. Increment `count` for each node
5. Return `count`

**Usage**: Used to show a student their position when added to waiting list.

**Example**: Queue [Alice, Bob, Charlie] → returns 3

#### `cleanupQueue(Queue &q)`
**Purpose**: Free all memory when program ends.

**Implementation**:
- While queue is not empty (`!isEmpty(q)`), repeatedly call `dequeue(q)`
- Each `dequeue` removes and deletes one node
- When done, `front` and `rear` are both `nullptr`

**Why needed**: Prevents memory leaks when the program exits.

---

## Book Module

### Book.h - Header File

#### `struct Book`
**Purpose**: Represents one book in the library's collection.

**Fields**:
- `int bookID` - Unique identifier for the book
- `string title` - Book title
- `string author` - Author name
- `int totalCopies` - Total number of copies the library owns
- `int availableCopies` - How many copies are currently available (not checked out)
- `Queue waitingList` - Queue of student IDs waiting when `availableCopies == 0`
- `Book* next` - Pointer to the next book in the linked list

**Important**: `availableCopies` can be less than `totalCopies` (books are checked out), but should never exceed `totalCopies`.

#### Function Declarations

**Insertion & Search**:
- `void insertBook(Book*& head, int id, string title, string author, int totalCopies)` - Add book in sorted order
- `Book* searchByID(Book* head, int id)` - Find book by ID
- `Book* searchByTitle(Book* head, const string &title)` - Find book by title
- `Book* searchByAuthor(Book* head, const string &author)` - Find book by author
- `void displayBooks(Book* head)` - Print all books

**Book Operations**:
- `void issueBook(Book* book, string studentID)` - Issue book or add to waiting list
- `void returnBook(Book* book)` - Return book or auto-issue to next in queue

**File Operations**:
- `void saveToFile(Book* head, const string &filename)` - Save all books to file
- `void loadFromFile(Book*& head, const string &filename)` - Load books from file

---

### Book.cpp - Implementation

#### `insertBook(Book*& head, int id, string title, string author, int totalCopies)`
**Purpose**: Add a new book to the linked list, maintaining sorted order by Book ID.

**Step-by-step**:

1. **Prevent duplicate IDs**:
   - Call `searchByID(head, id)` to check if ID already exists
   - If found, print error and return early
   - Ensures each book has a unique ID

2. **Validate total copies**:
   - If `totalCopies < 0`, print error and return
   - Prevents invalid data

3. **Create new book node**:
   - `new Book{id, title, author, totalCopies, totalCopies, Queue(), nullptr}`
   - `availableCopies` starts equal to `totalCopies` (all copies available)
   - `waitingList` is initialized as empty queue
   - `next` is `nullptr` (will be set during insertion)

4. **Insert at beginning** (if list is empty or new ID is smallest):
   - If `head == nullptr` (no books yet) OR `id < head->bookID`:
     - Set `newBook->next = head` (new book points to old head)
     - Set `head = newBook` (new book becomes head)
     - Return

5. **Insert in sorted position**:
   - Start from `head` and walk through the list
   - Find the right spot: `while (current->next && current->next->bookID < id)`
   - This stops when `current->next` has an ID >= new ID, or `current->next` is null
   - Insert: `newBook->next = current->next; current->next = newBook;`

**Example**: List has books [101, 103, 105]. Insert book 104:
- Walk: `current = head (101)`, `current->next (103) < 104` → continue
- Walk: `current = 103`, `current->next (105) < 104` → false, stop
- Insert: `newBook->next = 105`, `current->next = newBook`
- Result: [101, 103, 104, 105]

**Why sorted?**: Makes searching easier and keeps data organized.

#### `searchByID(Book* head, int id)`
**Purpose**: Find a book by its ID.

**Implementation**:
- Linear search: walk through linked list from `head`
- Compare `head->bookID == id` at each node
- If found, return pointer to that book
- If `head` becomes `nullptr`, return `nullptr` (not found)

**Time complexity**: O(n) where n is number of books

#### `searchByTitle(Book* head, const string &title)`
**Purpose**: Find a book by its exact title.

**Implementation**: Same as `searchByID`, but compares `head->title == title`

**Note**: Uses exact match. "The Hobbit" ≠ "the hobbit"

#### `searchByAuthor(Book* head, const string &author)`
**Purpose**: Find a book by its author name.

**Implementation**: Same as `searchByID`, but compares `head->author == author`

**Note**: Returns first match. If multiple books by same author exist, only first is returned.

#### `displayBooks(Book* head)`
**Purpose**: Print all books in the library.

**Step-by-step**:
1. Loop through linked list: `while (head)`
2. For each book, print:
   - ID, Title, Author, Total copies, Available copies
3. Check waiting list:
   - If `!isEmpty(head->waitingList)`, print "Waiting List: YES"
4. Move to next: `head = head->next`

**Output format**:
```
ID: 101    Title: The Hobbit    Author: Tolkien    Total: 5    Available: 2    Waiting List: YES
ID: 102    Title: 1984    Author: Orwell    Total: 3    Available: 3
```

#### `issueBook(Book* book, string studentID)`
**Purpose**: Issue a book to a student, or add them to waiting list if unavailable.

**Step-by-step**:

1. **If copies are available** (`book->availableCopies > 0`):
   - Decrement: `book->availableCopies--`
   - Print "Book issued."
   - Done

2. **If no copies available** (`book->availableCopies == 0`):
   - Get queue size before: `posBefore = getQueueSize(book->waitingList)`
   - Add student: `enqueue(book->waitingList, studentID)`
   - Get queue size after: `posAfter = getQueueSize(book->waitingList)`
   - If `posAfter > posBefore`: student was successfully added
     - Print "Added to waiting list. Position: <posAfter>"
   - Else: student was already in queue
     - Print "Student already in waiting list."

**Example**: Book has 0 available copies, queue has [Alice, Bob]:
- Student "Charlie" requests → added to queue
- Position: 3 (third in line)
- Output: "Added to waiting list. Position: 3"

#### `returnBook(Book* book)`
**Purpose**: Process a book return. If someone is waiting, automatically issue to them.

**Step-by-step**:

1. **Validate return**:
   - If `book->availableCopies >= book->totalCopies`:
     - All copies are already available (can't return more than total)
     - Print error and return

2. **If no waiting list** (`isEmpty(book->waitingList)`):
   - Increment: `book->availableCopies++`
   - Print "Book returned."
   - Done

3. **If waiting list exists**:
   - Remove first student: `string next = dequeue(book->waitingList)`
   - Print "Returned book issued to <next>"
   - **Important**: `availableCopies` does NOT increase!
   - Why? The book went from one student directly to another

**Example**: Book has 2 total copies, 0 available, queue [Alice, Bob]:
- Student returns book → Alice gets it immediately
- `availableCopies` stays 0 (still all checked out)
- Queue becomes [Bob]
- Output: "Returned book issued to Alice"

#### `saveToFile(Book* head, const string &filename)`
**Purpose**: Save all book data to a text file for persistence.

**Step-by-step**:
1. Open file: `ofstream file(filename);`
2. Loop through linked list: `while (head)`
3. Write each book as one line in CSV format:
   - Format: `bookID,title,author,totalCopies,availableCopies\n`
   - Example: `101,The Hobbit,Tolkien,5,2\n`
4. Move to next: `head = head->next`
5. File automatically closes when `file` goes out of scope

**File format** (CSV):
```
101,The Hobbit,Tolkien,5,2
102,1984,Orwell,3,3
103,To Kill a Mockingbird,Lee,4,0
```

**Note**: Waiting lists are NOT saved (they reset on program restart).

#### `loadFromFile(Book*& head, const string &filename)`
**Purpose**: Load books from file when program starts.

**Step-by-step**:

1. **Open file**:
   - `ifstream file(filename);`
   - If file doesn't exist (`!file`), return early (no saved data)

2. **Read line by line**:
   - `getline(file, line)` reads one line
   - Skip empty lines: `if (line.empty()) continue;`

3. **Parse CSV format**:
   - Manual parsing (simple approach):
     - Find commas: `pos = line.find(',')`
     - Extract substring before comma: `line.substr(0, pos)`
     - Store in `vals[i++]`
     - Remove parsed part: `line.erase(0, pos + 1)`
     - Repeat until no more commas
     - Last value (after final comma) goes into `vals[4]`

   **Example**: Line `"101,The Hobbit,Tolkien,5,2"`
   - Iteration 1: `vals[0] = "101"`, remaining: `"The Hobbit,Tolkien,5,2"`
   - Iteration 2: `vals[1] = "The Hobbit"`, remaining: `"Tolkien,5,2"`
   - Iteration 3: `vals[2] = "Tolkien"`, remaining: `"5,2"`
   - Iteration 4: `vals[3] = "5"`, remaining: `"2"`
   - Final: `vals[4] = "2"`

4. **Convert to correct types**:
   - `int id = stoi(vals[0]);` - Convert string to int
   - `stoi(vals[3])` for total copies
   - `stoi(vals[4])` for available copies

5. **Insert into linked list**:
   - `insertBook(head, id, vals[1], vals[2], stoi(vals[3]));`
   - This maintains sorted order

6. **Set available copies**:
   - Find the book: `Book* book = searchByID(head, id);`
   - Overwrite: `book->availableCopies = stoi(vals[4]);`
   - Why overwrite? `insertBook` sets `availableCopies = totalCopies`, but we need the saved value

**Why this approach?**: Simple CSV parsing without external libraries. More robust solutions exist, but this works for the format.

---

## Main Program

### main.cpp - User Interface

#### Includes and Setup
```cpp
#include <iostream>
#include "Book.h"
using namespace std;
```
- `iostream` for input/output (`cout`, `cin`)
- `Book.h` includes all book functions and `Queue.h`

#### Program Initialization
```cpp
Book* head = nullptr;
loadFromFile(head, "books.txt");
```
- `head` is the start of the linked list (initially empty)
- `loadFromFile` loads any previously saved books
- If `books.txt` doesn't exist, `head` remains `nullptr` (empty list)

#### Main Menu Loop
```cpp
int choice;
do {
    // Print menu
    // Read choice
    // Handle choice
} while (choice != 6);
```
- `do...while` ensures menu shows at least once
- Loop continues until user enters `6` (Exit)

#### Menu Options

##### Option 1: Add Book
**What it does**: Adds a new book to the library.

**Step-by-step**:
1. Read book details:
   - `cin >> id` - Book ID
   - `cin.ignore(); getline(cin, title)` - Full title (may have spaces)
   - `getline(cin, author)` - Author name
   - `cin >> total` - Total copies

2. Call `insertBook(head, id, title, author, total)`
   - Handles sorting, duplicate checking, validation

**Why `cin.ignore()`?**: After `cin >> id`, a newline remains in buffer. `getline` would read that empty line. `cin.ignore()` clears it.

##### Option 2: Search
**What it does**: Find a book by ID, Title, or Author.

**Step-by-step**:
1. Ask search type: `1. ID`, `2. Title`, `3. Author`
2. Read `searchType`
3. Based on type:
   - **ID**: `cin >> id`, call `searchByID(head, id)`
   - **Title**: `cin.ignore(); getline(cin, title)`, call `searchByTitle(head, title)`
   - **Author**: `cin.ignore(); getline(cin, author)`, call `searchByAuthor(head, author)`

4. Display result:
   - If found: Print ID, Title, Author, Total, Available
   - If waiting list exists: Print "Waiting List: YES"
   - If not found: Print "Not found."

**Note**: `cin.ignore()` needed before `getline` to clear newline from previous `cin >>`.

##### Option 3: Issue Book
**What it does**: Loan a book to a student.

**Step-by-step**:
1. Read `id` (book ID) and `studentID`
2. Find book: `Book* b = searchByID(head, id)`
3. If found: Call `issueBook(b, studentID)`
   - Either decrements `availableCopies` or adds to waiting list
4. If not found: Print "Not found."

##### Option 4: Return Book
**What it does**: Process a book return.

**Step-by-step**:
1. Read `id` (book ID)
2. Find book: `Book* b = searchByID(head, id)`
3. If found: Call `returnBook(b)`
   - Either increments `availableCopies` or auto-issues to next in queue

##### Option 5: Display All Books
**What it does**: Show all books in the library.

**Implementation**: Simply calls `displayBooks(head)`

**Output**: All books printed in sorted order (by ID), with waiting list indicators.

##### Option 6: Exit
**What it does**: Save data and exit program.

**Step-by-step**:
1. Call `saveToFile(head, "books.txt")`
   - Writes all books to file
2. Print confirmation: "Saved to books.txt"
3. Loop condition `choice != 6` becomes false, loop exits

#### Memory Cleanup
```cpp
Book* temp;
while (head) {
    temp = head;
    head = head->next;
    cleanupQueue(temp->waitingList);
    delete temp;
}
```

**Purpose**: Free all allocated memory before program ends.

**Step-by-step**:
1. Loop through linked list: `while (head)`
2. For each book:
   - Save `head` in `temp` (so we can delete it)
   - Move `head` forward: `head = head->next`
   - Free waiting list: `cleanupQueue(temp->waitingList)`
     - Deletes all `QueueNode`s in the queue
   - Delete book: `delete temp`
     - Frees the `Book` node itself

**Why needed**: Prevents memory leaks. Every `new` must have a corresponding `delete`.

**Order matters**: Must free queue before deleting book (queue is part of book).

---

## How Everything Works Together

### Program Flow

1. **Startup**:
   - `main()` creates empty linked list (`head = nullptr`)
   - `loadFromFile()` reads `books.txt` and populates the list
   - Books are inserted in sorted order (by ID)

2. **User Interaction**:
   - Menu loop presents options
   - User selects action (Add, Search, Issue, Return, Display, Exit)
   - Appropriate functions are called

3. **Book Operations**:
   - **Add**: Validates, inserts in sorted order
   - **Search**: Linear search through linked list
   - **Issue**: Decrements copies or enqueues student
   - **Return**: Increments copies or dequeues student

4. **Shutdown**:
   - `saveToFile()` writes all data to `books.txt`
   - Memory cleanup frees all nodes
   - Program exits

### Data Flow Example

**Scenario**: Student "Alice" wants book ID 101, but all copies are checked out.

1. User selects "3. Issue"
2. Enters: Book ID `101`, Student ID `Alice`
3. `main.cpp` calls `searchByID(head, 101)` → finds book
4. Calls `issueBook(book, "Alice")`
5. `issueBook` checks: `availableCopies == 0`
6. Calls `enqueue(book->waitingList, "Alice")`
7. `enqueue` checks for duplicates, creates `QueueNode`, adds to end
8. Calls `getQueueSize()` → returns position (e.g., 3)
9. Prints "Added to waiting list. Position: 3"

**Later**: Student "Bob" returns book ID 101.

1. User selects "4. Return"
2. Enters: Book ID `101`
3. `main.cpp` calls `searchByID(head, 101)` → finds book
4. Calls `returnBook(book)`
5. `returnBook` checks: `!isEmpty(waitingList)` → true
6. Calls `dequeue(book->waitingList)` → returns "Alice"
7. Prints "Returned book issued to Alice"
8. `availableCopies` stays 0 (book went directly to Alice)

### Key Design Decisions

1. **Linked List for Books**: 
   - Dynamic size (can add/remove books)
   - Sorted by ID for organization
   - Easy to traverse and display

2. **Queue for Waiting Lists**:
   - FIFO ensures fair ordering (first come, first served)
   - Separate queue per book (each book has its own `waitingList`)
   - Prevents duplicate students

3. **File Storage**:
   - CSV format is human-readable
   - Simple parsing without external libraries
   - Waiting lists reset on restart (simplifies persistence)

4. **Memory Management**:
   - All `new` operations have corresponding `delete`
   - Cleanup happens in reverse order (queue before book)
   - Prevents memory leaks

---

## Summary

This Library Management System demonstrates:
- **Linked Lists**: Dynamic data structure for books
- **Queues**: FIFO ordering for waiting lists
- **File I/O**: Persistent storage
- **Memory Management**: Proper allocation and deallocation
- **User Interface**: Menu-driven program flow

The code is modular, with clear separation between:
- Queue operations (`Queue.*`)
- Book operations (`Book.*`)
- User interface (`main.cpp`)

Each module can be understood independently, but they work together to create a complete system.

