public class Inode {
    private final static int iNodeSize = 32;       // fix to 32 bytes
    public final static int directSize = 11;      // # direct pointers

    public int length;                             // file size in bytes
    public short count;                            // # file-table entries pointing to this
    public short flag;                             // 0 = unused, 1 = used, ...
    public short direct[] = new short[directSize]; // direct pointers
    public short indirect;                         // a indirect pointer

    /**
     * Inode default constructor
     */
    Inode() {
        length = 0;
        count = 0;
        flag = 1;
        for (int i = 0; i < directSize; i++) {
            direct[i] = -1;
        }
        indirect = -1;
    }

    byte[] returnAndDeallocate() {
        if (indirect >= 0) {
            byte[] data = new byte[Disk.blockSize];
            SysLib.rawread(indirect, data);
            indirect = -1;
            return data;
        } else {
            return null;
        }
    }

    /**
     * Inode constructor
     * @param iNumber
     */
    Inode(short iNumber) {
        byte[] data = new byte[Disk.blockSize];
        SysLib.rawread(1 + iNumber / 16, data);
        int offset = (iNumber % 16) * iNodeSize;
        length = SysLib.bytes2int(data, offset);
        offset += 4;
        count = SysLib.bytes2short(data, offset);
        offset += 2;
        flag = SysLib.bytes2short(data, offset);
        offset += 2;

        for (int i = 0; i < directSize; i++) {
            direct[i] = SysLib.bytes2short(data, offset);
            offset += 2;
        }

        indirect = SysLib.bytes2short(data, offset);
        offset += 2;
    }

    /**
     * Writes to disk an inode at the ith inode
     * @param iNumber
     * @return
     */
    int toDisk(short iNumber) {
        byte[] b = new byte[iNodeSize];
        int offset = 0;

        SysLib.int2bytes(length, b, offset);
        offset += 4; // 32 bit int is 4 bytes, 00 00 00 00
        SysLib.short2bytes(count, b, offset);
        offset += 2; // short is 2 bytes 00 00
        SysLib.short2bytes(flag, b, offset);
        offset += 2;

        for (int i = 0; i < directSize; i++) {
            SysLib.short2bytes(direct[i], b, offset);
            offset += 2;
        }

        SysLib.short2bytes(indirect, b, offset);
        offset += 2;

        int blkid = 1 + iNumber / 16;
        byte[] tmpblk = new byte[Disk.blockSize];
        SysLib.rawread(blkid, tmpblk);
        System.arraycopy(b, 0, tmpblk, iNumber % 16 * iNodeSize, iNodeSize);
        SysLib.rawwrite(blkid, tmpblk);

        return 0;
    }

    /**
     * Returns the Index Block Num
     * @param e
     * @param offset
     * @return
     */
    int getInum(int e, short offset) {
        if ((e / Disk.blockSize) < directSize) {
            if (direct[(e / Disk.blockSize)] >= 0) return -1;
            if ((e / Disk.blockSize > 0) && (direct[(e / Disk.blockSize) - 1] == -1)) return -2;
            direct[(e / Disk.blockSize)] = offset;
            return 0;
        }

        if (indirect < 0) {
            return -3;
        } else {
            byte[] data = new byte[Disk.blockSize];
            SysLib.rawread(indirect, data);
            int len = ((e / Disk.blockSize) - directSize) * 2;
            if (SysLib.bytes2short(data, len) > 0) {
                return -1;
            } else {
                SysLib.short2bytes(offset, data, len);
                SysLib.rawwrite(indirect, data);
            }
        }

        return 0;
    }

    /**
     * Sets index block num.
     * @param iNumber
     * @return
     */
    boolean setInum(short iNumber) {
        if (indirect != -1) return false;
        for (int i = 0; i < directSize; i++) if (direct[i] == -1) return false;
        indirect = iNumber;
        byte[] data = new byte[Disk.blockSize];
        int i = 0;
        while (i < (Disk.blockSize / 2)) {
            SysLib.short2bytes((short) -1, data, i * 2);
            i++;
        }
        SysLib.rawwrite(iNumber, data);
        return true;

    }

    /**
     * Retrieval method for getting the location of the block.
     * @param offset
     * @return
     */
    int getBlock(int offset) {
        if (offset / Disk.blockSize < directSize) {
            return direct[offset / Disk.blockSize];
        } else {
            if (indirect < 0) return -1;
            byte[] data = new byte[Disk.blockSize];
            SysLib.rawread(indirect, data);

            int blockSpace = (offset / Disk.blockSize - directSize) * 2;
            return SysLib.bytes2short(data, blockSpace);
        }

    }
}
