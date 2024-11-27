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
    Min_priQueue<Car*> out_cars;
    Min_priQueue<Car*> back_cars;
    LinkedQueue<Patient*> finished_requests;
public:
    Organiser();
    void load_file(string file_name, int& hopitals_num, int& requests_num, Hospital*& hospital_list);
    bool move_to_finish(int hospitalID, string request_type, int& finished_num);
    bool assign_car(int hospitalID, char car_type, int current_time);
    bool carry_back(int current_time);
    bool backTo_hospital();
    LinkedQueue<Patient*>* get_finished_list(){return &finished_requests;}
    void get_cars_list(Min_priQueue<Car*> *&out, Min_priQueue<Car*> *&back){out=&out_cars;back=&back_cars;}
    ~Organiser();
};

Organiser::Organiser(){
    hospitals = nullptr;
    out_cars = Min_priQueue<Car*>();
    back_cars = Min_priQueue<Car*>();
    finished_requests=LinkedQueue<Patient*>();
}

void Organiser::load_file(string file_name, int& hopitals_num, int& requests_num, Hospital*& hospital_list){
    int normal_speed, special_speed;
    ifstream test(file_name);
    test >> numOf_hospitals >> special_speed >> normal_speed;
    hospitals = new Hospital[numOf_hospitals];
    for(int i{0}; i < numOf_hospitals; i++){
        int special_cars, normal_cars;
        test>>special_cars>>normal_cars;
        hospitals[i].set_cars(normal_speed, special_speed, special_cars, normal_cars, i);
    }
    int numOf_requests; test>>numOf_requests;
    string patient_type; int request_time, PID, HID, dist, severity;
    while (numOf_requests--){
        test >> patient_type>>request_time>>PID>>HID>>dist;
        Patient* new_request = new Patient(patient_type, request_time, PID, HID, dist);
        if(patient_type=="EP"){test>>severity; new_request->set_case_severity(severity);}
        hospitals[HID-1].add_request(new_request, patient_type);
    }
    test.close();
    hopitals_num = numOf_hospitals ;
    requests_num = numOf_requests;
    hospital_list=hospitals;
}

bool Organiser::move_to_finish(int hospitalID, string request_type, int& finished_num){
    Patient* p = hospitals[hospitalID].remove_request(request_type);
    if(!p){return false;}
    if(finished_requests.enqueue(p)){finished_num ++;return true;}
    return false;
}

bool Organiser::assign_car(int hospitalID, char car_type, int current_time){
    Car* c = hospitals[hospitalID].remove_available_car(car_type);
    if(!c){return false;}
    out_cars.enqueue(c, current_time);
    return true;
}

bool Organiser::carry_back(int current_time){
    Car* back{nullptr}; int assign_time;
    if(out_cars.dequeue(back, assign_time)){
        back_cars.enqueue(back, current_time); return true;
    }
    return false;
}

bool Organiser::backTo_hospital(){
    Car* c = nullptr;int p;
    if(back_cars.dequeue(c,p)){
        hospitals[c->get_owning_hospital()].add_free_car(c, c->get_type());
        return true;
    }
    return false;
}

Organiser::~Organiser(){
    delete[] hospitals;
    out_cars.~Min_priQueue();
    back_cars.~Min_priQueue();
    finished_requests.~LinkedQueue();
}
