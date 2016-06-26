/*
 Copyright (c) 2007, Distributed Computing Group (DCG)
                    ETH Zurich
                    Switzerland
                    dcg.ethz.ch

 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

 - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the
   distribution.

 - Neither the name 'Sinalgo' nor the names of its contributors may be
   used to endorse or promote products derived from this software
   without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
package projects.PIF.nodes.timers;

import sinalgo.nodes.Node;
import sinalgo.nodes.messages.Message;
import sinalgo.nodes.timers.Timer;
import sinalgo.runtime.Global;
import sinalgo.tools.Tools;

import java.awt.Color;

import projects.PIF.nodes.messages.FEEDBACKMessage;
import projects.PIF.nodes.messages.INFMessage;
import projects.PIF.nodes.nodeImplementations.PIFNode;

/**
 * A timer that sends a message at a given time.
 * The message may be unicast to a specific node or broadcast. 
 */
public class MessageTimer extends Timer {
	
	private boolean send_inf;
	private Node receiver; // the receiver of the message, null if the message should be broadcast
	private Message msg; // the message to be sent
	private double time;
	
	/**
	 * Creates a new MessageTimer object that unicasts a message to a given receiver when the timer fires.
	 * 
	 * Nothing happens when the message cannot be sent at the time when the timer fires.
	 *
	 * @param msg The message to be sent when this timer fires.
	 * @param receiver The receiver of the message.
	 */
	public MessageTimer(Message msg, Node receiver) {
		this.msg = msg;
		this.receiver = receiver;
		this.time = -1; // indicates aperiodicity 
		this.send_inf = true;
	}
	
	/**
	 * Creates a MessageTimer object that broadcasts a message when the timer fires.
	 *
	 * @param msg The message to be sent when this timer fires.
	 */
	public MessageTimer(Message msg) {
		this.msg = msg;
		this.time = -1; // indicates aperiodicity 
		this.receiver = null; // indicates broadcasting
		this.send_inf = true;
	}
	/**
	 * Creates a MessageTimer object that periodically broadcasts a message when the timer fires.
	 *
	 * @param msg The message to be sent when this timer fires.
	 * @param time The data rate 
	 */
	public MessageTimer(Message msg, double time) {
		this.msg = msg;
		this.time = time;
		this.receiver = null; // indicates broadcasting
		this.send_inf = true;
	}
		
	public void canSendInf (boolean send) {
		this.send_inf = send;
	}	

	@Override
	public void fire() {
		if(receiver != null) { // there's a receiver => unicast the message
			this.node.send(msg, receiver);
		} else { // there's no reciever => broadcast the message
			this.node.broadcast(msg);
					
			if(msg instanceof INFMessage && send_inf){ 
//				System.out.println("Node: "+ this.node.ID +" broadcast INF ");
				PIFNode.sentINF = PIFNode.sentINF + 1;
				//System.out.println("Enviadas: "+ PIFNode.SentINF); 
			}
			
			if(msg instanceof FEEDBACKMessage){
				PIFNode.sentFeedback = PIFNode.sentFeedback + 1;
				//System.out.println("Feedback Enviadas: "+ PIFNode.SentFeedback);
//				if ( this.node.ID == ((FEEDBACKMessage) msg).getSourceFeedbackID() ){
//					System.out.println("Node: "+ this.node.ID +" broadcast FEEDBACK ");
					this.node.setColor(Color.MAGENTA);
//				}
//				else
//					System.out.println("Node: "+ this.node.ID +" encaminhou FEEDBACK do Node "+((FEEDBACKMessage) msg).getSourceFeedbackID()+" para o Node "+ ((FEEDBACKMessage) msg).getDestinationID());
			}
				
		}
	}

}
