#pragma once
#include "Hospital.h"
#include "Car.h"
#include "Patient.h"
#include "LinkedQueue.h"
#include "priQueue.h"
#include "UI.h"
#include <fstream>
using namespace std;

class Organiser
{
private:
    int numOf_hospitals;
    int numOf_requests;
    Hospital* hospitals;
    Min_priQueue<Car*> out_cars;
    Min_priQueue<Car*> back_cars;
    LinkedQueue<Patient*> finished_requests;
public:
    Organiser();
    void main_simulation(bool silent);
    void handle_EP(int owning_hospitalID);
    void cancel_request(Patient* patientPtr, int current_timestep);
    void move_car_out(int hospitalID, char car_type);
    // deque a car of type {car_type} out of a hospital specified ID and enque it in out_cars
    void free_car(int current_time);
    // peek the back cars, access the carried patient, and check his finish time == current or not
    // if equal dequeue from out_car.
    //     then get_owning_hospital() ID 
    //     call move_to finish() and call drop_patient()
    void load_file(string file_name);
    // we should add canceld requests reading
    bool move_to_finish(string request_type, Patient* patient);
    bool assign_car(int hospitalID, char car_type, int current_time);
    bool carry_back(int current_time);
    bool backTo_hospital();
    bool back_to_free(int current_time);
    bool finished_patients(Patient* patient);
    bool free_to_out(int hospitalID, char car_type, int current_time);
    bool out_to_back(int current_time);
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

void Organiser::load_file(string file_name){
    int normal_speed, special_speed;
    ifstream test(file_name);
    test >> numOf_hospitals >> special_speed >> normal_speed;
    hospitals = new Hospital[numOf_hospitals];
    for(int i{0}; i < numOf_hospitals; i++){
        int special_cars, normal_cars;
        test>>special_cars>>normal_cars;
        hospitals[i].set_cars(normal_speed, special_speed, special_cars, normal_cars, i);
    }
    test>>numOf_requests;
    string patient_type; int request_time, PID, HID, dist, severity;
    while (numOf_requests--){
        test >> patient_type>>request_time>>PID>>HID>>dist;
        Patient* new_request = new Patient(patient_type, request_time, PID, HID, dist);
        if(patient_type=="EP"){test>>severity; new_request->set_case_severity(severity);}
        hospitals[HID-1].add_request(new_request, patient_type);
    }
    test.close();
}

bool Organiser::free_to_out(int hospitalID, char car_type, int current_time)
{
    Car* c = hospitals[hospitalID - 1].remove_available_car(car_type);
    if (!c || !c->get_carried_patient()) { return false; };
    out_cars.enqueue(c, c->get_carried_patient()->get_pick_time());
    return true;
}



bool Organiser::out_to_back(int current_time)
{
    Car* c = nullptr; Patient* patient{nullptr};
    int pickup_time = 0;
        if (out_cars.peek(c, pickup_time))
        {
            patient = c->get_carried_patient();
            int pp = patient->get_pick_time();
            if (current_time == pp)
            {
                out_cars.dequeue(c, pp);
                c->pick_patient();
                back_cars.enqueue(c, patient->get_finish_time());
                return true;
            }
        }
    return false;
}



bool Organiser::back_to_free(int current_time)
{
    Car* c = nullptr;
    int finish_time = 0;
    if (back_cars.peek(c, finish_time))
    {
        Patient* patient = c->get_carried_patient();
        if (!patient) { return false; }
        int finish_time = patient->get_finish_time();
        if (current_time == finish_time)
        {
            back_cars.dequeue(c, finish_time);
            patient = c->drop_patient();
            int hospitalID = c->get_owning_hospital();
            hospitals[hospitalID - 1].add_free_car(c, c->get_type());
            string pt = patient->get_patient_type();
            finished_patients(patient);
            return true;
        }
    }
    return false;
}


bool Organiser::finished_patients(Patient* patient)
{
    finished_requests.enqueue(patient);
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

void Organiser::handle_EP(int owning_hospitalID)
{
    Patient* ep =hospitals[owning_hospitalID-1].remove_request("EP");
    int to_HID;
    int numOf_trials{0};
    while(true)
    {
        if(numOf_trials >= (numOf_hospitals *2)){return;}
        srand(time(nullptr));
        to_HID = rand() % numOf_hospitals;
        if(to_HID != owning_hospitalID -1){
            Car* c = hospitals[to_HID].peek_available_car('n');
            if(c) {
                c->set_carried_patient(ep);
                c->set_status('a');
                return ;
            }
        }
        numOf_trials++;
    }
    
}

void Organiser::main_simulation(bool silent)
{
    int step{1};
    UI interface;
    while (true)
    {
        // check for cancelations
        for(int i{0}; i< numOf_hospitals; i++)
        {
            // Check for requests to handle
                // check EP
                if(hospitals[i].peek_request("EP")->get_request_time()==step){
                    if(hospitals[i].assign_EP(step)){
                        if(!free_to_out(i+1, 'n', step)){
                            free_to_out(i+1, 's', step);
                        }
                    }
                        // handle EP
                    else{
                        handle_EP(i+1);
                    }
                }
                // check SP
                if(hospitals[i].assign_SP(step)){
                    if(free_to_out(i+1, 's', step)){}
                }
                // check NP
                if(hospitals[i].assign_NP(step)){
                    if(free_to_out(i+1, 'n', step)){}
                }
            // assign requests that should be handled
            //      check for free to out cars
            //      check for out to back
            if(out_to_back(step)) {}
            //      check for back to free
            if(back_to_free(step)) {}
            // UI functions 
            if(!silent){
                interface.print_hospital(&hospitals[i]);
                interface.print_cars_info(this);
            }
            bool next;
            if(finished_requests.get_entries() == numOf_requests) {
                // generate output file    
                return;
            }
            cin>>next;   
        }
        step++;
    }
    
}