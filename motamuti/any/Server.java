package any;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Server {
    private static final int PORT = 56565; // Choose a suitable port number
    private Map<String, Socket> connectedClients;
    List<ClientHandler> allClientsNow;
    Map<String, Socket> disconnectedClients;
    public String baseDirectory = "D:\\ServerFiles";
    public String baseDirectoryClientSide = "D:\\ClientFiles";

    Map<String,ClientHandler> allRequests;

    Map <String, List <String>> publiclyUploadedFiles;
    Map <String, List <String>> privatelyUploadedFiles;

    public Server() {
        connectedClients = new HashMap<>();
        disconnectedClients = new HashMap<>();
        publiclyUploadedFiles=new HashMap<>();
        privatelyUploadedFiles=new HashMap<>();
        allClientsNow=new ArrayList <>();
        allRequests=new HashMap <>();

    }

    public void start() {
        try (ServerSocket serverSocket = new ServerSocket(PORT)) {
            System.out.println("Server started and listening on port " + PORT);

            while (true) {
                Socket clientSocket = serverSocket.accept();
                ClientHandler clientHandler = new ClientHandler(clientSocket, this);
                clientHandler.start();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public synchronized boolean isUsernameTaken(String username, Socket clientSocket) {
        return connectedClients.containsKey(username) && connectedClients.get(username) != clientSocket;
    }

    public synchronized void addClient(String username, Socket clientSocket) {
        connectedClients.put(username, clientSocket);
    }

    public synchronized void removeClient(String username) {
        connectedClients.remove(username);
    }

    public synchronized  void addFiles()
    {

    }

    public synchronized String getConnectedClients() {
        StringBuilder clientsList = new StringBuilder();
        for (String username : connectedClients.keySet()) {
            clientsList.append(username).append(",");
        }
        return clientsList.toString();
    }

    public static void main(String[] args) {
        Server server = new Server();
        server.start();

    }

    //who are not currently online but once was connected to server

    public String getDisconnectedClients()
    {
        StringBuilder clientsList = new StringBuilder();
        for (String username : disconnectedClients.keySet()) {
            clientsList.append(username).append(",");
        }

        return clientsList.toString();
    }


    public boolean searchInOfflineList(String name)
    {
        if(disconnectedClients.containsKey(name)) return true;
        else return false;
    }



}
