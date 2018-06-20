import std.stdio;
import core.sys.posix.unistd;

int main(string[] args)
{
    writefln("The user logged in is %s.", getlogin());
    return 0;
}
