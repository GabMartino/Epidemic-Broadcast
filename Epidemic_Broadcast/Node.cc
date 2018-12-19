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
    if(this->getIndex() == 0){
        selectStarterNode();
    }

    // Take the slotTime and retransmissionProbability parameters from the network parameters
    slotTime = getParentModule()->par("slotTime").doubleValue();
    retransmissionProbability = getParentModule()->par("P").doubleValue();

    // Registration of signals
    hopCountSignal = registerSignal("Infection");
    slotCountSignal = registerSignal("SlotCountToTransmit");
    collisionsOfInfectedSig = registerSignal("CollisionsOfInfectedNode");
    collisionsOfNOTinfectedSig = registerSignal("CollisionsOfNotInfectedNode");
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

                       // Emission of the signals
                       messageToRetransmit->setSlotTimeCount(messageToRetransmit->getSlotTimeCount()+1);
                       emit(slotCountSignal, messageToRetransmit->getSlotTimeCount());
                       messageToRetransmit->setHopCount(messageToRetransmit->getHopCount()+1);
                       emit(hopCountSignal, messageToRetransmit->getHopCount());
                       emit(collisionsOfInfectedSig, numberOfCollision);

                       numbrerInfNode= (getParentModule()->par("howManyNod").intValue());
                       ++numbrerInfNode;
                       getParentModule()->par("howManyNod")=numbrerInfNode;
                       EV<<numbrerInfNode<<"  NUMEROOOOOOOOOOOOO NODO";
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

                       // The transmission time is a slot so a selfmessage is sent when the transmission is finished
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
 * This method takes a message and tries to transmit with a certain probability
 * otherwise postpones to the next slot another try
 */
void Node::tryToSend(){
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
    /*
     * Given that we used inout gates and we have duplicated gate.
     * We need only out gate so we divided by 2 the number
     * SEE IF THERE'S A METHOD THAT RETURNS ONLY THE OUTPUT GATE, OR USE AN ITERATOR
     *
     */
    int numGateOut = this->gateCount()/2;

    for(int i= 0; i < numGateOut; i++){
        epidemicMessage* copy = messageToRetransmit->dup();
        send(copy, "gate$o",i);
    }
    delete messageToRetransmit;
    messageToRetransmit = nullptr;
}

void Node::finish(){
    if(!infected && numberOfCollision > 0){
        emit(collisionsOfNOTinfectedSig, numberOfCollision);
    }
    if(numbrerInfNode== (int)(getParentModule()->par("howManyNod").intValue()*getParentModule()->par("quantileBT").doubleValue()) ){
        emit(quantileSignal, slotOfInfection);
    }
}

void Node::selectStarterNode(){
    int criterio = getParentModule()->par("selectionCriterionStarterNode").intValue();
    double expectedPosX;
    double expectedPosY;
    switch(criterio){
        case RANDOM_STARTER_NODE:
            getParentModule()->par("indexOfStartingNode") = 0;
            break;
        case STARTER_NODE_NEAR_CENTER:
            expectedPosX=getParentModule()->par("sizeX").intValue()/2;
            expectedPosY=getParentModule()->par("sizeY").intValue()/2;
            setIndexOfStarterNode(expectedPosX, expectedPosY, false);
            break;
        case STARTER_NODE_NEAR_VERTEX:
            expectedPosX=getParentModule()->par("sizeX").intValue();
            expectedPosY=getParentModule()->par("sizeY").intValue();
            setIndexOfStarterNode(expectedPosX, expectedPosY, true);
            break;
        case STARTER_NODE_NEAR_TOP_LEFT:
            setIndexOfStarterNode(0, 0, false);
            break;

    }
}

void Node::setIndexOfStarterNode(double expectedPosX, double expectedPosY, bool anyVertex){
    /*  Se anyVertex==false questa funzione imposta come starter node il nodo più vicino al
     * punto (expectedPosX, expectedPosX).
     * Se invece anyVertex==true allora expectedPosX e expectedPosY vengono intese come le
     * dimenzioni del quadrilatero (stanza) e la funzione imposta come starter node il nodo
     * più vicino ad uno dei quattro vertici del quadrilatero.
     */

    cModule* net=getParentModule();
    int numberOfNodes=net->par("numberOfNodes").intValue();
    int bestIndex=0;
    double minDistSquare= -1;

    double expectedX = expectedPosX;
    double expectedY = expectedPosY;


    for(int i=0; i < numberOfNodes; ++i){
        cModule* nod= net->getSubmodule("node", i);
        double x = nod->par("posX");
        double y = nod->par("posY");

        if(anyVertex){
            expectedX = (expectedPosX/2 > x)? 0 : expectedPosX;
            expectedY = (expectedPosY/2 > y)? 0 : expectedPosY;
        }

        double distSquare = (x-expectedX)*(x-expectedX) + (y-expectedY)*(y-expectedY);

        if(distSquare < minDistSquare || minDistSquare == -1){
            minDistSquare = distSquare;
            bestIndex = i;
        }
    }

    net->par("indexOfStartingNode") = bestIndex;
}
