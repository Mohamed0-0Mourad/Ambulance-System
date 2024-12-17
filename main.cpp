#include<iostream>
#include"Organiser.h"
#include <stdlib.h>
#include<ctime>
#include "UI.h"
using namespace std;

int main()
{
    Organiser o;
    int random, step{1}, hospitals_num, requests_num, finished_till_now{0};Hospital* hospitals;
    o.load_file("test.txt", hospitals_num, requests_num, hospitals);
    UI interface;
    while(true)
    {
        for (int h = 0; h < hospitals_num; h++)
        {
            srand(time(nullptr));
            random = rand()%100+1;
            if(random>=10 && random<20){
                if(o.move_to_finish(h, "SP", finished_till_now)){
                    if(finished_till_now==requests_num) {return 0;}
                }
            }
            else if(random>=20 && random <25){
                if(o.move_to_finish(h, "EP", finished_till_now)){
                    if(finished_till_now==requests_num) {return 0;}
                }
            }
            else if(random>=30 && random <40){
                if(o.move_to_finish(h, "NP", finished_till_now)){
                    if(finished_till_now==requests_num) {return 0;}
                }
            }
            else if(random>=40 && random <45){
                if(o.assign_car(h, 's', step)){continue;}
            }
            else if(random>=70 && random<75){
                if(o.assign_car(h, 'n', step)){continue;}
            }
            else if(random>=80&& random<90){
                if(o.carry_back(step)){continue;}
            }
            else if (random>=91 && random <95){
                if(o.backTo_hospital()){continue;}
            }
            cout << "Current Timestep: " << step<<endl;
            for(int i{0};i<10;i++){cout<<'=';} cout << "Hospital #"<< h+1 << "Data";for(int i{0};i<10;i++){cout<<'=';} cout <<endl;
            interface.print_hospital(hospitals+h);
            for(int i{0};i<10;i++){cout<<'=';} cout << "Hospital #"<< h+1 << "END";for(int i{0};i<10;i++){cout<<'=';} cout <<endl;
            interface.print_cars_info(&o);
            bool next;
            if(finished_till_now == requests_num) {return 0;}
            cin>>next;
        }
        step++;
    }
}