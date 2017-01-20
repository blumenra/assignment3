package bgu.spl171.net.impl.TFTPtpc;


import bgu.spl171.net.impl.TFTPimplDown.*;
import bgu.spl171.net.srv.Server;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentSkipListSet;

/**
 * Created by blumenra on 1/17/17.
 */
public class TPCMain {

    public static void main(String[] args) {

//        // REMOVE FROM HERE
//
//        File file = new File("Files/removeMe.txt");
//        System.out.println(file.exists());
//
//        // REMOVE UP TO HERE







        Map<String, BidiFile> filesList = new ConcurrentHashMap<>();

        //start the server
        Server.threadPerClient(
                7777, //port
                () -> new BidiServerProtocolImpl(filesList), //protocol factory
                BidiEncDecImpl::new //message encoder decoder factory
        ).serve();
    }

}
