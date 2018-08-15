import java.util.*;

/**
 * FileTable: Spawns a new file table entry containing a FD.
 */
public class FileTable {
    private Vector<FileTableEntry> table;
    private Directory dir;

    /**
     * Constructor for FileTable. Assigns table and directory as argument.
     * @param directory
     */
    public FileTable(Directory directory) {
        table = new Vector();
        dir = directory;
    }

    public synchronized FileTableEntry falloc(String filename, String mode) {
        short id = -1;
        Inode inode;

        while (true) {
            if (filename.equals("/")) {
                id = (short) 0;
            } else {
                id = dir.namei(filename);
            }

            // File not found
            if (id < 0) {
                if (mode.equals("r")) {
                    return null;
                } else {
                    id = dir.ialloc(filename);
                    inode = new Inode(id);
                    inode.flag = 3;
                    break;
                }
            } else {
                inode = new Inode(id);
                if (mode.equals("r")) {
                    if (inode.flag != 2
                            && inode.flag != 1
                            && inode.flag != 0) {
                        if (inode.flag == 3) {
                            try {
                                wait();
                            } catch (InterruptedException e) {
                            }
                        }
                    } else {
                        inode.flag = 2;
                        break;
                    }
                } else {
                    if (inode.flag != 1 && inode.flag != 0) {
                        try {
                            wait();
                        } catch (InterruptedException e) {
                        }
                    } else {
                        inode.flag = 3;
                        break;
                    }
                }
            }
        }

        inode.count += 1;
        inode.toDisk(id);

        FileTableEntry e = new FileTableEntry(inode, id, mode);
        table.addElement(e);
        return e;
    }

    public synchronized boolean ffree(FileTableEntry fte) {
        Inode inode = new Inode(fte.iNumber);
        if (!table.remove(fte)) {
            return false; // return false if fte could not be found within table
        }

        if (inode.flag != 2) {
            if (inode.flag == 3) {
                inode.flag = 1;
                notifyAll(); // notify all waiting threads
            }
        } else {
            if (inode.count == 1) {
                notify();
                inode.flag = 1;
            }
        }

        inode.count -= 1;
        inode.toDisk(fte.iNumber);
        return true;
    }

    public synchronized boolean fempty() {
        return table.isEmpty();
    }
}
