void main()
{
  import std.socket, std.stdio;
  auto socket = new Socket(AddressFamily.INET, SocketType.STREAM);
  char[1024] buffer;
  socket.connect(new InternetAddress("localhost", 2525));
  auto received = socket.receive(buffer);
  writeln("Server answered: ", buffer[0 .. received]);
  foreach(line; stdin.byLine)
  {
    socket.send(line);
    writeln("Server answered: ", buffer[0 .. socket.receive(buffer)]);
  }
}
