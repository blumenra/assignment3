package bgu.spl171.net.impl.TFTPimplDown;


/**
 * Created by blumenra on 1/18/17.
 */
public class BidiMessage {

    private short opcode;
    private String fileName;
    private String userName;
    private short packetSize;
    private short blockNumber;
    private byte[] data;
    private byte deletedAdded;
    private short errorCode;
    private String errMsg;
    private byte aByte;


//    Constructors
    public BidiMessage(short opcode, String string, byte aByte) {
        this.opcode = opcode;
        switch (opcode){

            case 7: {
                this.userName = string;
            }

            case 1:
            case 2:
            case 8: {
                this.fileName = string;
            }


        }
        this.fileName = fileName;
        this.aByte = aByte;
    }

    public BidiMessage(short opcode) {

        this.opcode = opcode;
    }

    public BidiMessage(short opcode, short packetSize, short blockNumber, byte[] data) {

        this.opcode = opcode;
        this.packetSize = packetSize;
        this.blockNumber = blockNumber;
        this.data = data;
    }

    public BidiMessage(short opcode, short blockNumber) {

        this.opcode = opcode;
        this.blockNumber = blockNumber;
    }

    public BidiMessage(short opcode, byte deletedAdded, String fileName, byte aByte) {

        this.opcode = opcode;
        this.deletedAdded = deletedAdded;
        this.fileName = fileName;
        this.aByte = aByte;
    }

    public BidiMessage(short opcode, short errorCode, String errMsg, byte aByte) {

        this.opcode = opcode;
        this.errorCode = errorCode;
        this.errMsg = errMsg;
        this.aByte = aByte;
    }


//    getters
    public short getOpcode() {

        return opcode;
    }

    public String getFileName() {

        return this.fileName;
    }

    public byte getaByte() {

        return aByte;
    }

    public String getUserName() {
        return userName;
    }

    public short getPacketSize() {
        return packetSize;
    }

    public short getBlockNumber() {
        return blockNumber;
    }

    public byte[] getData() {
        return data;
    }

    public byte getDeletedAdded() {
        return deletedAdded;
    }

    public short getErrorCode() {
        return errorCode;
    }

    public String getErrMsg() {
        return errMsg;
    }
}
