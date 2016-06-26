package projects.PIF.nodes.messages;

import sinalgo.nodes.messages.Message;

public class INFMessage extends Message {
	public int senderID;
	public double time;
	
	public INFMessage(int senderID, double time) {
		this.senderID = senderID;
		this.time = time;
	}

	@Override
	public Message clone() {
		// TODO Auto-generated method stub
		return new INFMessage(this.senderID, this.time);
	}
	
	public int getSenderID() {
		return senderID;
	}

	public void setSenderID(int senderID) {
		this.senderID = senderID;
	}
	
}
