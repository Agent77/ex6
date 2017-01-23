#ifndef TRY_LUXURYCAB_H
#define TRY_LUXURYCAB_H

#include "Taxi.h"
#include <boost/serialization/access.hpp>

class LuxuryCab: public Taxi {
    friend class boost::serialization::access;

    template<class Archive>

    void serialize(Archive &ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Taxi>(*this);
    }

public:
    LuxuryCab(){};
    LuxuryCab(int id, int type, char manufacturer, char color):Taxi(id, type, manufacturer, color){};
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



#endif //TRY_LUXURYCAB_H
