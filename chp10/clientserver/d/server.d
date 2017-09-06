import std.socket;
import std.stdio;

void main()
{
  auto listener = new Socket(AddressFamily.INET, SocketType.STREAM);
  listener.bind(new InternetAddress("localhost", 2525));
  listener.listen(10);
  auto readSet = new SocketSet();
  Socket[] connectedClients;
  char[1024] buffer;
  bool isRunning = true;
  while(isRunning)
  {
    readSet.reset();
    readSet.add(listener);
    foreach(client; connectedClients) readSet.add(client);
    if(Socket.select(readSet, null, null))
    {
      foreach(client; connectedClients)
        if(readSet.isSet(client)) {
          auto got = client.receive(buffer);
          if (got != 0)
          {
            writeln("Server received: ", buffer[0 .. got]);
          }
          client.send(buffer[0 .. got]);
          if (got != 0)
          {
            writeln("Server answered: ", buffer[0 .. got]);
          }
        }
      if(readSet.isSet(listener)) {
        auto newSocket = listener.accept();
        newSocket.send("you are accepted");
        connectedClients ~= newSocket;
      }
    }
  }
}
