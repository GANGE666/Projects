package P2PServer;

import java.net.DatagramSocket;
import java.net.InetSocketAddress;
import java.net.SocketAddress;

public class ExeMsg extends Thread {
	
	//Message Form:
	public static final int PosFromIP = 0;
	public static final int PosFromPort = 1;
	public static final int PosCheckData = 2;
	public static final int PosMsgID = 3;
	public static final int PosInstrCode = 4;
	public static final int PosFromUID = 5;
	public static final int PosTargetUID = 6;
	public static final int PosTargetIP = 7;
	public static final int PosTargetPort = 8;
	public static final int PosText = 9;
	
	public static final int MustArrive = 0;
	public static final int OkYouShutUp = 1;
	private DatagramSocket client;
	
	ExeMsg(DatagramSocket client){
		this.client = client;
	}
	
	public void run(){
		System.out.println("ExeMsg Begin");
		while(true)
			try{
				if(!ServerMain.MsgQueueEmpty()){
					String msg = ServerMain.PollMsg();
					
					//不处理重复指令，维护完成过的指令：DoneList队列
					if(ServerMain.DoneList.contains(msg))
						continue;
					else{
						ServerMain.DoneList.push(msg);
						if(ServerMain.DoneList.size() > ServerMain.DoneListMaxNum)
							ServerMain.DoneList.pop();
					}
					
					System.out.println("Exe  : \t" + msg);
					
					
					
					String[] element = msg.split("_");
					SocketAddress target = new InetSocketAddress(element[PosFromIP].replace("/", ""), Integer.valueOf(element[PosFromPort]));
					String UID = element[PosFromUID];
					int MsgID = Integer.valueOf(element[PosMsgID]);
					
					
					
					switch (Integer.valueOf(element[PosInstrCode])) {
					case ServerMain.LOGIN_UID:
						ServerMain.LOGINTable.put(UID, element[PosFromIP] + "_" + element[PosFromPort]);
						
						//System.out.println("Put : \t" + UID + element[PosFromIP] + element[PosFromPort]);
						
//						ServerSendMsg LOGIN_UID_serverSendMsg = new ServerSendMsg(client, Integer.toString(ServerMain.SERVER_RECEIVE) + "_" + ServerMain.ServerUID, target, OkYouShutUp);
//						LOGIN_UID_serverSendMsg.start();
						break;
						
					case ServerMain.LOGOUT_UID:
						ServerMain.LOGINTable.remove(UID, element[PosFromIP] + "_" + element[PosFromPort]);
						
						ServerSendMsg LOGOUT_UID_serverSendMsg = new ServerSendMsg(client, Integer.toString(ServerMain.SERVER_RECEIVE) + "_" + ServerMain.ServerUID, target, OkYouShutUp);
						LOGOUT_UID_serverSendMsg.start();
						break;
						
					case ServerMain.ASK_CONNECT_UID:
						String B_IP_Port = ServerMain.LOGINTable.get(element[PosTargetUID]);
												
						String ClientAmsg = Integer.toString(ServerMain.CONFIRM_CONNECT) + "_" + ServerMain.ServerUID + "_" + element[PosTargetUID] + "_" + B_IP_Port;
						String ClientBmsg = Integer.toString(ServerMain.CONFIRM_CONNECT) + "_" + ServerMain.ServerUID + "_" + element[PosFromUID] + "_" + element[PosFromIP] + "_" + Integer.valueOf(element[PosFromPort]);
						
						System.out.println("ASK_Connect : \t" + B_IP_Port);
						
						ServerSendMsg ASK_CONNECT_UID_serverSendMsgA = new ServerSendMsg(client, ClientAmsg, element[PosFromIP].replace("/", ""), Integer.valueOf(element[PosFromPort]), MustArrive);
						ServerSendMsg ASK_CONNECT_UID_serverSendMsgB = new ServerSendMsg(client, ClientBmsg, B_IP_Port.split("_")[PosFromIP].replace("/", ""), Integer.valueOf(B_IP_Port.split("_")[PosFromPort]), MustArrive);
						
						ASK_CONNECT_UID_serverSendMsgA.start();
						ASK_CONNECT_UID_serverSendMsgB.start();
						
						//ServerMain.SendReceive.put(MsgID, false);
						break;
						
					case ServerMain.RECEIVE_CONFIRM_CONNECT:
						System.out.println("Received : \t" + UID);
						ServerMain.SendReceive.put(MsgID, true);
						ServerMain.SendReceive.remove(MsgID);
						//BUG!
						break;

					case ServerMain.AGENCY:
						System.out.println("Agency : \t" + element[PosText]);
						String Text = element[PosText];
						String TargetUID = element[PosTargetUID];
						
						String TargetIP_Port = ServerMain.LOGINTable.get(TargetUID);
						String AgencyMsg = Integer.toString(ServerMain.SEND_AGENCY) + "_" + ServerMain.ServerUID + "_" + TargetUID + "_0_0_" + Text;
						
						ServerSendMsg Agency = new ServerSendMsg(client, AgencyMsg, TargetIP_Port.split("_")[PosFromIP].replace("/", ""), Integer.valueOf(TargetIP_Port.split("_")[PosFromPort]), MustArrive);
						Agency.start();
						
						break;
					case ServerMain.RECEIVE_SEND_AGENCY:
						System.out.println("RECEIVE_SEND_AGENCY : \t" + UID);
						break;
					default:
						System.out.println("Function Code wrong!");
						break;
					}
				}
				else
					sleep(10);
			}catch (Exception e) {
				e.printStackTrace();
			}
	}
}
