/************************************************************
* File description: Server class  file.     				*
* Acts as server, receives clients, and controls the game
* flow of input/out, and serialization                      *
************************************************************/
#include "Server.h"
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

/*
 * Static function that is sent to each thread to perform.
 * It has a switch case which constantly checks what the next
 * action to be performed is, according to a global variable.
 */
static void* assist(void* s);

/*
 * The main thread is given this function, which
 * contains the main switch case which itself creates
 * the smaller threads and accepts input from user.
 */
static void* acceptClients(void* v);

pthread_mutex_t lockUpdate;
//global variable for sub threads to check to know next action
int threadCommand;
City city;
TaxiCenter tc;
Clock timeClock;
vector<Driver> waitingDrivers;
vector<Taxi> vehicles;
Socket* tcp;
int allClientsAssisted;
int numOfClients;
int threadsExited;

/***********************************************************************
	* function name: main										       *
	* The Input: port number										   *
	* The output: none										           *
	* The Function operation: calls neccessary functions to run server *
	*********************************************************************/
int main(int argc, char* argv[]) {
    Server s = Server();
    s.initialize();
    int portNum = stoi(argv[1]);
    // creates tcp for connection
    tcp = new Tcp(1, portNum);
    pthread_mutex_init(&lockUpdate, NULL);
    //creates all the threads
    pthread_t mainRun;
    pthread_create(&mainRun, NULL, acceptClients, (void*)1);
    pthread_join(mainRun,NULL);
    return 0;
}


/***********************************************************************
   * function name: run												   *
   * The Input: none												   *
   * The output: none										           *
   * The Function operation: contains switch case which runs the main
   * flow of the input and client/server interactions.
   * Depending on global variable, performs different actions           *
   ********************************************************************/
static void* acceptClients(void* dummy) {
    bool run = true;
    string action1;
    string input;
    string s;
    bool firstTime = true;

//Actions the user can perform
    while (run) {
        //While waits for a valid input
        do {
            /* prevents -1 being printing first time - otherwise, will
            print -1 if input is invalid */
            if (firstTime){
                firstTime = false;
            } else{
                cout<<" first time -1"<<endl;
            }
            cin >> action1;
        } while (action1.size()!=1 || !city.isNumber(action1));

        firstTime = true;
        int action = stoi(action1);

        switch(action) {

            case 1: //Insert Driver

                cin >> input; //how many drivers
                //Checks if input is a number
                if (!city.isNumber(input) || input.size()!=1){
                    cout<<" amount of drivers-1"<<endl;
                    break;
                }
                // Sets global variable to 1
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
                    int socket = tcp->acceptClient();
                    tempServer->setSocket(socket);
                    pthread_create(thread, NULL, assist, (void*)tempServer);
                }
                break;

            case 2: {//insert a trip
                cin >> input;
                Trip *t = city.createTrip(input);
                if (t == NULL) {
                    cout << " trip -1" << endl;
                    break;
                }
                t->setMap(tc.getMap());
                /*
                 * Returns whether the trip has a valid path,
                 * if not, it is not added to the taxi center inside
                 * function.
                 */
                bool validTrip = tc.addTrip(t);
                break;
            }
            case 3: {//Insert a taxi
                cin >> s;
                Taxi t = city.createTaxi(s);
                //Checks we dont already have that taxi
                if (t.getId() == -1 || tc.hasTaxi(t.getId())) {
                    cout << "taxi -1" << endl;
                    break;
                }
                tc.addTaxi(t);
                vehicles.push_back(t);
                break;
            }

            case 4: { //To check location of a driver
                string id;
                cin >> id;
                if(!city.isNumber(id)) {
                    cout << " 4 -1"<< endl;
                    break;
                }
                int drId=stoi(id);
                //Checks if the driver exists
                if(tc.getDrivers().size()>0 && tc.hasDriver(drId)) {
                    tc.requestDriverLocation(stoi(id));
                    break;
                } else {
                    /*
                     * Driver may be in waitingDriver vector,
                     * so checks there too
                     */
                    if (!tc.wdHasDriver(drId, waitingDrivers)) {
                        cout << "waitingDrivers: -1" << endl;
                    }
                }
                break;
            }
            case 9:
                timeClock.increaseTime();
                threadCommand = 9;
                break;

            case 7:
                threadCommand = 7;
                run = false;
                break;

            default:
                cout << " default -1" << endl;
                break;
        }

    }
    /*
     * Once 7 was inserted,
     * waits for all threads to exit, since they update
     * global variable in sendCommand
     * and only then will continued to delete itself
     * (ie, the main thread will exit)
     */
    while(threadsExited < numOfClients) {
        continue;
    }
    delete tcp;
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

        /*
         * If zero, means everyone completed theirs tasks,
         * and now can take on a new command, by setting
         * its variable assisted to false
         */
        if (allClientsAssisted == 0) {
            server->assisted = false;
        }

        switch (threadCommand) {
            case 1:
                //Means its the first time around the switch case
                if (missionTime < 0) {
                    server->receiveDriver();
                    server->assignVehicleToClient();
                    missionTime++;
                }
                break;

                //Moves client/ sends a new trip to client
            case 9:
                pthread_mutex_lock(&lockUpdate);
                /*
                 * Means its a new 9 command than last one
                 * since time was now updated, and our mission
                 * time was not yet, until end of if.
                 */
                if (missionTime != timeClock.getTime()) {
                    server->SendTripToClient();
                    server->sendNextLocation();
                    missionTime = timeClock.getTime();
                }
                pthread_mutex_unlock(&lockUpdate);
                break;
            case 7:
                //Notifies client to close themselves
                server->sendCommand(7);
                delete server;
                pthread_exit(0);
            default:
                break;
        }
        /*
         * If not assisted, means they can update the amount of
         * threads that have finished assisting, then set their
         * assisted to true so they wont enter the switch
         * case until the allClientsAssisted equals the numOfClients
         */
        if(!server->assisted) {
            //adds to global variable to show he assisted his client
            pthread_mutex_lock(&lockUpdate);
            allClientsAssisted += 1;
            pthread_mutex_unlock(&lockUpdate);
            server->assisted = true;
        }
        /*
         * if equal, means all threads can now
         * take the next command
         */
        if (allClientsAssisted == numOfClients) {
            //reset how many clients have been assisted
            allClientsAssisted = 0;
        }

    }
}

/*
 * constructor for server, which initializes
 * clock and assisted member
 */
Server::Server() {
    timeClock = Clock();
    assisted=false;
}

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
    int obstacleCount;
    Graph *grid;
    bool isValid= true;

    //Do while loop to get a valid input for grid
    do {
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

    if (!myDriver->hasTrip()) {
        //Finds how many trips start at the current time
        int numOfTrips = tc.checkTripTimes(timeClock.getTime());

        /*
         * If there is a trip to assign, it checks that the
         * threads driver id matches the driver waiting
         * in line in vector waitingDrivers to get the next
         * trip
         */
        if (numOfTrips > 0) {
            //Finds next trip for this time
            trip = tc.getNextTrip(timeClock.getTime());
           // cout << "Socket: " << clientSocket << endl;
           // cout << "Driver id: " << myDriver->getDriverId() << endl;
           // cout << "Wants to take trip: " << trip->getId();
            int i = 0;
            Point p = trip->getStart();
            /*
             * Finds the driver which is waiting
             * at the start point of that trip
            */
            while (!waitingDrivers[i].getTrip()->getEnd().equal(&p)
                   && i < waitingDrivers.size()) {
                i++;
            }
            /* if thread has same driver id as next driver in line,
             then takes the next trip */
            if (myDriver->getDriverId() == waitingDrivers[i].getDriverId()) {
                waitingDrivers.erase(waitingDrivers.begin() + i);
                myDriver->setTrip(trip);
                Graph *tempMap = tc.getMap();
                myDriver->setMap(tempMap);

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
                //adds trip back if id's didnt match
                tc.addTrip(trip);

            }
        }
    }
}



/***********************************************************************
	* function name: createString									       *
	* The Input: buffer and size of buffer								 *
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
        /*
         * adds to global variable so main thread
         * can know when it can exit itself
         */
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
            //int id = myDriver->getTrip()->getThreadId();
            //pthread_t* calc = tc.getTripCalculator(id);
            while(!myDriver->getTrip()->pathCalculated()) {
                /*
                 * waits for the trip to finish calculating.
                 * Cannot use join because the thread is in
                 * threadpool and will never exit until end of
                 * program - so we would never be able to
                 * continue
                 */
            }
            //Adds driver to taxi ccenter now that it has a trip
            if (!tc.hasDriver(myDriver->getDriverId())) {
                tc.addDriver(*myDriver);
            }
            //Gets next point to send to client
            Point *ptrPoint = myDriver->getTrip()->getNextInPath();
            sleep(1);
            //cout << "Socket: "<<clientSocket<<endl;
            cout << "Wants to send point: "<<*ptrPoint<<endl;
            //Updates location of same driver in taxi station
            tc.moveDriver(myDriver->getDriverId(), ptrPoint);
            //Updates thread's driver current location
            myDriver->getTrip()->updateStartPoint(*ptrPoint);
            std::string nextLocation;
            boost::iostreams::back_insert_device<std::string> inserter(nextLocation);
            boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s1(inserter);
            boost::archive::binary_oarchive oa(s1);
            oa << ptrPoint;
            s1.flush();
            //notifies clients they are about to receive a new location
            Server::sendCommand(9);
            tcp->sendData(nextLocation, clientSocket);
            //cout << "after send data"<<endl;
            verifyResponse();
            //cout << "after verify response"<<endl;
            delete ptrPoint;

            //need to assign driver a new trip if arrived
            if (myDriver->arrived()) {
                waitingDrivers.push_back(*myDriver);
                tc.deleteDriver(myDriver->getDriverId());
                //Variable 'hasTrip' is switched to false
                myDriver->eraseTrip();
                //Goes to see if there is a new trip to send to client
                Server::SendTripToClient();
            }
        }
    }
}


/***********************************************************************
	* function name: assignVehicleToClient							*
	* The Input: none													   *
	* The output: none										               *
	* The Function operation: finds the right vehicle depending on driver
     * id, serializes it, and sends it to the client                       *
	***********************************************************************/
void Server::assignVehicleToClient() {
    int counter = 0;
    vector<Taxi>::iterator taxiIter = vehicles.begin();
    int len = vehicles.size();

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
    tcp->sendData(serial_str, clientSocket);
    verifyResponse();
    //once taxi has been assigned to a driver, it can be deleted from vehicle vector
    vehicles.erase(vehicles.begin() + counter);

}

/*
 * Function to verify that the client received
 * what was sent
 */
void Server::verifyResponse() {
    char buffer[1024];
    int ygjygv = tcp->reciveData(buffer, sizeof(buffer), clientSocket);
}