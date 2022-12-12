package networksproject.simulations;

import networksproject.Coordinator;
import networksproject.Node0;


@license(LGPL);
//
// TODO documentation
//
network Network
{
	int N = 2;
	
	submodules:
        coordinator: Coordinator {
            @display("p=291,189");
        }
        nodes[N]: Node0;


	

	
    connections:
        nodes[1].out --> nodes[0].in;
        nodes[0].out --> nodes[1].in;
        coordinator.out1 --> nodes[1].inC;
        coordinator.out0 --> nodes[0].inC;
}

