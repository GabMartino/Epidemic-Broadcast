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
        @statistic[hopCountStat](title="Hop count"; source="Infection"; record = vector, count, mean, max; );
        
        
        @signal[SlotCountToTransmit](type="int");
        @statistic[SlotTimeStat](title="Slot count before receiving message"; source="SlotCountToTransmit"; record =vector, stats; );
        
        @signal[CollisionsOfInfectedNode](type="int");
        @statistic[collisionOfInfectedStat](title= "Number of Collisions of infected node"; source= "CollisionsOfInfectedNode"; record = vector,stats;);
        
        @signal[CollisionsOfNotInfectedNode](type="int");
        @statistic[collisionOfNotInfectedStat](title= "Number of Collisions of NOT infected node"; source= "CollisionsOfNotInfectedNode"; record = vector,stats;);
        
        @signal[detectionOfCollision](type="int");
      	@statistic[detectionCollisionStat](title= "Number of Collisions for each slot"; source= count(detectionOfCollision); record = vector,stats;);
        
        //Selection criterion of the starter node
        int selectionCriterionStarterNode;
        
        //Dimension of the floor
        int sizeX;
        int sizeY;
        @display("bgb=sizeX,sizeY;bgu=m");

        //Radius
        double radius;

        //Number of nodes
        int numberOfNodes;

        //Retransmission Probability
        double P;

        //Distribution of Nodes;
        volatile double PosX;
        volatile double PosY;

        //Index of Starting node
        int indexOfStartingNode =default(0);
        
        //SlotTIme
        double slotTime @unit(ms);
    
	submodules:
        node[numberOfNodes]: Node {
            parameters:
    			posX = PosX % sizeX;
    			posY = PosY % sizeY;
                @display("p=$posX,$posY");
                //@display("r=$radius");
        }
		
	connections:
        // set all connection inside a radius
        for i = 0..numberOfNodes-2, for j= i+1..numberOfNodes-1{
            node[i].gate++ <--> node[j].gate++ if ((node[i].posX - node[j].posX)^2 + (node[i].posY - node[j].posY)^2) <= radius^2; 
            
        }
        	
}
