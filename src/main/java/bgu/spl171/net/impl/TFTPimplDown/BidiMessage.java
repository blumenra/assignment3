package bgu.spl171.net.impl.TFTPimplDown;


/**
 *  opcode operation
 *      1 Read request (RRQ)
 *      2 Write request (WRQ)
 *      3 Data (DATA)
 *      4 Acknowledgment (ACK)
 *      5 Error (ERROR)
 *      6 Directory listing request (DIRQ)
 *      7 Login request (LOGRQ)
 *      8 Delete request (DELRQ)
 *      9 Broadcast file added/deleted (BCAST)
 *      10 Disconnect (DISC)
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

    public static BidiMessage createBcastMessage(int event, String fileName) {

        return new BidiMessage((short) 9, (byte) event, fileName, (byte) 0);
    }

    public static BidiMessage createAckMessage(int ackNum) {

        return new BidiMessage((short) 4, (short) ackNum);
    }

    public static BidiMessage createErrorMessage(int errNum, String errMsg) {

        return new BidiMessage((short) 7, (short) errNum, errMsg, (byte) 0);
    }

//    Constructors
    private BidiMessage(short opcode, String string, byte aByte) {
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

    private BidiMessage(short opcode) {

        this.opcode = opcode;
    }

    private BidiMessage(short opcode, short packetSize, short blockNumber, byte[] data) {

        this.opcode = opcode;
        this.packetSize = packetSize;
        this.blockNumber = blockNumber;
        this.data = data;
    }

    private BidiMessage(short opcode, short blockNumber) {

        this.opcode = opcode;
        this.blockNumber = blockNumber;
    }

    private BidiMessage(short opcode, byte deletedAdded, String fileName, byte aByte) {

        this.opcode = opcode;
        this.deletedAdded = deletedAdded;
        this.fileName = fileName;
        this.aByte = aByte;
    }

    private BidiMessage(short opcode, short errorCode, String errMsg, byte aByte) {

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
