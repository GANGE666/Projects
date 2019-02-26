package P2PClient;

import java.net.DatagramSocket;
import java.net.InetSocketAddress;
import java.net.SocketAddress;

import javax.swing.plaf.synth.SynthStyle;

public class ClientExeMsg extends Thread{
	
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
	
	private DatagramSocket client;
	
	ClientExeMsg(DatagramSocket client) {
		this.client = client;
	}
	
	public void run(){
		System.out.println("ClientExeMsg Begin");
		SocketAddress serverTarget = new InetSocketAddress(ClientMain.ServerIP, ClientMain.ServerPort);
		
		while(true){
			try{
				if(!ClientMain.MsgQueueEmpty()){
					String msg = ClientMain.PollMsg();
					
					//不处理重复指令，维护完成过的指令：DoneList队列
					if(ClientMain.DoneList.contains(msg))
						continue;
					else{
						ClientMain.DoneList.push(msg);
						if(ClientMain.DoneList.size() > ClientMain.DoneListMaxNum)
							ClientMain.DoneList.pop();
					}
					
					System.out.println("Exe : \t" + msg);
					
					String[] element = msg.split("_");
					SocketAddress target = new InetSocketAddress(element[PosFromIP].replace("/", ""), Integer.valueOf(element[PosFromPort]));
					String UID = element[PosFromUID];
					
					switch (Integer.valueOf(element[PosInstrCode])) {
					case ClientMain.SEND_MSG:
						/*
						 * Need to fix
						 */
						//ClientSendMsg clientSendMsg = new ClientSendMsg(client, msg.substring(msg.indexOf(element[PosInstrCode])), target, 1);
						ClientSendMsg clientSendMsg = new ClientSendMsg(client, Integer.toString(ClientMain.RECEIVE_MSG) + "_" + ClientMain.GetMyUID() + "_0_0_0_Receive", target, 0);
						clientSendMsg.start();
						break;
					case ClientMain.SEND_AGENCY:
						ClientSendMsg clientReceiveAgencyMsg = new ClientSendMsg(client, Integer.toString(ClientMain.RECEIVE_SEND_AGENCY) + "_" + ClientMain.GetMyUID() + "_0_0_0_Receive", target, 0);
						clientReceiveAgencyMsg.start();
						break;
					case ClientMain.CONFIRM_CONNECT:
						//回复服务器收到了
						ClientSendMsg clientSendMsg2 = new ClientSendMsg(client, Integer.toString(ClientMain.RECEIVE_CONFIRM_CONNECT) + "_" + ClientMain.GetMyUID(), serverTarget);
						clientSendMsg2.start();
						
						//登记ClientB的ip port
						ClientMain.ClientTable.put(Integer.valueOf(element[PosTargetUID]), element[PosTargetIP] + "_" + element[PosTargetPort]);
						
						System.out.println("Connect : \t" + element[PosTargetUID] + "\t" + element[PosTargetIP] + "\t" + element[PosTargetPort]);
						
						String ToTheOtherClient = Integer.toString(ClientMain.SEND_MSG) + "_" + UID + "_" + "0_0_0_Hello";
						SocketAddress Btarget = new InetSocketAddress(element[PosTargetIP].replace("/", ""), Integer.valueOf(element[PosTargetPort]));
						ClientSendMsg clientSendMsg3 = new ClientSendMsg(client, ToTheOtherClient, Btarget, ClientSendMsg.MustArrive);
						clientSendMsg3.start();
						
						break;
						
					case ClientMain.RECEIVE_MSG:
						ClientMain.SendReceive.put(Integer.valueOf(element[PosMsgID]), true);
						ClientMain.SendReceive.remove(Integer.valueOf(element[PosMsgID]));
						break;
						
					case ClientMain.SERVER_RECEIVE:
						ClientMain.SendReceive.put(Integer.valueOf(element[PosMsgID]), true);
						ClientMain.SendReceive.remove(Integer.valueOf(element[PosMsgID]));
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
	
	
}
