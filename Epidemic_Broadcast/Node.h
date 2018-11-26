
//        Performance Evaluation Project
//
//        Epidemic Broadcast
//
//        Authors:
//          Gabriele Martino - Maurizio Pulizzi - Antonio Di Tecco
//
//

#ifndef __EPIDEMIC_BROADCAST_1_NODE_H_
#define __EPIDEMIC_BROADCAST_1_NODE_H_

#include <omnetpp.h>

using namespace omnetpp;


class Node : public cSimpleModule
{
    private:
        // specifies infection (if an infection message is arrived with no collisions)
        bool infected = false;

        // specifies if the node sent the message in broadcast
        bool transmitted = false;

        // slotTime parameter
        simtime_t slotTime;

        // probability of retransmission parameter
        double retransmissionProbability;

        // stored message
        cMessage* messageToRetransmit;

        // counter of message received in a slot time of a reception attempt
        int messageCounter = 0;
    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
        virtual void broadcastMessage(cMessage* msg);
        virtual void tryToSend(cMessage* msg);
};

#endif
