import std.file;
import std.getopt;
import std.stdio;

int main (string[] args) {
   int ARGUMENT_COUNT = 2;
   string LED3_PATH = "/sys/class/leds/beaglebone:green:usr3";

   enum Command : string
   {
      on = "on",
      off = "off",
      flash = "flash"
   }

   enum ReturnCode : int
   {
      pass = 0,
      fail = 1
   }

   if (args.length != ARGUMENT_COUNT) {
      writeln("too many or too little arguments");
      return ReturnCode.fail;
   }

   string cmd = "invalid"; // no default, value force validation below to fail
   getopt(args, "command", &cmd);

   chdir(LED3_PATH);

   if (cmd == Command.on) {
      std.file.write("brightness", "1");
   } else if (cmd == Command.off) {
      std.file.write("brightness", "0");
   } else if (cmd == Command.flash) {
      std.file.write("trigger", "timer");
   } else {
      writeln("command not supported");
      return ReturnCode.fail;
   }

   return ReturnCode.pass;
}
