#include "Queue.h"
using namespace std;

void enqueue(Queue &q, string studentID) {
    // Prevent same students
    QueueNode* temp = q.front;
    while (temp) {
        if (temp->studentID == studentID) return;
        temp = temp->next;
    }

    // Create new node
    QueueNode* newNode = new QueueNode{studentID, nullptr};

    // if queue is empty
    if (!q.rear) {
        q.front = q.rear = newNode;
    } else {
        // add to end
        q.rear->next = newNode;
        q.rear = newNode;
    }
}

string dequeue(Queue &q) {
    // Nothing to remove
    if (!q.front) return "";

    QueueNode* temp = q.front;
    string id = temp->studentID;

    // Move front to next element
    q.front = q.front->next;

    // If queue becomes empty
    if (!q.front) q.rear = nullptr;

    delete temp;
    return id;
}

bool isEmpty(const Queue &q) {
    return q.front == nullptr;
}

int getQueueSize(const Queue &q) {
    // Count all queue nodes
    int count = 0;
    QueueNode* temp = q.front;
    while (temp) {
        count++;
        temp = temp->next;
    }
    return count;
}

void cleanupQueue(Queue &q) {
    // delete nodes
    while (!isEmpty(q)) {
        dequeue(q);
    }
}
