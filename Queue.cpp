// Queue.cpp
#include "Queue.h"
using namespace std;

void enqueue(Queue &q, string studentID) {
    QueueNode* temp = q.front;
    while (temp) {
        if (temp->studentID == studentID) return;
        temp = temp->next;
    }

    QueueNode* newNode = new QueueNode{studentID, nullptr};
    if (!q.rear) {
        q.front = q.rear = newNode;
    } else {
        q.rear->next = newNode;
        q.rear = newNode;
    }
}

string dequeue(Queue &q) {
    if (!q.front) return "";
    QueueNode* temp = q.front;
    string id = temp->studentID;

    q.front = q.front->next;
    if (!q.front) q.rear = nullptr;

    delete temp;
    return id;
}

bool isEmpty(const Queue &q) {
    return q.front == nullptr;
}

int getQueueSize(const Queue &q) {
    int count = 0;
    QueueNode* temp = q.front;
    while (temp) {
        count++;
        temp = temp->next;
    }
    return count;
}

void cleanupQueue(Queue &q) {
    while (!isEmpty(q)) {
        dequeue(q);
    }
}
