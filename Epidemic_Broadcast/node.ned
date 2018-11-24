
//        Performance Evaluation Project
//        
//        Epidemic Broadcast
//        
//        Authors:
//        	Gabriele Martino - Maurizio Pulizzi - Antonio Di Tecco
//        	
//



simple Node
{
    parameters:
        // Coordinates used for positioning
        int posX;
        int posY;
        
        //used to specifies who is the Broadcaster
        bool iAmTheBroadcaster = default(false);
        
        //Displaying
        @display("i=block/routing");
        @display("p=$posX,$posY");
    gates:
        input ingate[];
        output outgate[];

}
