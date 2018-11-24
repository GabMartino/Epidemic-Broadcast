
//        Performance Evaluation Project
//
//        Epidemic Broadcast
//
//        Authors:
//          Gabriele Martino - Maurizio Pulizzi - Antonio Di Tecco
//
//

#ifndef __PROJECT_TEST_NODE_H_
#define __PROJECT_TEST_NODE_H_

#include <omnetpp.h>

using namespace omnetpp;
//
//variables:
//    -unConnected : specifies if this module has connections or not
//    -infected : specifies if this module has been infected or not
//
//
class Node : public cSimpleModule
{
    private:
        bool unConnected = false;
        bool infected = false;
    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
};

#endif
