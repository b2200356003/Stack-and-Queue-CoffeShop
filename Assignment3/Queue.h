#include "QNode.h"
#include "Order.h"
class Queue {
public:
    QNode* root = NULL;
    void enqueue(int id);
    QNode * head();
    bool queueIsFull();
    void decreaseTime();
    bool takeOrder(float cashierTime);
    int takeOrder2(float cashierTime);
    QNode* minTimeEmployee();
};