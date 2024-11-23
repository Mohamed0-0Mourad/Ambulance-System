#pragma once
#include<iostream>
using namespace std;

class Patient
{
private:
    int request_time, dist,
        pick_time,      // AT + dist/speed 
        wait_time,      // PT - QT
        finish_time,    // PT + dist/speed
        case_severity,  //should initially be -1 if not Emergency patient(EP)
        cancel_time,    // should initially be -1 
        hospitalID, 
        patientID;
    char patient_type[2]; // Values range from only {SP, EP, NP} 
    
public:
    Patient();
    Patient(char P_type[2], int request_time, int patient_ID, int requested_hospital, int distance);
    void set_type(char P_type[2]);
    void request(int time);
    void set_distance(int distance);
    void set_ID();
    void request_hospital(int hospital_ID); 
    void set_case_severity(int severity);


    /*******************New needed functions**********************/
    void set_assign_time(int timestep, int car_speed); //should also calculate all other times
    void set_request_time(int timestep); 
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

    string get_patient_type() const { return patient_type; }
};

Patient::Patient(/* args */)
{

}
