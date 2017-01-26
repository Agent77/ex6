/************************************************************
* File description: Server class  file.     				*
* Acts as server, receives clients, and controls the game
* flow of input/out, and serialization                      *
************************************************************/
#include "Server.h"
//#include <boost/log/trivial.hpp>
//#include "easylogging++.h"
//#include <boost/lexical_cast.hpp>
//#include <boost/tokenizer.hpp>
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
static void* assist(void* s);
static void* acceptClients(void* v);
pthread_mutex_t lockUpdate, lock9;

bool commandCompleted;
int threadCommand; //global variable for sub threads to check to know next action
City city;
TaxiCenter tc;
Clock timeClock;
vector<Driver> waitingDrivers;
vector<Taxi> vehicles;
Graph* g;
Socket* tcp;
int rank;
int allClientsAssisted;
int numOfClients;
int threadsExited;
vector<waitingPoint> landmarks;
void* createMainSocket(string port);

/***********************************************************************
	* function name: main										       *
	* The Input: port number										   *
	* The output: none										           *
	* The Function operation: calls neccessary functions to run server *
	***********************************************************************/
int main(int argc, char* argv[]) {
    Server s = Server();
    s.initialize();
    // BOOST_LOG_TRIVIAL(debug)<<"trying debug";

    int portNum = stoi(argv[1]);

    // creates tcp for connection
    tcp = new Tcp(1, portNum);
    pthread_mutex_init(&lockUpdate, NULL);
    //creates all the threads
    pthread_t mainRun;
    pthread_create(&mainRun, NULL, acceptClients, (void*)1);
    pthread_join(mainRun,NULL);

    //delete tcp;

}


/***********************************************************************
	* function name: createClients										   *
	* The Input: string of port number													   *
	* The output: none										               *
	* The Function operation: opens a socket for the client to reach	   *
	***********************************************************************/
void* createMainSocket(void* port) {
    string* s = (string*)port;
    int portNum = stoi(*s);

    // creates tcp for connection
    tcp = new Tcp(1, portNum);

    pthread_t mainRun;
    pthread_create(&mainRun, NULL, acceptClients, NULL);
    pthread_join(mainRun,(void**)1);


}


/***********************************************************************
   * function name: run												   *
   * The Input: none												   *
   * The output: none										           *
   * The Function operation: contains switch case which runs the main
   * flow of the input and client/server interactions                  *
   ********************************************************************/
static void* acceptClients(void* dummy) {
    //Server* server = (Server*)s;
    bool run = true;
    string action1;
    string input;
    string s;
    bool firstTime=true;

//Actions the user can perform
    while (run) {
        commandCompleted = false;
        do {
            if (firstTime){
                firstTime=false;
            } else{
                cout<<" first time -1"<<endl;
            }
            cin >> action1;
        }while (action1.size()!=1 || !city.isNumber(action1));
        firstTime=true;
        int action = stoi(action1);
        switch(action) {
            case 1: //Insert Driver
            {
                cin >> input; //how many drivers
                if (!city.isNumber(input) || input.size()!=1){
                    cout<<" amount of drivers-1"<<endl;
                    break;
                }
                threadCommand = 1;
                //creating the threads given clients
                int num = stoi(input);
                numOfClients = num;
                int clientSockets = tcp->initialize(num);
                int i;
                //Opens a thread for each client received
                for(i = 0; i < numOfClients; i++) {
                    pthread_t* thread =  new pthread_t();
                    Server* tempServer = new Server();
                    tempServer->setRank(i);
                    int socket = tcp->acceptClient();
                    tempServer->setSocket(socket);
                    pthread_create(thread, NULL, assist, (void*)tempServer);
                }
                break;
            }
            case 2: {
                cin >> input;
                Trip* t = city.createTrip(input);
                if (t==NULL){
                    cout << " trip -1" << endl;
                    break;
                }
                t->setMap(tc.getMap());
                bool validTrip = tc.addTrip(t);
                break;
            }
            case 3: {
                cin >> s;
                Taxi t = city.createTaxi(s);
                if(t.getId()== -1 || tc.hasTaxi(t.getId())) {
                    cout << "taxi -1" << endl;
                    break;
                }
                tc.addTaxi(t);
                vehicles.push_back(t);
                break;
            }
            case 4: {
                string id;
                cin >> id;
                if(!city.isNumber(id)) {
                    cout << " 4 -1"<< endl;
                    break;
                }
                int drId=stoi(id);
                if(tc.getDrivers().size()>0 && tc.hasDriver(drId)) {
                    tc.requestDriverLocation(stoi(id));
                    break;
                } else {
                    if (!tc.wdHasDriver(drId, waitingDrivers)) {
                        cout << "waitingDrivers: -1" << endl;
                    }
                }
                break;
            }
            case 9:
                // ADVANCE TIME
                timeClock.increaseTime();
                threadCommand = 9;
                break;
            case 7:
                threadCommand = 7;
                run = false;

                break;
            default:
                cout << " default-1" << endl;
                break;
        }

    }
    //waits for all threads to exit
    while(threadsExited < numOfClients) {
        continue;
    }
    delete tcp;
    delete g;
    pthread_exit(0);
}

/*
 * function run by all threads.
 */
void* assist(void* s) {
    bool run = true;
    Server *server = (Server *) s;
    int missionTime = -1;
    int driverId;
    while (run) {

        //If zero,
        if (allClientsAssisted == 0) {
            server->assisted = false;
        }

        //if (!server->assisted) {
        switch (threadCommand) {
            case 1:
                // ASSIGNS A VEHICLE TO CLIENT ONLY IF TRIP TIME ARRIVES
                if (missionTime < 0) {
                    server->receiveDriver();
                    server->assignVehicleToClient();
                    missionTime++;
                }

                break;
            case 9:

                pthread_mutex_lock(&lockUpdate);
                if (missionTime != timeClock.getTime()) {
                    server->SendTripToClient();
                    server->sendNextLocation();
                    missionTime = timeClock.getTime();
                }
                pthread_mutex_unlock(&lockUpdate);


                break;
            case 7:
                //run = false;
                server->sendCommand(7);
                delete server;
                pthread_exit(0);
            default:
                break;
        }
        if(!server->assisted) {
            //adds to global variable to show he assisted his client
            pthread_mutex_lock(&lockUpdate);
            allClientsAssisted += 1;
            pthread_mutex_unlock(&lockUpdate);
            server->assisted = true;
        }

        if (allClientsAssisted == numOfClients) {
            //reset
            allClientsAssisted = 0;
        }

    }
}


Server::Server() {
    timeClock = Clock();
    assisted=false;
}

/***********************************************************************
	* function name: closeSockets										   *
	* The Input: none													   *
	* The output: none										               *
	* The Function operation: notifies all clients to close themselves     *
	***********************************************************************/


/**************************************************************************
	* function name: initialize											   *
	* The Input: none													   *
	* The output: none										               *
	* The Function operation: initializes the grid and obstacles for the
     * game                                                                *
****************************************************************************/
void Server::initialize() {
    string obstacles;
    string size;
    /*string size1;
    string size2;*/
    int obstacleCount;
    Graph *grid;
    bool isValid= true;

    do {
        //To be used later as graph size
        /*cin >> size1;
        cin >> size2;*/
        string size;
        getline(cin,size);
        grid = city.createGraph(size);
        if (grid == NULL) {
            cout << "-1" << endl;
            isValid=false;
        } else {
            isValid = true;
        }
        if(isValid) {
            //Checks for obstacles
            cin >> obstacles;
            if (!city.isNumber(obstacles)) {
                isValid = false;
            }
            std::istringstream(obstacles) >> obstacleCount;
            //Adds obstacles at given points in grid
            if (obstacleCount != 0 && isValid) {
                string obstacle;
                for (int count = 0; count < obstacleCount; count++) {
                    cin >> obstacle;
                    Coordinate *c = city.createCoordinate(obstacle);
                    if (c == NULL) {
                        cout << " obstqacle -1" << endl;
                        isValid = false;
                    } else {

                        grid->addObstacle(c);
                    }
                }
            }
        }
    }while (!isValid);
    //adds grid to the taxi center
    tc = TaxiCenter(grid);
    g = grid;
}

/***********************************************************************
	* function name: SendTripToClient								       *
	* The Input: none													   *
	* The output: none										               *
	* The Function operation: Finds the correct trip according to the      *
     * current time, serializes it, and send it to the client              *
	***********************************************************************/
void Server::SendTripToClient() {
    std::string serializedTrip;
    int counter = 0;
    Trip *trip;
    Trip *trip1;
    //pthread_mutex_lock(&lockUpdate);
    //Finds how many trips start at the current time
    if (!myDriver->hasTrip()) {
        int numOfTrips = tc.checkTripTimes(timeClock.getTime());


        if (numOfTrips > 0) {

            //Check if this driver is next
            trip = tc.getNextTrip(timeClock.getTime());
           // cout << "Socket: " << clientSocket << endl;
           // cout << "Driver id: " << myDriver->getDriverId() << endl;
           // cout << "Wants to take trip: " << trip->getId();
            int i = 0;
            Point p = trip->getStart();
            while (!waitingDrivers[i].getTrip()->getEnd().equal(&p) && i < waitingDrivers.size()) {
                i++;
            }
            //if thread has same driver id, then takes the next trip
            if (myDriver->getDriverId() == waitingDrivers[i].getDriverId()) {

                waitingDrivers.erase(waitingDrivers.begin() + i);
                myDriver->setTrip(trip);
                Graph *tempMap = tc.getMap();
                myDriver->setMap(tempMap);
                //tc.calculatePath(&calc, myDriver);

                trip1 = trip;
                //SERIALIZATION OF TRIP
                boost::iostreams::back_insert_device<std::string> inserter(serializedTrip);
                boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(
                        inserter);
                boost::archive::binary_oarchive oa(s);
                oa << trip1;
                s.flush();
                //Notifies client that they are going to receive a trip now
                Server::sendCommand(2);
                tcp->sendData(serializedTrip, clientSocket);
                verifyResponse();
            } else {
                //adds trip back if it didnt take it
                //Trip* addBackTrip = new Trip(trip1);
                tc.addTrip(trip);

            }
        }
    }
}



/***********************************************************************
	* function name: createString									       *
	* The Input: buffer and size of buffer													   *
	* The output: none										               *
	* The Function operation: creates a string from the buffer	           *
	***********************************************************************/
string Server::createString(char* buffer, int bufferSize) {
    std::string s(buffer, bufferSize);
    return s;
}

/***********************************************************************
	* function name: receiveDriver										   *
	* The Input: none													   *
	* The output: none										               *
	* The Function operation: waits to receive serialized version of driver
     * sent from client and adds it to a vector of temporary drivers       *
	***********************************************************************/
void Server::receiveDriver() {
    // RECEIVE DRIVER FROM CLIENT
    char buffer[1024];
    tcp->reciveData(buffer, sizeof(buffer), clientSocket);

    // DESERIALIZE BUFFER INTO DRIVER
    string s = createString(buffer, sizeof(buffer));
    Driver *receivedDriver;
    boost::iostreams::basic_array_source<char> device(s.c_str(), s.size());
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(device);
    boost::archive::binary_iarchive ia(s2);
    ia >> receivedDriver;
    //adds received driver to temp vector of drivers, until it is assigned a trip
    myDriver = receivedDriver;
    //Trip* t = new Trip(0,0,0,0,0,0,0,0);
    //myDriver->setTrip(t);
    waitingDrivers.push_back(*myDriver);
}

/***********************************************************************
   * function name: sendCommand										   *
   * The Input: int													   *
   * The output: none										               *
   * The Function operation: serializes and sends a command to client
    * Sends a number, depending on whether it wants the client to wait for
    * a trip, a new location, or a vehicle, or to close itself            *
   ***********************************************************************/
void Server::sendCommand(int command) {

    // SERIALIZATION OF COMMAND
    std::string commandString;
    boost::iostreams::back_insert_device<std::string> inserter(commandString);
    boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s1(inserter);
    boost::archive::binary_oarchive oa(s1);
    oa << command;
    s1.flush();
    tcp->sendData(commandString, clientSocket);

    if(command == 7) {
        threadsExited += 1;
    }
    verifyResponse();
}

/***********************************************************************
	* function name: sendNextLocation									   *
	* The Input: none													   *
	* The output: none										               *
	* The Function operation: drives the drivers, gets its next location,
     * serializes the point and sends it to client                         *
	***********************************************************************/
void Server::sendNextLocation() {
    int x = 0;
    int y = 0;
    if(myDriver->hasTrip()) {
        if (myDriver->getTrip()->getTripTime() < timeClock.getTime()) {
            int id = myDriver->getTrip()->getThreadId();
            pthread_t* calc = tc.getTripCalculator(id);
            //pthread_join(*calc, NULL);
            while(!myDriver->getTrip()->pathCalculated()) {
                //cout<< "waiting for calculation to finish..."<<endl;
            }
            //cout << "Calculated?"<<myDriver->getTrip()->pathCalculated()<<endl;
            if (!tc.hasDriver(myDriver->getDriverId())) {
                tc.addDriver(*myDriver);
            }
            Point *ptrPoint = myDriver->getTrip()->getNextInPath();
            sleep(1);
            //cout << "Socket: "<<clientSocket<<endl;
            cout << "Wants to send point: "<<*ptrPoint<<endl;
            tc.moveDriver(myDriver->getDriverId(), ptrPoint);
            myDriver->getTrip()->updateStartPoint(*ptrPoint);
            std::string nextLocation;
            boost::iostreams::back_insert_device<std::string> inserter(nextLocation);
            boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s1(inserter);
            boost::archive::binary_oarchive oa(s1);
            oa << ptrPoint;
            s1.flush();
            //notifies clients they are about to receive a new location
            //cout << "before send Command"<<endl;
            //BOOST_HAS_LOG1P() << "trying log info"<<endl;
            Server::sendCommand(9);
            //cout << "before send data"<<endl;

            tcp->sendData(nextLocation, clientSocket);
            //cout << "after send data"<<endl;
            //pthread_mutex_unlock(&lockSend);
            verifyResponse();
            //cout << "after verify response"<<endl;

            delete ptrPoint;
            //need to assign driver a new trip
            if (myDriver->arrived()) {
                waitingDrivers.push_back(*myDriver);
                tc.deleteDriver(myDriver->getDriverId());

                myDriver->eraseTrip();
                Server::SendTripToClient();
            }
        }// }
    }
}


/***********************************************************************
	* function name: assignVehicleToClient												   *
	* The Input: none													   *
	* The output: none										               *
	* The Function operation: finds the right vehicle depending on driver
     * id, serializes it, and sends it to the client                       *
	***********************************************************************/
void Server::assignVehicleToClient() {
    int counter = 0;
    vector<Taxi>::iterator taxiIter = vehicles.begin();
    int len = vehicles.size();
    //int id = waitingDrivers.front().getDriverId();

    // FINDS CORRECT TAXI FOR DRIVER ID
    while (counter<len && vehicles[counter].getId()!= myDriver->getDriverId()){
        taxiIter++;
        counter++;
    }
    //sets taxi to driver with matching id, who still doesnt have a trip
    Taxi t = vehicles[counter];

    Taxi* taxiPointer = &(vehicles[counter]);
    // SERIALIZATION OF TAXI
    std::string serial_str;
    boost::iostreams::back_insert_device<std::string> inserter(serial_str);
    boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s1(inserter);
    boost::archive::binary_oarchive oa(s1);
    oa << taxiPointer;
    s1.flush();
    // RETURN TAXI TO CLIENT
    //pthread_mutex_lock(&lockSend);
    tcp->sendData(serial_str, clientSocket);
    //pthread_mutex_unlock(&lockSend);
    verifyResponse();
    //once taxi has been assigned to a driver, it can be deleted from vehicle vector
    vehicles.erase(vehicles.begin() + counter);

}

void Server::verifyResponse() {
    char buffer[1024];
    //pthread_mutex_lock(&lockRec);
    int ygjygv = tcp->reciveData(buffer, sizeof(buffer), clientSocket);
    //pthread_mutex_unlock(&lockRec);
    string commandString = createString(buffer, sizeof(buffer));
    /*int command = 0;
    boost::iostreams::basic_array_source<char> device2(commandString.c_str(), commandString.size());
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s4(device2);
    boost::archive::binary_iarchive ia2(s4);
    ia2 >> command;*/
    //cout << "received verification in " << clientSocket <<endl;
}

void Server::setRank(int r) {
    rank = r;
}