//
//import java.io.*;
//import java.net.*;
//import java.util.*;
//
//public class FileServer {
//    private Map<String, ClientHandler> connectedClients; // Map to store connected clients
//    private static final int MAX_BUFFER_SIZE = 10485760; // Maximum size of combined chunks in bytes
//    private static final int MIN_CHUNK_SIZE = 10240; // Minimum chunk size in bytes
//    private static final int MAX_CHUNK_SIZE = 1048576; // Maximum chunk size in bytes
//
//    public FileServer() {
//        connectedClients = new HashMap<>();
//    }
//
//    public void start(int port) {
//        try (ServerSocket serverSocket = new ServerSocket(port)) {
//            System.out.println("File Server started on port " + port);
//
//            while (true) {
//                Socket clientSocket = serverSocket.accept();
//                ClientHandler clientHandler = new ClientHandler(clientSocket);
//                clientHandler.start();
//            }
//        } catch (IOException e) {
//            e.printStackTrace();
//        }
//    }
//
//    private class ClientHandler extends Thread {
//        private Socket clientSocket;
//        private PrintWriter out;
//        private BufferedReader in;
//        private String username;
//        private Map<Integer, FileInfo> fileInfos; // Map to store file information
//
//        public ClientHandler(Socket socket) {
//            clientSocket = socket;
//            fileInfos = new HashMap<>();
//        }
//
//        public void run() {
//            try {
//                out = new PrintWriter(clientSocket.getOutputStream(), true);
//                in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
//
//                // Handle client login
//                handleLogin();
//
//                // Handle client requests
//                String request;
//                while ((request = in.readLine()) != null) {
//                    handleRequest(request);
//                }
//
//                // Handle client logout
//                handleLogout();
//
//                in.close();
//                out.close();
//                clientSocket.close();
//            } catch (IOException e) {
//                e.printStackTrace();
//            }
//        }
//
//        private void handleLogin() throws IOException {
//            // Read client username
//            username = in.readLine();
//
//            // Check if the username is already connected
//            if (connectedClients.containsKey(username)) {
//                out.println("ERROR: Username already connected");
//                clientSocket.close();
//                return;
//            }
//
//            // Create directory for the client
//            createClientDirectory(username);
//
//            // Add the client to the connected clients map
//            connectedClients.put(username, this);
//
//            // Send login success message to the client
//            out.println("Login successful");
//        }
//
//        private void handleRequest(String request) {
//            // Handle client requests based on the request type
//            // e.g., list connected clients, upload file, download file, etc.
//        }
//
//        private void handleLogout() {
//            // Remove the client from connected clients
//            connectedClients.remove(username);
//
//            // Discard incomplete files if uploader goes offline
//            for (FileInfo fileInfo : fileInfos.values()) {
//                if (fileInfo.status == FileStatus.IN_PROGRESS && fileInfo.uploader.equals(username)) {
//                    deleteChunks(fileInfo.fileID);
//                }
//            }
//
//            // Perform any necessary cleanup tasks
//        }
//
//        // Other methods for handling various client requests and file management
//
//        private void createClientDirectory(String username) {
//            // Create a directory with the client's username
//            File directory = new File(username);
//            if (!directory.exists()) {
//                directory.mkdir();
//            }
//        }
//
//        private void deleteChunks(int fileID) {
//            // Delete all chunks for a given fileID
//            // This method should be called when an uploader goes offline or a transmission is interrupted
//            FileInfo fileInfo = fileInfos.get(fileID);
//            if (fileInfo != null) {
//                for (int i = 0; i < fileInfo.numChunks; i++) {
//                    File chunkFile = new File(fileInfo.directory, getChunkFileName(fileID, i));
//                    chunkFile.delete();
//                }
//            }
//        }
//
//        private String getChunkFileName(int fileID, int chunkNumber) {
//            return "chunk_" + fileID + "_" + chunkNumber;
//        }
//
//        private int generateChunkSize() {
//            // Generate a random chunk size between MIN_CHUNK_SIZE and MAX_CHUNK_SIZE
//            return new Random().nextInt(MAX_CHUNK_SIZE - MIN_CHUNK_SIZE + 1) + MIN_CHUNK_SIZE;
//        }
//
//        private class FileInfo {
//            private String fileName;
//            private int fileSize;
//            private int chunkSize;
//            private int numChunks;
//            private String directory;
//            private String uploader;
//            private FileStatus status;
//
//            public FileInfo(String fileName, int fileSize, String directory, String uploader) {
//                this.fileName = fileName;
//                this.fileSize = fileSize;
//                this.directory = directory;
//                this.uploader = uploader;
//                this.status = FileStatus.IN_PROGRESS;
//
//                this.chunkSize = generateChunkSize();
//                this.numChunks = (int) Math.ceil((double) fileSize / chunkSize);
//            }
//        }
//
//        // Maint
//        private Map<Integer, FileInfo> fileInfos;
//        private static enum FileStatus {
//            IN_PROGRESS,
//            COMPLETED,
//            ERROR
//        }
//    }
//
//    public static void main(String[] args) {
//        FileServer server = new FileServer();
//        server.start(8080);
//    }
//}
//
