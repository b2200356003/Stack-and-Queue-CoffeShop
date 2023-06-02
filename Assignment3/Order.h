//
// Created by dayte on 9.12.2022.
//

#ifndef ASSIGNMENT3_4_ORDER_H
#define ASSIGNMENT3_4_ORDER_H


class Order {
public:
    int No;
    int cashierID;
    int baristaID;
    float arrival;
    float cashierTime;
    float baristaTime;
    float price;
    bool BaristaDone = false;
    float TurnAroundTime = 0;
    float CameCashier = 0;
    float OutCashier = 0;
    float CameBarista = 0;
    float OutBarista = 0;
};


#endif //ASSIGNMENT3_4_ORDER_H
