package P2PClient;

import java.net.InetSocketAddress;

public class UserAddInstr {
	
	public static boolean SendMessage(int UID, int TargetUID, String message){
		if(ClientMain.ClientTable.containsKey(TargetUID)){
			String string = Integer.toString(ClientMain.SEND_MSG) + "_" + Integer.toString(UID) + "_" + Integer.toString(TargetUID) + "_0_0_" + message;
			//ClientMain.InsertMsg(string);
			
			String ip = ClientMain.ClientTable.get(TargetUID).split("_")[0].replace("/", "");
			int port = Integer.valueOf(ClientMain.ClientTable.get(TargetUID).split("_")[1]);
			
			ClientSendMsg clientSendMsg = new ClientSendMsg(ClientMain.GetClient(), string, new InetSocketAddress(ip, port), 1);
			clientSendMsg.start();
			return true;
		}
		
		return false;
	}

	public static void AskAgency(int UID, int TargetUID, String message){
		String string = Integer.toString(ClientMain.AGENCY) + "_" + Integer.toString(UID) + "_" + Integer.toString(TargetUID) + "_0_0_" + message;
		ClientSendMsg clientSendMsg = new ClientSendMsg(ClientMain.GetClient(), string, new InetSocketAddress(ClientMain.ServerIP, ClientMain.ServerPort));
		clientSendMsg.start();
	}
	
	public static void AskConnect(int UID, int TargetUID){
		String string = Integer.toString(ClientMain.ASK_CONNECT_UID) + "_"  + Integer.toString(UID) + "_" + Integer.toString(TargetUID);
		ClientSendMsg clientSendMsg = new ClientSendMsg(ClientMain.GetClient(), string, new InetSocketAddress(ClientMain.ServerIP, ClientMain.ServerPort));
		clientSendMsg.start();
	}
	
	public static void Login(int UID){
		String string = Integer.toString(ClientMain.LOGIN_UID) + "_" + Integer.toString(UID);
		ClientSendMsg clientSendMsg = new ClientSendMsg(ClientMain.GetClient(), string, new InetSocketAddress(ClientMain.ServerIP, ClientMain.ServerPort));
		clientSendMsg.start();
	}
	
	public static void Logout(int UID){
		String string = Integer.toString(ClientMain.LOGOUT_UID) + "_" + Integer.toString(UID);
		ClientSendMsg clientSendMsg = new ClientSendMsg(ClientMain.GetClient(), string, new InetSocketAddress(ClientMain.ServerIP, ClientMain.ServerPort));
		clientSendMsg.start();
	}
	
}
