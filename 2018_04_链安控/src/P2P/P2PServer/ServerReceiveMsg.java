package P2PServer;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;

public class ServerReceiveMsg extends Thread {

	private DatagramSocket client;
	
	ServerReceiveMsg(DatagramSocket client){
		this.client = client;
	}
	
	public void run() {
		while(true)
			try{
				//��������
				byte[] buf = new byte[1024];
				DatagramPacket recpack = new DatagramPacket(buf, buf.length);
				client.receive(recpack);
				String receiveMessage = new String(recpack.getData(), 0, recpack.getLength()); 		
    		
				//System.out.println("Receive Msg : \t" + receiveMessage);
				
				//����ظ�����
				//if(! receiveMessage.contains("_" + Integer.toString(ServerMain.RECEIVE_CONFIRM_CONNECT) + "_")){
    			
					//ȥ��
					if(ServerMain.GetLastMsg() != null && ServerMain.GetLastMsg().endsWith(receiveMessage))
						continue;
					    		
					ServerMain.InsertMsg(recpack.getAddress() + "_" + recpack.getPort() + "_" + receiveMessage);
				//}
    		
    	
			}catch (IOException e) {
				e.printStackTrace();
			}
	
	}
}
