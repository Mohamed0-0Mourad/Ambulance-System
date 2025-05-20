# 🚑 Hospital Ambulance Requests Simulation

A C++ simulation project designed to emulate the dispatch and patient management system of a multi-hospital network. The project is structured to handle real-time request simulation, dynamic patient allocation, car dispatch, and return cycles using queue-based data structures.

## 📦 Project Structure

- **Patients**: Three types — Normal (NP), Special (SP), and Emergency (EP), each handled differently.
- **Cars**: Two categories — Ncars (normal) and Scars (specialized).
- **Hospitals**: Each maintains its own queues for patients and free cars.
- **Organiser**: A class responsible for coordinating cars and moving patients.
- **UI**: Outputs the current state of the simulation for hospitals and cars.

## 🛠 Features

- File-based input system to configure hospitals, cars, and request queues.
- Dynamic simulation loop processing events based on time and random chance.
- Movement of patients and cars through queues using object-oriented techniques.
- Output console interface for real-time simulation feedback.
- Organized by separate classes for clear structure and inheritance.

## 🧪 Input Format

The simulator loads data from a `.txt` file following a structured format:

```txt
4             → number of hospitals
110 75        → speed of Scars and Ncars
3 11          → hospital 1: 3 Scars, 11 Ncars
...
150           → number of patient requests
NP 3 1 2 159  → type, time, PID, hospital ID, distance
SP 2 4 1 588  → includes distance
EP 13 4 4 433 5 → includes severity
```
## 🔁 Simulation Logic
### At each timestep, a *random number* determines which operation to execute:

* Move patients to the finished list based on type.

* Move cars from free → out → back → free.

* Probability-based logic for operations (e.g., if rand is between 30–40, finish an NP request).

## 🧠 Concepts Covered
* Object Oriented Programming, handled multiple object classes.
* C++ inheritance, virtual functions, and destructors.

* File I/O using fstream.

* Queues: linked and priority queues.

* Dynamic memory management and ownership.

* Simulation algorithms with randomization.

--- 
Designed for coursework in Data Structures and Algorithms (Fall 2024).


