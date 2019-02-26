package P2PClient;

import java.util.Scanner;

public class Test {

	public static void main(String[] arg){
		
		ClientMain clientMain = new ClientMain();
		clientMain.start();
		
		
		System.out.println("Choose wait(0) or connect(1)");
		Scanner sin = new Scanner(System.in);
		
		if(sin.nextInt() == 0)
			new Thread(new Runnable() {
				
				public void run() {
					try{
						java.lang.Thread.sleep(500);
						/*
						 * C1 Login
						 */
						UserAddInstr.Login(Integer.valueOf(ClientMain.GetMyUID()));
					}catch (Exception e) {
						e.printStackTrace();
					}				
				}
			}).start();
		else 		
			new Thread(new Runnable() {
				public void run() {
				try{
					java.lang.Thread.sleep(500);
				/*
				 * C2 Login and connect C1 send Msg to C1
				 */
				UserAddInstr.Login(Integer.valueOf(ClientMain.GetMyUID() + 1));
				UserAddInstr.AskConnect(Integer.valueOf(ClientMain.GetMyUID() + 1), Integer.valueOf(ClientMain.GetMyUID()));
				
				java.lang.Thread.sleep(5000);
				System.out.println(UserAddInstr.SendMessage(Integer.valueOf(ClientMain.GetMyUID() + 1), Integer.valueOf(ClientMain.GetMyUID()), "Hello C1"));
				}
				catch (Exception e) {
					e.printStackTrace();
				}
			}
		}).start();
		
		
	}
	
}
