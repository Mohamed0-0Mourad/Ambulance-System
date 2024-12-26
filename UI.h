#pragma once
#include <iostream>
#include "Hospital.h"
using namespace std;

class UI
{
public:
    UI();
    void print_hospital(Hospital* H, int id);
    void print_cars_info(Min_priQueue<Car*>* out,Min_priQueue<Car*>* back, LinkedQueue<Patient*>* finished);
    ~UI();
};

UI::UI(){}

void UI::print_hospital(Hospital* H, int id){
    for(int i{0};i<10;i++){cout<<'=';} cout << "Hospital #"<< id << "Data";for(int i{0};i<10;i++){cout<<'=';} cout <<endl;
            
    priQueue<Patient*> *emergency ;LinkedQueue<Patient*> *special;CancelQueue<Patient*> *normal;
    H->get_patient_lists(emergency, special, normal);

    cout << emergency->get_entries() << " EP requests: "; emergency->print_patients();
    cout << special->get_entries() << " SP requests: "; special->print();
    cout << normal->get_entries() << " NP requests: "; normal->print();
    
    LinkedQueue<Car*>* nc; LinkedQueue<Car*>* sc;
    H->get_car_lists(nc, sc);
    cout << "Free cars: " << nc->get_entries() << " Normal, " << sc->get_entries()<<" Special" << endl;    
    for(int i{0};i<10;i++){cout<<'=';} cout << "Hospital #"<< id << "END";for(int i{0};i<10;i++){cout<<'=';} cout <<endl;
}

void UI::print_cars_info(Min_priQueue<Car*>* out,Min_priQueue<Car*>* back, LinkedQueue<Patient*>* finished){
    for(int i{0}; i<20;i++){cout <<'-';} cout<<endl;
    cout << out->get_entries() << " ==> Out Cars: "; out->print_cars();
    cout << back->get_entries() << " <== Back Cars: "; back->print_cars();
    for(int i{0}; i<20;i++){cout <<'-';} cout<<endl;
    cout<<finished->get_entries() << " Finished Patients: "; finished->print();
}

UI::~UI() {}
