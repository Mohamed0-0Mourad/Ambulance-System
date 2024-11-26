#pragma once
#include <iostream>
#include"Patient.h"
using namespace std;

class Car
{
private:
    char type;
    int speed;
    char status;          // has only three possiple values {r, a, l} crossponding to (Ready, Assigned, Loaded)
    Patient* carried_patient;
    int owning_hospitalID;
public:
    Car();      
    Car(char car_type, int car_speed) : type(car_type), speed(car_speed){};
    
    void set_type(char car_type);
    void set_speed(int car_speed);
    void set_status(char car_status);//has only three possiple values {r, a, l} crossponding to (Ready, Assigned, Loaded)
    void pick_patient(Patient* patient); 
    void set_owning_hospital(int hospitalID); 

    Patient* drop_patient(); // should set status free and set carried patient nullptr
    char get_type()const;
    int get_speed()const;
    char get_status()const;
    int get_owning_hospital()const;
    Patient* get_carried_patient()const {return carried_patient;}
    

};
