package any;
//sever side er jonno
import java.io.*;
import java.net.Socket;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class ClientHandler extends Thread {
    private Socket clientSocket;
    private Server server;
    private BufferedReader reader;
    private PrintWriter writer;
    private String username;
    File clientDirectory;
    private static final String END_OF_FILE_MARKER = "END_OF_FILE";
    private static final String END_OF_RESPONSE = "END_OF_RESPONSE";


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
            performLogin();
            boolean isConnected = true;
             
            //ekhane sob client request handle korbe
            while (isConnected)
            {
                String request = reader.readLine();
                if (request != null) {
                    if (request.equalsIgnoreCase("q")) {
                        isConnected = false;
                        server.removeClient(username);
                        System.out.println("hereeeeeee");
                        if(!server.searchInOfflineList(username))
                            server.disconnectedClients.put(username,clientSocket);
                        clientSocket.close();
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

                    else if (request.equalsIgnoreCase("lookup")) {
                        sendFileList();
                    }

                    else {
                        // Process other client requests

                         // writer.println("ei else e ");
                        // ..
                    }

                }

                //writer.println("req null hoye gese ");
            }

//            // Handle client requests
//
//            // Close connections
//            reader.close();
//            writer.close();
//            clientSocket.close();
//
//            // Remove client from server's connected clients
//            server.removeClient(username);
//            System.out.println("Client " + username + " disconnected.");
        } catch (IOException e) {
            System.out.println("Connection error for client " + username + ": " + e.getMessage());


        }
    }

    private void sendFileList() throws IOException {
        writer.println("Listing uploaded files:");


       // System.out.println("ekhon map e ache ");
//        for (Map.Entry<String, List<String>> entry : publiclyUploadedFiles.entrySet()) {
//            String key = entry.getKey();
//            List<String> value = entry.getValue();
//            System.out.println("Key: " + key);
//            System.out.println("Value: " + value);
//        }


        // Iterate over the uploadedFiles map
        for (Map.Entry<String, List<String>> entry : server.publiclyUploadedFiles.entrySet()) {
            String username = entry.getKey();

            List<String> files = entry.getValue();

            writer.println("User: " + username);
            //writer.println("User: " + this.username);

            if(username.equals(this.username))
            {    //writer.println("same");
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

            //writer.println();  // Add an empty line between users
        }
        writer.println("List sent successfully ");
        writer.println(END_OF_RESPONSE);

    }

    private void performLogin() throws IOException {
        writer.println("Please enter your username:");
        String loginRequest = reader.readLine();
        if (server.isUsernameTaken(loginRequest,clientSocket)) {
            writer.println("Username already in use. Connection terminated.");
            clientSocket.close();
            return;
        }
        username = loginRequest;
        server.addClient(username, clientSocket);

        // Create directory for the client if it doesn't exist
         clientDirectory = new File(server.baseDirectory,username);
        if (!clientDirectory.exists()) {
            if (clientDirectory.mkdir()) {
                System.out.println("Directory created for client " + username);
            } else {
                System.out.println("Failed to create directory for client " + username);
            }
        }

        writer.println("Welcome, " + username + "! You are now connected to the server.");
        System.out.println("Client " + username + " connected.");  //server side e
    }

    //file receive er jonno ja client side theke pathacche


//    private void handleFileUpload() throws IOException {
//        String filename = reader.readLine();
//        File file = new File(clientDirectory, filename);
//
//        if (file.exists()) {
//            writer.println("File already exists.");
//            return;
//        }
//
//        try (BufferedOutputStream fileOutputStream = new BufferedOutputStream(new FileOutputStream(file))) {
//            byte[] buffer = new byte[4096];
//            int bytesRead;
//            while ((bytesRead = clientSocket.getInputStream().read(buffer)) != -1) {
//                fileOutputStream.write(buffer, 0, bytesRead);
//            }
//        }
//
//        writer.println("File uploaded successfully.");
//    }


    private void handleFileUpload() throws IOException {
        String filename = reader.readLine();
        String visibility = reader.readLine();
        File file = new File(clientDirectory, filename);

        try (BufferedOutputStream fileOutputStream = new BufferedOutputStream(new FileOutputStream(file))) {
            String line;
            while ((line = reader.readLine()) != null && !line.equals(END_OF_FILE_MARKER)) {
                byte[] buffer = line.getBytes();
                fileOutputStream.write(buffer);
            }

            fileOutputStream.flush();  // Flush the file output stream( nao dite pari)
        }


        if (visibility.equalsIgnoreCase("public")) {

            List<String> list = server.publiclyUploadedFiles.getOrDefault(username, new ArrayList <>());
            list.add(filename);
            server.publiclyUploadedFiles.put(username, list);


        }

        else {
            List<String> list = server.privatelyUploadedFiles.getOrDefault(username, new ArrayList <>());
            list.add(filename);
            server.privatelyUploadedFiles.put(username, list);
            System.out.println("private e rakhsi "+server.privatelyUploadedFiles.size());
        }

        writer.println("File uploaded successfully.");
       // writer.println(END_OF_RESPONSE);
    }
}

