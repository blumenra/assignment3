package bgu.spl171.net.impl.TFTPimplDown;

import bgu.spl171.net.api.MessageEncoderDecoder;
import bgu.spl171.net.impl.TFTPimplDown.BidiMessage;

/**
 * Created by blumenra on 1/18/17.
 */
public class BidiEncDecImpl<T> implements MessageEncoderDecoder<BidiMessage>{
    @Override
    public BidiMessage decodeNextByte(byte nextByte) {
        return null;
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

    private void putInByteArray(byte[] toPut, byte[] container, int fromIndex){

        for(int putIndex = 0, containerIndex = fromIndex; putIndex < toPut.length ; putIndex++, containerIndex++){

            container[containerIndex] = toPut[putIndex];
        }
    }

}
