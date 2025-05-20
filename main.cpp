#include<iostream>
#include"Organiser.h"
using namespace std;

int main()
{
    Organiser o;
    // choose the scenario file
    o.load_file("test.txt");
    int mode; 
    bool silent= false;
    cout<< "Hello, Choose mode of operation(by typing index integer):" <<endl;
    cout << "1. Silent\n2.Interactive"<<endl;
    cin>>mode;
    if(mode==1){silent = true;}
    o.main_simulation(silent);

}