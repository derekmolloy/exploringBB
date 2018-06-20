import std.file;
import std.stdio;
import std.conv;

enum string LDR_PATH = "/sys/bus/iio/devices/iio:device0/in_voltage";

int readAnalog(int number){
   string filePath = LDR_PATH;
   filePath ~= to!string(number);
   filePath ~= "_raw";
   auto ldr = File(filePath, "r");
   int value = to!int(ldr.readln());
   ldr.close();
   return value;
}

int main(string[] args)
{
   writeln("Starting the readLDR program");
   int value = readAnalog(0);
   writeln("The LDR value was %d out of 4095.", value);
   return 0;
}
