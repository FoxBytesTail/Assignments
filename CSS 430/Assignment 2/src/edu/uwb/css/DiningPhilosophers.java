package edu.uwb.css;

/**
 * Created by mike on 7/11/2016.
 */
public class DiningPhilosophers {
    DiningState[] state;
    int nPhil;

    public DiningPhilosophers(int nPhilosophers) {
        this.nPhil = nPhilosophers;
        this.state = new DiningState[nPhilosophers];
        for (int i = 0; i < nPhilosophers; i++) {
            Main.TPrint(String.format("Initializing philosopher %d of %d", i, nPhilosophers - 1));
            state[i] = DiningState.THINKING;
            Main.TPrint(String.format("Philosopher %d has transitioned to THINKING", i));
        }
    }

    public synchronized void takeForks(int i) {
        Main.TPrint(String.format("Taking fork for %d", i));

        state[i] = DiningState.HUNGRY;
        Main.TPrint(String.format("Philosopher %d has transitioned to HUNGRY", i));

        testAndEat(i);

        if (state[i] != DiningState.EATING) {
            try {
                Main.TPrint(String.format("Waiting", i));
                synchronized (state[i]) {
                    // Time out instead of block forever potentially
                    state[i].wait(500);
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

    }

    public synchronized void returnForks(int i) {
        Main.TPrint(String.format("Returning fork for %d", i));
        state[i] = DiningState.THINKING;
        Main.TPrint(String.format("Philosopher %d has transitioned to THINKING", i));

        testAndEat((i + numPhilosophers() - 1) % numPhilosophers());
        testAndEat((i + 1) % numPhilosophers());
    }

    private void testAndEat(int i) {
        // Don't do anything if either side of us is currently eating (therefore utensils not available)
        if (
                (getDiningState((i + numPhilosophers() - 1) % numPhilosophers()) == DiningState.EATING)
                        || (getDiningState(i) != DiningState.HUNGRY)
                        || (getDiningState((i + 1) % numPhilosophers()) == DiningState.EATING)
                ) {
            return;
        }
        state[i] = DiningState.EATING;
        Main.TPrint(String.format("Philosopher %d has transitioned to EATING", i));

        synchronized (state[i]) {
            state[i].notify();
        }
    }

    public int numPhilosophers() {
        return nPhil;
    }

    public synchronized DiningState getDiningState(int i) {
        return state[i];
    }
}
