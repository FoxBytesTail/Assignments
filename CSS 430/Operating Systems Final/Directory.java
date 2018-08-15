public class Directory {
    private static int maxChars = 30; // max Num of chars allowable for filename
    private int fsize[];
    private char fnames[][];

    /**
     * Directory Constructor
     * @param maxInumber
     */
    public Directory(int maxInumber) {
        fsize = new int[maxInumber]; //allocates an array specified by max files
        for (int i = 0; i < maxInumber; i++) //init array to zero
            fsize[i] = 0;
        fnames = new char[maxInumber][maxChars];
        String root = "/";
        fsize[0] = root.length();
        root.getChars(0, fsize[0], fnames[0], 0);
    }

    /**
     * Translates byte information into directory information
     * @param data
     */
    public void bytes2directory(byte data[]) {
        int offset = 0, i = 0;
        while (i < fsize.length) {
            fsize[i] = SysLib.bytes2int(data, offset);
            offset += 4;
            i++;
        }

        i = 0; // keep fread'ing
        while (i < fsize.length) {
            String tmp = new String(data, offset, 60);
            tmp.getChars(0, fsize[i], fnames[i], 0);
            offset += 60;
            i++;
        }
    }

    /**
     * Translates dir info into byte array, and write to disk.
     * @return
     */
    public byte[] directory2bytes() {
        byte[] dir = new byte[64 * fsize.length];
        int offset = 0;

        int i = 0;
        while (i < fsize.length) {
            SysLib.int2bytes(fsize[i], dir, offset);
            offset += 4;
            i++;
        }

        i = 0;
        while (i < fsize.length) {
            String tmp = new String(fnames[i], 0, fsize[i]);
            byte[] bytes = tmp.getBytes();
            System.arraycopy(bytes, 0, dir, offset, bytes.length);
            offset += 60;
            i++;
        }
        return dir;
    }

    /**
     * Allocates a newly intizialized inode num for a filename supplied as an argument.
     * @param filename
     * @return
     */
    public short ialloc(String filename) {
        int i = 0;
        while (i < fsize.length) {
            if (fsize[i] != 0) {
                i++;
            } else {
                // allocates a new inode number for this filename
                int file;
                if (filename.length() > maxChars) {
                    file = maxChars;
                } else {
                    file = filename.length();
                }

                fsize[i] = file;
                filename.getChars(0, fsize[i], fnames[i], 0);
                return (short) i;
            }
        }

        return (short) -1;
    }


    public boolean ifree(short iNumber) {
        if (iNumber < 0) {
            return false;
        }

        if ((fsize[iNumber] <= 0)) {
            return false;
        }

        if (iNumber >= maxChars) {
            return false;
        }

        fsize[iNumber] = 0;
        return true;
    }

    public short namei(String filename) {
        int i = 0;
        while (i < fsize.length) {
            if (filename.length() != fsize[i]) {
                i++;

            } else {
                if (filename.equals(new String(fnames[i], 0, fsize[i]))) {
                    return (short) i;
                }

                i++;
                continue;
            }
        }

        return -1;
    }
}
