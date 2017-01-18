package bgu.spl171.net.impl.TFTPimplDown;

import bgu.spl171.net.api.bidi.Connections;
import bgu.spl171.net.srv.ConnectionHandler;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

/**
 * Created by blumenra on 1/18/17.
 */
public class ConnectionsImpl<T> implements Connections<T> {

    Map<Integer, ConnectionHandler> clients = new HashMap<>();

    @Override
    public boolean send(int connectionId, T msg) {

        clients.get(connectionId).send(msg);
        //TODO: decide when to return what boolean
        return false;
    }

    @Override
    public void broadcast(T msg) {

        for(Integer key : clients.keySet()) {

            clients.get(key).send(msg);
        }
    }

    @Override
    public void disconnect(int connectionId) {

    }
}
