
#include <string>
#include "Taxi.h"

Taxi::Taxi() {

}

void Taxi::updateKms() {
    this->kmPassed += 1;
}

int Taxi::getSpeed() {
    return speed;
}


char Taxi::getManufacturer() {
    return cabManufacturer;
}

int Taxi::getTariff() {
    return tariff;
}

int Taxi::getId() {
return cabId;
}

int Taxi::getType() {
    return cabType;
}

Taxi::Taxi(int id, int type, char manufacturer, char color) {
    cabId = id;
    cabType=type;
    cabManufacturer = manufacturer;
    cabColor = color;
    kmPassed = 0;
}

int Taxi::getKms() {
    return kmPassed;
}