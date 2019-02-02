//
// 	Performance Evaluation Project
//
//	Epidemic Broadcast
// 
//   	Authors:
//
//   	Di Tecco Antonio - Martino Gabriele - Pulizzi Maurizio

network Floor
{
    parameters:
        @signal[Infection](type="int");
        @statistic[hopCountStat](title="Hop count"; source = Infection; record = count, mean, max, histogram );

        @signal[Quantile](type="int");
        @statistic[Quantile](title="Quantile of broadcast time"; source = Quantile; record = max );

        @signal[SlotCountToTransmit](type="int");
        @statistic[SlotTimeStat](title="Slot count before receiving message"; source= SlotCountToTransmit; record = histogram, max, mean, count; );

        @signal[CollisionsOfInfectedNode](type="int");
        @statistic[collisionOfInfectedStat](title= "Number of Collisions of infected nodes"; source= CollisionsOfInfectedNode; record = sum, count, mean, max;);

        @signal[CollisionsOfNotInfectedNode](type="int");
        @statistic[collisionOfNotInfectedStat](title= "Number of Collisions of not infected nodes"; source = CollisionsOfNotInfectedNode; record = sum, count, mean, max;);


        //Selection criterion of the starter node
        int selectionCriterionStarterNode;

        //Dimension of the floor
        int sizeX;
        int sizeY;
        @display("bgb=sizeX,sizeY;bgu=m");

        //Radius
        double radius;


        //quantile broadcast tyme
        double quantileBT=default(0.9);

        //index of how many nodes have been received the message
        int counterInfectedNodes=0;

        //Retransmission Probability
        double P;


        //Index of Starting node
        int indexOfStartingNode =default(0);

        //SlotTIme
        double slotTime @unit(ms);


    	//Number of nodes
        int numberOfNodes;

        //Distribution of Nodes;
        volatile double PosX;
        volatile double PosY;

	submodules:
        node[numberOfNodes]: Node {
            parameters:
    			posX = PosX % sizeX;
    			posY = PosY % sizeY;
                @display("p=$posX,$posY");
                @display("r=$radius");
        }

	connections:
        // set all connection inside a radius
        for i = 0..numberOfNodes-2, for j= i+1..numberOfNodes-1{
            node[i].gate++ <--> node[j].gate++ if ((node[i].posX - node[j].posX)^2 + (node[i].posY - node[j].posY)^2) <= radius^2;

        }

}
