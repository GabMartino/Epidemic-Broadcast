
//        Performance Evaluation Project
//
//        Epidemic Broadcast
//
//        Authors:
//          Gabriele Martino - Maurizio Pulizzi - Antonio Di Tecco
//
//

#include "node.h"

Define_Module(Node);

void Node::initialize()
{
    //set the variable if there are no connections
    if(this->gateCount() == 0){
        this->unConnected = true;
    }

    //if I am the broadcaster, start
    if(getParentModule()->par("indexBroadcastNode").intValue() == this->getIndex()){

        // if I am the broadcaster change the icon to visualize in the simulation the starting point
        cDisplayString& displayString = getDisplayString();
        displayString.setTagArg("i", 0, "block/broadcast");

        //Broadcast a message
        BroadcastMessage* msg = new BroadcastMessage("BROADCAST");
        for (int i = 0; i < this->gateCount()/2; i++)
        {
            cMessage *copy = msg->dup();
            send(copy, "outgate", i);
        }
        delete msg;
        //setting the variable of infected
        infected = true;
    }
}

void Node::handleMessage(cMessage *msg)
{

    BroadcastMessage* bmsg = check_and_cast<BroadcastMessage*>(msg);
    bubble("MESSAGGIO RICEVUTO");
    // if I'm not infected, infect. and rebroadcast the message
    if(!infected){
        infected = true;
        bmsg->setHopCount(bmsg->getHopCount()+1);

        for (int i = 0; i < this->gateCount()/2; i++)
           {
            BroadcastMessage *copy = bmsg->dup();
               send(copy, "outgate", i);
           }
           delete msg;

    }
}
