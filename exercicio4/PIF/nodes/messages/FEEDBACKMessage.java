package projects.PIF.nodes.messages;

import java.util.ArrayList;

import sinalgo.nodes.messages.Message;

public class FEEDBACKMessage extends Message {
	public int destinationID;
	public int sourceFeedbackID;
	public ArrayList<Integer> ids;

	public FEEDBACKMessage(int sourceFeedbackID, int destinationID, ArrayList<Integer> ids) {
		this.destinationID = destinationID;
		this.sourceFeedbackID = sourceFeedbackID;
		this.ids = new ArrayList<Integer>();
		this.ids.addAll(ids);
	}

	@Override
	public Message clone() {
		// TODO Auto-generated method stub
		return new FEEDBACKMessage(this.sourceFeedbackID, this.destinationID, this.ids);
	}

	public int getSourceFeedbackID() {
		return sourceFeedbackID;
	}

	public void setSourceFeedbackID(int sourceFeedbackID) {
		this.sourceFeedbackID = sourceFeedbackID;
	}

	public int getDestinationID() {
		return destinationID;
	}

	public void setDestinationID(int destinationID) {
		this.destinationID = destinationID;
	}
	
}
