/**
 * tasks:
 * implement protocol for Server (for bidiMessage)
 * implement protocol for client (for bidiMessage)
 * if the last sent block's size is exactly 512 send another empty packet {
 *     client side: for every data packet will check if the size is less than 512. if yes so its the last packet. else contonue to wait
 *     server side: keep a field which holds the accumulated
 * }
 * ---implement EncDec for server
 * test EncDec for server
 *
 * implement EndDec for client
 *
 * implement the connections inteface {
 *      add field map for client id and conectionHandler in connections
 * }
 * when a new client is connecting to the server, add him to the connections list
 * broadcast of connections {
 *     his send function
 * }
 * send function of connections {
 *     send of CH
 * }
 * send function of CH {
 *     out.write...
 * }
 * files to bytes {
 *     implementation in saved in bookmark in chromium
 * }
 * use boost in client
 * create a fileData class which holds the data of the read file to be sent and the one that is recieved {
 *     this instance will only receive the file name and will do all the construction of the byte array and blocks and so on...
 *     this will be the only object with access to the disk and also the only one to delete files from it
 *     practical details needs to be decided before implementing
 * }
 * use the already implemented shorttobytes and vise versa functions inside process
 *
 * Create TFTPConnections that implements Connections interface {
 *     Save this for the comments of this Object's Class file
 *     It will hold a list of files that is initialized with the list of folder contents{
 *         and is synchronized when it is updated and when the fileData object opens the file until it is closed
 *     }
 *     Add a map that maps between clienID and its connectionHandler -> this will be initialized in the server
 * }
 * remove the removeMe.txt file in the Files folder
 * handle all 'new' object in client side (delete if necessary)
 * test:
 *      prevent cliet from logging in with empty name
 *      handle illegale input in converter
 *
 *
 */
// protocol for




/**
 * Created by blumenra on 1/17/17.
 */
public class masterPlan {
}
