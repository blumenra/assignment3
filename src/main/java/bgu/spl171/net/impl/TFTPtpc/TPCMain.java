package bgu.spl171.net.impl.TFTPtpc;

import bgu.spl171.net.srv.BaseServer;
import bgu.spl171.net.srv.BlockingConnectionHandler;

import java.util.function.Supplier;

/**
 * Created by blumenra on 1/17/17.
 */
public class TPCMain extends BaseServer<> {

    public TPCMain(int port, Supplier protocolFactory, Supplier encdecFactory) {
        super(port, protocolFactory, encdecFactory);
    }

    public static void main(String[] args) {

        System.out.println("bla");
    }

    @Override
    protected void execute(BlockingConnectionHandler handler) {

    }
}
