package any;

import java.io.*;
import java.net.Socket;

public class Client {
    private static final String SERVER_IP = "127.0.0.1";
    private static final int SERVER_PORT = 56565;
    private BufferedReader reader;
    private PrintWriter writer;
    private Socket socket;
    private static final String END_OF_FILE_MARKER = "END_OF_FILE";
    private static final String END_OF_RESPONSE = "END_OF_RESPONSE";


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

            while (true)

            {

                System.out.print("Enter a command: ");
                command = userInputReader.readLine();


                if (command.equalsIgnoreCase("q"))
                {
                    writer.println(command); // clienthandler handle korbe
                    System.out.println("Your connection is terminated");
                    break;
                }

                else if (command.equalsIgnoreCase("u"))
                {
                    handleFileUploadFromConsole();
                }

                else if (command.equalsIgnoreCase("r"))
                {
                    sendFileRequest();
                }
                else if (command.equalsIgnoreCase("lookup"))
                {
                    handleFileLookup();

                }

                else if (command.equalsIgnoreCase("d"))
                {
                    handleFileDownloadFromConsole();

                }

                else
                {
                    writer.println(command);
                    writer.flush();
                    String response;
                    response=reader.readLine();
                    while (!(response.equals(END_OF_RESPONSE)) )
                    {
                        System.out.println(response);
                        response=reader.readLine();


                    }

                }
            }


        }
        catch (IOException e) {
            e.printStackTrace();
        }

    }


    private void handleFileLookup() throws IOException {
        writer.println("lookup");
        writer.flush();

        // Receive and display the list of files
        String response;
        response=reader.readLine();
        while (!(response.equals(END_OF_RESPONSE)) )
        {
            System.out.println(response);
            response=reader.readLine();


        }

//        writer.flush();
//        String response = reader.readLine(); //server er response pawar jonno
//        System.out.println("Server response: " + response);

//        while ((response = reader.readLine()) != null) {
//            if (response.equals(END_OF_RESPONSE)) {
//                break;
//            }
//            System.out.println(response);
//        }



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


        String visibility ;
        String reqID;

        System.out.println("Is this a upload based on any request? (Y/N): ");
        String ans=consoleReader.readLine();
        if(ans.equalsIgnoreCase("y"))
        {
            System.out.println("Please  provide the request id");
            reqID= consoleReader.readLine();
            System.out.println(reqID);
            visibility="public";
        }
        else
        {
            System.out.println("Specify file visibility (public/private): ");
           visibility = consoleReader.readLine();
           reqID="0";
        }
        //System.out.println(String.valueOf(reqID));
        String filename = file.getName();
        writer.println("u");
        writer.println(filename); //to send commands and data to server
        writer.println(visibility);     // public or private
        writer.println(ans);
        writer.println((reqID));



//        try (BufferedInputStream fileInputStream = new BufferedInputStream(new FileInputStream(file))) {
//            byte[] buffer = new byte[1024];
//            int bytesRead;
//            while ((bytesRead = fileInputStream.read(buffer)) != -1) {
//                socket.getOutputStream().write(buffer, 0, bytesRead);  //clientSocket
//            }
//        }

        try (BufferedInputStream fileInputStream = new BufferedInputStream(new FileInputStream(file))) {
            byte[] buffer = new byte[4096];  // Adjust the buffer size as per your requirements
            int bytesRead;
            while ((bytesRead = fileInputStream.read(buffer)) != -1) {
                writer.println(new String(buffer, 0, bytesRead));
            }

            writer.println(END_OF_FILE_MARKER);
        }
        writer.flush();
        String response = reader.readLine(); //server er response pawar jonno
        System.out.println("Server response: " + response);
    }



    private void handleFileDownloadFromConsole() throws IOException {
        BufferedReader consoleReader = new BufferedReader(new InputStreamReader(System.in));
        System.out.println("Whose file do you want to download?");
        String clientName = consoleReader.readLine();
        System.out.println("Which file?");
        String filename = consoleReader.readLine();
        String path = "D:\\ServerFiles\\" + clientName + "\\" + filename;
        String savePath = "D:\\ClientFiles\\" + clientName + "\\" + filename;

        writer.println("d"); // Send the download command to the server
        writer.println(path); // Send the file path to the server

        String response = reader.readLine(); // Read the server's response
        if (response.equals("File found")) {
            System.out.println("Downloading file...");


            try (BufferedOutputStream fileOutputStream = new BufferedOutputStream(new FileOutputStream(savePath))) {
                String line;
                while ((line = reader.readLine()) != null && !line.equals(END_OF_FILE_MARKER)) {
                    byte[] buffer = line.getBytes();
                    fileOutputStream.write(buffer);
                }

                fileOutputStream.flush();  // Flush the file output stream( nao dite pari)
            }

            System.out.println("File Download is successful");

        } else {
            System.out.println("File not found on the server.");
        }
    }

    private String getFileNameFromPath(String filePath) {
        File file = new File(filePath);
        return file.getName();
    }


    private void sendFileRequest() throws IOException {
        BufferedReader consoleReader = new BufferedReader(new InputStreamReader(System.in));
        System.out.print("What request do you want to make?");
        String fileDescription = consoleReader.readLine();
        //System.out.print("Enter request ID: ");
        long requestId = System.currentTimeMillis();

        writer.println("r"); // Send the command to the server
        writer.println(fileDescription); // Send the file description
        writer.println(requestId); // Send the request ID

        // Handle the server's response (if needed)
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
