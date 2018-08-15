import java.util.*;

public class Shell extends Thread {
  private String cmdLine = "";

  public Shell() {}

  private static String trimLast(String s) {
    return s.substring(0, s.length() - 1);
  }

  public void run() {
    int commandCounter = 0;

    while (true) {
      commandCounter += 1;

      StringBuffer inputLine = new StringBuffer();

      // shell[0]% prompt
      SysLib.cout(String.format("Shell[%d]%% ", commandCounter));
      SysLib.cin(inputLine);
      cmdLine = inputLine.toString().trim();

      if (cmdLine.equals("exit") || cmdLine.equals("q")) {
        SysLib.exit();
        break;
      }
      // Split the entire line of commands at ; and &
      // The ?<= format includes the ; and & in each token.
      String[] commands = cmdLine.split("(?<=;)|(?<=&)");
      // "test & test & test"
      // split("&") => [test, test, test]
      // split("(?<=&)") => [test &, test &, test]
      if (commands.length < 1) {
        continue;
      }

      for (String command : commands) {
        command = command.trim();
        if (command.endsWith("&")) {
          SysLib.exec(SysLib.stringToArgs(trimLast(command)));
        } else if (command.endsWith(";")) {
          int pid = SysLib.exec(SysLib.stringToArgs(trimLast(command)));
          while (pid != SysLib.join()) {
            /* spin until our specific PID thread joins */
          }
        } else {
          if (!command.trim().replace("\n", "").replace("\r", "").equals("") && command != null) {
            SysLib.exec(SysLib.stringToArgs(command));
          }
        }
      }
    }
  }
}

