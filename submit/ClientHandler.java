package submit;
//sever side er jonno
import java.io.*;
import java.net.Socket;
import java.util.*;

public class ClientHandler extends Thread {
    private Socket clientSocket;
    private Server server;
    private BufferedReader reader;
    private PrintWriter writer;
    private FileInputStream rd;
    private FileInputStream wrt;
    private String username;
    File clientDirectory;
    File clientDirectoryClientSide;
    private static final String END_OF_FILE_MARKER = "END_OF_FILE";
    private static final String END_OF_RESPONSE = "END_OF_RESPONSE";
    private static final int MAX_CHUNK_SIZE = 1024 * 100; // Maximum chunk size in bytes (e.g., 100 KB)
    private static final int MIN_CHUNK_SIZE = 1024; // Minimum chunk size in bytes (e.g., 1 KB)
    static final long  MAX_CAPACITY = 1024L * 1024L*1024L*1024L;


    public ClientHandler(Socket clientSocket, Server server) {
        this.clientSocket = clientSocket;
        this.server = server;

    }

    @Override
    public void run() {
        try {
            // Initialize reader and writer
            reader = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
            writer = new PrintWriter(clientSocket.getOutputStream(), true);

            // Perform login process
            boolean isConnected= performLogin();


            //ekhane sob client request handle korbe
            while (isConnected)
            {
                if (!clientSocket.isConnected()) {
                    // Client connection has been terminated, break the loop
                    break;
                }

                String request = reader.readLine();
                if (request != null) {
                    if (request.equalsIgnoreCase("q"))
                    {
                        isConnected = false;
                        server.removeClient(username);
                        server.allClientsNow.remove(server.allClientsNow.indexOf(this));
                        //System.out.println("hereeeeeee");
                        if(!server.searchInOfflineList(username))
                        { server.disconnectedClients.put(username,clientSocket);

                        }
                        //
                        writer.println("Server: Connection terminated for now");
                        writer.flush();
                        clientSocket.close();
                        break;
                    }
                    if (request.equalsIgnoreCase("l"))
                    {
                        // Get a list of connected client names from the server
                        String connectedClients = server.getConnectedClients();
                        String offlineClients = server.getDisconnectedClients();

                        // Send the list of connected clients to the client
                        writer.println("Connected clients (online):     " + connectedClients);
                        writer.println(" Other Connected clients (offline currently):     " + offlineClients);
                        writer.println(END_OF_RESPONSE);

                        //request = reader.readLine();

                    }

                    else if (request.equalsIgnoreCase("u"))
                    {
                        handleFileUpload();
                    }
                    else if (request.equalsIgnoreCase("d"))
                    {
                        handleFileDownload();
                    }

                    else if (request.equalsIgnoreCase("r"))
                    {
                        String des=reader.readLine();
                        long reqid= Long.parseLong(reader.readLine());
                        handleFileRequest(username,reqid,des);
                    }



                    else if (request.equalsIgnoreCase("lookup")) {
                        sendFileList();
                    }

                    else if (request.equalsIgnoreCase("my")) {
                        sendOwnFileList();
                    }

                    else if (request.equalsIgnoreCase("v")) {
                        showMyinbox();
                    }
                    
                    

                    else {
                        // Process other client requests

                        // writer.println("ei else e ");
                        // ..
                    }

                }

                //writer.println("req null hoye gese ");
            }

        } catch (IOException e) {
            System.out.println("Connection error for client " + username + ": " + e.getMessage());


        }
    }

    private void sendOwnFileList()
    {
        writer.println("Listing own uploaded files:");
        List<String> associatedList = server.privatelyUploadedFiles.get(this.username);
        if (associatedList != null)
        {
            writer.println("Private " + ": " + associatedList);
        }
        List<String> associatedList2 = server.publiclyUploadedFiles.get(this.username);

        if (associatedList2 != null) {
            writer.println("Public " + ": " + associatedList2);
        }
        writer.println("These are all your own files");
        writer.println(END_OF_RESPONSE);

    }

    private void sendFileList() throws IOException {
        writer.println("Listing uploaded files:");

        // Iterate over the uploadedFiles map
        for (Map.Entry<String, List<String>> entry : server.publiclyUploadedFiles.entrySet()) {
            String username = entry.getKey();

            List<String> files = entry.getValue();

            writer.println("User: " + username);


            if(username.equals(this.username))
            {
                List<String> associatedList = server.privatelyUploadedFiles.get(username);
                //System.out.println("size hocce "+server.privatelyUploadedFiles.size() +" list er size "+associatedList.size());

                if (associatedList != null) {
                    writer.println("Own Privately uploaded files " + ": " + associatedList);}

            }

            if (files.isEmpty()) {
                writer.println("No public files uploaded.");
            } else {
                writer.println("Public Files:");
                for (String file : files) {
                    writer.println(file);
                }
            }


        }
        writer.println("List sent successfully ");
        writer.println(END_OF_RESPONSE);

    }

    private boolean performLogin() throws IOException {
        writer.println("Please enter your username:");
        String loginRequest = reader.readLine();
        if (server.isUsernameTaken(loginRequest,clientSocket)) {
            writer.println("Username already in use. Connection terminated.");
            clientSocket.close();
            return false;
        }
        username = loginRequest;
        if(server.searchInOfflineList(username))
        {
            server.disconnectedClients.remove(username);
        }
        else  server.anyClient.add(username);/////////////////////////////////////////////////////////
        server.addClient(username, clientSocket);
        server.allClientsNow.add(this);


        // Create directory for the client if it doesn't exist
        clientDirectory = new File(server.baseDirectory,username);
        if (!clientDirectory.exists()) {
            if (clientDirectory.mkdir()) {
                System.out.println("Directory created for client " + username);
            } else {
                System.out.println("Failed to create directory for client " + username);
            }
        }


        clientDirectoryClientSide = new File(server.baseDirectoryClientSide, username);
        if (!clientDirectoryClientSide.exists()) {
            if (clientDirectoryClientSide.mkdir()) {
                System.out.println("Directory created for client on client side" + username);
            } else {
                System.out.println("Failed to create directory for client on client side" + username);
            }
        }

        writer.println("Welcome, " + username + "! You are now connected to the server.");
        System.out.println("Client " + username + " connected.");  //server side e
        return true;
    }

    //file receive er jonno ja client side theke pathacche


    /**
     *
     * ===================eita hocche server side e  uploaded  file receive er jonno.......
     * =================== eita te modify korbi=========================================
     * **/
    private void handleFileUpload() throws IOException {
        String filename = reader.readLine();
        String visibility = reader.readLine();
        String ans=reader.readLine();
        String reqID= (reader.readLine());
        int sz= Integer.parseInt(reader.readLine());
        //System.out.println("size porsi "+sz);
        if (sz + server.currentServerSize > MAX_CAPACITY) {
            System.out.println(server.currentServerSize);
            //System.out.println("hobe na "+MAX_CAPACITY);
            writer.println("File upload exceeds maximum buffer size. Please try again with a smaller file.");
            return;
        }
        else
            writer.println("yes");
       // System.out.println(reqID);
        File file = new File(clientDirectory, filename);
        Random random = new Random();
        int allocatedChunk = random.nextInt(MAX_CHUNK_SIZE - MIN_CHUNK_SIZE + 1) + MIN_CHUNK_SIZE;
        writer.println(allocatedChunk);
        String fileID = generateFileID();
        writer.println(fileID);
        int cnum= Integer.parseInt(reader.readLine());


        boolean allOkay=true;
//        try (BufferedOutputStream fileOutputStream = new BufferedOutputStream(new FileOutputStream(file))) {
//            String line;
//            int rchunk = 0;
//
//            while ((line = reader.readLine()) != null && !line.equals(END_OF_FILE_MARKER)) {
//                byte[] buffer = line.getBytes();
//                System.out.println(buffer.length);
//                fileOutputStream.write(buffer);
//                fileOutputStream.write('\n');
//                if(reader.readLine().equals(END_OF_FILE_MARKER))
//                { flag=true;
//                    System.out.println("korsiiiiiiiiiiiii");}
//                if(!flag)
//                {writer.println("OK");
//                    System.out.println(flag);
//
//                } // Send acknowledgement to the client
//                // Send acknowledgement to the client
//
//            }
//        }

        //okay jinishhhhhhhhhhhhhhh
       InputStream inputStream = clientSocket.getInputStream();
       FileOutputStream fileOutputStream = new FileOutputStream(file);
      BufferedOutputStream bufferedOutputStream = new BufferedOutputStream(fileOutputStream);

        byte[] buffer = new byte[allocatedChunk]; // Adjust the buffer size as per your requirements
        int bytesRead;
        int sum=0;
        int ct=0;
        while ((bytesRead = inputStream.read(buffer)) != -1) {
            if(sum>sz)break;
            bufferedOutputStream.write(buffer, 0, bytesRead);
            bufferedOutputStream.flush(); // Flush after each chunk
            if(ct<cnum)
            writer.println("OK"); // Send acknowledgement to the client
            ct++;
            writer.flush();
            sum+=bytesRead;
        }
        //System.out.println("ber hoisi");

        bufferedOutputStream.close();
//
//        System.out.println("File upload completed: " + file.getAbsolutePath());
//        byte[] buffer = new byte[1024];
//        int bytesRead;
//        while ((bytesRead = inputStream.read(buffer)) != -1) {
//            // Process the received chunk (e.g., save to a file, perform some operation)
//            // Here, we'll print the received chunk size and send an acknowledgement back to the client
//            System.out.println("Received chunk: " + bytesRead + " bytes");
//
//            // Send acknowledgement to the client
//            writer.println("OK");
//        }
//
//// After receiving all the chunks, you can send the final response to the client
//        writer.println("File upload completed");



        if(sum<sz) allOkay=false;
        if(allOkay==true) {
            if (visibility.equalsIgnoreCase("public")) {

                List <String> list = server.publiclyUploadedFiles.getOrDefault(username, new ArrayList <>());
                list.add(filename);
                server.publiclyUploadedFiles.put(username, list);


            } else {
                List <String> list = server.privatelyUploadedFiles.getOrDefault(username, new ArrayList <>());
                list.add(filename);
                server.privatelyUploadedFiles.put(username, list);
                //System.out.println("private e rakhsi "+server.privatelyUploadedFiles.size());
            }

            if (ans.equalsIgnoreCase("y")) {
                if (server.allRequests.containsKey(reqID)) {
                    ClientHandler client = server.allRequests.get(reqID);
                    String msg = "There is a message for you\nSender: Server" + "\nOne of Your Requested files is uploaded by an user named " + this.username + "\nHis uploaded file on this request :" + filename + "\n";
                    List <String> list = server.inbox.getOrDefault(client.username, new ArrayList <>());
                    list.add(msg);
                    server.inbox.put(client.username, list);
                    //client.writer.println(END_OF_RESPONSE);
                    //client.writer.flush();
                } else
                    writer.println("Invalid request id. But your file is uploaded publicly");
            }
            //System.out.println("here");
            writer.println("File uploaded successfully.");
            server.currentServerSize += sz;
            writer.println(END_OF_RESPONSE);
        }
        else
        {   file.delete();
            writer.println("Failed Upload");
            writer.println(END_OF_RESPONSE);
        }
    }


    private void handleFileDownload() throws IOException {

        String filename = reader.readLine();
        System.out.println(filename);
        File file = new File(filename);
        if (file.exists()) {
            writer.println("File found");
            try (BufferedInputStream fileInputStream = new BufferedInputStream(new FileInputStream(file))) {
                byte[] buffer = new byte[MAX_CHUNK_SIZE];
                int bytesRead;
                while ((bytesRead = fileInputStream.read(buffer)) != -1) {
                    writer.println(new String(buffer, 0, bytesRead));
                }

                writer.println(END_OF_FILE_MARKER);
            }
            writer.flush();

        } else {
            writer.println("File not found");
            writer.flush();
        }
    }


    private void handleFileRequest(String username, long reqid, String des) {

        server.allRequests.put(String.valueOf(reqid),this);

           String msg="There is a message for you\nSender: "+username+"\n"+des+"\nRequest ID: "+ reqid+"\n";

//        for (ClientHandler client : server.allClientsNow) {
//            if (client !=this ) {
//                client.writer.println("There is a Message for you");
//                client.writer.println("Sender: "+username);
//                client.writer.println(des);
//                client.writer.println("Request ID: " + reqid);
//                client.writer.println(END_OF_RESPONSE);
//                client.writer.flush();
//            }
//        }

        for (String user:server.anyClient) {

            //String user = entry.getKey();
            if(!user.equalsIgnoreCase(username))
            {
                List<String> list = server.inbox.getOrDefault(user, new ArrayList <>());
                list.add(msg);
                server.inbox.put(user, list);
               // System.out.println("rakhsiiiiiiiiii");
            }


        }



        // Send a response to the requester
        writer.println("File request sent successfully.");

    }

    private void showMyinbox()
    {
        writer.println("Listing own  inbox:");
        List<String> associatedList = server.inbox.get(this.username);
        if (associatedList != null)
        {

            for (String str : associatedList) {
                writer.println("==============================================");
                writer.println(str);
            }

        }

        writer.println("These are all your inbox Messages");
        writer.println(END_OF_RESPONSE);
    }

    private String generateFileID() {
        return UUID.randomUUID().toString();
    }

}
