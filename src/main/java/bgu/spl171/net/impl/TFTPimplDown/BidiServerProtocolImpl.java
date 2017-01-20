package bgu.spl171.net.impl.TFTPimplDown;

import bgu.spl171.net.api.bidi.BidiMessagingProtocol;
import bgu.spl171.net.api.bidi.Connections;
import bgu.spl171.net.srv.ConnectionHandler;

import java.io.File;
import java.io.FileNotFoundException;
import java.nio.file.Files;
import java.util.Set;

/**
 * Created by blumenra on 1/18/17.
 */
public class BidiServerProtocolImpl implements BidiMessagingProtocol<BidiMessage> {

    private Set<String> filesList;
    private ConnectionsImpl connections;
    private int ownerClientId;


    public BidiServerProtocolImpl(Set<String> filesList) {

        this.filesList = filesList;
    }



    @Override
    public void start(int connectionId, Connections<BidiMessage> connections) {

        this.connections = (ConnectionsImpl) connections;
        this.ownerClientId = connectionId;

        initializeFilesSet();
    }

    @Override
    public void process(BidiMessage message) {

        short opcode = message.getOpcode();
        BidiMessage response = null;

        switch(opcode) {

            //TODO: in all cases check if the user is logged in first!!
            //TODO: when  file was deleted/added => broadcast the shit out of it baby!!

            case 1: //RRQ

                if(!connections.isLoggedIn(ownerClientId)) {

                    response = BidiMessage.createErrorMessage(6, "User not logged in – Any opcode received before Login completes.");
                }
                else {

                    synchronized (filesList) {

                    }
                }

                break;

            case 7: //LOGRQ
                String userName = message.getUserName();

                // if the user is NOT logged in
                if(!connections.isLoggedIn(ownerClientId)) {

                    connections.getLoggedInUsers().put(ownerClientId, userName);
                    response = BidiMessage.createAckMessage(0);
                }
                else {

                    response = BidiMessage.createErrorMessage(7, "User already logged in – Login username already connected.");
                }

                connections.send(ownerClientId, response);
                break;

            case 8: //DELRQ

                if(!connections.isLoggedIn(ownerClientId)) {

                    response = BidiMessage.createErrorMessage(6, "User not logged in – Any opcode received before Login completes.");
                }
                else {

                    synchronized (filesList) {

                        if(filesList.contains(message.getFileName())) {

                            File fileToDelete = new File("Files/" + message.getFileName());
                            fileToDelete.delete();
                            filesList.remove(message.getFileName());

                            connections.broadcast(BidiMessage.createBcastMessage(0, message.getFileName()));
                            response = BidiMessage.createAckMessage(0);
                        }
                        else {

                            response = BidiMessage.createErrorMessage(1, "File not found – RRQ \\ DELRQ of non-existing file");
                        }
                    }
                }

                connections.send(ownerClientId, response);
                break;

            case 10: //DISC

                if(!connections.isLoggedIn(ownerClientId)) {

                    response = BidiMessage.createErrorMessage(6, "User not logged in – Any opcode received before Login completes.");
                }
                else {

                    connections.disconnect(ownerClientId);
                    response = BidiMessage.createAckMessage(0);
                }

                connections.send(ownerClientId, response);
                break;
        }

    }


    @Override
    public boolean shouldTerminate() {
        return false;
    }

    private void initializeFilesSet() {

        File folder = new File("Files/");
        File[] listOfFiles = folder.listFiles();
        for(File file : listOfFiles) {

            if(file.isFile()) {

                filesList.add(file.getName());
            }
        }
    }
}
