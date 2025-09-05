// Created by Navarro Guzmán Ángel Gustavo
// Bridge and Torch Problem for N people
//
// Description:
// This program simulates the classic "bridge and torch" problem, where multiple people
// need to cross a bridge at night using a single torch. Each person takes a different 
// amount of time to cross, and at most two people can cross simultaneously.
// The crossing time for two people is determined by the slower person.
// The goal is to determine if the group can cross the bridge within a given time limit.

#include <iostream>
#include <vector>
#include <algorithm> // for sort

using namespace std;

// Data structure to represent a person
struct Person {
    int id;     // Person identifier (1, 2, 3, ...)
    int time;   // Time required for this person to cross the bridge
};

// Comparator for sorting (descending order by crossing time)
bool compareDesc(const Person &a, const Person &b) {
    return a.time > b.time;
}

int main() {
    int count;           // Number of people
    int time_limit;      // Maximum allowed crossing time

    // Input: number of people
    cout << "Number of people crossing the bridge: ";
    if (!(cin >> count) || count <= 0) {
        cout << "Error: Enter a valid number greater than 0\n";
        return 1;
    }

    vector<Person> people(count);

    // Input: time for each person
    for (int i = 0; i < count; i++) {
        people[i].id = i + 1;
        cout << "Enter the time of person (" << people[i].id << "): ";
        if (!(cin >> people[i].time) || people[i].time <= 0) {
            cout << "Error: Enter a valid time greater than 0\n";
            return 1;
        }
    }

    // Input: total time limit
    cout << "Time limit to cross: ";
    if (!(cin >> time_limit) || time_limit <= 0) {
        cout << "Error: Enter a valid number greater than 0\n";
        return 1;
    }

    // Sort people in descending order
    sort(people.begin(), people.end(), compareDesc);

    int time_result = 0; // Total crossing time accumulator

    if (count == 1) {
        cout << "\n(" << people[0].id << ")-> time cost: " << people[0].time << "\n";
        time_result = people[0].time;
    }
    else if (count == 2) {
        cout << "\n(" << people[0].id << "+" << people[1].id << ")-> time cost: " << people[0].time << "\n";
        time_result = people[0].time;
    }
    else {
        cout << "\n(" << people[count-2].id << "+" << people[count-1].id
             << ")-> time cost: " << people[count-2].time
             << "\n(" << people[count-1].id << ")<- time cost: " << people[count-1].time << "\n";
        time_result += (people[count-2].time + people[count-1].time);

        for (int i = 0; i < count-2; i += 2) {
            if (i + 2 == count - 2) {
                cout << "(" << people[i].id << "+" << people[i+1].id
                     << ")-> time cost: " << people[i].time << "\n";
                cout << "(" << people[count-2].id << ")<- time cost: " << people[count-2].time << "\n";
                cout << "(" << people[count-2].id << "+" << people[count-1].id
                     << ")-> time cost: " << people[count-2].time << "\n";
                time_result += (people[i].time + people[count-2].time + people[count-2].time);
            }
            else if (i + 1 == count - 2) {
                cout << "(" << people[i].id << "+" << people[count-1].id
                     << ")-> time cost: " << people[i].time << "\n";
                time_result += people[i].time;
            }
            else {
                cout << "(" << people[i].id << "+" << people[i+1].id
                     << ")-> time cost: " << people[i].time << "\n";
                cout << "(" << people[count-1].id << ")<- time cost: " << people[count-2].id << "\n";
                cout << "(" << people[count-1].id << "+" << people[count-2].id << ")-> time cost: "
                     << people[count-2].id << "\n";
                cout << "(" << people[count-2].id << ")<- time cost: " << people[count-2].id << "\n";
                time_result += (people[i].time + people[count-1].time +
                                people[count-2].time + people[count-2].time);
            }
        }
    }

    // Final results
    cout << "\nTime result: " << time_result << "\n";
    if (time_result <= time_limit)
        cout << "The problem has a solution\n";
    else
        cout << "The problem doesn't have a solution\n";

    return 0;
}
