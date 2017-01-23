
#ifndef TRY_STANDARDCAB_H
#define TRY_STANDARDCAB_H

#include <boost/serialization/access.hpp>
#include "Taxi.h"

class StandardCab: public Taxi {
    friend class boost::serialization::access;

    template<class Archive>

    void serialize(Archive &ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Taxi>(*this);
    }
public:
    StandardCab() {};
    StandardCab(int id, int type, char manufacturer, char color):Taxi(id, type, manufacturer, color){};
    void updateKms(){};
    int getSpeed(){};
    char getManufacturer(){};
    int getTariff(){};
    int getId() {};
    int getKms(){};
    int getType() {
        return cabType;
    }
};


#endif //TRY_STANDARDCAB_H
