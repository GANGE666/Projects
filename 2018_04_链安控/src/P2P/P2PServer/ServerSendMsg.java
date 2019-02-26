package P2PServer;

import java.net.DatagramSocket;
import java.net.InetSocketAddress;
import java.net.SocketAddress;

public class ServerSendMsg extends Thread {
	public static final int MustArrive = 0;
	public static final int OkYouShutUp = 1;
	
	//控制小车：数量为1，间隔30ms
	//下面循环也做了更改
	public static final int SendInterval = 30;
	public static final int SendNum = 10;
	
	private DatagramSocket client;
	private String msg;
	private String UID;
	private SocketAddress target;
	
	//发送模式，必须送达、just tell you
	private int Model = 1;
	
	ServerSendMsg(DatagramSocket client, String msg, String ip, int port, int Model){
		this.client = client;
		this.msg = msg;
		this.target = new InetSocketAddress(ip, port);
		this.Model = Model;
	}
	ServerSendMsg(DatagramSocket client, String msg, String ip, int port){
		this.client = client;
		this.msg = msg;
		this.target = new InetSocketAddress(ip, port);
	}
	ServerSendMsg(DatagramSocket client, String msg, SocketAddress target, int Model){
		this.client = client;
		this.msg = msg;
		this.target = target;
		this.Model = Model;
	}
	ServerSendMsg(DatagramSocket client, String msg, SocketAddress target){
		this.client = client;
		this.msg = msg;
		this.target = target;
	}
	
	public void run(){
		try{
			int MsgID = ++ServerMain.MsgID;
			ServerMain.SendReceive.put(MsgID, false);
			
			//if(msg.contains(Integer.toString(ServerMain.CONFIRM_CONNECT)))
			//	System.out.println(msg + "     \t" + (SendNum >> Model));
			
			msg = ServerMain.CHECK_DATA + "_" + Integer.valueOf(MsgID) + "_" + msg;
			
			//for(int i = 1; i <= (SendNum >> Model); i++){
			//小车：发送数量为1
			for(int i = 1; i <= 1; i++){
				if(Model == 0 && ServerMain.SendReceive.containsKey(MsgID) && ServerMain.SendReceive.get(MsgID))
					break;
				UDPTools.SendMsg(client, target, msg);
				sleep(SendInterval);
			}
			
		}catch (Exception e) {
			e.printStackTrace();
		}
	}
}
