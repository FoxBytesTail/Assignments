import java.util.Arrays;
import java.util.Random;
import java.util.stream.IntStream;

public class Test4 extends Thread {
    private static final int MAX_BLOCK = 512;
    private static final int MAX_ARRAY = 50;


    private int argument = 0;
    private Random random = new Random(System.currentTimeMillis());

    private boolean IS_CACHE_ENABLED = false;
    private byte[] w;
    private byte[] r;

    public Test4(String args[]) {
        argument = Integer.parseInt(args[1]);
        w = new byte[MAX_BLOCK];
        r = new byte[MAX_BLOCK];
        random.nextBytes(w);

        if (args[0].equals("enabled")) IS_CACHE_ENABLED = true;
    }

    private void read(int blockId, byte buffer[]) {
        if (IS_CACHE_ENABLED) {
            SysLib.cread(blockId, buffer);
        } else {
            SysLib.rawread(blockId, buffer);
        }
    }

    private void write(int blockId, byte buffer[]) {
        if (IS_CACHE_ENABLED) {
            SysLib.cwrite(blockId, buffer);
        } else {
            SysLib.rawwrite(blockId, buffer);
        }
    }

    private int rand(int upper) {
        return (Math.abs(random.nextInt() % upper));
    }

    public void run() {
        SysLib.flush();

        if (argument == 1) {
            randomAccess();
        } else if (argument == 2) {
            localizedAccess();

        } else if (argument == 3) {
            mixedAccess();
        } else if (argument == 4) {
            adversaryAccess();
        } else if (argument == 5) {
            randomAccess();
            localizedAccess();
            mixedAccess();
            adversaryAccess();
        } else {
            SysLib.cout("args: [enabled/disabled] [1/2/3/4]\n");
            SysLib.exit();
        }

        if (IS_CACHE_ENABLED) {
            SysLib.csync();
        } else {
            SysLib.sync();
        }
        SysLib.exit();
    }

    private void randomAccess() {
        long start, stop;
        String output;

        // Randomly access blocks throughout
        int[] rBlock = IntStream.range(0, MAX_ARRAY).map((int i) -> rand(MAX_BLOCK)).toArray();

        start = System.currentTimeMillis();
        IntStream.range(0, MAX_ARRAY).forEach((int i) -> write(rBlock[i], w));
        stop = System.currentTimeMillis();
        output = String.format("Random %s access write time:\n\t total %dms\n\t avg %dms\n\t %d runs",
                (IS_CACHE_ENABLED ? "cached" : "uncached"), (stop - start), (stop - start) / MAX_ARRAY, MAX_ARRAY);
        SysLib.cout(output + "\n");

        start = System.currentTimeMillis();
        IntStream.range(0, MAX_ARRAY).forEach((int i) -> read(rBlock[i], r));
        stop = System.currentTimeMillis();
        output = String.format("Random %s access read time:\n\t total %dms\n\t avg %dms\n\t %d runs",
                (IS_CACHE_ENABLED ? "cached" : "uncached"), (stop - start), (stop - start) / MAX_ARRAY, MAX_ARRAY);
        SysLib.cout(output + "\n");

        if (IS_CACHE_ENABLED) {
            SysLib.csync();
        } else {
            SysLib.sync();
        }

        if (!Arrays.equals(r, w)) SysLib.cout("Error: new and old blocks do not match. This test failed.\n");
    }

    private void localizedAccess() {
        long start, stop;
        String output;

        // Hit p0-10 repeatedly throughout the range

        start = System.currentTimeMillis();
        IntStream.range(0, MAX_ARRAY).flatMap(runSize -> IntStream.range(0, 10)).forEach(writeTenTimesPerLoop -> write(writeTenTimesPerLoop, w));
        stop = System.currentTimeMillis();
        output = String.format("Localized %s access write time:\n\t total %dms\n\t avg %dms\n\t %d runs",
                (IS_CACHE_ENABLED ? "cached" : "uncached"), (stop - start), (stop - start) / MAX_ARRAY, MAX_ARRAY);
        SysLib.cout(output + "\n");

        start = System.currentTimeMillis();
        IntStream.range(0, MAX_ARRAY).flatMap(runSize -> IntStream.range(0, 10)).forEach(writeTenTimesPerLoop -> read(writeTenTimesPerLoop, r));
        stop = System.currentTimeMillis();
        output = String.format("Localized %s access read time:\n\t total %dms\n\t avg %dms\n\t %d runs",
                (IS_CACHE_ENABLED ? "cached" : "uncached"), (stop - start), (stop - start) / MAX_ARRAY, MAX_ARRAY);
        SysLib.cout(output + "\n");

        if (IS_CACHE_ENABLED) {
            SysLib.csync();
        } else {
            SysLib.sync();
        }

        if (!Arrays.equals(r, w)) SysLib.cout("Error: new and old blocks do not match. This test failed.\n");
    }

    private void mixedAccess() {
        long start, stop;
        String output;

        // Mixed access
        // 90% = localized access
        // 10% are random accesses
        // Roll random number up to 0-9, if 0-8 then constrain to 0-10, otherwise constrain to 0-MAX.
        int[] rBlock = IntStream.range(0, MAX_ARRAY).map(i -> rand(10) < 9 ? rand(10) : rand(MAX_BLOCK)).toArray();

        start = System.currentTimeMillis();
        IntStream.range(0, MAX_ARRAY).forEach((int i) -> write(rBlock[i], w));
        stop = System.currentTimeMillis();
        output = String.format("Random %s access write time:\n\t total %dms\n\t avg %dms\n\t %d runs",
                (IS_CACHE_ENABLED ? "cached" : "uncached"), (stop - start), (stop - start) / MAX_ARRAY, MAX_ARRAY);
        SysLib.cout(output + "\n");

        start = System.currentTimeMillis();
        IntStream.range(0, MAX_ARRAY).forEach((int i) -> read(rBlock[i], r));
        stop = System.currentTimeMillis();
        output = String.format("Random %s access read time:\n\t total %dms\n\t avg %dms\n\t %d runs",
                (IS_CACHE_ENABLED ? "cached" : "uncached"), (stop - start), (stop - start) / MAX_ARRAY, MAX_ARRAY);
        SysLib.cout(output + "\n");

        if (IS_CACHE_ENABLED) {
            SysLib.csync();
        } else {
            SysLib.sync();
        }

        if (!Arrays.equals(r, w)) SysLib.cout("Error: new and old blocks do not match. This test failed.\n");
    }

    private void adversaryAccess() {
        long start, stop;
        String output;

        int[] rBlock = IntStream.range(0, MAX_BLOCK).map((int i) -> i).toArray();

        start = System.currentTimeMillis();
        IntStream.range(0, MAX_ARRAY).forEach((int i) -> write(rBlock[i], w));
        stop = System.currentTimeMillis();
        output = String.format("Adversarial %s access write time:\n\t total %dms\n\t avg %dms\n\t %d runs",
                (IS_CACHE_ENABLED ? "cached" : "uncached"), (stop - start), (stop - start) / MAX_ARRAY, MAX_ARRAY);
        SysLib.cout(output + "\n");

        start = System.currentTimeMillis();
        IntStream.range(0, MAX_ARRAY).forEach((int i) -> read(rBlock[i], r));
        stop = System.currentTimeMillis();
        output = String.format("Adversarial %s access read time:\n\t total %dms\n\t avg %dms\n\t %d runs",
                (IS_CACHE_ENABLED ? "cached" : "uncached"), (stop - start), (stop - start) / MAX_ARRAY, MAX_ARRAY);
        SysLib.cout(output + "\n");

        if (IS_CACHE_ENABLED) {
            SysLib.csync();
        } else {
            SysLib.sync();
        }

        if (!Arrays.equals(r, w)) SysLib.cout("Error: new and old blocks do not match. This test failed.\n");
    }

}
