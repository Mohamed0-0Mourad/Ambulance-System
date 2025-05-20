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
    Car(){
        type = char();
        speed = int();
        status = char();
        carried_patient = nullptr;
        owning_hospitalID = 0;
    }
    Car (char CT, int CS, char S, Patient* CP) : type(CT), speed(CS), status(S), carried_patient(CP) {}    
    Car(char car_type, int car_speed) : type(car_type), speed(car_speed){Car();}
    
    void set_type(char ct)
    {
        type = ct;
    }

    void set_speed(int cs)
    {
        speed = cs;
    }

    void set_status(char s)
    {
        status = s;
    }

    void pick_patient()
    {
        set_status('l');
    }

    void set_carried_patient(Patient* patient) {carried_patient = patient;}

    void set_owning_hospital(int hospitalID)
    {
        owning_hospitalID = hospitalID;
    }

    Patient* drop_patient() // should set status free and set carried patient nullptr
    {
        set_status('r');
        Patient* cp = carried_patient;
        carried_patient = nullptr;
        return cp;
    }

    char get_type()
    {
        return type;
    }

    int get_speed()
    {
        return speed;
    }

    char get_status()
    {
        return status;
    }

    int get_owning_hospital()
    {
        return owning_hospitalID;
    }
    
    Patient* get_carried_patient() 
    {
        return carried_patient;
    }
    
};
