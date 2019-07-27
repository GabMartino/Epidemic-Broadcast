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
    //set the node as starter
    if(this->getIndex() == 0){
        selectStarterNode();
    }


    cModule* floor = getParentModule();

    // Take the slotTime and retransmissionProbability parameters from the network parameters
    slotTime = floor->par("slotTime").doubleValue();
    retransmissionProbability = floor->par("P").doubleValue();

    // Registration of signals
    hopCountSignal = registerSignal("Infection");
    slotCountSignal = registerSignal("SlotCountToTransmit");
    collisionsOfInfectedSig = registerSignal("CollisionsOfInfectedNode");
    collisionsOfNotInfectedSig = registerSignal("CollisionsOfNotInfectedNode");
    collisionDetection = registerSignal("detectionOfCollision");
    quantileSignal = registerSignal("Quantile");

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
        messageToRetransmit->setHopCount(0);
        messageToRetransmit->setSlotTimeCount(0);
        broadcastMessage();
    }
}

/*
 * This method handles the arrived messages
 */
void Node::handleMessage(cMessage *msg)
{

    if(!infected){
            // a selfMessage points that a slot time of transmission is finished
            if(msg->isSelfMessage()){

                   // if in a slot time only one message has been received, there were no collisions
                   if(messageCounter == 1){
                       infected = true;
                       bubble("INFECTED");

                       // Emission of the signals
                       messageToRetransmit->setSlotTimeCount(messageToRetransmit->getSlotTimeCount()+1);
                       emit(slotCountSignal, messageToRetransmit->getSlotTimeCount());


                       messageToRetransmit->setHopCount(messageToRetransmit->getHopCount()+1);
                       emit(hopCountSignal, messageToRetransmit->getHopCount());


                       emit(collisionsOfInfectedSig, numberOfCollision);

                       //Increment the generic counter of infected nodes
                       countInfectedNode = (getParentModule()->par("counterInfectedNodes").intValue());
                       ++countInfectedNode;
                       getParentModule()->par("counterInfectedNodes") = countInfectedNode;


                       slotOfInfection = messageToRetransmit->getSlotTimeCount();


                       //change icon of the simulation to show the infection
                       if(hasGUI()){
                           cDisplayString& displayString = getDisplayString();
                           displayString.setTagArg("i", 0, "block/circle");
                       }


                       // try to transmit
                       tryToSend();

                   }else{// otherwise collision: other messages arrived in a slotTime
                       bubble("COLLISION");
                       emit(collisionDetection, 1);

                       //reset counter
                       messageCounter = 0;
                       numberOfCollision++;

                       //delete the stored message
                       delete messageToRetransmit;
                       messageToRetransmit = nullptr;
                   }
           }else{// New message is arrived

                   // count messages arrived
                   messageCounter++;
                   if(messageCounter == 1){ // first attempt

                       // storing of the message
                       messageToRetransmit = check_and_cast<epidemicMessage*>(msg)->dup();

                       // The transmission time is a slot so a selfmessage will be sent when the transmission ends
                       scheduleAt(simTime() + slotTime, new cMessage("Reception finished"));
                   }
           }
    }else if(infected && !transmitted && msg->isSelfMessage()){// this node is infected and tries to broadcast infection
            tryToSend();                                           // when a node is infected, it must listen to self-messages only
    }

    // delete anyway the message: if a new one, 'cause has been stored, if is collision one or if is a selfMessage one
    delete msg;

}

/*
 * This method tries to transmit the stored message with a certain probability
 * fixed by the floor
 */
void Node::tryToSend(){
    // extracts a 0 or a 1 with a Bernoullian probability set
    int check = bernoulli(retransmissionProbability);

    if(check){// broadcast message

        transmitted = true;
        bubble("TRANSMITTED");

        // broadcast the message
        broadcastMessage();
    }else{
        EV<< "Try to transmit next time"<<endl;
        messageToRetransmit->setSlotTimeCount(messageToRetransmit->getSlotTimeCount()+1);

        // Postpone another try to the next slot
        scheduleAt(simTime() + slotTime, new cMessage("Retry to send"));
    }


}

/*
 * This method broadcasts a message to all output gates connected.
 *
 */
void Node::broadcastMessage(){
    int numGateOut = this->gateCount()/2;

    //transmit the message to all output gates
    for(int i= 0; i < numGateOut; i++){
        epidemicMessage* copy = messageToRetransmit->dup();
        send(copy, "gate$o",i);
    }
    delete messageToRetransmit;
    messageToRetransmit = nullptr;
}


/*
 * This method emits the latest signals
 */
void Node::finish(){
    //if this is a not infected node and it had some collisions emit the signal
    if(!infected && numberOfCollision > 0){
        emit(collisionsOfNotInfectedSig, numberOfCollision);
    }

    /*
     * if this is the Xth node among the infected ones, the slot in which it has been infected will be sent
     */
    if(countInfectedNode == (int)(getParentModule()->par("counterInfectedNodes").intValue()*getParentModule()->par("quantileBT").doubleValue()) ){
        emit(quantileSignal, slotOfInfection);
    }
}


/*
 * This method fetchs the critirion of the starter node
 * and sets the position near which the node will be
 *
 */
void Node::selectStarterNode(){

    cModule* floor=getParentModule();

    //extract the critirion from parameters of the floor
    int criterion = floor->par("selectionCriterionStarterNode").intValue();

    //if the critirion is random, the starter node can simply be the node zero
    if(criterion == RANDOM_STARTER_NODE){
        getParentModule()->par("indexOfStartingNode") = 0;
        return;
    }

    switch(criterion){
        case STARTER_NODE_NEAR_CENTER:
            setIndexOfStarterNode(floor->par("sizeX").doubleValue()/2, floor->par("sizeY").doubleValue()/2);
            break;
        case STARTER_NODE_NEAR_VERTEX:
            setIndexOfStarterNode(0, 0);
            break;
    }
}

/*
 * This method receives a point of the space near which the closest node
 * will be set as starter
 *
 * parameters:
 *      double expectedPosX : x point representing the x position of starter node
 *      double expectedPosY : y point representing the y position of starter node
 */
void Node::setIndexOfStarterNode(double expectedPosX, double expectedPosY){

    cModule* net=getParentModule();
    int numberOfNodes=net->par("numberOfNodes").intValue();
    int bestIndex=0;
    double minDistSquare= -1;

    double expectedX = expectedPosX;
    double expectedY = expectedPosY;

    cModule* node;
    double x;
    double y;

    // find the closest node near the point
    for(int i=0; i < numberOfNodes; ++i){
        node = net->getSubmodule("node", i);
        x = node->par("posX");
        y = node->par("posY");


        double distSquare = pow((x-expectedX),2) + pow((y-expectedY),2);

        if(distSquare < minDistSquare || minDistSquare == -1){
            minDistSquare = distSquare;
            bestIndex = i;
        }
    }

    //set the starter node
    net->par("indexOfStartingNode") = bestIndex;
}
