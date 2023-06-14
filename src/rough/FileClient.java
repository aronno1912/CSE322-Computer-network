package rough;

import java.io.*;
import java.net.*;

public class FileClient {
    private String serverAddress;
    private int serverPort;
    private String username;
    private Socket socket;
    private PrintWriter out;
    private BufferedReader in;
    private static final int MAX_CHUNK_SIZE = 1024;

    public FileClient(String serverAddress, int serverPort) {
        this.serverAddress = serverAddress;
        this.serverPort = serverPort;
    }

    public void connect() {
        try {
            socket = new Socket(serverAddress, serverPort);
            out = new PrintWriter(socket.getOutputStream(), true);
            in = new BufferedReader(new InputStreamReader(socket.getInputStream()));

            // Send username to the server
            out.println(username);

            // Receive login response from the server
            String response = in.readLine();
            System.out.println(response);

            // Handle user input and perform actions
            // e.g., list connected clients, upload file, download file, etc.

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        String serverAddress = "localhost";
        int serverPort = 8080;
        String username = "John"; // Specify the client's username

        FileClient client = new FileClient(serverAddress, serverPort);
        client.setUsername(username);
        client.connect();
    }

    public void setUsername(String username) {
        this.username = username;
    }

    // Other methods for handling user input, sending requests to the server, and receiving responses

    private void uploadFile(String filePath, boolean isPublic) {
        try {
            File file = new File(filePath);

            // Send file name and file size to the server
            out.println(file.getName());
            out.println(file.length());

            // Receive confirmation message from the server
            String confirmation = in.readLine();
            if (confirmation.startsWith("SUCCESS:")) {
                int fileID = Integer.parseInt(confirmation.substring(9));

                // Split file into chunks and send to the server
                FileInputStream fis = new FileInputStream(file);
                byte[] buffer = new byte[MAX_CHUNK_SIZE];
                int bytesRead;
                int chunkNumber = 0;

                while ((bytesRead = fis.read(buffer)) != -1) {
                    // Create a new chunk
                    byte[] chunk = new byte[bytesRead];
                    System.arraycopy(buffer, 0, chunk, 0, bytesRead);

                    // Send the chunk to the server
                    sendChunk(fileID, chunkNumber, chunk);

                    // Wait for acknowledgement from the server
                    if (!waitForAcknowledgement()) {
                        System.out.println("Timeout occurred. Upload terminated.");
                        deleteIncompleteChunks(fileID, chunkNumber);
                        break;
                    }

                    chunkNumber++;
                }

                // Send completion message to the server
                out.println("COMPLETE:" + fileID);

                // Wait for upload status from the server
                String uploadStatus = in.readLine();
                if (uploadStatus.startsWith("SUCCESS:")) {
                    System.out.println(uploadStatus);
                } else {
                    System.out.println("Upload failed: " + uploadStatus);
                    deleteIncompleteChunks(fileID, chunkNumber);
                }

                fis.close();
            } else {
                System.out.println("Upload failed: " + confirmation);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void sendChunk(int fileID, int chunkNumber, byte[] chunk) {
        // Implement code to send the chunk to the server
        // You can use sockets or other appropriate methods to send the chunk
    }

    private boolean waitForAcknowledgement() {
        // Implement code to wait for acknowledgement from the server
        // Return true if acknowledgement received within timeout period, false otherwise
        return true; // Placeholder implementation
    }

    private void deleteIncompleteChunks(int fileID, int lastChunkNumber) {
        // Implement code to delete incomplete chunks from the server
    }
}
