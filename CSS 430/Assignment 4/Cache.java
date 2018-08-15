import java.util.Vector;
import java.util.stream.IntStream;

public class Cache {
    private int blocksize;
    private final boolean DEBUG = false; // set false for less output

    private class CacheEntry {
        // if this entry does not have valid block information,
        // the disk block number should be set to -1
        private int FrameId = -1;

        // when accessed
        private boolean Reference = false;

        // when written but before commit
        private boolean Dirty = false;

        CacheEntry() {
        }

        public int getFrameId() {
            return FrameId;
        }

        public void setFrameId(int frameId) {
            FrameId = frameId;
        }

        public void clearEntry() {
            FrameId = -1;
            Reference = false;
            Dirty = false;
        }

        public boolean hasBlockBeenAccessedRecently() {
            return Reference;
        }

        public void setReference(boolean reference) {
            Reference = reference;
        }

        public boolean hasBlockBeenModifiedButNotWritten() {
            return Dirty;
        }

        public void setDirty(boolean dirty) {
            Dirty = dirty;
        }
    }

    /*
        Using a vector instead of arraylist here because
        all vector methods are synchronized; arraylist is only safe
        when your app is threadsafe or single threaded here
    */
    private Vector<byte[]> cache = new Vector<>();
    private CacheEntry[] pages;

    /*
        Constructor
        The constructor allocates a cacheBlocks number of cache blocks in memory.
        Each cache Block should contain a blockSize-byte of data.
    */
    public Cache(int blockSize, int cacheBlocks) {
        this.blocksize = blockSize;
        this.pages = new CacheEntry[cacheBlocks];

        IntStream.range(0, cacheBlocks).forEach((int i) -> {
            if (!cache.add(new byte[this.blocksize])) {
                if (DEBUG) System.err.println("Cache block was not pushed into vector successfully!");
            }

            pages[i] = new CacheEntry();
        });
    }

    private int findUnused() {
        if (DEBUG) System.out.println("\tLooking for an unused block.");

        int unused = -1;

        for (int i = 0; i < pages.length; i++) {
            if (pages[i].getFrameId() == -1) {
                if (DEBUG) System.out.println("Found unused frame at " + i + "");
                return i;
            }

            i++;
        }

        return unused;
    }

    private int secondChanceFinder() {
        if (DEBUG) System.out.println("Starting findVictim method !");
        // this looks awful


        // First chance
        for (int blkId = 0; blkId < pages.length; blkId++) {
            // scan the frame buffer
            // the first frame encountered with NOT ACCESSED RECENTLY + NOT MODIFIED selected for replacement
            if (pages[blkId].hasBlockBeenAccessedRecently() == false && pages[blkId].hasBlockBeenModifiedButNotWritten() == false) {
                if (DEBUG) System.out.println("u=0;m=0 Found victim on 1 scan, vId=" + blkId);
                return blkId;
            }
        }

        for (int blkId = 0; blkId < pages.length; blkId++) {
            // if step 1 fails, scan again
            // looking for frame with NOT ACCESSED RECENTLY BUT MODIFIED WITHOUT COMMIT
            if (pages[blkId].hasBlockBeenAccessedRecently() == false && pages[blkId].hasBlockBeenModifiedButNotWritten() == true) {
                if (DEBUG) System.out.println("u=0;m=1 Found victim on 2 scan, vId=" + blkId);
                return blkId;
            } else {
                // During this scan, set the use bit to 0 on each frame that is bypassed
                pages[blkId].setReference(false);
            }
        }

        // step3 rescan
        for (int blkId = 0; blkId < pages.length; blkId++) {
            if (pages[blkId].hasBlockBeenAccessedRecently() == false && pages[blkId].hasBlockBeenModifiedButNotWritten() == false) {
                if (DEBUG) System.out.println("u=0;m=0 Found victim on 3 scan, vId=" + blkId);
                return blkId;
            }
        }

        for (int blkId = 0; blkId < pages.length; blkId++) {
            // if step 1 fails, scan again
            // looking for frame with NOT ACCESSED RECENTLY BUT MODIFIED WITHOUT COMMIT
            if (pages[blkId].hasBlockBeenAccessedRecently() == false && pages[blkId].hasBlockBeenModifiedButNotWritten() == true) {
                if (DEBUG) System.out.println("u=0;m=1 Found victim on 4 scan, vId=" + blkId);
                return blkId;
            } else {
                // During this scan, set the use bit to 0 on each frame that is bypassed
                pages[blkId].setReference(false);
            }
        }

        return -1;
    }

    private boolean isValidFrame(int frameId) {
        return pages[frameId].getFrameId() != -1;
    }

    private void commitPage(int page) {
        if (!isValidFrame(page)) {
            return;
        }

        // Write back to DISK.java DISKfile
        SysLib.rawwrite(pages[page].getFrameId(), cache.elementAt(page));

        /* Reset it to 0 or false when this block is written back to the disk.  */
        pages[page].setDirty(false);
    }

    private int findPage() {
        int fp = findUnused();
        if (DEBUG) System.out.println("<1> No free pages found, trying second chance finder.");
        if (fp == -1) fp = secondChanceFinder();
        return fp;
    }

    public synchronized boolean read(int blockId, byte buffer[]) {
        if (blockId < 0 || blockId > 1000) {
            if (DEBUG) System.err.println("Attempted to read invalid BlockId");
            return false;
        }

        // Scans the page table for blockID to determine whether the data is in memory
        int bound = pages.length;
        for (int i = 0; i < bound; i++) {
            // If it is in memory
            if (pages[i].getFrameId() == blockId) {
                // reads into the buffer[] the contents of the cache block specified by blockId
                System.arraycopy(cache.elementAt(i), 0, buffer, 0, blocksize);
                // reference: is set to 1 or true whenever this block is accessed
                pages[i].setReference(true);
                if (DEBUG) System.out.println("Cache HIT for " + blockId);

                /* Successful cache hit */
                return true;
            }
        }

        int page = findPage();
        if (pages[page].hasBlockBeenModifiedButNotWritten()) {
            commitPage(page);
        }

        // b: If it is not in memory
        if (DEBUG) System.out.println("Cache MISS for " + blockId);

        // reads the corresponding disk block from the ThreadOS disk (Disk.java)
        SysLib.rawread(blockId, buffer);

        // and load it into the main memory
        byte[] mem = new byte[blocksize];
        System.arraycopy(buffer, 0, mem, 0, blocksize);

        // and add it to the page table
        cache.set(page, mem);
        pages[page].setFrameId(blockId);

        // reference: is set to 1 or true whenever this block is accessed
        pages[page].setReference(true);

        return true;
    }

    public synchronized boolean write(int blockId, byte buffer[]) {
        if (blockId < 0 || blockId > 1000) {
            if (DEBUG) System.err.println("Attempted to write invalid BlockId");
            return false;
        }

        // Writes the contents of buffer[ ] array to the cache block specified by blockId 
        // Look for cache block specified by blockId
        int bound = pages.length;
        for (int i = 0; i < bound; i++) {
            // Is found the cache block
            if (pages[i].getFrameId() == blockId) {
                if (DEBUG) System.out.println("Found cache block to write to at b" + i);
                byte[] temp = new byte[blocksize];

                // Write buffer into block
                System.arraycopy(buffer, 0, temp, 0, blocksize);
                cache.set(i, temp);

                // Block accessed, set ref = true
                pages[i].setReference(true);

                // This page is dirty too since it was written to
                pages[i].setDirty(true);
                return true;
            }
        }

        // b: IF THE BLOCK IS NOT IN CACHE AND THERE ARE NO FREE BOCKS
        // Find victim for replacement (findPage() here does second chance discovery automatically)
        int page = findPage();
        // Write back the dirty page, then we take over that page for this block
        if (pages[page].hasBlockBeenModifiedButNotWritten()) {
            commitPage(page);
        }

        // note: only writes to cache and pages
        // does _NOT_ write to threados DISKFile here.
        byte[] temp = new byte[blocksize];
        System.arraycopy(buffer, 0, temp, 0, blocksize);
        cache.set(page, temp);

        pages[page].setFrameId(blockId);
        pages[page].setReference(true);
        pages[page].setDirty(true);
        return true;
    }

    public synchronized void sync() {
        // For each dirty block, write back.
        // Do not touch cache.
        IntStream.range(0, pages.length).filter((int i) -> pages[i].hasBlockBeenModifiedButNotWritten()).forEach(this::commitPage);
        SysLib.sync();
    }

    public synchronized void flush() {
        // Write back all blocks, this.sync() also makes Disk.java writeback to DISKfile.
        sync();

        // Wipe all cached blocks.
        IntStream.range(0, pages.length).forEach((int i) -> {
            pages[i].clearEntry();
        });
    }
}
