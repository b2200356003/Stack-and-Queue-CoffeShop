//
// Created by dayte on 7.12.2022.
//

#include "Program.h"
#include <vector>
#include <algorithm>

void Program::Action(string File,string outputFile) {
    ofstream writer;
    writer.open(outputFile);
    Queue * BaristaQueue = new Queue();
    Queue * BaristaQueue2 = new Queue();
    Queue * CashierQueue = new Queue();
    Queue * CashierQueue2 = new Queue();
    vector<int> orderList;
    vector<vector<int>> orderListMatrix;
    int cashierNumber;
    int orderNumber;
    ifstream read(File);
    string line;
    getline(read,line);
    stringstream ss;
    ss << line;
    ss >> cashierNumber;

    int maxBaristaQueue = 0;
    int tempno = 0;
    for (int i = 0; i < cashierNumber; i++) {
        CashierQueue->enqueue(i+1);
        CashierQueue2->enqueue(i+1);
        if(i%3==0){
            tempno++;
            BaristaQueue->enqueue(tempno);
            BaristaQueue2->enqueue(tempno);
        }
    }

    getline(read,line);

    stringstream ss2;
    ss2<<line;
    ss2>>orderNumber;

    Order Orders[orderNumber];
    Order Orders2[orderNumber];

    int orderNo=1;
    while (getline(read,line)){
        stringstream ss3;
        ss3<<line;
        ss3 >> Orders[orderNo-1].arrival;
        Orders2[orderNo-1].arrival = Orders[orderNo-1].arrival;

        ss3 >> Orders[orderNo-1].cashierTime;
        Orders2[orderNo-1].cashierTime = Orders[orderNo-1].cashierTime;

        ss3 >> Orders[orderNo-1].baristaTime;
        Orders2[orderNo-1].baristaTime = Orders[orderNo-1].baristaTime;

        ss3 >> Orders[orderNo-1].price;
        Orders2[orderNo-1].price = Orders[orderNo-1].price;

        Orders[orderNo-1].No=orderNo;
        Orders2[orderNo-1].No = Orders[orderNo-1].No;

        orderNo++;

    }

    bool Done = true;

    int Fig = 1;
    int TotalTime = 0;
    int orderTaken1 = 0;
    int orderTaken2 = 0;


    for (int i = 0; i < cashierNumber/3; ++i) {
        vector<int> tempVector;
        orderListMatrix.push_back(tempVector);
    }

    while (Done){
        CashierQueue->decreaseTime();
        CashierQueue2->decreaseTime();
        BaristaQueue->decreaseTime();
        BaristaQueue2->decreaseTime();

        // geçen zamanı meşgul olanlardan azalttık geçen zaman sonucu boşa çıkan insa (busytime= 0 insa) onun busy booleanını falsea çevirdik.

        if (Fig == 1) {
            for (int i = 0; i < orderNumber; ++i) {
                int ArrivalMultiplied = Orders[i].arrival * 100;
                if (ArrivalMultiplied == TotalTime) {
                    // Kasiyere sipariş geldi

                    bool orderTaken = CashierQueue->takeOrder(Orders[i].cashierTime);
                    if (orderTaken == true) {
                        Orders[i].CameCashier = Orders[i].arrival;
                        Orders[i].OutCashier = Orders[i].CameCashier + Orders[i].cashierTime;

                    }
                    else if (orderTaken == false) {
                        QNode *temp = CashierQueue->minTimeEmployee();
                        Orders[i].CameCashier = Orders[i].arrival + temp->busyTime;
                        Orders[i].OutCashier = Orders[i].CameCashier + Orders[i].cashierTime;
                        temp->busyTime += Orders[i].cashierTime;
                        temp->workingTime += Orders[i].cashierTime;

                    }
                }


                // Barista kısmı

                QNode *headOfBarista2 = BaristaQueue->head();
                while (headOfBarista2->next != nullptr) {
                    headOfBarista2 = headOfBarista2->next;
                }
                int OutCashierMultiplied2 = Orders[i].OutCashier * 100;
                if ((OutCashierMultiplied2 == TotalTime && Orders[i].OutCashier > 0) || headOfBarista2->workingTime > 0) {
                    int OutCashierMultiplied2 = Orders[i].OutCashier * 100;
                    bool orderTaken = false;
                    QNode *headOfBarista = BaristaQueue->head();
                    while (headOfBarista->next != nullptr) {
                        headOfBarista = headOfBarista->next;
                    }
                    if (headOfBarista->workingTime == 0 && OutCashierMultiplied2 == TotalTime) {
                        orderTaken = BaristaQueue->takeOrder(Orders[i].baristaTime);
                    }
                    if (orderTaken == true) {
                        Orders[i].CameBarista = Orders[i].OutCashier;
                        Orders[i].OutBarista = Orders[i].CameBarista + Orders[i].baristaTime;
                        Orders[i].BaristaDone = true;
                        orderTaken1++;

                    }
                    else if (orderTaken == false) {
                        for (int j = 0; j < orderNumber; ++j) {
                            int OutCashierMultiplied = Orders[j].OutCashier * 100;
                            if (OutCashierMultiplied == TotalTime && Orders[j].BaristaDone == false) {
                                bool in = false;
                                for (int k = 0; k < orderList.size(); ++k) {
                                    if (orderList.at(k) == j) {
                                        in = true;
                                    }
                                }
                                if (!in) {
                                    orderList.push_back(j);
                                }
                            }
                        }
                        int maxPriceIndex = -1;
                        if (orderList.size() > 0) {
                            maxPriceIndex = orderList.at(0);
                            for (int j = 1; j < orderList.size(); ++j) {
                                if (Orders[orderList.at(j)].price > Orders[maxPriceIndex].price) {
                                    maxPriceIndex = orderList.at(j);
                                }
                            }
                        }
                        if (orderList.size() > maxBaristaQueue) {
                            maxBaristaQueue = orderList.size();
                        }

                        if (!BaristaQueue->queueIsFull() && maxPriceIndex != -1) {
                            BaristaQueue->takeOrder(Orders[maxPriceIndex].baristaTime);
                            float time = TotalTime;
                            Orders[maxPriceIndex].CameBarista = time / 100;
                            Orders[maxPriceIndex].OutBarista =
                                    Orders[maxPriceIndex].CameBarista + Orders[maxPriceIndex].baristaTime + 0.01;
                            Orders[maxPriceIndex].BaristaDone = true;
                            orderTaken1++;

                            for (int j = 0; j < orderList.size(); ++j) {
                                if (orderList.at(j) == maxPriceIndex) {
                                    orderList.erase(orderList.begin() + j);
                                }
                            }
                        }
                    }
                }
            }
            Fig = 2;
        }

        if (Fig == 2){
            for (int i = 0; i < orderNumber; ++i) {
                int ArrivalMultiplied = Orders2[i].arrival * 100;
                if (ArrivalMultiplied == TotalTime) {
                    // Kasiyere sipariş geldi
                    int orderTaken = CashierQueue2->takeOrder2(Orders2[i].cashierTime);

                    if (orderTaken == 50) {
                        QNode *temp = CashierQueue2->minTimeEmployee();
                        Orders2[i].CameCashier = Orders2[i].arrival + temp->busyTime;
                        Orders2[i].OutCashier = Orders2[i].CameCashier + Orders2[i].cashierTime;
                        Orders2[i].cashierID=temp->no;

                        temp->busyTime += Orders2[i].cashierTime;
                        temp->workingTime += Orders2[i].cashierTime;
                    }

                    else {
                        Orders2[i].CameCashier = Orders2[i].arrival;
                        Orders2[i].OutCashier = Orders2[i].CameCashier + Orders2[i].cashierTime;
                        Orders2[i].cashierID=orderTaken;
                    }
                    if (Orders2[i].cashierID%3==0){
                        Orders2[i].baristaID=Orders2[i].cashierID/3;
                    }
                    else{
                        Orders2[i].baristaID=Orders2[i].cashierID/3+1;
                    }
                }
            }

            // Barista Kısmı


            // find proper orderList
            for (int i = 0; i < orderNumber; ++i) {
                for (int j = 0; j < cashierNumber/3; j++) {
                    if (Orders2[i].baristaID==j+1){
                        int OutCashierMultiplied2 = Orders2[i].OutCashier * 100;
                        if (OutCashierMultiplied2 == TotalTime) {
                                orderListMatrix.at(j).push_back(Orders2[i].No);
                        }
                    }
                }
            }

            QNode *headOfBarista = BaristaQueue2->head();
            for (int i = 0; i < orderListMatrix.size(); ++i) {
                for (int j = 0; j < orderListMatrix.at(i).size(); ++j) {

                    if (orderListMatrix.at(i).size() > 1) {
                        // pricea göre sırala

                        int maxOrderNo = orderListMatrix.at(i).at(0);
                        if (Orders2[orderListMatrix.at(i).at(j) - 1].price > Orders2[maxOrderNo - 1].price) {
                            maxOrderNo = orderListMatrix.at(i).at(j);
                        }

                        if (orderListMatrix.at(i).at(j) == maxOrderNo) {
                            orderListMatrix.at(i).erase(find(orderListMatrix.at(i).begin(), orderListMatrix.at(i).end(), maxOrderNo));
                            orderListMatrix.at(i).insert(orderListMatrix.at(i).begin(), maxOrderNo);
                        }
                    }
                }



                float busytime = headOfBarista->busyTime;
                int intbusytime = busytime * 100;


                if (intbusytime == 0 && orderListMatrix.at(i).size() > 0) {
                    headOfBarista->busyTime += Orders2[orderListMatrix.at(i).at(0) - 1].baristaTime;
                    headOfBarista->workingTime += headOfBarista->busyTime ;
                    float time = TotalTime;
                    Orders2[orderListMatrix.at(i).at(0) - 1].CameBarista = time / 100;
                    Orders2[orderListMatrix.at(i).at(0) - 1].OutBarista = Orders2[orderListMatrix.at(i).at(0) - 1].CameBarista +Orders2[orderListMatrix.at(i).at(0) - 1].baristaTime;
                    orderListMatrix.at(i).erase(orderListMatrix.at(i).begin());
                    orderTaken2++;
                }
                headOfBarista=headOfBarista->next;

            }


            Fig = 1;
        }


        TotalTime+=1;
        if (orderTaken1==orderNumber && orderTaken2==orderNumber){
            Done= false;
        }

    }


    float maxTime = 0;
    for (int i = 0; i < orderNumber; ++i) {
        if (Orders[i].OutBarista>maxTime){
            maxTime=Orders[i].OutBarista;
        }
    }


    writer << maxTime << endl;
    writer<<"-1"<<endl;
    writer<<maxBaristaQueue<<endl;

    QNode* temp = CashierQueue->head();
    QNode* temp2 = BaristaQueue->head();

    for (int i = 0; i < cashierNumber; ++i) {
        writer << Round(temp->workingTime/maxTime) << endl;
        temp= temp->next;
    }

    for (int i = 0; i < cashierNumber/3; ++i) {
        writer<<Round(temp2->workingTime/maxTime) << endl;
        temp2= temp2->next;
    }

    for (int i = 0; i < orderNumber; ++i) {
        writer << (Orders[i].OutBarista-Orders[i].arrival) << endl;
    }

    float maxTime2 = 0;
    for (int i = 0; i < orderNumber; ++i) {
        if (Orders2[i].OutBarista>maxTime2){
            maxTime2=Orders2[i].OutBarista+0.01;
        }

    }

    writer<<endl;
    writer << maxTime2 << endl;
    writer<<"-1"<<endl;
    writer<<"-1"<<endl;

    QNode* temp3 = CashierQueue2->head();
    QNode* temp5 = BaristaQueue2->head();

    for (int i = 0; i < cashierNumber; ++i) {
        writer<<Round(temp3->workingTime/maxTime2) << endl;
        temp3= temp3->next;
    }

    for (int i = 0; i < cashierNumber/3; ++i) {
        writer<<Round(temp5->workingTime/maxTime2) << endl;
        temp5= temp5->next;
    }

    for (int i = 0; i < orderNumber; ++i) {
        writer << (Orders2[i].OutBarista-Orders2[i].arrival+0.01) << endl;
    }
}

float Program::Round(float workTime) {
    float value = (int)(workTime*100 + .5);
    return (float)value/100;
}




