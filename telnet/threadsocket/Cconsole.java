package threadsocket;


import java.io.*;
import java.net.*;



class Cconsole implements Runnable
{
	private volatile boolean running;
    private Socket channel=null;
    private BufferedOutputStream out = null;
    private BufferedReader in = null;
    private String inputL=null;
    
    public Cconsole(Socket Soc, BufferedOutputStream out) throws Exception {
    	
        this.channel=Soc;
        this.out = out;
        this.in = new BufferedReader(new InputStreamReader(System.in));
        running = true;
    }

    public void stop () {
		running = false;
		Thread.currentThread().interrupt();
	}
    

	
    
    public void run () {

    	while (running) {
    		
	        try {
	        	//If input exists and is ready
	            if(in.ready()) {
	            	
	            	inputL = in.readLine();
	           
	            	//If ^] is found then interpret as command
	            	if (inputL.contains("^]")) {
	            		
	            		String input = inputL.replace("^]","");
	            		CommandCode(input);
	            		
	            	} else {
	            		//If not write the string to buffer
	            		out.write(new String(inputL + "\r\n").getBytes());
	            	}
	            	//flush the buffer to the server
	            	out.flush();
	            }
	            
	        } catch (Exception e) {
	        	
	            System.out.println("Exception while proccessing input");
	        }
    	}
    	return;
    }
    
    
    void CommandCode (String input) throws IOException {
    	
    	switch (input) {
			case "IP":
				System.out.println("sent IP");
				out.write(TelnetCommand.IAC);
				out.write(TelnetCommand.IP);
				break;
			case "AYT":
				System.out.println("sent AYT");
				out.write(TelnetCommand.IAC);
				out.write(TelnetCommand.AYT);
				break;
			case "AO":
				System.out.println("sent AO");
				out.write(TelnetCommand.IAC);
				out.write(TelnetCommand.AO);
				break;
			case "BRK":
				System.out.println("sent BRK");
				out.write(TelnetCommand.IAC);
				out.write(TelnetCommand.BREAK);
				break;
			case "SYNCH":
				System.out.println("sent DM");
				channel.sendUrgentData(242);
				break;
			default:
				System.out.println("No such command.\nsupported commands:\nAYT, IP, AO, BREAK, SYNCH.\nFormat: '^]Command'");
				break;
    	}	
    }

}