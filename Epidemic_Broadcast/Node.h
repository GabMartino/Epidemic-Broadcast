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

//Macro for the initialization of the starter node
#define RANDOM_STARTER_NODE         0
#define STARTER_NODE_NEAR_CENTER    1
#define STARTER_NODE_NEAR_VERTEX    2
#define STARTER_NODE_NEAR_TOP_LEFT  3

class Node : public cSimpleModule
{
    private:
        //signals
        simsignal_t slotCountSignal;
        simsignal_t hopCountSignal;
        simsignal_t collisionsOfInfectedSig;
        simsignal_t collisionsOfNOTinfectedSig;
        simsignal_t collisionDetection;
        simsignal_t quantileSignal;
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

        int slotOfInfection=-1; //slot nel quale il nodo viene infettato
        int numbrerInfNode=-1; // conterrà quanti nodi si sono infettati prima di lui +1

        virtual void broadcastMessage();
        virtual void tryToSend();
        virtual void selectStarterNode();
        virtual void setIndexOfStarterNode(double expectedPosX, double expectedPosY, bool anyVertex);
    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual void finish() override;

};
