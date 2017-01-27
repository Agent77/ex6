#include <sstream>
#include "City.h"



City::City() {
    gridSizeX=0;
    gridSizeY=0;
}

void City::CallTaxiCenter(Passenger p) {

}

Passenger City::checkForPassengerCalls() {

}

/*
 * The function parses a given string and creates new driver. if input is incorrect, returns fake
 * driver with id -1.
 */
Driver City::createDriver(string s) {
    char status;
    char cId[3];
    char cAge[3];
    char cExp[3];
    char cVId[3];
    int index=0;
    int counter = 0;
    Driver d;
    bool valid=true;
    for (int j=0; j<s.size();j++){
        if (s[j]!=',') {
            if (counter != 2) {
                if (isdigit(s[j])) {
                    switch (counter) {
                        case 0:
                            cId[index] = s[j];
                            break;
                        case 1:
                            cAge[index] = s[j];
                            break;
                        case 3:
                            cExp[index] = s[j];
                            break;
                        case 4:
                            cVId[index] = s[j];
                            break;
                    }
                    index++;
                }else {
                    valid=false;
                    break;
                }
            }else{
                status=s[j];
                valid = isValidStatus(status);
            }
        }else{
            counter++;
            index=0;
        }
    }
    if (counter==4 && valid){
        string sId=string (cId);
        string sAge=string (cAge);
        string sExp=string (cExp);
        string sVId=string (cVId);
        int id = stoi(sId);
        int age = stoi(sAge);
        int exp = stoi(sExp);
        int vId = stoi(sVId);
        if (id>=0 && age>=0 && exp>=0 && vId>=0){
            d = Driver(id, age, status, exp, vId);
            return d;
        }
    }
   d = Driver (-1,-1,'M',-1,-1);
    return d;
}

/*
 * The function parses a given string and creates new trip. if input is incorrect returns NULL.
 */
Trip* City::createTrip(string s) {
    //8 ints
    int i = 0;
    int value[13];
    int f;
    string g;
    if (validTrip(s)) {
        std::istringstream sep(s);
        std::getline(sep, g, ',');
        std::istringstream(g) >> f;
        for (int j = 0; j < s.size() / 2; j++) {
            value[i] = f;
            i++;
            std::getline(sep, g, ',');
            std::istringstream(g) >> f;
        }
        Trip* trip = new Trip(value[0], value[1], value[2], value[3],
                              value[4], value[5], value[6], value[7]);
        return trip;
    } else {
        return NULL;
    }
}

/*
 * The function parses a given string and creates new coordinate. if input is incorrect returns NULL.
 */
Coordinate* City::createCoordinate(string s) {
    int size=s.size()/2 +1;
    int counter=0;
    char c1[4]={0};
    char c2[4]={0};
    int index=0;
    if(size!=2){
        return NULL;
    }
    for (int i=0; i < s.size(); i++) {
        if (s[i] != ',') {
            switch (counter) {
                case 0:
                    c1[index] = s[i];
                    index++;
                    break;
                case 1:
                    c2[index] = s[i];
                    index++;
                    break;
            }
        } else {
            counter++;
            index=0;
        }
    }
    string s1 = string(c1);
    string s2 = string(c2);
    if(!isNumber(s1) || !isNumber((s2))){
        return NULL;
    }
    int x = stoi(s1);
    int y = stoi(s2);
    if (x<0 || x>gridSizeX || y<0 || y>gridSizeY || x<0 || x>gridSizeX
        || y<0 || y>gridSizeY){
        return NULL;
    }
    Coordinate *point = new Point(x, y);
    return point;
}

/*
 * The function parses a given string and creates new graph. if input is incorrect returns NULL.
 */
Graph* City::createGraph(string s) {
    char c1[4]={0};
    char c2[4]={0};
    int counter =0;
    int index=0;
    for (int i=0; i<s.size(); i++){
        if(isdigit(s[i])){
            switch (counter) {
                case 0:
                    c1[index] = s[i];
                    index++;
                    break;
                case 1:
                    c2[index] = s[i];
                    index++;
                    break;
            }
        } else if (s[i]==' ' && counter==0){
            index=0;
            counter++;
        } else{
            return NULL;
        }
    }
    if (c1[0]==0 || c2[0]==0){
        return NULL;
    }
    string s1= string (c1);
    string s2= string (c2);
    gridSizeX = stoi(s1);
    gridSizeY = stoi(s2);
    if (gridSizeX <= 0 || gridSizeY <= 0) {
        return NULL;
    }
    Graph *graphPointer = new Grid(gridSizeX, gridSizeY);
    return graphPointer;
}

/*
 * checks whether a given char is a correct driver status
 */
bool City::isValidStatus(char c){
    switch(c) {
        case 'S':
            return true;
        case 'M':
            return true;
        case 'W':
            return true;
        case 'D':
            return true;
        default:
            return false;
    }
}

/*
 * checks if a given string has only numbers and ','
 */
bool City::isNumber(string s) {
    for (int i=0; i<s.size(); i++){
        if (!isdigit(s[i]) && !s[i]==','){
            return false;
        }
    }
    return true;
}

/*
 * The function parses a given string and creates new taxi. if input incorrect returns fake taxi
 * with id -1.
 */
Taxi City::createTaxi(string s) {
    int counter=0;
    int size= s.size();
    int index=0;
    char s1[3];
    char s2[3];
    bool valid=true;
    int i=0;
    int id;
    int type;
    for(; i<size; i++){
        if (s[i]!=',') {
            if (!isdigit(s[i])){
                valid = false;
                break;
            }
            if (counter==0) {
                s1[index] = s[i];
            } else {
                s2[index] = s[i];
            }
            index++;
        } else {
            counter++;
            index=0;
        }
        if (counter==2){
            i++;
            break;
        }
    }
    string s3= string(s1);
    string s4= string(s2);
    char c3= (char)s[i];
    char c4= (char)s[i+2];
    int counter2=0;
    if (counter==2) {
        if (isNumber(s3) && isNumber((s4))) {
            id = stoi(s3);
            type = stoi(s4);
            if (id >= 0 && (type == 1 || type == 2)) {
                switch (c3) {
                    case 'H':
                        counter2++;
                        break;
                    case 'S':
                        counter2++;
                        break;
                    case 'T':
                        counter2++;
                        break;
                    case 'F':
                        counter2++;
                        break;
                    default:
                        valid=false;
                        break;
                }
                switch (c4) {
                    case 'R':
                        counter2++;
                        break;
                    case 'B':
                        counter2++;
                        break;
                    case 'G':
                        counter2++;
                        break;
                    case 'P':
                        counter2++;
                        break;
                    case 'W':
                        counter2++;
                        break;
                    default:
                        valid=false;
                        break;
                }
            }
        }
    } else {
        valid = false;
    }
    if (counter2==2 || valid){
        if (type == 1) {
            StandardCab t = StandardCab(id, type, c3, c4);
            return t;
        } else if (type == 2) {
            LuxuryCab t = LuxuryCab(id, type, c3, c4);
            return t;
        }
    }
    StandardCab t = StandardCab(-1, 1, 'L', 'N');
    return t;

}

/*
 * checks whether a given string is correct input for a trip or not
 */
bool City::validTrip(string s) {
    int size=s.size();
    int i=0;
    int index=0;
    int id;
    char sId[3];
    int startX;
    char sStartX[3];
    int startY;
    char sStartY[3];
    int endX;
    char sEndX[3];
    int endY;
    char sEndY[3];
    int pass;
    char sPass[3];
    double tariff;
    char sTariff[3];
    int time;
    char sTime[3];
    int counter=0;
    char c[3];
    while (i<size) {
        if (s[i] != ','){
            if (!isdigit(s[i])) {
                return false;
            }
            switch (counter) {
                case 0:
                    sId[index] = s[i];
                    break;
                case 1:
                    sStartX[index] = s[i];
                    break;
                case 2:
                    sStartY[index] = s[i];
                    break;
                case 3:
                    sEndX[index] = s[i];
                    break;
                case 4:
                    sEndY[index] = s[i];
                    break;
                case 5:
                    sPass[index] = s[i];
                    break;
                case 6:
                    sTariff[index]=s[i];
                    break;
                case 7:
                    sTime[index]=s[i];
                    break;
                default:
                    break;

            }
            i++;
            index++;
        }else {
            i++;
            counter++;
            index=0;
        }
    }
    id= stoi(sId);
    startX= stoi(sStartX);
    startY= stoi(sStartY);
    endX= stoi(sEndX);
    endY= stoi(sEndY);
    pass=stoi(sPass);
    tariff= stoi(sTariff);
    time= stoi(sTime);
    if (counter!=7 || id<0 || pass<0 || tariff<0 || time<=0){
        return false;
    }
    if (startX<0 || startX>=gridSizeX || startY<0 || startY>=gridSizeY || endX<0 || endX>=gridSizeX
        || endY<0 || endY>=gridSizeY){
        return false;
    }
    if(startX == endX && startY == endY) {
        return false;
    }
    return true;
}