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
public:
    Car();      
    Car(char car_type, int car_speed) : type(car_type), speed(car_speed){};
    
    void set_type(char car_type);
    void set_speed(int car_speed);
    void set_status(char car_status);//has only three possiple values {r, a, l} crossponding to (Ready, Assigned, Loaded)
    void pick_patient(Patient* patient); 
    
    Patient* drop_patient();
    char get_type();
    int get_speed();
    char get_status();
    
    

};
