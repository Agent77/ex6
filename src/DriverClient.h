//
// Created by Tiki Lobel on 12/26/16.
//

#ifndef PRACTICE6_DRIVERCLIENT_H
#define PRACTICE6_DRIVERCLIENT_H

#include "City.h"
#include <boost/iostreams/device/array.hpp>
#include "Clock.h"
#include <boost/serialization/access.hpp>
#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

class DriverClient {
private:
    Socket* client;
    Driver driver;
    Clock clock;
    int portNum;
    string ip;
public:
    DriverClient();
    /***********************************************************************
	* function name: openSockets										   *
	* The Input: driver, string, and port								   *
	* The output: none										               *
	* The Function operation: opens a socket with inserted ip/port num.
     * Also saves the driver inserted by user for later use                *
	***********************************************************************/
    void openSocket(Driver* driver, string currentIp, string port);
    /***********************************************************************
	* function name: receiveTrip										   *
	* The Input: none													   *
	* The output: none										               *
	* The Function operation: Client has been notified to receive a trip,
     * and assigns its driver the trip it receives                         *
	***********************************************************************/
    void receiveTrip();
    /***********************************************************************
	* function name: receiveCommand										   *
	* The Input: none													   *
	* The output: none										               *
	* The Function operation: After every function, the client again waits
     * to receive a command from server, to know what to expect as next
     * serialized reception                                                *
	***********************************************************************/
    int receiveCommand();
    /***********************************************************************
	* function name: receiveNextPoint									   *
	* The Input: none													   *
	* The output: none										               *
	* The Function operation: receives next point from server and updates
     * the drivers trip                                                    *
	***********************************************************************/
    void receiveNextPoint();
    /***********************************************************************
	* function name: closeSocket    									   *
	* The Input: none													   *
	* The output: none										               *
	* The Function operation: closes socket to server                      *
	***********************************************************************/
    void closeSocket();
    Driver getDriver();
    /***********************************************************************
	* function name: createString										   *
	* The Input: buffer													   *
	* The output: string of buffer										   *
	* The Function operation: creates a string from a buffer               *
	***********************************************************************/
    string createString(char* buffer, int bufferSize);
    /***********************************************************************
	* function name: receiveVehicle										   *
	* The Input: none													   *
	* The output: none										               *
	* The Function operation: receives a Taxi vehicle from server and
     * assigns it to its driver                                            *
	***********************************************************************/
    void receiveVehicle();
    void sendVerification();
};


#endif //PRACTICE6_DRIVERCLIENT_H