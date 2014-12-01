/*
 * ConnectionHandler.cpp  Created on: 18 Jul 2014
 * Copyright (c) 2014 Derek Molloy (www.derekmolloy.ie)
 * Made available for the book "Exploring BeagleBone" 
 * See: www.exploringbeaglebone.com
 * Licensed under the EUPL V.1.1
 *
 * This Software is provided to You under the terms of the European 
 * Union Public License (the "EUPL") version 1.1 as published by the 
 * European Union. Any use of this Software, other than as authorized 
 * under this License is strictly prohibited (to the extent such use 
 * is covered by a right of the copyright holder of this Software).
 * 
 * This Software is provided under the License on an "AS IS" basis and 
 * without warranties of any kind concerning the Software, including 
 * without limitation merchantability, fitness for a particular purpose, 
 * absence of defects or errors, accuracy, and non-infringement of 
 * intellectual property rights other than copyright. This disclaimer 
 * of warranty is an essential part of the License and a condition for 
 * the grant of any rights to this Software.
 * 
 * For more details, see http://www.derekmolloy.ie/
 */

#include "ConnectionHandler.h"
#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include <string.h>
#include "SocketServer.h"
using namespace std;

namespace exploringBB {

ConnectionHandler::ConnectionHandler(SocketServer *parent, sockaddr_in *client, int clientSocketfd) {
	this->parent = parent;
	this->client = client;
	this->clientSocketfd = clientSocketfd;
	this->running = true;
}

ConnectionHandler::~ConnectionHandler() {
	delete this->client;
	cout << "Destroyed a Connection Handler" << endl;
}

int ConnectionHandler::start(){
	//cout << "Starting the Connection Handler thread" << endl;
	return (pthread_create(&(this->thread), NULL, threadHelper, this)==0);
}

void ConnectionHandler::wait(){
	(void) pthread_join(this->thread, NULL);
}

int ConnectionHandler::send(std::string message){
	const char *writeBuffer = message.data();
	int length = message.length();
    int n = write(this->clientSocketfd, writeBuffer, length);
    if (n < 0){
       perror("Connection Handler: error writing to server socket.");
       return 1;
    }
    return 0;
}

string ConnectionHandler::receive(int size=1024){
    char readBuffer[size];
    bzero(readBuffer, size);
    int n = read(this->clientSocketfd, readBuffer, sizeof(readBuffer));
    if (n < 0){
       perror("Connection Handler: error reading from server socket.");
    }
    return string(readBuffer);
}

void ConnectionHandler::threadLoop(){
    //cout << "*** Created a Connection Handler threaded Function" << endl;
    while(this->running){
       string rec = this->receive(1024);
       cout << "Received from the client [" << rec << "]" << endl;
       string message("The Server says thanks!");
       cout << "Sending back [" << message << "]" << endl;
       cout << "  but going asleep for 5 seconds first...." << endl;
       usleep(5000000);
       this->send(message);
       this->running = false;
	}
    //cout << "*** End of the Connection Handler Function" << endl;
    this->parent->notifyHandlerDeath(this);
}

} /* namespace exploringBB */
