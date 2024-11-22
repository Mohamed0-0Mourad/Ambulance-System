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
    Patient(/* args */);                    // should handle the patient ID automatically
    // should also set the request time
    void set_type(char P_type[2]);
    void request(int time);
    void set_distance(int distance);
    void set_ID();
    void request_hospital(int hospital_ID); //should set the request time


    /*******************New needed functions**********************/
    void set_assign_time(int timestep, int car_speed); //should also calculate all other times
    void set_request_time(int timestep); 
    // Geters for each data member____________________________________________________

};

Patient::Patient(/* args */)
{

}
