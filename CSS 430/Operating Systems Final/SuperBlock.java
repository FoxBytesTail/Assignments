public class SuperBlock {
    public int totalBlocks; // the number of disk blocks
    public int totalInodes; // the number of inodes
    public int freeList;    // the block number of the free list's head
    public int blocks;

    /**
     * Superblock constructor
     * @param numBlocks
     */
    public SuperBlock(int numBlocks) {
        byte[] sb = new byte[Disk.blockSize];
        SysLib.rawread(0, sb);
        totalBlocks = SysLib.bytes2int(sb, 0);
        totalInodes = SysLib.bytes2int(sb, 4);
        freeList = SysLib.bytes2int(sb, 8);
        blocks = totalInodes;

        if (totalBlocks == numBlocks && totalInodes > 0 && freeList >= 2) return;
        totalBlocks = numBlocks;
        format(64);
    }

    /**
     * Write back totalblocks, totalinodes, and freelist
     */
    public void sync() {
        byte[] tmp = new byte[Disk.blockSize];
        SysLib.int2bytes(totalBlocks, tmp, 0);
        SysLib.int2bytes(totalInodes, tmp, 4);
        SysLib.int2bytes(freeList, tmp, 8);
        SysLib.rawwrite(0, tmp);
    }

    /**
     * Remove block from freelist
     * @return
     */
    public int nextFreeBlock() {
        // Bail out early if none free.
        if (freeList <= 0 || freeList >= totalBlocks) {
            return -1;
        }

        byte[] tmp = new byte[Disk.blockSize];
        SysLib.rawread(freeList, tmp);
        int newFreeList = freeList;
        freeList = SysLib.bytes2int(tmp, 0);
        return newFreeList;
    }

    public boolean emptyBlock(int blkid) {
        if (blkid <= 0 || blkid >= totalBlocks) {
            return false;
        }

        int nextInode = freeList;
        int tmp = 0;

        byte[] next = new byte[Disk.blockSize];
        int cur = 0;
        while (nextInode != -1) {
            SysLib.rawread(nextInode, next);
            tmp = SysLib.bytes2int(next, 0);
            if (tmp == -1) {
                SysLib.int2bytes(blkid, next, 0);
                SysLib.rawwrite(nextInode, next);
                byte[] blankBlock = new byte[Disk.blockSize];
                SysLib.int2bytes(-1, blankBlock, 0);
                while (cur < Disk.blockSize) blankBlock[cur++] = 0;
                SysLib.rawwrite(blkid, blankBlock);
                return true;
            }

            nextInode = tmp;
        }

        return false;
    }

    /**
     * Given format num, re-format inode and superblocks
     * @param iNumber
     */
    public void format(int iNumber) {
        if (iNumber < 0) iNumber = 64;

        totalInodes = iNumber;
        blocks = iNumber;

        Inode tmp = null;
        for (int i = 0; i < totalInodes; i++) {
            tmp = new Inode();
            tmp.flag = 0;
            tmp.toDisk((short) i);
        }

        freeList = totalInodes / 16 + 2;
        byte[] tempBlockToReplace = null;
        int i = freeList;

        while (i < 999) {
            tempBlockToReplace = new byte[Disk.blockSize];
            for (int j = 0; j < Disk.blockSize; j++) tempBlockToReplace[j] = 0;
            SysLib.int2bytes(i + 1, tempBlockToReplace, 0);
            SysLib.rawwrite(i, tempBlockToReplace);
            i++;
        }

        tempBlockToReplace = new byte[Disk.blockSize];
        for (int j = 0; j < Disk.blockSize; j++) tempBlockToReplace[j] = 0;
        SysLib.int2bytes(-1, tempBlockToReplace, 0);
        SysLib.rawwrite(999, tempBlockToReplace);
        byte[] newSuperblock = new byte[Disk.blockSize];
        SysLib.int2bytes(totalBlocks, newSuperblock, 0);
        SysLib.int2bytes(totalInodes, newSuperblock, 4);
        SysLib.int2bytes(freeList, newSuperblock, 8);
        SysLib.rawwrite(0, newSuperblock);
    }
}
