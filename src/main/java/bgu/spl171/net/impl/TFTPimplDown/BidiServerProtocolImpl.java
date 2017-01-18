package bgu.spl171.net.impl.TFTPimplDown;

import bgu.spl171.net.api.bidi.BidiMessagingProtocol;
import bgu.spl171.net.api.bidi.Connections;
import bgu.spl171.net.srv.ConnectionHandler;

import java.util.Set;

/**
 * Created by blumenra on 1/18/17.
 */
public class BidiServerProtocolImpl<T> implements BidiMessagingProtocol<T> {

    Set<String> filesList;
    Connections<ConnectionHandler> connections;
    int ownerClientId;


    public BidiServerProtocolImpl(Set<String> filesList) {

        this.filesList = filesList;
    }



    @Override
    public void start(int connectionId, Connections connections) {

        this.connections = connections;
        this.ownerClientId = connectionId;
    }

    @Override
    public void process(T message) {

    }


    @Override
    public boolean shouldTerminate() {
        return false;
    }
}
