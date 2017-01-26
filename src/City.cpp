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
 * The function parses a given string and creates new driver
 */
Driver City::createDriver(string s) {
    int info[6];
    char status;
    int f;
    int i = 0;
    string g;
    std::istringstream sep(s);
    std::getline(sep, g,',');
    std::istringstream (g)>>f;
    for (int j=0; j<(s.size()/2)+1;j++){
        if (i!=2){
            std::istringstream (g)>>f;
            info[i]= f;
        } else{
            status=g[0];
        }
        std::getline(sep, g,',');
        i++;
    }
    Driver d = Driver(info[0], info[1], status, info[3], info[4]);
    return d;
}

/*
 * The function parses a given string and creates new trip
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
 * The function parses a given string and creates new coordinate
 */
Coordinate* City::createCoordinate(string s) {
    int size=s.size()/2 +1;
    if(size!=2){
        return NULL;
    }
    /*char c1=s[0];
    char c2=s[2];
    char* c3=&c1;
    char* c4=&c2;
    string s1=string(&c1);
    string s2=string(&c2);*/
    if (isNumber(s)) {
        int x = (int) s[0] - 48;
        int y = (int) s[2] - 48;
        if(x<0 || y<0) {
            return NULL;
        }
        Coordinate *point = new Point(x, y);
        return point;
    }
    return NULL;
}

/*
 * The function parses a given string and creates new graph
 */
Graph* City::createGraph(string s) {
    /*char* c1=(char*)s;
    char* c2=(char*)s1[0];*/
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
 * The function parses a given string and creates new taxi
 */
/*Taxi City::createTaxi(string s) {
    int id;// = (int)s[0] - 48;
    int type;// = (int)s[2] - 48;
    if (validTaxi(s)) {
        if (type == 1) {
            StandardCab t = StandardCab(id, type, s[4], s[6]);
            return t;
        } else if (type == 2) {
            LuxuryCab t = LuxuryCab(id, type, s[4], s[6]);
            return t;
        }
    } else {
        StandardCab t = StandardCab(-1, type, s[4], s[6]);
        return t;
    }
}*/

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


bool City::isNumber(string s) {
    //string s1=string(s);
    for (int i=0; i<s.size(); i++){
        if (!isdigit(s[i]) && !s[i]==','){
            return false;
        }
    }
    return true;
}

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
   /* if (size!=8){
        return false;
    }
    for (int j=0; j<size; j+=2){
        char* c= (char*)s[i];
        if (!isNumber(c)){
            return false;
        }
    }*/
    while (i<size) {
        //while (s[i] != ',') {
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
    if (startX<0 && startX>gridSizeX || startY<0 && startY>gridSizeY || endX<0 && endX>gridSizeX
            || endY<0 && endY>gridSizeY){
        return false;
    }
    if(startX == endX && startY == endY) {
        return false;
    }
    return true;
}