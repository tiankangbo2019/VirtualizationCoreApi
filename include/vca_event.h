//
// Created by root on 2019/12/2.
//

#ifndef VIRTUALIZATIONCOREAPI_VCA_EVENT_H
#define VIRTUALIZATIONCOREAPI_VCA_EVENT_H


#include <iostream>
#include "../include/libvbox.h"
#include <thread>

class Listener {

public:

    Listener();
    ~Listener();
//    typedef int (*callback)(int,int);
//    typedef void (*callback)(VBox::COMPtr<VBox::IVirtualBox> vbox);
    void listenObject(VBox::COMPtr<VBox::IVirtualBox> vbox);
};


#endif //VIRTUALIZATIONCOREAPI_VCA_EVENT_H
