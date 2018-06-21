// This code comes from pololu.com directly
// Uses POSIX functions to send and receive data from the virtual serial
// port of a Pololu Simple Motor Controller.
// NOTE: The Simple Motor Controller's Input Mode must be set to Serial/USB.
// NOTE: You must change the 'const char * device' line below.
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#define SERIAL_ERROR -9999

// Reads a variable from the SMC and returns it as number between 0 and 65535.
// Returns SERIAL_ERROR if there was an error.
// The 'variableId' argument must be one of IDs listed in the
// "Controller Variables" section of the user's guide.
// For variables that are actually signed, additional processing is required
// (see smcGetTargetSpeed for an example).
int smcGetVariable(int fd, unsigned char variableId) {
  unsigned char command[] = {0xA1, variableId};
  if(write(fd, &command, sizeof(command)) == -1)
  {
    perror("error writing");
    return SERIAL_ERROR;
  }
  unsigned char response[2];
  if(read(fd,response,2) != 2)
  {
    perror("error reading");
    return SERIAL_ERROR;
  }
  return response[0] + 256*response[1];
}

// Returns the target speed (-3200 to 3200).
// Returns SERIAL_ERROR if there is an error.
int smcGetTargetSpeed(int fd) {
  int val = smcGetVariable(fd, 20);
  return val == SERIAL_ERROR ? SERIAL_ERROR : (signed short)val;
}

// Returns a number where each bit represents a different error, and the
// bit is 1 if the error is currently active.
// See the user's guide for definitions of the different error bits.
// Returns SERIAL_ERROR if there is an error.
int smcGetErrorStatus(int fd) {
  return smcGetVariable(fd,0);
}

// Sends the Exit Safe Start command, which is required to drive the motor.
// Returns 0 if successful, SERIAL_ERROR if there was an error sending.
int smcExitSafeStart(int fd) {
  const unsigned char command = 0x83;
  if (write(fd, &command, 1) == -1)
  {
    perror("error writing");
    return SERIAL_ERROR;
  }
  return 0;
}

// Sets the SMC's target speed (-3200 to 3200).
// Returns 0 if successful, SERIAL_ERROR if there was an error sending.
int smcSetTargetSpeed(int fd, int speed)
{
  unsigned char command[3];
  if (speed < 0) {
    command[0] = 0x86; // Motor Reverse
    speed = -speed;
  }
  else {
    command[0] = 0x85; // Motor Forward
  }
  command[1] = speed & 0x1F;
  command[2] = speed >> 5 & 0x7F;
  if (write(fd, command, sizeof(command)) == -1)
  {
    perror("error writing");
    return SERIAL_ERROR;
  }
  return 0;
}

int main()
{
  printf("Starting the motor controller example\n");
  const char * device = "/dev/ttyO4";  // Linux
  int fd = open(device, O_RDWR | O_NOCTTY);
  if (fd == -1) {
    perror(device);
    return 1;
  }
  struct termios options;
  tcgetattr(fd, &options);
  options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
  options.c_oflag &= ~(ONLCR | OCRNL);
  tcsetattr(fd, TCSANOW, &options);
  smcExitSafeStart(fd);
  printf("Error status: 0x%04x\n", smcGetErrorStatus(fd));
  int speed = smcGetTargetSpeed(fd);
  printf("Current Target Speed is %d.\n", speed);
  int newSpeed = (speed <= 0) ? 3200 : -3200;
  printf("Setting Target Speed to %d.\n", newSpeed);
  smcSetTargetSpeed(fd, newSpeed);
  close(fd);
  return 0;
}
