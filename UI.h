#pragma once
#include <iostream>
#include"Organiser.h"
using namespace std;

class UI
{
public:
    UI();
    void print_hospital(Hospital* H);
    void print_cars_info( Organiser* o);
    ~UI();
};

UI::UI(){}

void UI::print_hospital(Hospital* H){
    priQueue<Patient*> *emergency ;LinkedQueue<Patient*> *special;CancelQueue<Patient*> *normal;
    H->get_patient_lists(emergency, special, normal);

    cout << emergency->get_entries() << " EP requests: "; emergency->print_patients();
    cout << special->get_entries() << " SP requests: "; special->print();
    cout << normal->get_entries() << " NP requests: "; normal->print();
    
    LinkedQueue<Car*>* nc; LinkedQueue<Car*>* sc;
    H->get_car_lists(nc, sc);
    cout << "Free cars: " << nc->get_entries() << " Normal, " << sc->get_entries()<<" Special" << endl;    
}

void UI::print_cars_info(Organiser* o){
    for(int i{0}; i<20;i++){cout <<'-';} cout<<endl;
    Min_priQueue<Car*>* out;Min_priQueue<Car*>* back;
    o->get_cars_list(out , back);
    cout << out->get_entries() << " ==> Out Cars: "; out->print_cars();
    cout << back->get_entries() << " <== Back Cars: "; back->print_cars();
    for(int i{0}; i<20;i++){cout <<'-';} cout<<endl;
    LinkedQueue<Patient*>* finished = o->get_finished_list();
    cout<<finished->get_entries() << " Finished Patients: "; finished->print();
}

UI::~UI() {}
