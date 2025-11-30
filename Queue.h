#pragma once
#include <string>
using namespace std;

// Node of queue for waiting students
struct QueueNode {
    string studentID;
    QueueNode* next;
};

// Queue with front and rear pointers
struct Queue {
    QueueNode* front;
    QueueNode* rear;

    // Initializes empty queue
    Queue() : front(nullptr), rear(nullptr) {}
};

void enqueue(Queue &q, string studentID);   // add to queue
string dequeue(Queue &q);                   // remoce from queue
bool isEmpty(const Queue &q);               // check if empty
int getQueueSize(const Queue &q);           // count elements
void cleanupQueue(Queue &q);                // free all memory