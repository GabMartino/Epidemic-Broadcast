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
        //Dimension of the floor
        double sizeX;
        double sizeY;
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
        int indexOfStartingNode = 0;
        
        //SlotTIme
        double slotTime @unit(ms);
    submodules:
        node[numberOfNodes]: Node {
            parameters:
    			posX = PosX;
    			posY = PosY;
                @display("p=$posX,$posY");
                @display("r=$radius");
        }
		

    connections:
        
        // set all connection inside a radius
        for i = 0..numberOfNodes-2, for j= i+1..numberOfNodes-1{
            node[i].gate++ <--> node[j].gate++ if ((node[i].posX - node[j].posX)^2 + (node[i].posY - node[j].posY)^2) <= radius^2; 
            
        }
        	
}
