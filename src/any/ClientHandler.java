package any;

import java.io.*;
import java.net.Socket;

public class ClientHandler extends Thread {
    private Socket clientSocket;
    private Server server;
    private BufferedReader reader;
    private PrintWriter writer;
    private String username;
    File clientDirectory;

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

                    }

                    else if (request.equalsIgnoreCase("u"))
                    {
                        handleFileUpload();
                    }

                    else {
                        // Process other client requests
                        // ...
                    }
                }
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
        System.out.println("Client " + username + " connected.");
    }


    private void handleFileUpload() throws IOException {
        String filename = reader.readLine();
        File file = new File(clientDirectory, filename);

        if (file.exists()) {
            writer.println("File already exists.");
            return;
        }

        try (BufferedOutputStream fileOutputStream = new BufferedOutputStream(new FileOutputStream(file))) {
            byte[] buffer = new byte[1024];
            int bytesRead;
            while ((bytesRead = clientSocket.getInputStream().read(buffer)) != -1) {
                fileOutputStream.write(buffer, 0, bytesRead);
            }
        }

        writer.println("File uploaded successfully.");
    }
}

