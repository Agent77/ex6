#include "sockets/Tcp.h"
#include "Driver.h"
#include "DriverClient.h"
#include "LuxuryCab.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>



using namespace std;
using namespace boost::archive;


int main(int argc, char* argv[]) {
    City c = City();
    DriverClient client = DriverClient();
    string d;
    cin >> d;
    Driver dr = c.createDriver(d);
    if(dr.getDriverId()!=-1) {
        Driver *driver = &dr;
        string ipStr = argv[1];
        string portStr = argv[2];
        client.openSocket(driver, ipStr, portStr);
        client.receiveVehicle();
        client.receiveCommand();
    } else {
        client.closeSocket();
    }
    return 0;
}


DriverClient::DriverClient() {
    clock = Clock();
}

/***********************************************************************
	* function name: receiveTrip										   *
	* The Input: none													   *
	* The output: none										               *
	* The Function operation: Client has been notified to receive a trip,
     * and assigns its driver the trip it receives                         *
	***********************************************************************/
void DriverClient::receiveTrip() {


    char buffer[1024];
    // RECEIVE TRIP FROM SERVER
    client->reciveData(buffer, sizeof(buffer) ,0);
    string tripString = createString(buffer, sizeof(buffer));
    Trip* trip = NULL;
    boost::iostreams::basic_array_source<char> device1(tripString.c_str(), tripString.size());
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s1(device1);
    boost::archive::binary_iarchive ia1(s1);
    ia1 >> trip;

    // GIVE DRIVER THE TRIP
    driver.setTrip(trip);
    //delete trip;
}

/***********************************************************************
	* function name: receiveCommand										   *
	* The Input: none													   *
	* The output: none										               *
	* The Function operation: After every function, the client again waits
     * to receive a command from server, to know what to expect as next
     * serialized reception                                                *
	***********************************************************************/
int DriverClient::receiveCommand() {
    char buffer[1024];
    const int newPoint = 9;
    const int newTrip = 2;
    const int close = 7;
    int command = 0;
    while(command!=7) {
        sendVerification();
        //RECEIVE COMMAND
        client->reciveData(buffer, sizeof(buffer), 0);

        string commandString = createString(buffer, sizeof(buffer));
        boost::iostreams::basic_array_source<char> device2(commandString.c_str(), commandString.size());
        boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s4(device2);
        boost::archive::binary_iarchive ia2(s4);
        ia2 >> command;
        sendVerification();

        if (command == newPoint) {
            DriverClient::receiveNextPoint();
        }
        if (command == newTrip) {
            DriverClient::receiveTrip();
        }
        if (command == close) {
            DriverClient::closeSocket();
        }
    }
}

/***********************************************************************
	* function name: closeSocket    									   *
	* The Input: none													   *
	* The output: none										               *
	* The Function operation: closes socket to server                      *
	***********************************************************************/
void DriverClient::closeSocket() {
    delete driver.getTrip();
    delete client;
}

/***************************************************************************
	* function name: receiveNextPoint									   *
	* The Input: none													   *
	* The output: none										               *
	* The Function operation: receives next point from server and updates  *
     * the drivers trip                                                    *
	***********************************************************************/
void DriverClient::receiveNextPoint() {
    char buffer[1024];
    Trip newTrip;
    //RECEIVE POINT
    client->reciveData(buffer, sizeof(buffer),0);

    Point* p;
    string nextLocation = createString(buffer, sizeof(buffer));
    boost::iostreams::basic_array_source<char> device3(nextLocation.c_str(), nextLocation.size());
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s3(device3);
    boost::archive::binary_iarchive ia(s3);
    ia >> p;

    Trip* tripP = driver.getTrip();

    tripP->updateStartPoint(p);
    driver.getTrip()->updateStartPoint(p);
    //delete p;

}


/***********************************************************************
	* function name: openSockets										   *
	* The Input: driver, string, and port								   *
	* The output: none										               *
	* The Function operation: opens a socket with inserted ip/port num.
     * Also saves the driver inserted by user for later use                *
	***********************************************************************/
void DriverClient::openSocket(Driver *driverSent, string currentIp, string port) {
    driver = *driverSent;
    portNum=stoi(port);
    client = new Tcp(0,portNum);
    client->setIP(currentIp);
    int result = client->initialize(0);

    // SERIALIZATION
    std::string serial_str;
    boost::iostreams::back_insert_device<std::string> inserter(serial_str);
    boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
    boost::archive::binary_oarchive oa(s);
    oa << driverSent;
    s.flush();
    // SENDS DRIVER TO SERVER
    int result1 = client->sendData(serial_str,0);

}

/***********************************************************************
	* function name: receiveVehicle										   *
	* The Input: none													   *
	* The output: none										               *
	* The Function operation: receives a Taxi vehicle from server and
     * assigns it to its driver                                            *
	***********************************************************************/
void DriverClient::receiveVehicle() {
    char buffer[1024];
    // GETS TAXI IN RETURN
    int resultData = client->reciveData(buffer, sizeof(buffer),0);
    // DESERIALIZATION
    string taxiString = createString(buffer, sizeof(buffer));
    Taxi *taxi;
    boost::iostreams::basic_array_source<char> device(taxiString.c_str(), taxiString.size());
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(device);
    boost::archive::binary_iarchive ia(s2);
    ia >> taxi;
    //GIVE DRIVER TAXI

    driver.setTaxi(*taxi);
    delete taxi;
}


Driver DriverClient::getDriver() {
    return this->driver;
}

/***********************************************************************
   * function name: createString										   *
   * The Input: buffer													   *
   * The output: string of buffer										   *
   * The Function operation: creates a string from a buffer               *
   ***********************************************************************/
string DriverClient::createString(char* buffer, int bufferSize) {
    std::string str(buffer, bufferSize);
    return str;
}

/*
 * Function to send verification that
 * client received information
 */
void DriverClient::sendVerification() {
    int verification = 77;
    // SERIALIZATION OF COMMAND
    std::string verify;
    boost::iostreams::back_insert_device<std::string> inserter(verify);
    boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s1(inserter);
    boost::archive::binary_oarchive oa(s1);
    oa << verification;
    s1.flush();
    client->sendData(verify, 0);


}