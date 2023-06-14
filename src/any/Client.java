package any;

import java.io.*;
import java.net.Socket;

public class Client {
    private static final String SERVER_IP = "127.0.0.1";
    private static final int SERVER_PORT = 56565;
    private BufferedReader reader;
    private PrintWriter writer;
    private Socket socket;


    public void start(String host, int port)
    {
        try{
          socket = new Socket(SERVER_IP, SERVER_PORT);
            reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            writer = new PrintWriter(socket.getOutputStream(), true);
             BufferedReader userInputReader = new BufferedReader(new InputStreamReader(System.in));

            // Read and display the server's welcome message
            String serverMessage = reader.readLine();
            System.out.println(serverMessage);

            // Get the client's username from the user
            String username = userInputReader.readLine();

            // Send the username to the server
            writer.println(username);

            // Receive and display the server's response
            serverMessage = reader.readLine();
            System.out.println(serverMessage);

            // Perform file operations based on user commands
            String command;


//            while (true) {
//                System.out.print("> ");
//                command = userInputReader.readLine();
//                writer.println(command);
//
//                if (command.equalsIgnoreCase("quit")) {
//                    break;
//                }
//
//                // Receive and display the server's response
//                serverMessage = reader.readLine();
//                System.out.println(serverMessage);
//            }

            while ((command = userInputReader.readLine()) != null)

            {
                if (command.equalsIgnoreCase("q")) {
                    break;
                } else if (command.equalsIgnoreCase("u")) {
                    handleFileUploadFromConsole();
                } else {
                    writer.println(command);
                    String response = reader.readLine();
                    System.out.println("Server response: " + response);
                }
            }


        }
        catch (IOException e) {
            e.printStackTrace();
        }
        
    }

    private void handleFileUploadFromConsole() throws IOException {
        BufferedReader consoleReader = new BufferedReader(new InputStreamReader(System.in));
        System.out.println("Enter the file path: ");
        String filePath = consoleReader.readLine();

        File file = new File(filePath);
        if (!file.exists()) {
            System.out.println("File does not exist.");
            return;
        }

        String filename = file.getName();
        writer.println("u");
        writer.println(filename);

        try (BufferedInputStream fileInputStream = new BufferedInputStream(new FileInputStream(file))) {
            byte[] buffer = new byte[1024];
            int bytesRead;
            while ((bytesRead = fileInputStream.read(buffer)) != -1) {
                socket.getOutputStream().write(buffer, 0, bytesRead);  //clientSocket
            }
        }

        String response = reader.readLine();
        System.out.println("Server response: " + response);
    }

    public static void main(String[] args) {

        String host = "localhost";
        int port = 56565;

        Client client = new Client();
        client.start(host, port);
       
    }
}
