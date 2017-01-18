package bgu.spl171.net.impl.TFTPimplDown;

import bgu.spl171.net.api.MessageEncoderDecoder;

/**
 * Created by blumenra on 1/18/17.
 */
public class BidiEncDecImpl implements MessageEncoderDecoder<T>{
    @Override
    public T decodeNextByte(byte nextByte) {
        return null;
    }

    @Override
    public byte[] encode(T message) {
        return new byte[0];
    }
}
