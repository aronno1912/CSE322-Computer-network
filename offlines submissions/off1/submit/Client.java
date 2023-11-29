package submit;

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
    private String myOwnUserName;


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
            myOwnUserName=username;

            // Send the username to the server
            writer.println(username);

            // Receive and display the server's response
            serverMessage = reader.readLine();
            if(serverMessage.equalsIgnoreCase("Username already in use. Connection terminated."))
            {
                System.out.println(serverMessage);
                return;
            }
            System.out.println(serverMessage);

            // Perform file operations based on user commands
            String command;


            while (true)

            {
                System.out.println("======================================================");
                System.out.print("Enter a command: ");

                System.out.println("Press u to upload");
                System.out.println("Press d to download");
                System.out.println("Press l to see the userList on this server");
                System.out.println("Press r to request for any file");
                System.out.println("Write 'lookup' to lookup for any publicly uploaded files of users");
                System.out.println("Write 'my' to see own uploaded files");
                System.out.println("Press v to see own inbox messages");
                System.out.println("Press q to go offline");
                System.out.println("======================================================");
                command = userInputReader.readLine();


                if (command.equalsIgnoreCase("q"))
                {
                    writer.println(command); // clienthandler handle korbe
                    System.out.println("Going offline...");
                    System.out.println(reader.readLine());
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

                else if (command.equalsIgnoreCase("my"))
                {
                    handleOwnFileListSeperatively();

                }

                else if (command.equalsIgnoreCase("v"))
                {
                    viewMsg();

                }

                else if (command.equalsIgnoreCase("d"))
                {
                    handleFileDownloadFromConsole();

                }

                else if (command.equalsIgnoreCase("l"))
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

                else
                {

                    System.out.println("Invalid command....!");


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

    }

    private void handleOwnFileListSeperatively() throws IOException {
        writer.println("my");
        writer.flush();

        // Receive and display the list of files
        String response;
        response=reader.readLine();
        while (!(response.equals(END_OF_RESPONSE)) )
        {
            System.out.println(response);
            response=reader.readLine();


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

        String filename = file.getName();
        writer.println("u");
        writer.println(filename); //to send commands and data to server
        writer.println(visibility);     // public or private
        writer.println(ans);
        writer.println((reqID));

         long fileSz=file.length();
        //System.out.println(fileSz);
        writer.println((fileSz));
       // System.out.println("size disi "+fileSz);
        String said=reader.readLine();
        if(said.equalsIgnoreCase("yes"))
        {
            int allocatedChunkSize = Integer.parseInt(reader.readLine());
            String fileId=reader.readLine();
            //System.out.println("allocated "+allocatedChunkSize);
            int ct = (int) Math.ceil(fileSz / allocatedChunkSize);
            if(ct==0)ct++;
            writer.println(ct);
            //System.out.println("num of chunk "+ct);

            boolean timeout=false;
            //okay jinishhhhhhhhhhhhhhh
            try (BufferedInputStream fileInputStream = new BufferedInputStream(new FileInputStream(file))) {
                byte[] buffer = new byte[allocatedChunkSize];  // Adjust the buffer size as per your requirements
                int bytesRead;
                while ((bytesRead = fileInputStream.read(buffer)) != -1) {
                    writer.println(new String(buffer, 0, bytesRead));  // Send chunk to the server
                    long startTime = System.currentTimeMillis();
                    String acknowledgement = reader.readLine();  // Receive acknowledgement from the server
                    long endTime = System.currentTimeMillis();
                    long elapsedTimeInSeconds = (endTime - startTime) / 1000;
                    if(elapsedTimeInSeconds>30)
                    {
                        timeout=true;
                        break;
                    }
                    if (!acknowledgement.equals("OK")) {
                        System.out.println(acknowledgement);

                        System.out.println("Error: Chunk not acknowledged by the server");
                        break;
                    } else {
                        System.out.println(acknowledgement);
                        //System.out.println(bytesRead);
                        System.out.println("Uploading  chunk");
                    }
                }
                 if(timeout)writer.println("Timeout");
                 else
                writer.println(END_OF_FILE_MARKER);  // Send end marker to the server
            }
            if(timeout)
                System.out.println("Response Timeout!!!!");
            else{
            writer.flush();
            String response = reader.readLine(); //server er response pawar jonno
            System.out.println("Server response: " + response);
                while (!(response.equals(END_OF_RESPONSE)) )
                {
                    System.out.println(response);
                    response=reader.readLine();


                }
            }
        }
        else
        {System.out.println(reader.readLine());
        reader.readLine();
        }
    }



    private void handleFileDownloadFromConsole() throws IOException {
        BufferedReader consoleReader = new BufferedReader(new InputStreamReader(System.in));
        System.out.println("Whose file do you want to download?");
        String clientName = consoleReader.readLine();
        System.out.println("Which file?");
        String filename = consoleReader.readLine();
        String path = "D:\\ServerFiles\\" + clientName + "\\" + filename;
        String savePath = "D:\\ClientFiles\\" + myOwnUserName + "\\" + filename;

        writer.println("d"); // Send the download command to the server
        writer.println(path); // Send the file path to the server
        writer.println(clientName);
        writer.println(filename);
       // System.out.println("pathaisi");
        String response = reader.readLine(); // Read the server's response
       // System.out.println(response);
        //System.out.println(response);
        if (response.equals("File found")) {
            System.out.println("Downloading file...");


            try (BufferedOutputStream fileOutputStream = new BufferedOutputStream(new FileOutputStream(savePath))) {
                String line;
                while ((line = reader.readLine()) != null && !line.equals(END_OF_FILE_MARKER)) {
                    byte[] buffer = line.getBytes();
                    fileOutputStream.write(buffer);
                    fileOutputStream.write('\n');
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

    private void viewMsg() throws IOException {

        writer.println("v");
        writer.flush();

        // Receive and display the list of files
        String response;
        response=reader.readLine();
        while (!(response.equals(END_OF_RESPONSE)) )
        {
            System.out.println(response);
            response=reader.readLine();


        }

    }


    public static void main(String[] args) {

        String host = "localhost";
        int port = 56565;

        Client client = new Client();
        client.start(host, port);

    }
}
