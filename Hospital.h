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
    // should check if it's emergency patient enqueue with severity as priority (get_case_severity()) 
    void add_free_car(Car* const car_ptr, char car_type);//should use the Enqueue method of crossponding car type list
    
    /* Return nullptr if empty */
    Patient* peek_request(string patient_type)const;   // should use peek method
    Car* peek_available_car(char car_type)const;
    
    Patient* remove_request(string patient_type); // should use dequeue method
    Car* remove_available_car(char car_type); // should return nullptr if empty

    void set_cars(int normal_car_speed, int special_car_speed, int numOfSC, int numOfNC, int owning_hospital);
    ~Hospital(); //should delete the dynamically allocated members of the queues: delete <array name>
};

Hospital::Hospital()
{
    ER = priQueue<Patient*>();
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
