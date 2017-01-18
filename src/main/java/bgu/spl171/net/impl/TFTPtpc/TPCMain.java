package bgu.spl171.net.impl.TFTPtpc;


import bgu.spl171.net.api.bidi.BidiMessagingProtocol;
import bgu.spl171.net.srv.Server;

import java.util.Set;
import java.util.concurrent.ConcurrentSkipListSet;

/**
 * Created by blumenra on 1/17/17.
 */
public class TPCMain {

    public static void main(String[] args) {

        Set<String> filesList = new ConcurrentSkipListSet<>();

        Server.threadPerClient(
                7777, //port
                () -> new BidiMessagingProtocol<>(filesList), //protocol factory
                TFTPEncoderDecoder::new //message encoder decoder factory
        ).serve();
    }

}
