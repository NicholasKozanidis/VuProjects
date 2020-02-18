package threadsocket;

import java.io.*;
import java.net.*;



public class Telnet extends Thread {
	
	static final int DEFAULT_PORT = 23;
	
	static Socket channel = null;
	static BufferedOutputStream out = null;
	static BufferedInputStream in = null;
	static Clistener listener = null;
	static Cconsole console = null;
	

	public static void main(String args[]) throws IOException {
		
		String server = null;
		InetAddress address = null;
		
		int port = DEFAULT_PORT;
		
		if (args.length == 2) {
			
			port = Integer.parseInt(args[1]);
			
		}else if (args.length != 1) {
			
			System.out.println("Usage threadsocket.telnet <server> <port>(Default port is 23)");
			return;
		
		}
		
		server = args[0];
		address = InetAddress.getByName(server);
		
		try 
		{
			System.out.println("Connecting to " + address.getHostAddress() +"...");
			channel = new Socket (server, port);
			System.out.println("Connected");
			System.out.println("For command code write ^]+command");

			out = new BufferedOutputStream(channel.getOutputStream());
			in = new BufferedInputStream (channel.getInputStream());
			
			listener = new Clistener(out, in);
			console = new Cconsole(channel, out);
			
			// Start listener thread for server input
			new Thread(listener).start();
			// Start output stream thread to the server
			new Thread(console).start();
						
		}catch (UnknownHostException e) {
			
			System.out.println("IP address of Host could be determined");
			
		}catch (IOException e) {
			
			System.out.println("Connection timed out");
			
		} catch (Exception e) {	
			
			e.printStackTrace();
			
		}
		while (!channel.isClosed()) {
		 //The loop waits for the connection to close then closes the application
		}
		listener.stop();
		console.stop();
		return;
		
	}
	
	

	
	public static void closeConnection() throws IOException{
		if (!channel.isClosed()) {
			channel.close();
			System.out.println("Connection closed");
		}
	}

}
