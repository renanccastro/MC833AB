package projects.PIF.nodes.timers;

import projects.PIF.nodes.nodeImplementations.PIFNode;
import sinalgo.nodes.Node;
import sinalgo.nodes.timers.Timer;

public class ResetTimer extends Timer {
	PIFNode n;
	
	public ResetTimer (PIFNode n){
		this.n = n;
	}

	@Override
	public void fire() {
		// TODO Auto-generated method stub
		n.reset();
	}

}




	
	
	


