package networksproject.simulations;

import networksproject.Coordinator;
import networksproject.Node0;


@license(LGPL);
//
// TODO documentation
//
network Network
{
    @display("bgb=709,295");
    submodules:
        node0: Node0 {
            @display("p=45,65");
        }
        coordinator: Coordinator {
            @display("p=291,189");
        }
        node1: Node0 {
            @display("p=561,63");
        }
    connections:
        node1.out --> node0.in;
        node0.out --> node1.in;
        coordinator.out1 --> node1.inC;
        coordinator.out0 --> node0.inC;
}

