package wave;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;

/**
 * Custom wave header reader which use class
 *
 * @see WavHeader
 */
public class WavHeaderReader {
    /**
     * Size for the wave header
     */
    private static final int HEADER_SIZE = 44;

    /**
     * Buffer which contain bytes of the wave header
     */
    private byte[] buf = new byte[HEADER_SIZE];

    /**
     * Wave header
     */
    private WavHeader header = new WavHeader();

    /**
     * InputStream to the wave file
     */
    private InputStream inputStream;

    /**
     * Empty constructor
     */
    public WavHeaderReader() {

    }

    /**
     * Constructor which create own FileInputStream
     *
     * @param source absolute path to the file
     * @throws IOException
     */
    public WavHeaderReader(String source) throws IOException {
        inputStream = new FileInputStream(source);
    }

    /**
     * Constructor which use created InputStream
     *
     * @param inputStream InputStream to the wave file
     */
    public WavHeaderReader(InputStream inputStream) {
        this.inputStream = inputStream;
    }

    /**
     * Read wave header of the file
     *
     * @return WavHeader object
     * @throws IOException
     * @see WavHeader
     */
    public WavHeader read() throws IOException {
        int res = inputStream.read(buf);
        if (res != HEADER_SIZE) {
            throw new IOException("Could not read header.");
        }
        header.setChunkID(Arrays.copyOfRange(buf, 0, 4));
        if (new String(header.getChunkID()).compareTo("RIFF") != 0) {
            throw new IOException("Illegal format.");
        }
        header.setChunkSize(toInt(4, false));
        header.setFormat(Arrays.copyOfRange(buf, 8, 12));
        header.setSubChunk1ID(Arrays.copyOfRange(buf, 12, 16));
        header.setSubChunk1Size(toInt(16, false));
        header.setAudioFormat(toShort(20, false));
        header.setNumChannels(toShort(22, false));
        header.setSampleRate(toInt(24, false));
        header.setByteRate(toInt(28, false));
        header.setBlockAlign(toShort(32, false));
        header.setBitsPerSample(toShort(34, false));
        header.setSubChunk2ID(Arrays.copyOfRange(buf, 36, 40));
        header.setSubChunk2Size(toInt(40, false));
        return header;
    }

    /**
     * Convert byte[] array to int number
     *
     * @param start  start position of the buffer
     * @param endian <code>true</code> for big-endian
     *               <code>false</code> for little-endian
     * @return converted number
     */
    private int toInt(int start, boolean endian) {
        int k = (endian) ? 1 : -1;
        if (!endian) {
            start += 3;
        }
        return (buf[start] << 24) + (buf[start + k * 1] << 16) +
                (buf[start + k * 2] << 8) + buf[start + k * 3];
    }

    /**
     * Convert byte[] array to short number
     *
     * @param start  start position of the buffer
     * @param endian <code>true</code> for big-endian
     *               <code>false</code> for little-endian
     * @return converted number
     */
    private short toShort(int start, boolean endian) {
        short k = (endian) ? (short) 1 : -1;
        if (!endian) {
            start++;
        }
        return (short) ((buf[start] << 8) + (buf[start + k * 1]));
    }

    /**
     * Return the buffer which contain wave hader
     *
     * @return buffer
     */
    public byte[] getBuf() {
        return buf;
    }

    /**
     * Return WavHeader object which contain wave header
     *
     * @return WavHeader object
     */
    public WavHeader getHeader() {
        return header;
    }

    /**
     * Return input stream to the wave file
     *
     * @return Input steam to the wave file
     */
    public InputStream getInputStream() {
        return inputStream;
    }

    public void setInputStream(InputStream inputStream) {
        this.inputStream = inputStream;
    }

}
