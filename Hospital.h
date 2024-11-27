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
    priQueue<Patient*> ER;
    LinkedQueue<Patient*> SR;
    CancelQueue<Patient*> NR;
    LinkedQueue<Car*> Scars;
    LinkedQueue<Car*> Ncars;
    
public:
    Hospital();     
    // constructor should create an empty queue for each of the data members
    // ex: ER = PriQueue<Patient*>();
    void add_request(Patient* const patient_ptr, string patient_type);//should use the Enqueue method of crosponding patient type list
    // should check if it's emergency patient enqueue with severity as priority (get_case_severity()) EP, SP, NP
    void add_free_car(Car* const car_ptr, char car_type);//should use the Enqueue method of crossponding car type list n, s
    
    /* Return nullptr if empty */
    Patient* peek_request(string patient_type)const;   // should use peek method
    Car* peek_available_car(char car_type)const;
    
    Patient* remove_request(string patient_type); // should use dequeue method
    Car* remove_available_car(char car_type); // should return nullptr if empty

    void get_car_lists(LinkedQueue<Car*> *&n, LinkedQueue<Car*> *&s) {n=&Ncars;s=&Scars;}
    void get_patient_lists(priQueue<Patient*> *&e,LinkedQueue<Patient*> *&s,CancelQueue<Patient*> *&n)
    {e = & ER; s=&SR; n=& NR;}
    void set_cars(int normal_car_speed, int special_car_speed, int numOfSC, int numOfNC, int owning_hospital);
    ~Hospital(); //should delete the dynamically allocated members of the queues: delete <array name>
};

Hospital::Hospital()
{
    ER = priQueue<Patient*>();
    SR = LinkedQueue<Patient*>();
    NR = CancelQueue<Patient*>();
}

void Hospital::add_request(Patient* const patient_ptr, string patient_type) {
    if (patient_type == "EP") {
        ER.enqueue(patient_ptr, patient_ptr->get_case_severity());
    } else if (patient_type == "SP") {
        SR.enqueue(patient_ptr);
    } else if (patient_type == "NP") {
        NR.enqueue(patient_ptr);
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
        int severity;
        if (ER.peek(patient, severity)) {
            return patient;
        }
    } else if (patient_type == "SP") {
        if (SR.peek(patient)) {
            return patient;
        }
    } else if (patient_type == "NP") {
        if (NR.peek(patient)) {
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
        int severity;
        if (ER.dequeue(patient, severity)) {
            return patient;
        }
    } else if (patient_type == "SP") {
        if (SR.dequeue(patient)) {
            return patient;
        }
    } else if (patient_type == "NP") {
        if (NR.dequeue(patient)) {
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
    }
    for (int i = 0; i < numOfNC; i++)
    {
        Car* new_car= new Car('n', normal_car_speed);
        add_free_car(new_car, 'n');
    }
}
