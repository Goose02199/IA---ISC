# Created by Navarro Guzmán Ángel Gustavo
# Bridge and Torch Problem for N people
#
# Description:
# This program simulates the classic "bridge and torch" problem, where multiple people
# need to cross a bridge at night using a single torch. Each person takes a different 
# amount of time to cross, and at most two people can cross simultaneously.
# The crossing time for two people is determined by the slower person.
# The goal is to determine if the group can cross the bridge within a given time limit.

def main():
    try:
        count = int(input("Number of people crossing the bridge: "))
        if count <= 0:
            print("Error: Enter a valid number greater than 0")
            return
    except ValueError:
        print("Error: Invalid input")
        return

    people = []
    for i in range(count):
        try:
            time = int(input(f"Enter the time of person ({i+1}): "))
            if time <= 0:
                print("Error: Enter a valid time greater than 0")
                return
            people.append((i+1, time))  # (id, time)
        except ValueError:
            print("Error: Invalid input")
            return

    try:
        time_limit = int(input("Time limit to cross: "))
        if time_limit <= 0:
            print("Error: Enter a valid number greater than 0")
            return
    except ValueError:
        print("Error: Invalid input")
        return

    # Sort people in descending order of time
    people.sort(key=lambda x: x[1], reverse=True)

    time_result = 0

    if count == 1:
        print(f"\n({people[0][0]})-> time cost: {people[0][1]}")
        time_result = people[0][1]

    elif count == 2:
        print(f"\n({people[0][0]}+{people[1][0]})-> time cost: {people[0][1]}")
        time_result = people[0][1]

    else:
        print(f"\n({people[-2][0]}+{people[-1][0]})-> time cost: {people[-2][1]}")
        print(f"({people[-1][0]})<- time cost: {people[-1][1]}")
        time_result += people[-2][1] + people[-1][1]

        i = 0
        while i < count - 2:
            if i + 2 == count - 2:
                print(f"({people[i][0]}+{people[i+1][0]})-> time cost: {people[i][1]}")
                print(f"({people[-2][0]})<- time cost: {people[-2][1]}")
                print(f"({people[-2][0]}+{people[-1][0]})-> time cost: {people[-2][1]}")
                time_result += people[i][1] + people[-2][1] + people[-2][1]
            elif i + 1 == count - 2:
                print(f"({people[i][0]}+{people[-1][0]})-> time cost: {people[i][1]}")
                time_result += people[i][1]
            else:
                print(f"({people[i][0]}+{people[i+1][0]})-> time cost: {people[i][1]}")
                print(f"({people[-1][0]})<- time cost: {people[-2][0]}")
                print(f"({people[-1][0]}+{people[-2][0]})-> time cost: {people[-2][0]}")
                print(f"({people[-2][0]})<- time cost: {people[-2][0]}")
                time_result += people[i][1] + people[-1][1] + people[-2][1] + people[-2][1]
            i += 2

    # Final results
    print(f"\nTime result: {time_result}")
    if time_result <= time_limit:
        print("The problem has a solution")
    else:
        print("The problem doesn't have a solution")


if __name__ == "__main__":
    main()
