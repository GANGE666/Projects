package P2PClient;

import java.net.DatagramSocket;
import java.net.InetSocketAddress;
import java.net.SocketAddress;


public class ClientSendMsg extends Thread {

	public static final int MustArrive = 0;
	public static final int OkYouShutUp = 1;
	
	public static final int SendInterval = 1;
	public static final int SendNum = 100;
	
	private DatagramSocket client;
	private String msg;
	private String UID;
	private SocketAddress target;
	
	//发送模式，必须送达、just tell you
	private int Model = 1;
	
	ClientSendMsg(DatagramSocket client, String msg, String ip, int port, int Model){
		this.client = client;
		this.msg = msg;
		this.target = new InetSocketAddress(ip, port);
		this.Model = Model;
	}
	ClientSendMsg(DatagramSocket client, String msg, String ip, int port){
		this.client = client;
		this.msg = msg;
		this.target = new InetSocketAddress(ip, port);
	}
	ClientSendMsg(DatagramSocket client, String msg, SocketAddress target, int Model){
		this.client = client;
		this.msg = msg;
		this.target = target;
		this.Model = Model;
	}
	ClientSendMsg(DatagramSocket client, String msg, SocketAddress target){
		this.client = client;
		this.msg = msg;
		this.target = target;
	}
	
	public void run(){
		try{
			int MsgID = ++ClientMain.MsgID;
			ClientMain.SendReceive.put(MsgID, false);
			
						
			msg = ClientMain.CHECK_DATA + "_" + Integer.valueOf(MsgID) + "_" + msg;
			
			for(int i = 1; i <= SendNum >> Model; i++){
				if(Model == 0 && ClientMain.SendReceive.containsKey(MsgID) && ClientMain.SendReceive.get(MsgID))
					break;
				UDPTools.SendMsg(client, target, msg);
				sleep(SendInterval);
			}
			if(Model == MustArrive){
				System.out.println("Send : \t" + target.toString() + " \t _____" + msg);
			}
			
		}catch (Exception e) {
			e.printStackTrace();
		}
	}
}
