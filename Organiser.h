#pragma once
#include "Hospital.h"
#include "Car.h"
#include "Patient.h"
#include "LinkedQueue.h"
#include "priQueue.h"
#include <fstream>
using namespace std;

class Organiser
{
private:
    int numOf_hospitals;
    Hospital* hospitals;
    Der_priQueue<Car*> out_cars;
    priQueue<Car*> back_cars;
    LinkedQueue<Patient*> finished_requests;
public:
    Organiser();
    void load_file(string file_name);

    ~Organiser();
};

Organiser::Organiser(){
    hospitals = nullptr;
    out_cars = Der_priQueue<Car*>();
    back_cars = priQueue<Car*>();
    finished_requests=LinkedQueue<Patient*>();
}

void Organiser::load_file(string file_name){
    int normal_speed, special_speed;
    ifstream test(file_name);
    test >> numOf_hospitals >> special_speed >> normal_speed;
    hospitals = new Hospital[numOf_hospitals];
    for(int i{0}; i < numOf_hospitals; i++){
        int special_cars, normal_cars;
        test>>special_cars>>normal_cars;
        hospitals[i].set_cars(normal_speed, special_speed, special_cars, normal_cars);
    }
    test.close();
}

Organiser::~Organiser(){
    delete[] hospitals;
    out_cars.~Der_priQueue();
    back_cars.~priQueue();
    finished_requests.~LinkedQueue();
}
