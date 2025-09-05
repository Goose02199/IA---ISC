// Created by Navarro Guzmán Ángel Gustavo
// Bridge and Torch Problem for N people
//
// Description:
// This program simulates the classic "bridge and torch" problem, where multiple people
// need to cross a bridge at night using a single torch. Each person takes a different 
// amount of time to cross, and at most two people can cross simultaneously.
// The crossing time for two people is determined by the slower person.
// The goal is to determine if the group can cross the bridge within a given time limit.

#include <stdio.h>
#include <stdlib.h>

// Data structure to represent a person
typedef struct {
    int id;     // Person identifier (1, 2, 3, ...)
    int time;   // Time required for this person to cross the bridge
} Person;

// Comparator for qsort (sorts by descending crossing time)
// This ensures that the slowest people are placed at the beginning of the array.
int compareDesc(const void *a, const void *b) {
    Person *p1 = (Person*)a;
    Person *p2 = (Person*)b;
    return p2->time - p1->time; // Sort in descending order
}

int main() {
    int count;           // Number of people
    Person *people = NULL;
    int time_limit;      // Maximum allowed crossing time

    // Input: number of people
    printf("Number of people crossing the bridge: ");
    if (scanf("%d", &count) != 1 || count <= 0) {
        printf("Error: Enter a valid number greater than 0\n");
        return 1;
    }

    // Dynamic allocation for array of people
    people = (Person*)malloc(count * sizeof(Person));
    if (people == NULL) {
        printf("Error: Memory allocation failed\n");
        return 1;
    }

    // Input: time for each person
    for (int i = 0; i < count; i++) {
        people[i].id = i + 1;
        printf("Enter the time of person (%d): ", people[i].id);
        if (scanf("%d", &people[i].time) != 1 || people[i].time <= 0) {
            printf("Error: Enter a valid time greater than 0\n");
            free(people);
            return 1;
        }
    }

    // Input: total time limit for the crossing
    printf("Time limit to cross: ");
    if (scanf("%d", &time_limit) != 1 || time_limit <= 0) {
        printf("Error: Enter a valid number greater than 0\n");
        free(people);
        return 1;
    }

    // Sort people in descending order by their crossing time
    qsort(people, count, sizeof(Person), compareDesc);

    int time_result = 0; // Total crossing time accumulator

    // Special case: only 1 person
    if (count == 1) {
        // Trivial case, only one crossing needed
        printf("\n(%d)-> time cost: %d\n", people[0].id, people[0].time);
        time_result = people[0].time;
    }
    // Special case: only 2 people
    else if (count == 2) {
        // Both cross together, time is determined by the slower person
        printf("\n(%d+%d)-> time cost: %d\n", people[0].id, people[1].id, people[0].time);
        time_result = people[0].time;
    }
    // General case: 3 or more people
    else {
        // First crossing: the two fastest (from the end of array)
        printf("\n(%d+%d)-> time cost: %d\n(%d)<- time cost: %d\n", people[count-2].id, people[count-1].id, people[count-2].time, people[count-1].id, people[count-1].time);
        time_result += (people[count-2].time + people[count-1].time);

        // Iterate through pairs
        for (int i = 0; i < count-2; i += 2) {
            if (i + 2 == count - 2) {
                // Last two people crossing sequence
                printf("(%d+%d)-> time cost: %d\n", people[i].id, people[i+1].id, people[i].time);
                printf("(%d)<- time cost: %d\n(%d+%d)-> time cost: %d\n", people[count-2].id, people[count-2].time, people[count-2].id, people[count-1].id, people[count-2].time);
                time_result += (people[i].time + people[count-2].time + people[count-2].time);
            }
            else if (i + 1 == count - 2) {
                // Case when one person remains with the slowest one
                printf("(%d+%d)-> time cost: %d\n", people[i].id, people[count-1].id, people[i].time);
                time_result += people[i].time;
            } 
            else {
                // General crossing sequence
                printf("(%d+%d)-> time cost: %d\n", people[i].id, people[i+1].id, people[i].time);
                printf("(%d)<- time cost: %d\n(%d+%d)-> time cost: %d\n(%d)<- time cost: %d\n", people[count-1].id, people[count-2].id, people[count-1].id, people[count-2].id, people[count-2].id);
                time_result += (people[i].time + people[count-1].time + people[count-2].time + people[count-2].time);
            }
        }
    }

    // Final results
    printf("\nTime result: %d\n", time_result);
    if (time_result <= time_limit)
        printf("The problem has a solution\n");
    else
        printf("The problem doesn't have a solution\n");

    // Free allocated memory
    free(people);
    people = NULL;

    return 0;
}
