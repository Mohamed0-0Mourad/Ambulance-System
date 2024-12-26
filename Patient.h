#pragma once
#include<iostream>
using namespace std;

class Patient
{
private:
    int request_time, dist,
        assign_time,
        pick_time,      // AT + dist/speed 
        wait_time,      // PT - QT
        finish_time,    // PT + dist/speed
        case_severity,  //should initially be -1 if not Emergency patient(EP)
        cancel_time,    // should initially be -1 
        hospitalID, 
        patientID;
    string patient_type; // Values range from only {SP, EP, NP} 
    
public:
    Patient();
    Patient(string P_type, int request_time, int patient_ID, int requested_hospital, int distance);
    void set_type(string P_type);
    void request(int time);
    void set_distance(int distance);
    void set_ID(int ID);
    void request_hospital(int hospital_ID); 
    void set_case_severity(int severity);


    /*******************New needed functions**********************/
    void set_assign_time(int AT, int car_speed);
    void set_request_time(int timestep) {request_time = timestep;} 
    // Geters for each data member____________________________________________________
    int get_request_time() const { return request_time; }

    int get_pick_time() const { return pick_time; }

    int get_dist() const { return dist; }

    int get_wait_time() const { return wait_time; }

    int get_finish_time() const { return finish_time; }

    int get_case_severity() const { return case_severity; }

    int get_cancel_time() const { return cancel_time; }

    int get_hospitalID() const { return hospitalID; }

    int get_patientID() const { return patientID; }
    int get_assign_time() const {return assign_time;}
    string get_patient_type() const { return patient_type; }
};

Patient::Patient()
{
    request_time = 0;
    pick_time = 0;
    dist = 0;
    wait_time = 0;
    finish_time = 0;
    case_severity = -1;
    cancel_time = -1;
    hospitalID = 0;
    patientID = 1;
    assign_time = 0;
    patient_type = "";
}

Patient::Patient(string P_type, int request_time, int patient_ID, int requested_hospital, int distance)
{
    this->request_time = request_time;
    pick_time = 0;
    dist = distance;
    wait_time = 0;
    finish_time = 0;
    case_severity = -1;
    cancel_time = -1;
    hospitalID = requested_hospital;
    patientID = patient_ID;
    patient_type = P_type;
}


void Patient::set_type(string P_type) {
    if (P_type == "SP" || P_type == "EP" || P_type == "NP") {
        patient_type = P_type;
    } else {
        cout << "Invalid patient type. you should use SP, EP, or NP.";
    }
}

void Patient::request(int time) {
    if (time >= 0) {
        request_time = time;
    } else {
        cout << "Invalid request time.";
    }
}

void Patient::set_distance(int distance) {
    if (distance >= 0) {
        dist = distance;
    } else {
        cout << "Invalid distance.";
    }
}

void Patient::set_ID(int ID) {
    patientID = ID;
}

void Patient::request_hospital(int hospital_ID) {
    if (hospital_ID >= 0) {
        hospitalID = hospital_ID;
    } else {
        cout << "Invalid hospital ID.";
    }
}

void Patient::set_case_severity(int severity){
    case_severity=severity;
}

void Patient::set_assign_time(int AT, int car_speed){
        int rood = dist / car_speed;
        assign_time= AT;
        pick_time = AT + rood;
        wait_time = pick_time - request_time;
        finish_time = pick_time + rood;
    } //should also calculate all other times