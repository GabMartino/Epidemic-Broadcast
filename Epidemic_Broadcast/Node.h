/*
 *  Performance Evaluation Project
 *
 *  Epidemic Broadcast
 *
 *  Authors:
 *
 *  Di Tecco Antonio - Martino Gabriele - Pulizzi Maurizio
 *
 */

#include <omnetpp.h>
#include "EpidemicMessage_m.h"

using namespace omnetpp;

class Node : public cSimpleModule
{
    private:
        //signals
        simsignal_t hopCountSignal;
        simsignal_t collisionNumber;
        simsignal_t slotCountSignal;

        // specifies infection (if an infection message is arrived with no collisions)
        bool infected = false;

        // specifies if the node sent the message in broadcast
        bool transmitted = false;

        // slotTime parameter
        simtime_t slotTime;

        // probability of retransmission parameter
        double retransmissionProbability;

        // stored message
        epidemicMessage* messageToRetransmit = nullptr;

        // counter of message received in a slot time of a reception attempt
        int messageCounter = 0;

        //number of collision
        int numberOfCollision = 0;

        virtual void broadcastMessage();
        virtual void tryToSend();
    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual void finish() override;

};
