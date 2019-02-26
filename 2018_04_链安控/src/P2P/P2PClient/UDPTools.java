package P2PClient;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetSocketAddress;
import java.net.SocketAddress;

public class UDPTools {

	public static boolean SendMsg(DatagramSocket client, String IP, int Port, String Msg){
		try{
			SocketAddress target = new InetSocketAddress(IP, Port);
			byte[] sendbuf = Msg.getBytes();
			DatagramPacket pack = new DatagramPacket(sendbuf, sendbuf.length, target);
			client.send(pack);			
			return true;
		}catch(Exception e){
			e.printStackTrace();
			return false;
		}
	}
	
	public static boolean SendMsg(DatagramSocket client, SocketAddress target, String Msg){
		try{
			byte[] sendbuf = Msg.getBytes();
			DatagramPacket pack = new DatagramPacket(sendbuf, sendbuf.length, target);
			client.send(pack);			
			return true;
		}catch(Exception e){
			e.printStackTrace();
			return false;
		}
	}
	
	public static String ReceiveMsg(DatagramSocket client){
		try{
			byte[] buf = new byte[1024];
            DatagramPacket recpack = new DatagramPacket(buf, buf.length);
            client.receive(recpack);
            String receiveMessage = new String(recpack.getData(), 0, recpack.getLength());
            return receiveMessage;
		}catch (Exception e) {
			e.printStackTrace();
			return null;
		}
	}
	
}
