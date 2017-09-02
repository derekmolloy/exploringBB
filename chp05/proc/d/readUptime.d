import std.file;
import std.stdio;
import std.conv;
import std.string;

int main(string[] args)
{
   enum int SECONDS_PER_MINUTE = 60;
   enum int PERCENT_BASE = 100;
   writeln("Starting the read uptime program");
   enum string PROC_UPTIME_PATH = "/proc/uptime";
   auto procUptime = File(PROC_UPTIME_PATH, "r");
   string upAndIdleTime = chop(procUptime.readln());
   auto upAndIdleTimeInArray = upAndIdleTime.split(" ");
   string uptimeString = upAndIdleTimeInArray[0];
   string idletimeString = upAndIdleTimeInArray[1];
   float uptime = to!float(uptimeString);
   float idletime = to!float(idletimeString);
   writefln("The system up time is %f minutes.", uptime/SECONDS_PER_MINUTE, " minutes.");
   writefln("It was idle for %f minutes, or %f percent.", idletime/SECONDS_PER_MINUTE, PERCENT_BASE*(idletime/uptime));
   procUptime.close();
   return 0;
}
