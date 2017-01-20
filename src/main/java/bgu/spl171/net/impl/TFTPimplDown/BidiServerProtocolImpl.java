package bgu.spl171.net.impl.TFTPimplDown;

import bgu.spl171.net.api.bidi.BidiMessagingProtocol;
import bgu.spl171.net.api.bidi.Connections;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

/**
 * Created by blumenra on 1/18/17.
 */
public class BidiServerProtocolImpl implements BidiMessagingProtocol<BidiMessage> {

    private final Map<String, Boolean> filesList;
    private ConnectionsImpl connections;
    private int ownerClientId;


    public BidiServerProtocolImpl(Map<String, Boolean> filesList) {

        this.filesList = filesList;
    }



    @Override
    public void start(int connectionId, Connections<BidiMessage> connections) {

        this.connections = (ConnectionsImpl) connections;
        this.ownerClientId = connectionId;

        initializeFilesMap();
    }

    @Override
    public void process(BidiMessage message) {

        short opcode = message.getOpcode();
        BidiMessage response = null;

        // Illegal Opcode
        if (message.getOpcode() == -1) {

            sendIllegalOpcodeError(message);
        }
        // attempt to do something before logging in
        else if (!connections.isLoggedIn(ownerClientId) && (opcode != 7)) {

            sendPleaseLoginFirstError(message);
        }
        else {
            switch (opcode) {

                //TODO: when  file was deleted/added => broadcast the shit out of it!!

                case 1: //RRQ

                    synchronized (filesList) {

                        if(rejectIfNotAccessible(message)) {

                            break;
                        }
                        else {

                            filesList.put(message.getFileName(), false);
                        }
                    }

                    try {
                        byte[] array = Files.readAllBytes(new File("Files/"+message.getFileName()).toPath());
                        filesList.put(message.getFileName(), true);

                        int x = 512;  // chunk size
                        int len = array.length;
                        int counter = 0;

                        for (int i = 0; i < len - x + 1; i += x) {

                            byte[] newArray = Arrays.copyOfRange(array, i, i + x);
                            response = BidiMessage.createDataMessage(newArray.length, counter++, newArray);
                            connections.send(ownerClientId, response);
                        }

                        if (len % x != 0) {
                            byte[] newArray = Arrays.copyOfRange(array, len - len % x, len);
                            response = BidiMessage.createDataMessage(newArray.length, counter, newArray);
                            connections.send(ownerClientId, response);
                        }

                    } catch (IOException e) {
                        response = BidiMessage.createErrorMessage(6, "User not logged in – Any opcode received before Login completes.");
                        connections.send(ownerClientId, response);
                    }

                    break;

                case 7: //LOGRQ
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

                    synchronized (filesList) {

                        // Files does NOT exist
                        if (!filesList.containsKey(message.getFileName())) {

                            response = BidiMessage.createErrorMessage(1, "File not found – RRQ \\ DELRQ of non-existing file");
                        }
                        // File exists but not accessible
                        else if (!filesList.get(message.getFileName())) {

                            response = BidiMessage.createErrorMessage(2, "Access violation – File cannot be written, read or deleted");
                        } else {

                            filesList.put(message.getFileName(), false);
                            response = BidiMessage.createAckMessage(0);
                        }
                    }

                    removeFile(message.getFileName()); //remove file from directory and map

                    connections.broadcast(BidiMessage.createBcastMessage(0, message.getFileName())); // broadcast all user of the deletion

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

                filesList.put(file.getName(), true);
            }
        }
    }

    private void sendIllegalOpcodeError(BidiMessage message) {

        BidiMessage response = BidiMessage.createErrorMessage(4, "Illegal TFTP operation – Unknown Opcode.");
        connections.send(ownerClientId, response);
    }

    private void sendPleaseLoginFirstError(BidiMessage message) {

        BidiMessage response = BidiMessage.createErrorMessage(6, "User not logged in – Any opcode received before Login completes.");
        connections.send(ownerClientId, response);
    }

    private void removeFile(String fileName) {

        File fileToDelete = new File("Files/" + fileName);
        fileToDelete.delete();
        filesList.remove(fileName);
    }

    private boolean rejectIfNotAccessible(BidiMessage message) {

        boolean ans = false;
        BidiMessage response = null;

        // Files does NOT exist
        if (!filesList.containsKey(message.getFileName())) {

            response = BidiMessage.createErrorMessage(1, "File not found – RRQ \\ DELRQ of non-existing file");
            ans = true;
        }
        // File exists but not accessible
        else if (!filesList.get(message.getFileName())) {

            response = BidiMessage.createErrorMessage(2, "Access violation – File cannot be written, read or deleted");
            ans = true;
        }

        if(ans)
            connections.send(ownerClientId, response);

        return ans;
    }
}
