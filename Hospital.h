#pragma once
#include<iostream>
#include"Patient.h"
#include"LinkedQueue.h"
#include"priQueue.h"
#include "Car.h"
using namespace std;

class Hospital
{
private:
    priQueue<Patient*> EP;
    LinkedQueue<Patient*> SP;
    CancelQueue<Patient*> NP;
    LinkedQueue<Car*> Scars;
    LinkedQueue<Car*> Ncars;

public:
    Hospital();     
    void add_request(Patient* const patient_ptr, string patient_type);
    void add_free_car(Car* const car_ptr, char car_type);
    
    Patient* peek_request(string patient_type)const;   
    Car* peek_available_car(char car_type)const;
    
    Patient* remove_request(string patient_type);
    Car* remove_available_car(char car_type);

    void get_car_lists(LinkedQueue<Car*> *&n, LinkedQueue<Car*> *&s) {n=&Ncars;s=&Scars;}
    void get_patient_lists(priQueue<Patient*> *&e,LinkedQueue<Patient*> *&s,CancelQueue<Patient*> *&n)
    {e = & EP; s=&SP; n=& NP;}

    bool assign_EP(int current_timestep); 
    bool assign_SP(int current_timestep); // set the crossponding car's carreid patient to this request
    bool assign_NP(int current_timestep); // check if the peek reuest time equal the timestep

    void set_cars(int normal_car_speed, int special_car_speed, int numOfSC, int numOfNC, int owning_hospital);
    ~Hospital(); 
    bool cancel_request(int patient_ID){
        return NP.cancel_request(patient_ID);
    }
};

Hospital::Hospital()
{
    EP = priQueue<Patient*>();
    SP = LinkedQueue<Patient*>();
    NP = CancelQueue<Patient*>();
}

void Hospital::add_request(Patient* const patient_ptr, string patient_type) {
    if (patient_type == "EP") {
        EP.enqueue(patient_ptr, patient_ptr->get_case_severity());
    } else if (patient_type == "SP") {
        SP.enqueue(patient_ptr);
    } else if (patient_type == "NP") {
        NP.enqueue(patient_ptr);
    }
}

void Hospital::add_free_car(Car* const car_ptr, char car_type) {
    if (car_type == 's') {
        Scars.enqueue(car_ptr);
    } else if (car_type == 'n') {
        Ncars.enqueue(car_ptr);
    }
}

Patient* Hospital::peek_request(string patient_type) const {
    Patient* patient = nullptr;
    if (patient_type == "EP") {
        int sevEPity;
        if (EP.peek(patient, sevEPity)) {
            return patient;
        }
    } else if (patient_type == "SP") {
        if (SP.peek(patient)) {
            return patient;
        }
    } else if (patient_type == "NP") {
        if (NP.peek(patient)) {
            return patient;
        }
    }
    return nullptr;
}

Car* Hospital::peek_available_car(char car_type) const {
    Car* car = nullptr;
    if (car_type == 's') {
        if (Scars.peek(car)) {
            return car;
        }
    } else if (car_type == 'n') {
        if (Ncars.peek(car)) {
            return car;
        }
    }
    return nullptr;
}

Patient* Hospital::remove_request(string patient_type) {
    Patient* patient = nullptr;
    if (patient_type == "EP") {
        int sevEPity;
        if (EP.dequeue(patient, sevEPity)) {
            return patient;
        }
    } else if (patient_type == "SP") {
        if (SP.dequeue(patient)) {
            return patient;
        }
    } else if (patient_type == "NP") {
        if (NP.dequeue(patient)) {
            return patient;
        }
    }
    return nullptr;
}

Car* Hospital::remove_available_car(char car_type) {
    Car* car = nullptr;
    if (car_type == 's') {
        if (Scars.dequeue(car)) {
            return car;
        }
    } else if (car_type == 'n') {
        if (Ncars.dequeue(car)) {
            return car;
        }
    }
    return nullptr;
}

void Hospital::set_cars(int normal_car_speed, int special_car_speed, int numOfSC, int numOfNC, int owning_hospital){
    for (int i = 0; i < numOfSC; i++)
    {
        Car* new_car= new Car('s', special_car_speed);
        add_free_car(new_car, 's');
        new_car->set_owning_hospital(owning_hospital);
    }
    for (int i = 0; i < numOfNC; i++)
    {
        Car* new_car= new Car('n', normal_car_speed);
        add_free_car(new_car, 'n');
        new_car->set_owning_hospital(owning_hospital);
    }
}

bool Hospital::assign_EP(int current_timestep) {
    int car_speed = 0;
    Car* ec;
    bool a = Ncars.peek(ec);
    if (a == false) {
        a = Scars.peek(ec);
        if (a == false)
            return false;
    }
    Patient* ep;
    int pri;
    bool b = EP.dequeue(ep, pri);
    if (b == false) {
        return false;
    }

    car_speed = ec->get_speed();
    ec->set_carried_patient(ep);
    ep->set_assign_time(current_timestep, car_speed);
    ec->set_status('a');

    return true;
}



bool Hospital::assign_NP(int current_timestep) {
    Car* nc;
    bool a = Ncars.peek(nc);
    if (a == false) {
        return false;
    }
    
    Patient* np;
    bool b = NP.peek(np);
    if (b == false) {
        return false;
    }
    if (np->get_request_time() != current_timestep) {
        return false;
    }

    nc->set_carried_patient(np);
    np->set_assign_time(current_timestep, nc->get_speed());
    nc->set_status('a');

    return true;
}


bool Hospital::assign_SP(int current_timestep) {
    Car* sc;
    bool a = Scars.peek(sc);
    if (a == false) {
        return false;
    }
    
    Patient* sp;
    bool b = SP.peek(sp);
    if (b == false) {
        return false;
    }
    if (sp->get_request_time() != current_timestep) {
        return false;
    }

    sc->set_carried_patient(sp);
    sc->set_status('a');

    return true;
}

Hospital::~Hospital(){
    // EP.~priQueue();
    // SP.~LinkedQueue();
    // NP.~CancelQueue();
    // Scars.~LinkedQueue();
    // Ncars.~LinkedQueue();
}