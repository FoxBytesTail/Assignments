public class FileSystem {
    private SuperBlock superblock;
    private Directory directory;
    private FileTable filetable;

    /**
     * File System Constructor
     * @param diskSize
     */
    public FileSystem(int diskSize) {
        superblock = new SuperBlock(diskSize);
        directory = new Directory(superblock.blocks);
        filetable = new FileTable(directory);
        // read directory entry from disk
        FileTableEntry dentry = open("/", "r");
        int sz = fsize(dentry);
        if (sz > 0) { // if size greater than zero go ahead and read, otherwise just close the d-entry
            byte[] data = new byte[sz];
            read(dentry, data);
            directory.bytes2directory(data);
        }

        this.close(dentry);
    }

    /**
     * Open fte, write to byte array, and sync superblock so it writes to disk.
     */
    public void sync() {
        FileTableEntry root = open("/", "w");
        write(root, directory.directory2bytes());
        close(root);
        superblock.sync();
    }

    /**
     * Formats files
     * @param files
     * @return
     */
    public boolean format(int files) {
        superblock.format(files);
        directory = new Directory(superblock.blocks);
        filetable = new FileTable(directory);
        return true;
    }

    /**
     * Returns size of inode
     * @param entry
     * @return
     */
    public synchronized int fsize(FileTableEntry entry) {
        synchronized (entry) {
            return entry.inode.length;
        }
    }

    public FileTableEntry open(String filename, String mode) {
        FileTableEntry fte = filetable.falloc(filename, mode);
        if (!mode.equals("w")) {
            return fte;
        }

        boolean status = deallocAllBlocks(fte); //deallocate blocks
        if (status) return fte;
        else return null;

    }

    public boolean close(FileTableEntry entry) {
        synchronized (entry) {
            entry.count -= 1;
            if (entry.count == 0) {
                return filetable.ffree(entry);
            }

            return true;
        }
    }

    /**
     * Read FTE supplied w/ buffer byte arr.
     * @param fte
     * @param buf
     * @return
     */
    public int read(FileTableEntry fte, byte[] buf) {
        if (!fte.mode.equals("r") && !fte.mode.equals("w+")) return -1;
        synchronized (fte) {
            int size = buf.length, read = 0, remain = 0;
            if (fte.seekPtr < fsize(fte)) {
                do {
                    if (size < 1) break;
                    if (fte.inode.getBlock(fte.seekPtr) == -1) break;
                    byte[] data = new byte[512];
                    SysLib.rawread(fte.inode.getBlock(fte.seekPtr), data);
                    int offset = fte.seekPtr % 512;
                    remain = 512 - remain < fsize(fte) - fte.seekPtr ? 512 - remain : fsize(fte) - fte.seekPtr;
                    remain = remain > size ? size : remain;
                    System.arraycopy(data, offset, buf, read, remain);
                    read = read + remain;
                    fte.seekPtr = fte.seekPtr + remain;
                    size = size - remain;
                } while (fte.seekPtr < fsize(fte)); //set seek ptr
            }

            return read;
        }
    }

    /**
     * Write from seek ptr, and increment seek ptr. Returns num of bytes written
     * @param entry
     * @param buffer
     * @return
     */
    public int write(FileTableEntry entry, byte[] buffer) {
        int written = 0;
        int towrite = buffer.length;

        if (entry != null && entry.mode != "r") {
            synchronized (entry) {
                while (true) {
                    if (towrite > 0) {
                        int location = entry.inode.getBlock(entry.seekPtr);
                        if (location == -1) {
                            short nextFreeBlock = (short) superblock.nextFreeBlock();
                            switch (entry.inode.getInum(entry.seekPtr, nextFreeBlock)) {
                                case -3:
                                    short nextAvailableBlkId = (short) this.superblock.nextFreeBlock();
                                    if (!entry.inode.setInum(nextAvailableBlkId) || entry.inode.getInum(entry.seekPtr, nextFreeBlock) != 0) {
                                        return -1;
                                    }
                                    break;
                                case -2:
                                    return -1;
                                case -1:
                                    return -1;
                            }

                            location = nextFreeBlock;
                        }

                        byte[] writebuffer = new byte[512];
                        SysLib.rawread(location, writebuffer);

                        if (512 - entry.seekPtr % 512 > towrite) {
                            System.arraycopy(buffer, written, writebuffer, entry.seekPtr % 512, towrite);
                            SysLib.rawwrite(location, writebuffer);

                            entry.seekPtr += towrite;
                            written += towrite;
                            towrite = 0;
                        } else {
                            System.arraycopy(buffer, written, writebuffer, entry.seekPtr % 512, 512 - entry.seekPtr % 512);
                            SysLib.rawwrite(location, writebuffer);

                            entry.seekPtr += 512 - entry.seekPtr % 512;
                            written += 512 - entry.seekPtr % 512;
                            towrite -= 512 - entry.seekPtr % 512;
                        }
                    } else {
                        break;
                    }
                }

                entry.inode.length = (entry.seekPtr > entry.inode.length) ? entry.seekPtr : entry.inode.length;
                entry.inode.toDisk(entry.iNumber);
                return written;
            }
        } else {
            return -1;
        }
    }

    /**
     * Method updates seek ptr given FTE as argument.
     * @param entry
     * @param offset
     * @param whence
     * @return
     */
    public int seek(FileTableEntry entry, int offset, int whence) {
        if (entry == null) return -1;
        if (entry.seekPtr < 0) return -1;
        if (whence > 2) return -1;

        synchronized (entry) {
            if (whence == 0) {
                entry.seekPtr = offset;
            } else if (whence == 1) {
                entry.seekPtr += offset;
            } else if (whence == 2) {
                entry.seekPtr = entry.inode.length + offset;
            } else {
                return -1;
            }

            return (entry.seekPtr < 0) ? 0 : entry.seekPtr;
        }
    }

    private boolean deallocAllBlocks(FileTableEntry fte) {
        if (fte == null) return false;
        if (fte.inode == null) return false;
        if (fte.inode.count > 1) return false;

        int i = 0;
        while (i < fte.inode.directSize) {
            short[] thisBlock = fte.inode.direct;
            if (thisBlock[i] != (short) -1) {
                superblock.emptyBlock(i);
            }
            thisBlock[i] = (short) -1;
            i++;
        }

        byte[] prev = fte.inode.returnAndDeallocate();
        if (prev != null) {
            int blockId;
            while (true) {
                if ((blockId = SysLib.bytes2short(prev, 0)) != -1) {
                    superblock.emptyBlock(blockId);
                } else {
                    break;
                }
            }
        }

        fte.inode.toDisk(fte.iNumber);
        return true;
    }

    boolean delete(String filename) {
        FileTableEntry fte = open(filename, "w");
        return directory.ifree(fte.iNumber) && this.close(fte) ? true : false;
    }
}
