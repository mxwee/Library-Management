// Queue.h
#pragma once
#include <string>
using namespace std;

struct QueueNode {
    string studentID;
    QueueNode* next;
};

struct Queue {
    QueueNode* front;
    QueueNode* rear;

    Queue() : front(nullptr), rear(nullptr) {}
};

void enqueue(Queue &q, string studentID);
string dequeue(Queue &q);
bool isEmpty(const Queue &q);
int getQueueSize(const Queue &q);
void cleanupQueue(Queue &q);