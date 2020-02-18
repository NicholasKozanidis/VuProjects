package threadsocket;

import java.awt.Toolkit;
import java.io.*;

class Clistener implements Runnable {
	
	private volatile boolean running;
	boolean respond;
	private BufferedInputStream in = null;
	private BufferedOutputStream out = null;
	private int srvIn;
	Clistener() {}
	
	public Clistener(BufferedOutputStream out, BufferedInputStream in) throws IOException {

		this.in = in;
		this.out = out;
		running  = true;
		respond = false;
		
	}
	
	public void stop () {
		running = false;
		Thread.currentThread().interrupt();
	}
	
	public void run () {	
		

		while(running) {

			try {
				srvIn = in.read();
				
				//replay to all negotiations
				if (in.available() == 0 && respond) {
					out.flush();
					respond = false;
				}
				
				//if an Interpret As Command code has been received
				if (srvIn == (char)TelnetCommand.IAC) {
					//command operation
					int o_code = in.read();
					commandCode(o_code);
				
				} else if (srvIn == TelnetCommand.EOF)  {
					//if the server closed the connection
					Telnet.closeConnection();
				} else {
					//print the character received from the server
					print(srvIn);
				}
				
			} catch (IOException e) {
				System.out.println("Could not read from server");
				System.exit(1);
			}
		}
		
		return;
	}
	
	public void print(int message) {
		
		switch (message) {	
		//Play sound
		case (char)TelnetOptions.BELL:
			Toolkit.getDefaultToolkit().beep();
			break;
		// Horizontal Tab
		case (char)TelnetOptions.HT:
			System.out.print("\t");
			break;
		//Back space
		case (char)TelnetOptions.BS:
			System.out.print("\b");
			break;
		//Carriage Return	
		case (char)TelnetOptions.CR:
			System.out.print("\r");
			break;
		//Line Feed
		case (char)TelnetOptions.LF:
			System.out.print("\n");
			break;
		//Vertical Tab	
		case (char)TelnetOptions.VT:
			System.out.print("\u000b");
			break;
		//Form Feed		
		case (char)TelnetOptions.FF:
			System.out.print("\f");
			break;

		default:
			System.out.print((char)message);
			break;
		}
		
	}
	
	public String commandCode(int o_code) throws IOException {
		
		String response = "";
		int m_code = 0;
		switch (o_code) {
			
			//Do command, reply with Wont
			case (char)TelnetCommand.DO:
				out.write(TelnetCommand.IAC);
				out.write(TelnetCommand.WONT);
				m_code = in.read();
				out.write(m_code);
				respond = true;
				break;
			//Don't command
			case (char)TelnetCommand.DONT:
				m_code = in.read();
				System.out.println("Received DONT " + m_code);
				break;
			//Will Command, reply with Don't
			case (char)TelnetCommand.WILL:
				out.write(TelnetCommand.IAC);
				out.write(TelnetCommand.DONT);
				m_code = in.read();
				out.write(m_code);
				respond = true;
				break;
			//Won't  command
			case (char)TelnetCommand.WONT:
				m_code = in.read();
				System.out.println("Received WONT " + m_code);
				out.flush();
				break;		
			//Sub negotiation, read until Sub negotiation end
			case (char)TelnetCommand.SB:
				System.out.println("sb");
				while (m_code != TelnetCommand.SE) {
					m_code = in.read();
				}
				break;
			//Go ahead
			case (char)TelnetCommand.GA:
				System.out.println("GO AHEAD received");
				break;
			//print the command code
			default:
				m_code = in.read();
				System.out.println("IAC " + o_code + " " + m_code);
				break;
				
		}
		return response;
	}
	
}