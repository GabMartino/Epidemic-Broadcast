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

#include "Node.h"

Define_Module(Node);

void Node::initialize()
{
    // Take the slotTime and retransmissionProbability parameters from the network parameters
    slotTime = getParentModule()->par("slotTime").doubleValue();
    retransmissionProbability = getParentModule()->par("P").doubleValue();

    hopCountSignal = registerSignal("Infection");
    collisionNumber = registerSignal("Collisions");
    slotCountSignal = registerSignal("SlotCountToTransmit");
    // if this is the starter node
    if(this->getIndex() == getParentModule()->par("indexOfStartingNode").intValue()){
        //being the starting node is also infected
        infected = true;
        transmitted = true;

        //change icon of the simulation to show the infection (is used with hasGUi to distiguish graphic or console simulation)
        if(hasGUI()){
            cDisplayString& displayString = getDisplayString();
            displayString.setTagArg("i", 0, "block/broadcast");
        }

        // make the first message and broadcast
        messageToRetransmit = new epidemicMessage("Broadcast");
        messageToRetransmit->setHopCount(-1);
        messageToRetransmit->setSlotTimeCount(0);
        broadcastMessage();
    }
}

/*
 * This method handle the messages arrived
 */
void Node::handleMessage(cMessage *msg)
{
    // this node is not infected: collisions happend
    if(!infected){

        // a selfMessage points that a slot time of transmission is finished
        if(msg->isSelfMessage()){

           // if in a slot time has been received only a message, everything is ok
           if(messageCounter == 1){
               infected = true;
               bubble("INFECTED");

               emit(slotCountSignal, messageToRetransmit->getSlotTimeCount());


               //change icon of the simulation to show the infection
               if(hasGUI()){
                   cDisplayString& displayString = getDisplayString();
                   displayString.setTagArg("i", 0, "block/circle");
               }
               // try to transmitt
               tryToSend();
           }else{// otherwise collision: other messages arrived in a slotTime
               bubble("COLLISION");

               //reset counter
               messageCounter = 0;
               numberOfCollision++;
               //delete the stored message
               delete messageToRetransmit;
           }
       }else{// New message is arrived

           // count messages arrived
           messageCounter++;
           if(messageCounter == 1){ // first attempt
               // storing of the message
               messageToRetransmit = check_and_cast<epidemicMessage*>(msg)->dup();

               // The transmission time is a slot so a selfmessage is sent when the transmission is finished
               scheduleAt(simTime() + slotTime, new cMessage("Reception finished"));
           }
       }
    }else if(!transmitted && infected){// this node is infected and tries to broadcast infection
        tryToSend();
    }

    // delete anyway the message: if a new one, 'cause has been stored, if is collision one or if is a selfMessage one
    delete msg;

}

/*
 * This method takes a message and tries to transmit with a certain probability
 * otherwise postpones to the next slot another try
 */
void Node::tryToSend(){


    messageToRetransmit->setSlotTimeCount(messageToRetransmit->getSlotTimeCount()+1);


    // extracts a 0 or a 1 with a Bernuallian probability setted
    int check = bernoulli(retransmissionProbability);

    if(check){// broadcast message
        EV<< "Transmission in progress"<<endl;
        EV<< "slot time "<<messageToRetransmit->getSlotTimeCount()<<endl;
        // set a variable of transmission
        transmitted = true;
        bubble("TRANSMITTED");
        // broadcast the message
        broadcastMessage();
    }else{
        EV<< "Try to transmit next time"<<endl;
        // Postpone another try to the next slot

        scheduleAt(simTime() + slotTime, new cMessage("Retry to send"));
    }

}

/*
 * This method broadcasts a message to all output gates connected.
 *
 */
void Node::broadcastMessage(){
    /*
     * Given that we used inout gates and we have duplicated gate.
     * We need only out gate so we divided by 2 the number
     * SEE IF THERE'S A METHOD THAT RETURNS ONLY THE OUTPUT GATE, OR USE AN ITERATOR
     *
     */
    int numGateOut = this->gateCount()/2;
    // increment the hopcount of the message
    messageToRetransmit->setHopCount(messageToRetransmit->getHopCount()+1);
    emit(hopCountSignal, messageToRetransmit->getHopCount());
    for(int i= 0; i < numGateOut; i++){
        epidemicMessage* copy = messageToRetransmit->dup();
        send(copy, "gate$o",i);
    }
    delete messageToRetransmit;
}

void Node::finish(){
    if(numberOfCollision){
        emit(collisionNumber, numberOfCollision);
    }
}
