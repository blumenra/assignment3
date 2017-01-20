package bgu.spl171.net.impl.TFTPimplDown;

import bgu.spl171.net.api.MessageEncoderDecoder;

import java.util.ArrayList;

/**
 * Created by blumenra on 1/18/17.
 */
public class BidiEncDecImpl implements MessageEncoderDecoder<BidiMessage>{

    private ArrayList<Byte> incomingBytes;
    private BidiMessage incomingMessage;
    private int currentMessageFieldNumber;


    @Override
    public BidiMessage decodeNextByte(byte nextByte) {

        boolean done = false;

        if(incomingBytes == null){

            incomingMessage = new BidiMessage();
            currentMessageFieldNumber = 0;
        }

        if(currentMessageFieldNumber == 0){

            bytesToOpcode(nextByte);
        }
        else{

            short opcode = incomingMessage.getOpcode();

            switch (opcode){

//                DELRQ, RRQ, WRQ
                case 1:
                case 2:
                case 8: {

                    done = delrqRrqWrqMessageCreator(nextByte);
                }

//                DATA
                case 3: {

                    done = dataMessageCreator(nextByte);
                }

//                ACK
                case 4: {

                    done = ackMessageCreator(nextByte);
                }
            }
        }


        if(done){
            BidiMessage newMessage = new BidiMessage(this.incomingMessage);

            doneWithMessage();
            return newMessage;
        }
        else {

            return null;
        }
    }

    private boolean ackMessageCreator(byte nextByte) {

        boolean done = false;

        if(currentMessageFieldNumber == 1){

            done = bytesToBlockNumber(nextByte);
        }
        return done;
    }

    private void doneWithMessage() {

        this.incomingMessage = null;
        doneWithField();
        currentMessageFieldNumber = 0;
    }

    private boolean dataMessageCreator(byte nextByte) {

        boolean done = false;

        switch (currentMessageFieldNumber){

//            working on the packetSize
            case 1: {

                bytesToPacketSize(nextByte);
            }

//            working on blockNumber
            case 2: {

                bytesToBlockNumber(nextByte);
            }

//            working on data
            case 3: {

                done = bytesToData(nextByte);
            }
        }

        return done;
    }

    private boolean bytesToData(byte nextByte) {

        incomingBytes.add(nextByte);

        if(incomingMessage.getPacketSize() != (short) 512){

            byte[] data = byteListToArr();
            incomingMessage.setData(data);
            doneWithField();
            return true;
        }

        return false;
    }

    private boolean bytesToBlockNumber(byte nextByte) {

        short blockNumber = incomingBytesToShort(nextByte);

        if(blockNumber != -1){

            incomingMessage.setPacketSize(blockNumber);
            doneWithField();
            return true;
        }
        return false;
    }

    private boolean bytesToPacketSize(byte nextByte) {

        short packetSize = incomingBytesToShort(nextByte);

        if(packetSize != -1){

            incomingMessage.setPacketSize(packetSize);
            doneWithField();
            return true;
        }

        return false;
    }

    private boolean delrqRrqWrqMessageCreator(byte nextByte) {

        boolean done = false;

        String fileName = incomingBytesToString(nextByte);

        if(currentMessageFieldNumber == 1){

            if(fileName != null){

                incomingMessage.setFileName(fileName);
                incomingMessage.setaByte(nextByte);
                done = true;
            }
            else{

                incomingBytes.add(nextByte);
            }

        }
        return done;
    }

    private String incomingBytesToString(byte nextByte) {

            if(nextByte == (byte) 0){

                String string = bytesToString();
                doneWithField();
                return string;
            }
            else{

                incomingBytes.add(nextByte);
                return null;
            }
    }

    private String bytesToString() {

        byte[] fileNameBytes = byteListToArr();

        String string = new String(fileNameBytes);

        return string;
    }

    private byte[] byteListToArr() {

        byte[] byteArr = new byte[incomingBytes.size()];

        for(int i=0; 1<byteArr.length-1; i++){

            byteArr[i] = incomingBytes.get(i);
        }

        return byteArr;
    }

    private void bytesToOpcode(byte nextByte) {

        short opcode = incomingBytesToShort(nextByte);

        if(opcode != -1){

            incomingMessage.setOpcode(opcode);
        }
    }

    private short incomingBytesToShort(byte nextByte) {

        incomingBytes.add(nextByte);

        if(incomingBytes.size() == 2){

            byte[] shortBytes = {incomingBytes.get(0), incomingBytes.get(1)};
            short num = bytesToShort(shortBytes);

            doneWithField();

            return num;
        }
        return (short) -1;
    }

    private void doneWithField() {

        emptyIncomingBytes();
        currentMessageFieldNumber++;
    }

    private void emptyIncomingBytes(){

        for(byte aByte : incomingBytes){

            incomingBytes.remove(aByte);
        }
    }

    @Override
    public byte[] encode(BidiMessage message) {

        byte[] encoded;

        short msgType = message.getOpcode();
        byte[] msgTypeBytes = shortToBytes(msgType);

        switch (msgType){

//            DELRQ, RRQ, WRQ
            case 1:
            case 2:
            case 8:{

                String fileName = message.getFileName();
                byte[] fileNameBytes = fileName.getBytes();

                byte[] aByte = {message.getaByte()};

                encoded = new byte[fileNameBytes.length + 3];

                putInByteArray(msgTypeBytes, encoded, 0);
                putInByteArray(fileNameBytes, encoded, 2);
                putInByteArray(aByte, encoded, encoded.length-1);

            }

//            DATA
            case 3: {

                short packetSize = message.getPacketSize();
                byte[] packetSizeBytes = shortToBytes(packetSize);

                short blockNumber = message.getBlockNumber();
                byte[] blockNumberBytes = shortToBytes(blockNumber);

                byte[] data = message.getData();

                encoded = new byte[data.length + 7];

                putInByteArray(msgTypeBytes, encoded, 0);
                putInByteArray(packetSizeBytes, encoded, 2);
                putInByteArray(blockNumberBytes, encoded, 4);
                putInByteArray(data, encoded, 6);

            }

//            ACK
            case 4: {

                short blockNumber = message.getBlockNumber();
                byte[] blockNumberBytes = shortToBytes(blockNumber);

                encoded = new byte[4];

                putInByteArray(msgTypeBytes, encoded, 0);
                putInByteArray(blockNumberBytes, encoded, 2);
            }

//            ERROR
            case 5: {

                short errorCode = message.getErrorCode();
                byte[] errorCodeBytes = shortToBytes(errorCode);

                String errMsg = message.getErrMsg();
                byte[] errMsgBytes = errMsg.getBytes();

                byte[] aByte = {message.getaByte()};

                encoded = new byte[errMsgBytes.length + 5];

                putInByteArray(msgTypeBytes, encoded, 0);
                putInByteArray(errorCodeBytes, encoded, 2);
                putInByteArray(errMsgBytes, encoded, 4);
                putInByteArray(aByte, encoded, encoded.length-1);

            }

//            DIRQ, DISC
            case 6:
            case 10: {

                encoded = msgTypeBytes;
            }

//            LOGRQ
            case 7: {

                String userName = message.getUserName();
                byte[] userNameBytes = userName.getBytes();

                byte[] aByte = {message.getaByte()};

                encoded = new byte[userNameBytes.length + 3];

                putInByteArray(msgTypeBytes, encoded, 0);
                putInByteArray(userNameBytes, encoded, 2);
                putInByteArray(aByte, encoded, encoded.length-1);
            }

//            BCAST
            case 9: {

                byte[] deletedAdded = {message.getDeletedAdded()};

                String fileName = message.getFileName();
                byte[] fileNameBytes = fileName.getBytes();

                byte[] aByte = {message.getaByte()};

                encoded = new byte[fileNameBytes.length + 4];

                putInByteArray(msgTypeBytes, encoded, 0);
                putInByteArray(deletedAdded, encoded, 2);
                putInByteArray(fileNameBytes, encoded, 3);
                putInByteArray(aByte, encoded, encoded.length-1);

            }

            default:{
                encoded = new byte[0];
            }
        }

        return encoded;
    }

    private byte[] shortToBytes(short num) {

        byte[] bytesArr = new byte[2];
        bytesArr[0] = (byte)((num >> 8) & 0xFF);
        bytesArr[1] = (byte)(num & 0xFF);
        return bytesArr;
    }

    public short bytesToShort(byte[] byteArr) {

        short result = (short)((byteArr[0] & 0xff) << 8);
        result += (short)(byteArr[1] & 0xff);
        return result;
    }

    private void putInByteArray(byte[] toPut, byte[] container, int fromIndex){

        for(int putIndex = 0, containerIndex = fromIndex; putIndex < toPut.length ; putIndex++, containerIndex++){

            container[containerIndex] = toPut[putIndex];
        }
    }

}
