#include "Queue.h"
#include "Order.h"
using namespace std;

void Queue::enqueue(int id) {
    if (root==nullptr){
        QNode *newQNode = new QNode(id);
        root = newQNode;
    }
    else{
        QNode *newQNode = new QNode(id);
        QNode* temp = root;
        while (temp->next!=nullptr){
            temp = temp->next;
        }
        temp->next = newQNode;
    }
}


QNode * Queue::head() {
    return root;
}

bool Queue::queueIsFull() {
    QNode* temp = root;
    bool full = true;
    while (temp!= nullptr){
        if (temp->busy== false){
            full = false;
        }
        temp = temp->next;
    }
    return full;
}

void Queue::decreaseTime() {
    QNode* temp = root;
    while(temp!= nullptr){
        if (temp->busyTime>0) {
            temp->busyTime -= 0.01;
            int a = temp->busyTime*100;
            if (a==0){
                temp->busy= false;
            }
        }

        temp=temp->next;
    }
}

bool Queue::takeOrder(float cashierTime) {
    QNode* temp = root;
    bool orderTaken = false;
    while(temp!= nullptr){
        if (temp->busy== false && orderTaken == false){
            temp->busy= true;
            temp->busyTime+=cashierTime;
            temp->workingTime+=cashierTime;
            orderTaken= true;
        }
        temp=temp->next;
    }

    return orderTaken;

}


int Queue::takeOrder2(float cashierTime) {
    QNode* temp = root;
    int No;
    bool orderTaken = false;
    while(temp!= nullptr){
        if (temp->busy== false && orderTaken == false){
            temp->busy= true;
            temp->busyTime+=cashierTime;
            temp->workingTime+=cashierTime;
            No = temp->no;
            orderTaken= true;
        }
        temp=temp->next;
    }

    if (orderTaken == false){
        return 50;
    }
    if (orderTaken == true) {
        return No;
    }

}

QNode* Queue::minTimeEmployee() {
    QNode* temp = root;
    QNode* temp2 = root;
    float min = temp->busyTime;
    temp = temp->next;
    while (temp!= nullptr){
        if (temp->busyTime< min){
            min = temp->busyTime;
            temp2=temp;
        }
        temp=temp->next;
    }
    return temp2;
}

