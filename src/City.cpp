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
    bool isValid=true;
    Driver d;
    string g;
    int size = s.size()/2+1;
    if(size!=5){
        isValid=false;
    }
    std::istringstream sep(s);
    std::getline(sep, g,',');
    std::istringstream (g)>>f;
    for (int j=0; j<(s.size()/2)+1;j++){
        if (i!=2){
            std::istringstream (g)>>f;
            char*c= (char*)g[0];
            if (isNumber(c)){
                info[i]= f;
            } else{
                isValid=false;
            }
        } else{
            if (g.size()<2 && g[0]>='A' && g[0]<='Z'){
                if (isValidStatus(g[0])){
                    status=g[0];
                } else{
                    isValid=false;
                }
            } else{
                isValid=false;
            }
        }
        std::getline(sep, g,',');
        i++;
    }
    if (i>5 || i<5){       //means that driver's string is not 5
        isValid=false;
    }
    if (isValid) {
        d = Driver(info[0], info[1], status, info[3], info[4]);
    } else {
        d = Driver(-1, info[1], status, info[3], info[4]);
    }
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
Coordinate* City::createCoordinate(string s) { /*TODO fix*/
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
Graph* City::createGraph(string s, string s1) {
    /*char* c1=(char*)s;
    char* c2=(char*)s1[0];*/
    if (!(isNumber(s)&&isNumber(s1))) {
        return NULL;
    }
    gridSizeX = stoi(s);
    gridSizeY = stoi(s1);
    if (gridSizeX <= 0 || gridSizeY <= 0) {
        return NULL;
    }
    Graph *graphPointer = new Grid(gridSizeX, gridSizeY);
    return graphPointer;
}

/*
 * The function parses a given string and creates new taxi
 */
Taxi City::createTaxi(string s) {
    int id = (int)s[0] - 48;
    int type = (int)s[2] - 48;
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
}

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

bool City::validTaxi(string s) {
    int counter=0;
    int index=0;
    char s1[3];
    char s2[3];
    int i=0;
    for(; i<s.size()/2+1; i++){
        if (s[i]!=',') {
            if (counter==0) {
                s1[index] = s[i];
            } else {
                s2[index]=s[i];
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
    string s3=string(s1);
    string s4= string(s2);
    char c3= (char)s[4];
    char c4= (char)s[6];
    int counter2=0;
    int size= s.size()/2+1;
    if (size!=4){
        return false;
    }
    if (isNumber(s3)&&isNumber((s4))){
        int firstInt=stoi(s3);
        int secondInt=stoi(s4);
        if (firstInt>=0 && (secondInt==1|| secondInt==2)) {
            switch (c3) {
                case 'H':
                    counter2++;
                    break;
                case 's':
                    counter2++;
                    break;
                case 'T':
                    counter2++;
                    break;
                case 'F':
                    counter2++;
                    break;
                default:
                    break;
            }
            switch(c4){
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
                    break;
            }
        }
    }
    if (counter2==2){
        return true;
    }
    return false;
}

bool City::validTrip(string s) {
    int size=s.size()/2+1;
    if (size!=8){
        return false;
    }
    for (int i=0; i<size; i+=2){
        char* c= (char*)s[i];
        if (!isNumber(c)){
            return false;
        }
    }
    int id=(int)s[0]-48;
    int startX=(int)s[2]-48;
    int startY=(int)s[4]-48;
    int endX=(int)s[6]-48;
    int endY=(int)s[8]-48;
    int pass=(int)s[10]-48;
    double tariff=(int)s[12]-48;
    int time=(int)s[14]-48;
    if (id<0 || pass<0 || tariff<0 || time<=0){
        return false;
    }
    if (startX<0 && startX>gridSizeX || startY<0 && startY>gridSizeY || endX<0 && endX>gridSizeX
            || endY<0 && endY>gridSizeY){
        return false;
    }
    return true;
}