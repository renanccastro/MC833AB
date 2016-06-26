package projects.PIF.nodes.nodeImplementations;
import java.awt.Color;
import java.awt.Graphics;
import java.util.ArrayList;

import projects.PIF.nodes.messages.FEEDBACKMessage;
import projects.PIF.nodes.messages.INFMessage;
import projects.PIF.nodes.timers.PIF_FeedbackTimer;
import projects.PIF.nodes.timers.MessageTimer;
import projects.PIF.nodes.timers.ResetTimer;
import sinalgo.configuration.WrongConfigurationException;
import sinalgo.gui.transformation.PositionTransformation;
import sinalgo.nodes.Node;
import sinalgo.nodes.messages.Inbox;
import sinalgo.nodes.messages.Message;
import sinalgo.runtime.Global;
import sinalgo.tools.Tools;

public class PIFNode extends Node {
	private boolean reached = false;
	private boolean leaf = false;
	private int nextHopToSource;
	public static int messages = 0;
	public static int sentINF = 0;
	public static int receivedINF = 0;
	public static int sentFeedback = 0;
	public static int receivedFeedback = 0;
	public ArrayList<Integer> ids = new ArrayList<Integer>();
	public boolean has_sent_feedback = false;
	double time;
	double timeSent;
	
	/// Constantes
	public static int timeToReset = 4000; 
	public static int timeToResend = 5000;
	public static int timeToWaitFeedback = 20000;

	int numerador = 500;
	int inf_count = 0;
	MessageTimer infMSG = null;	

	public enum TNO {TNO_FEEDBACK, TNO_FIRST_FEEDBACK };
	private PIF_FeedbackTimer feedbackTimer;
	private ResetTimer resetTimer;
	
	@Override
	public void handleMessages(Inbox inbox) {
		// TODO Auto-generated method stub
		int sender;
		
		while(inbox.hasNext()) {
			Message msg = inbox.next();
			sender = inbox.getSender().ID;
			
			//N� recebeu uma mensagem INF	
			if(msg instanceof INFMessage) {
				inf_count++;
				//System.out.println("Node: "+this.ID+" recebeu INF do Node  "+sender);
				INFMessage msgINF = (INFMessage) msg;
				
				if(!this.reached)
				{
					time = Global.currentTime;
					receivedINF++;
					this.setColor(Color.GREEN);
					this.reached = true;
					this.nextHopToSource = msgINF.getSenderID();
					msgINF.setSenderID(this.ID);
					infMSG = new MessageTimer(msgINF);
					infMSG.startRelative(1,this);
					resetTimer = new ResetTimer(this);
					resetTimer.startRelative(timeToReset, this);
					
					//Agenda o FEEDBACK
					ids.add(this.ID);
					feedbackTimer = new PIF_FeedbackTimer(this, TNO.TNO_FIRST_FEEDBACK);
					feedbackTimer.tnoStartRelative(1.1, this, TNO.TNO_FIRST_FEEDBACK);	
				}
				
				if (inf_count > 2 && infMSG != null) {
					infMSG.canSendInf(false);
					leaf = true;
				}

			}
			
			//Mensagem de Confirma��o
			if(msg instanceof FEEDBACKMessage) {
				
				FEEDBACKMessage msgFeedback = (FEEDBACKMessage) msg;
			
				if (this.ID != 1){
								
					if (msgFeedback.getDestinationID() == this.ID) {

						if (has_sent_feedback) {
							ids.clear();
							has_sent_feedback = false;
							feedbackTimer = new PIF_FeedbackTimer(this, TNO.TNO_FEEDBACK);
							feedbackTimer.tnoStartRelative((float)timeToWaitFeedback/(time-timeSent), this, TNO.TNO_FEEDBACK);
							//System.out.println("----- criando nova mensagem de feedback ----- ");
						}
						
						ids.addAll(msgFeedback.ids);
						//System.out.println("Node: " + this.ID + " Recebeu Feedback do Node "+ msgFeedback.getSourceFeedbackID());	

					}
						
				} else { 
					//System.out.println("Source node recebeu Feedback do Node "+ msgFeedback.getSourceFeedbackID());
					//System.out.println("List size: " + msgFeedback.ids.size() );
					receivedFeedback += msgFeedback.ids.size();
				}
			}
		}
	}
			
		
	public void feedbackStart(){
		has_sent_feedback= true;
		//System.out.println("List size de " + this.ID + " quando vai mandar: " + ids.size() );
		MessageTimer feedbackMSG = new MessageTimer (new FEEDBACKMessage(this.ID, this.nextHopToSource, this.ids));
		feedbackMSG.startRelative(0.1, this);
	}
	
	public void timeout(TNO tno){
		switch(tno){
			case TNO_FEEDBACK:
				feedbackStart();
				break;
			case TNO_FIRST_FEEDBACK:
				if (leaf)
					feedbackStart();
				else
					feedbackTimer.tnoStartRelative((float)numerador/time, this, TNO.TNO_FEEDBACK);
		}
	}

    @Override
	public void init() {
 
		//Considerando que o n� 1 tem a mensagem inf
		if (this.ID==1){
			int i;
			for (i = 1; i <= 1;i++) {
				timeSent = Global.currentTime;
				messages++;
				this.setColor(Color.RED);
				this.nextHopToSource = this.ID;
				this.reached = true;
				
				MessageTimer infMSG = new MessageTimer (new INFMessage(this.ID, timeSent));
		  		infMSG.startRelative(timeToResend*i+0.1, this);
		  		
			}
		}
	}
    
    public void reset() {
    	this.setColor(Color.BLACK);
    	this.has_sent_feedback = false;
    	this.ids.clear();
    	this.inf_count = 0;
		this.reached = false;
		
    }
	@Override
	public void postStep() {
		// TODO Auto-generated method stub
	}

	@Override
	public void preStep() {
		// TODO Auto-generated method stub
	}
	
	@Override
	public void draw(Graphics g, PositionTransformation pt, boolean highlight) {
		// TODO Auto-generated method stub
		//if (this.ID == 1) highlight = true;
		super.drawNodeAsDiskWithText(g, pt, highlight, Integer.toString(this.ID), 6, Color.WHITE);
		
		
	}
	
	@Override
	public void neighborhoodChange() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void checkRequirements() throws WrongConfigurationException {
		// TODO Auto-generated method stub
		
	}

}
