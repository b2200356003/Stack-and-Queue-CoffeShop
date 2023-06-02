//
// Created by dayte on 7.12.2022.
//

#ifndef ASSIGNMENT3_4_QNODE_H
#define ASSIGNMENT3_4_QNODE_H
#include <string>

using namespace std;
class QNode {
public:

    QNode* next = nullptr;
    int no;
    float busyTime = 0;
    bool busy = false;
    float workingTime = 0;


    QNode(int id){
        this->no = id;
    }

};


#endif //ASSIGNMENT3_4_QNODE_H
