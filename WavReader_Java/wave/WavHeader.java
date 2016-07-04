package wave;

/**
 * This class contains a structure of WAVE file.
 *
 * @see <a href =
 * "https://github.com/tkacz-/WavReader/blob/master/wav-sound-format.gif">
 * Structure of wave format
 * </a>
 */
public class WavHeader {
    private byte[] chunkID = new byte[4];
    private int chunkSize;
    private byte[] format = new byte[4];
    private byte[] subChunk1ID = new byte[4];
    private int subChunk1Size;
    private short audioFormat;
    private short numChannels;
    private int sampleRate;
    private int byteRate;
    private short blockAlign;
    private short bitsPerSample;
    private byte[] subChunk2ID = new byte[4];
    private int subChunk2Size;

    @Override
    public String toString() {
        return "The RIFF chunk desriptor: " + new String(this.getChunkID()) + "\n" +
                "Size of this chunk: " + this.getChunkSize() + "\n" +
                "Format: " + new String(this.getFormat()) + "\n" + "\n" +
                "fmt subchunk: " + new String(this.getSubChunk1ID()) + "\n" +
                "Size of this chunk: " + this.getSubChunk1Size() + "\n" +
                "Audio format: " + this.getAudioFormat() + "\n" +
                "Number of channels: " + this.getNumChannels() + "\n" +
                "Sample rate: " + this.getSampleRate() + "\n" +
                "Byte rate: " + this.getByteRate() + "\n" +
                "Block align: " + this.getBlockAlign() + "\n" +
                "Bits per sample: " + this.getBitsPerSample() + "\n" + "\n" +
                "data subchunk: " + new String(this.getSubChunk2ID()) + "\n" +
                "Size of this chunk: " + this.getSubChunk2Size();
    }

    public byte[] getChunkID() {
        return chunkID;
    }

    public void setChunkID(byte[] chunkID) {
        this.chunkID = chunkID;
    }

    public int getChunkSize() {
        return chunkSize;
    }

    public void setChunkSize(int chunkSize) {
        this.chunkSize = chunkSize;
    }

    public byte[] getFormat() {
        return format;
    }

    public void setFormat(byte[] format) {
        this.format = format;
    }

    public byte[] getSubChunk1ID() {
        return subChunk1ID;
    }

    public void setSubChunk1ID(byte[] subChunk1ID) {
        this.subChunk1ID = subChunk1ID;
    }

    public int getSubChunk1Size() {
        return subChunk1Size;
    }

    public void setSubChunk1Size(int subChunk1Size) {
        this.subChunk1Size = subChunk1Size;
    }

    public short getAudioFormat() {
        return audioFormat;
    }

    public void setAudioFormat(short audioFormat) {
        this.audioFormat = audioFormat;
    }

    public short getNumChannels() {
        return numChannels;
    }

    public void setNumChannels(short numChannels) {
        this.numChannels = numChannels;
    }

    public int getSampleRate() {
        return sampleRate;
    }

    public void setSampleRate(int sampleRate) {
        this.sampleRate = sampleRate;
    }

    public int getByteRate() {
        return byteRate;
    }

    public void setByteRate(int byteRate) {
        this.byteRate = byteRate;
    }

    public short getBlockAlign() {
        return blockAlign;
    }

    public void setBlockAlign(short blockAlign) {
        this.blockAlign = blockAlign;
    }

    public short getBitsPerSample() {
        return bitsPerSample;
    }

    public void setBitsPerSample(short bitsPerSample) {
        this.bitsPerSample = bitsPerSample;
    }

    public byte[] getSubChunk2ID() {
        return subChunk2ID;
    }

    public void setSubChunk2ID(byte[] subChunk2ID) {
        this.subChunk2ID = subChunk2ID;
    }

    public int getSubChunk2Size() {
        return subChunk2Size;
    }

    public void setSubChunk2Size(int subChunk2Size) {
        this.subChunk2Size = subChunk2Size;
    }
}