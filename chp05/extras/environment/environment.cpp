// A short example that demonstrates how to read the environment variables
// from within a C/C++ program using  in two different ways.
// Written by Derek Molloy for the book Exploring BeagleBone

#include<iostream>
#include<cstdlib>
using namespace std;

// You can use a third argument on main() to receive the environment settings
int main(int argc, char *argv[], char *env[]){

	// Display all of the environment settings using a loop as follows
	char **env_ptr;
	for(env_ptr = env; *env_ptr != NULL; env_ptr++){
		cout << *env_ptr << endl;
	}

	cout << "**************************************************" << endl;

	// Alternatively, you can use the getenv() function
	char *val = getenv("SHELL");
	cout << "The user shell is: " << val << endl;
	cout << "The user path is: "  << getenv("PATH") << endl;

	return 0;
}

