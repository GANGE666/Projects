package P2PServer;

import java.net.DatagramSocket;
import java.net.SocketAddress;
import java.util.ArrayList;
import java.util.Deque;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;


public class ServerMain {
	
	public static final String CHECK_DATA 				= "CHECKDATA";

	public static final int LOGIN_UID 					= 0000;
	public static final int LOGOUT_UID 					= 0001;
	public static final int ASK_CONNECT_UID				= 0002;
	
	public static final int AGENCY						= 0003;
	public static final int SEND_AGENCY					= 0004;
	public static final int RECEIVE_SEND_AGENCY			= 0005;
	
	public static final int SERVER_RECEIVE 				= 1000;
	public static final int CONFIRM_CONNECT				= 1001;
	public static final int RECEIVE_CONFIRM_CONNECT		= 1002;
	
	public static final int SEND_MSG					= 2000;
	public static final int RECEIVE_MSG					= 2001;
	
	private static Deque<String> MsgQueue = new LinkedList<>();
	//public static Deque<SocketAddress> RepeatQueue = new LinkedList<>();
	public static Map<Integer, Boolean> SendReceive = new HashMap<>();
	public static Map<String, String> LOGINTable = new HashMap<>();
	public static Deque<String> DoneList = new LinkedList<String>();
	
	public static final int DoneListMaxNum = 100;
	
    private static DatagramSocket client;
    private static int MyUDPListenPort = 30000;
    
    public static String ServerUID = "123456789";
	
	public static int MsgID = 1000000000;
	
	public static void main(String[] arg){
		try{
			
			System.out.println("Server Begin");
			
			MsgQueue.clear();
			//RepeatQueue.clear();
		
			client = new DatagramSocket(MyUDPListenPort);
			
			ServerReceiveMsg serverReceiveMsg = new ServerReceiveMsg(client);
			serverReceiveMsg.start();
			
			ExeMsg exeMsg = new ExeMsg(client);
			exeMsg.start();
			
			//Not Exit
			exeMsg.join();
		
		}catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public static boolean InsertMsg(String Msg){
		try{
			MsgQueue.offer(Msg);
			//System.out.println("Insert : \t" + Msg + "  " + MsgQueueEmpty());
			return true;
		}
		catch (Exception e) {
			e.printStackTrace();
			return false;
		}
	}
	public static String GetLastMsg(){
		if(!MsgQueue.isEmpty())
			return MsgQueue.getLast();
		else
			return null;
		
	}
	public static String PollMsg(){
		if(!MsgQueue.isEmpty())
			return MsgQueue.poll();
		else
			return null;
	}
	public static boolean MsgQueueEmpty(){
		if(MsgQueue.isEmpty())
			return true;
		else 
			return false;
	}
	
}
