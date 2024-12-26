#pragma once
#include "Hospital.h"
#include "Car.h"
#include "Patient.h"
#include "LinkedQueue.h"
#include "priQueue.h"
#include "UI.h"
#include <fstream>
#include <vector>
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
    vector<int> canceled_IDs;
    vector<int> canceled_H;
    vector<int> canceled_T;
public:
    Organiser();
    void main_simulation(bool silent);
    void handle_EP(int owning_hospitalID);
    void cancel_request(int current_time);
    void move_car_out(int hospitalID, char car_type);
    void free_car(int current_time);
    void load_file(string file_name);
    bool move_to_finish(string request_type, Patient* patient);
    bool assign_car(int hospitalID, char car_type, int current_time);
    bool carry_back(int current_time);
    bool backTo_hospital();
    void generate_output_file(const string& output_file_name, int total_simulation_time);
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
    for(int i=0; i < numOf_hospitals; i++){
        int special_cars, normal_cars;
        test>>special_cars>>normal_cars;
        hospitals[i].set_cars(normal_speed, special_speed, special_cars, normal_cars, i+1);
    }
    test>>numOf_requests;
    int count=numOf_requests;
    string patient_type; int request_time, PID, HID, dist, severity;
    while (count--){
        test >> patient_type>>request_time>>PID>>HID>>dist;
        Patient* new_request = new Patient(patient_type, request_time, PID, HID, dist);
        if(patient_type=="EP"){test>>severity; new_request->set_case_severity(severity);}
        hospitals[HID-1].add_request(new_request, patient_type);
    }
    int numOf_cancels;
    test>>numOf_cancels;
    count=numOf_cancels;
    int cancel_time;
    canceled_IDs= vector<int>(count);
    canceled_H=vector<int>(count);
    canceled_T = vector<int>(count);
    for(int i=0; i<count; i++){
        test >> PID>>HID>>cancel_time;
        canceled_IDs[i] = PID;
        canceled_H[i] = HID;
        canceled_T[i] = cancel_time;
    }
    test.close();
}

bool Organiser::free_to_out(int hospitalID, char car_type, int current_time)
{
    Car* c = hospitals[hospitalID - 1].remove_available_car(car_type);
    if (!c || !c->get_carried_patient()) { 
        return false; }
    out_cars.enqueue(c, c->get_carried_patient()->get_pick_time());
    return true;
}



bool Organiser::out_to_back(int current_time)
{
    Car* c = nullptr; Patient* patient = nullptr;
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
    return true;
}

bool Organiser::assign_car(int hospitalID, char car_type, int current_time){
    Car* c = hospitals[hospitalID-1].remove_available_car(car_type);
    if(!c){return false;}
    out_cars.enqueue(c, current_time);
    return true;
}

bool Organiser::carry_back(int current_time){
    Car* back= nullptr; int assign_time;
    if(out_cars.dequeue(back, assign_time)){
        back_cars.enqueue(back, current_time); return true;
    }
    return false;
}

bool Organiser::backTo_hospital(){
    Car* c = nullptr;int p;
    if(back_cars.dequeue(c,p)){
        hospitals[c->get_owning_hospital()-1].add_free_car(c, c->get_type());
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


void Organiser::generate_output_file(const string& output_file_name, int total_simulation_time) {
    ofstream outputFile("output.txt");

    if (!outputFile) {
        cout << "Error opening file for writing!" << endl;
        return;
    }
    int AllPatients = finished_requests.get_entries();
        int EP_Patients_Num = 0;
        int SP_Patients_Num = 0;
        int NP_Patients_Num = 0;
        int NC_NUM = 0;
        int SC_NUM = 0;
        int TOTAL_CARS = 0;
        int wait_time_sum=0;
        int busy_time_sum=0;

    Patient* tempPateint;
    while (!finished_requests.isEmpty()) {
        outputFile << "FT\tPID\tQT\tWT" << endl;

        finished_requests.dequeue(tempPateint);

        outputFile << tempPateint->get_finish_time() << '\t' 
                   << tempPateint->get_patientID() << '\t'
                   << tempPateint->get_pick_time() << '\t'
                   << tempPateint->get_wait_time() << endl;
        wait_time_sum+=tempPateint->get_wait_time();
        busy_time_sum+=tempPateint->get_finish_time() - tempPateint->get_assign_time();
        if (tempPateint->get_patient_type() == "EP") {
            EP_Patients_Num++;
        } else if (tempPateint->get_patient_type() == "NP") {
            NP_Patients_Num++;
        } else if (tempPateint->get_patient_type() == "SP") {
            SP_Patients_Num++;
        }

        

        outputFile << "Pateins: " << AllPatients << "\t\t"
                   << "[NP: " << NP_Patients_Num << ", EP: " << EP_Patients_Num
                   << ", SP: " << SP_Patients_Num << "]" << endl;

        outputFile << "Hospitals: " << numOf_hospitals;

        for (int i = 0; i < numOf_hospitals; i++) {
            LinkedQueue<Car*> * NC; LinkedQueue<Car*> * SC;
            hospitals[i].get_car_lists(NC, SC);
            NC_NUM += NC->get_entries(); 
            SC_NUM += SC->get_entries();
        }
        TOTAL_CARS = NC_NUM + SC_NUM;

        outputFile << "Cars: " << TOTAL_CARS << "\t\t"
                   << "[Scars: " << SC_NUM << ", Ncars: " << NC_NUM << "]" << endl;

        outputFile << "Avg wait Time: \n\n" << wait_time_sum/numOf_requests << endl;
        outputFile << "Avg Busy Time: " << busy_time_sum/numOf_requests << endl;
        outputFile << "Avg ulitization: " << busy_time_sum/total_simulation_time << endl;
    }

    outputFile.close();

    return;
}

void Organiser::cancel_request(int current_time){
    if(current_time == canceled_T.front()){
        if(hospitals[canceled_H.front()-1].cancel_request(canceled_IDs.front())){
            canceled_H.erase(canceled_H.begin());
            canceled_IDs.erase(canceled_H.begin());
            canceled_T.erase(canceled_H.begin());
            return;}
        Car* c= nullptr;
        if(out_cars.cancel_car(canceled_IDs.front(), c)){}
        hospitals[canceled_H.front()-1].add_free_car(c, c->get_type());
        canceled_H.erase(canceled_H.begin());
        canceled_IDs.erase(canceled_H.begin());
        canceled_T.erase(canceled_H.begin());
    }
}

void Organiser::handle_EP(int owning_hospitalID)
{
    Patient* ep =hospitals[owning_hospitalID-1].remove_request("EP");
    int to_HID;
    int numOf_trials=0;
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
    int step=1; Patient* ep;
    UI interface;
    while (true)
    {
        cancel_request(step);
        for(int i=0; i< numOf_hospitals; i++)
        {
                ep = hospitals[i].peek_request("EP");
                if(ep && ep->get_request_time()==step){
                    if(hospitals[i].assign_EP(step)){
                        if(!free_to_out(i+1, 'n', step)){
                            free_to_out(i+1, 's', step);
                        }
                    }

                    else{
                        handle_EP(i+1);
                    }
                }
                if(hospitals[i].assign_SP(step)){
                    if(free_to_out(i+1, 's', step)){}
                }
                if(hospitals[i].assign_NP(step)){
                    if(free_to_out(i+1, 'n', step)){}
                }
            if(out_to_back(step)) {}
            if(back_to_free(step)) {}
            if(!silent){
                cout << "Current Timestep: " << step<<endl;
                interface.print_hospital(&hospitals[i], i+1);
                interface.print_cars_info(&out_cars, &back_cars, &finished_requests);
            }
            bool next;
            if(finished_requests.get_entries() == numOf_requests) {
                generate_output_file("output.txt", step);    
                return;
            }
            cin>>next;   
        }
        step++;
    }
    
}
