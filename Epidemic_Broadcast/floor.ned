
//        Performance Evaluation Project
//        
//        Epidemic Broadcast
//        
//        Authors:
//        	Gabriele Martino - Maurizio Pulizzi - Antonio Di Tecco
//        	
//	

network Floor
{
    parameters:
        
        //parameter for the number of nodes
        int numberOfNodes = default(50);
        
        //parameter for the radius
        int R = default(100);
        
        //parameter for the first Broadcasting node
        int indexBroadcastNode = default(intuniform(0,numberOfNodes));
        
        //parameters for the positioning
        volatile int rposX = default(intuniform (0,500));
        volatile int rposY = default(intuniform (0,350));
        @display("bgb=500,350;bgu=m");
    submodules:
        //making modules and positioning them
        
        node[numberOfNodes]: Node {
            parameters:
                posX = rposX;
                posY = rposY;
                @display("r=$R");

        }
    connections allowunconnected:
        //making connections according to the constraint of the distance [ (x1-x2)^2 + (y1-y2)^2] <= R^2
        
        
        for i=0..numberOfNodes-1, for j=0..numberOfNodes-1 {
            node[i].outgate++ --> node[j].ingate++ 
            								if i != j && ((node[i].posX - node[j].posX)^2 + (node[i].posY- node[j].posY)^2) <= R^2;
	
        }


}
