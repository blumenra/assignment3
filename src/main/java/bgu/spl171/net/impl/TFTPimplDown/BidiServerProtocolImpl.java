package bgu.spl171.net.impl.TFTPimplDown;

import bgu.spl171.net.api.bidi.BidiMessagingProtocol;
import bgu.spl171.net.api.bidi.Connections;

import java.io.*;
import java.nio.file.Files;
import java.util.Arrays;
import java.util.Map;

/**
 * Created by blumenra on 1/18/17.
 */
public class BidiServerProtocolImpl implements BidiMessagingProtocol<BidiMessage> {

    private final Map<String, BidiFile> filesList;
    private ConnectionsImpl<BidiMessage> connections;
    private int ownerClientId;
    private ByteArrayOutputStream byteOutPutStream = new ByteArrayOutputStream();
    private String writingFileName = "";


    public BidiServerProtocolImpl(Map<String, BidiFile> filesList) {

        this.filesList = filesList;
    }



    @Override
    public void start(int connectionId, Connections<BidiMessage> connections) {

        this.connections = (ConnectionsImpl<BidiMessage>) connections;
        this.ownerClientId = connectionId;

        initializeFilesMap();
    }

    @Override
    public void process(BidiMessage message) {

        System.out.println("protocol opcode: " + message.getOpcode());
        short opcode = message.getOpcode();
        BidiMessage response = null;

        // Illegal Opcode
        if (message.getOpcode() == -1) {

            sendIllegalOpcodeError();
        }
        // attempt to do something before logging in
        else if (!connections.isLoggedIn(ownerClientId) && (opcode != 7)) {

            sendPleaseLoginFirstError();
        }
        else {
            switch (opcode) {

                //TODO: when  file was deleted/added => broadcast the shit out of it!!

                case 1: //RRQ

                    synchronized (filesList){

                        if(!filesList.containsKey(message.getFileName())) {

                            response = BidiMessage.createErrorMessage(1, "File not found – RRQ \\ DELRQ of non-existing file");
                            connections.send(ownerClientId, response);
                            break;
                        }
                        else if(!filesList.get(message.getFileName()).isReadable()) {

                            response = BidiMessage.createErrorMessage(2, "Access violation – File cannot be written, read or deleted");
                            connections.send(ownerClientId, response);
                            break;
                        }
                        else {

                            filesList.get(message.getFileName()).setDeleable(false);
                        }
                    }

                    try {
                        byte[] array = Files.readAllBytes(new File("Files/"+message.getFileName()).toPath());
                        filesList.get(message.getFileName()).setDeleable(true);

                        sendDataMessages(array);

                    } catch (IOException e) {
                        response = BidiMessage.createErrorMessage(6, "User not logged in – Any opcode received before Login completes.");
                        connections.send(ownerClientId, response);
                    }

                    break;

                case 2: //WRQ

                    synchronized (filesList) {

                        if(filesList.containsKey(message.getFileName())) {

                            response = BidiMessage.createErrorMessage(5, "File already exists – File name exists on WRQ.");
                            connections.send(ownerClientId, response);
                            break;
                        }
                        else if(filesList.get(message.getFileName()).isUploading()) {

                            response = BidiMessage.createErrorMessage(2, "Access violation – File cannot be written, read or deleted.");
                            connections.send(ownerClientId, response);
                            break;
                        }
                        else {

                            filesList.get(message.getFileName()).setUploading(true);
                            response = BidiMessage.createAckMessage(0);
                            writingFileName = message.getFileName();
                        }
                    }

                    connections.send(ownerClientId, response);

                case 3: //DATA

                    if(message.getPacketSize() < 512) {

                        FileOutputStream fos = null;
                        try {

                            byteOutPutStream.write(message.getData());

                            fos = new FileOutputStream("Files/" + writingFileName);
                            fos.write(byteOutPutStream.toByteArray());
                            fos.close();

                            byteOutPutStream.reset();

                            filesList.get(writingFileName).setUploading(false);

                            connections.broadcast(BidiMessage.createBcastMessage(1, message.getFileName()));

                            response = BidiMessage.createAckMessage(message.getBlockNumber());
                            writingFileName = "";

                        } catch (FileNotFoundException e) {
                            response = BidiMessage.createErrorMessage(1, "File not found – RRQ \\ DELRQ of non-existing file");
                        } catch (IOException e) {
                            response = BidiMessage.createErrorMessage(0, e.getMessage());
                        }
                    }
                    else {

                        try {

                            byteOutPutStream.write(message.getData());
                            response = BidiMessage.createAckMessage(message.getBlockNumber());

                        } catch (IOException e) {
                            response = BidiMessage.createErrorMessage(0, e.getMessage());
                        }
                    }

                    connections.send(ownerClientId, response);
                    break;

                case 6: //DIRQ

                    String files = "";

                    synchronized (filesList) {

                        for(String key : filesList.keySet()) {

                            if(filesList.get(key).isReadable()){

                                files += key + "-1";
                            }
                        }
                    }


                    byte[] byteFiles = files.getBytes();
                    System.out.println("bytesFiles: " + Arrays.toString(byteFiles));
                    sendDataMessages(byteFiles);
                    break;

                case 7: //LOGRQ

                    System.out.println("user name: " + message.getUserName());
                    String userName = message.getUserName();

                    // if the user is NOT logged in
                    if (!connections.isLoggedIn(ownerClientId)) {

                        connections.getLoggedInUsers().put(ownerClientId, userName);
                        response = BidiMessage.createAckMessage(0);
                    } else {

                        response = BidiMessage.createErrorMessage(7, "User already logged in – Login username already connected.");
                    }

                    connections.send(ownerClientId, response);
                    break;

                case 8: //DELRQ

                    boolean toDelete = false;

                    synchronized (filesList) {

                        // Files does NOT exist
                        if (!filesList.containsKey(message.getFileName())) {

                            System.out.println("exist");
                            response = BidiMessage.createErrorMessage(1, "File not found – RRQ \\ DELRQ of non-existing file");
                        }
                        // File exists but not accessible
                        else if (!filesList.get(message.getFileName()).isDeletable()) {
                            System.out.println("access");
                            response = BidiMessage.createErrorMessage(2, "Access violation – File cannot be written, read or deleted");
                        }
                        else {

                            filesList.remove(message.getFileName());
                            response = BidiMessage.createAckMessage(0);
                            connections.broadcast(BidiMessage.createBcastMessage(0, message.getFileName())); // broadcast all user of the deletion
                            toDelete = true;
                        }
                    }

                    if(toDelete) {

                        removeFile(message.getFileName()); //remove file from directory and map
                    }

                    connections.send(ownerClientId, response);
                    break;


                case 10: //DISC

                    connections.disconnect(ownerClientId);
                    response = BidiMessage.createAckMessage(0);

                    connections.send(ownerClientId, response);
                    break;
            }
        }

    }

    @Override
    public boolean shouldTerminate() {
        return false;
    }

    private void initializeFilesMap() {

        File folder = new File("Files/");
        File[] listOfFiles = folder.listFiles();
        for(File file : listOfFiles) {

            if(file.isFile()) {

                filesList.put(file.getName(), new BidiFile(file.getName()));
            }
        }
    }

    private void sendIllegalOpcodeError() {

        BidiMessage response = BidiMessage.createErrorMessage(4, "Illegal TFTP operation – Unknown Opcode.");
        connections.send(ownerClientId, response);
    }

    private void sendPleaseLoginFirstError() {

        BidiMessage response = BidiMessage.createErrorMessage(6, "User not logged in – Any opcode received before Login completes.");
        connections.send(ownerClientId, response);
    }

    private void sendDataMessages(byte[] data) {

        int x = 512;  // chunk size
        int len = data.length;
        int counter = 0;

        for (int i = 0; i < len - x + 1; i += x) {

            byte[] newArray = Arrays.copyOfRange(data, i, i + x);
            BidiMessage response = BidiMessage.createDataMessage(newArray.length, counter++, newArray);
            connections.send(ownerClientId, response);
        }

        if (len % x != 0) {
            byte[] newArray = Arrays.copyOfRange(data, len - len % x, len);
            BidiMessage response = BidiMessage.createDataMessage(newArray.length, counter+1, newArray);
            connections.send(ownerClientId, response);
        }
    }

    private void removeFile(String fileName) {

        File fileToDelete = new File("Files/" + fileName);
        fileToDelete.delete();
    }
}
