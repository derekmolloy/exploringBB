/** Simple LED Java Example. Written by Derek Molloy (derekmolloy.ie) for the book
*   Exploring Beaglebone
*
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

package exploringBB;

import java.io.*;

public class LEDExample {

  private static String LED3_PATH = "/sys/class/leds/beaglebone:green:usr3";

  private static void writeLED(String filename, String value, String path){
     try{
        BufferedWriter bw = new BufferedWriter(new FileWriter (path+filename));
        bw.write(value);
        bw.close();
     }
     catch(IOException e){
        System.err.println("Failed to access the BBB Sysfs file: " + filename);
     }
  }

  private static void removeTrigger(){
     writeLED("/trigger", "none", LED3_PATH);
  }

  public static void main(String[] args) {
     System.out.println("Starting the LED Java Application");
     if(args.length!=1) {
        System.out.println("There are an incorrect number of arguments.");
        System.out.println("  usage is: LEDExample command");
        System.out.println("where command is one of on, off, flash or status.");
        System.exit(2);
     }
     if (args[0].equalsIgnoreCase("On") || args[0].equalsIgnoreCase("Off")){
        System.out.println("Turning the LED " + args[0]);
        removeTrigger();
        writeLED("/brightness", args[0].equalsIgnoreCase("On")? "1":"0", LED3_PATH);
     }
     else if (args[0].equalsIgnoreCase("flash")){
        System.out.println("Flashing the LED");
        writeLED("/trigger", "timer", LED3_PATH);
        writeLED("/delay_on", "50", LED3_PATH);
        writeLED("/delay_off", "50", LED3_PATH);
     }
     else if (args[0].equalsIgnoreCase("status")){
        try{
           BufferedReader br = new BufferedReader(new FileReader(LED3_PATH+"/trigger"));
           String line;
           while ((line = br.readLine()) != null) {
              System.out.println(line);
           }
           br.close();
        }
        catch(IOException e){
           System.err.println("Failed to access the BBB Sysfs file: /trigger");
        }
     }
     else {
        System.out.println("Invalid command");
     }
  }
}
