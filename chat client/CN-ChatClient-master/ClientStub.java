package chat;

import java.io.*;
import java.net.*;
import java.util.Scanner;

/**
 * VUnetID: <fill in your VUnetID here>
 **/
class client
{
  BufferedReader in;
  PrintStream out;
  boolean exit;

  client() {
    in   = new BufferedReader(new InputStreamReader(System.in));
    out  = new PrintStream(System.out);
    exit = false;
  }

  boolean sayHello(BufferedReader srvIn, PrintWriter srvOut) {
	try {

		
		out.println("Enter your name :");
		String name = in.readLine();
		
		while (!name.matches("[A-Za-z0-9]+")) {
			out.println("Name must be alphanumeric");
			out.println("Enter your name :");
			name = in.readLine();
		}
	
	    srvOut.println("HELLO-FROM " + name);
	    String message = srvIn.readLine(); 
		out.println(message);
	} catch (Exception e) {
	    	return false;
	    }
    return true;
  }

  void handleInput(BufferedReader srvIn, PrintWriter srvOut)
    throws IOException {
    String line = in.readLine();
    if(line == null) {
      out.println("could not read line from user");
      return;
    }

    /* supported commands:
       !who          - perform a WHO request to the server
       @<user> <msg> - send <msg> to <user>
       !exit         - stop the program */
    if(line.equals("!who")) {
      requestUserList(srvIn, srvOut);
    }
    else if(line.startsWith("@")) {
      sendMessage(line, srvIn, srvOut);
    }
    else if(line.equals("!exit")) {
      exit = true;
    }
    else {
      out.println("unknown command");
    }
  }

  void requestUserList(BufferedReader srvIn, PrintWriter srvOut)
    throws IOException {
	  
	  srvOut.println("WHO");
	  String message = srvIn.readLine(); 
	  out.println(message);
	  
  }

  void sendMessage(String line, BufferedReader srvIn, PrintWriter srvOut) 
    throws IOException {
	  
    line = line.replaceAll("@", "SEND ");
    srvOut.println(line);
	String message = srvIn.readLine();
	out.println(message);
	
  }

  void recvMessage(BufferedReader srvIn) 
    throws IOException {
	String message = srvIn.readLine();
	System.out.println(message);
  }

  void start(String[] argv) {

    if(argv.length != 2) {
      out.println("usage: java ClientStub <server> <port>");
      return;
    }

    Socket sock          = null;
    BufferedReader srvIn = null;
    PrintWriter srvOut   = null;
    
    String ip = argv[0];
    int port = Integer.parseInt(argv[1]) ;
    
    try {
    	sock = new Socket (ip,port);
    	
    	srvIn = new BufferedReader(
    				new InputStreamReader(
    					sock.getInputStream()
    				)
    			);
    		
    	srvOut = new  PrintWriter(
    				sock.getOutputStream(),true  
    				);
    	
    } catch (Exception e) {
    	 out.println(e.getMessage());
    }

    if(!sayHello(srvIn, srvOut)) {
      try {
		sock.close();
	} catch (IOException e) {
		e.printStackTrace();
	}
      return;
    }

    while(!exit) {
      try {
        if(in.ready()) {
          handleInput(srvIn, srvOut);
        }
        if(srvIn.ready()) {
          recvMessage(srvIn);
        }
        Thread.sleep(200);
      }
      catch(IOException e) {
        out.println(e.getMessage());
      }
      catch(InterruptedException e) {}
    }

    try {
		sock.close();
	} catch (IOException e) {
		 out.println(e.getMessage());
	}
  }

  public static void main(String[] argv) {
    new client().start(argv);
  }
}
