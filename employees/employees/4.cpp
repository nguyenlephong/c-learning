#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>
#include <algorithm>

using namespace std;

struct Employee {
    int id;
    string fullName;
    char gender;
    int yearOfBirth;
    string address;
    float salaryLevel;
    int yearOfEmployment;
    Employee* next;
    Employee* prev;
};

class EmployeeList {
private:
    Employee* head;
    Employee* tail;

public:
    EmployeeList() : head(nullptr), tail(nullptr) {}
    ~EmployeeList() {
        Employee* current = head;
        while (current != nullptr) {
            Employee* next = current->next;
            delete current;
            current = next;
        }
    }

    void addEmployee(const Employee& emp) {
        Employee* newEmp = new Employee(emp);
        if (head == nullptr || head->id > emp.id) {
            newEmp->next = head;
            newEmp->prev = nullptr;
            if (head != nullptr) {
                head->prev = newEmp;
            }
            head = newEmp;
            if (tail == nullptr) {
                tail = newEmp;
            }
            return;
        }

        Employee* curr = head;
        while (curr->next != nullptr && curr->next->id < emp.id) {
            curr = curr->next;
        }

        newEmp->next = curr->next;
        if (curr->next != nullptr) {
            curr->next->prev = newEmp;
        }
        newEmp->prev = curr;
        curr->next = newEmp;
        if (newEmp->next == nullptr) {
            tail = newEmp;
        }
    }

    void removeEmployee(int id) {
        Employee* curr = head;
        while (curr != nullptr && curr->id != id) {
            curr = curr->next;
        }
        if (curr == nullptr) return;

        if (curr->prev != nullptr) {
            curr->prev->next = curr->next;
        } else {
            head = curr->next;
        }
        if (curr->next != nullptr) {
            curr->next->prev = curr->prev;
        } else {
            tail = curr->prev;
        }
        delete curr;
    }

    void printEmployees() const {
        Employee* curr = head;
        while (curr != nullptr) {
            cout << "ID: " << curr->id << ", Name: " << curr->fullName
                 << ", Gender: " << curr->gender << ", Year of Birth: "
                 << curr->yearOfBirth << ", Address: " << curr->address
                 << ", Salary Level: " << curr->salaryLevel
                 << ", Year of Employment: " << curr->yearOfEmployment << endl;
            curr = curr->next;
        }
    }

    Employee* findHighestSeniority() const {
        if (head == nullptr) return nullptr;
        Employee* senior = head;
        for (Employee* curr = head->next; curr != nullptr; curr = curr->next) {
            if (curr->yearOfEmployment < senior->yearOfEmployment) {
                senior = curr;
            }
        }
        return senior;
    }

    Employee* findLowestSalary() const {
        if (head == nullptr) return nullptr;
        Employee* lowest = head;
        for (Employee* curr = head->next; curr != nullptr; curr = curr->next) {
            if (curr->salaryLevel < lowest->salaryLevel) {
                lowest = curr;
            }
        }
        return lowest;
    }

    int countMaleEmployees() const {
        int count = 0;
        for (Employee* curr = head; curr != nullptr; curr = curr->next) {
            if (curr->gender == 'M' || curr->gender == 'm') {
                ++count;
            }
        }
        return count;
    }

    int countLastNameNguyen() const {
        int count = 0;
        for (Employee* curr = head; curr != nullptr; curr = curr->next) {
            if (curr->fullName.find("Nguyễn") != string::npos) {
                ++count;
            }
        }
        return count;
    }
};

void readEmployeeFromKeyboard(EmployeeList& list) {
    Employee emp;
    cout << "Enter ID: ";
    cin >> emp.id;
   cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Clear the input buffer
    cout << "Enter Full Name: ";
    getline(cin, emp.fullName);
    cout << "Enter Gender (M/F): ";
    cin >> emp.gender;
    cout << "Enter Year of Birth: ";
    cin >> emp.yearOfBirth;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Clear the input buffer again
    cout << "Enter Address: ";
    getline(cin, emp.address);
    cout << "Enter Salary Level: ";
    cin >> emp.salaryLevel;
    cout << "Enter Year of Employment: ";
    cin >> emp.yearOfEmployment;
    list.addEmployee(emp);
}

void loadDataFromFile(const string& filename, EmployeeList& list) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return;
    }

    string line, temp;
    while (getline(file, line)) {
        stringstream ss(line);
        Employee emp;

        getline(ss, temp, ','); emp.id = stoi(temp);
        getline(ss, emp.fullName, ',');
        getline(ss, temp, ','); emp.gender = temp[0];
        getline(ss, temp, ','); emp.yearOfBirth = stoi(temp);
        getline(ss, emp.address, ',');
        getline(ss, temp, ','); emp.salaryLevel = stof(temp);
        getline(ss, temp, ','); emp.yearOfEmployment = stoi(temp);

        list.addEmployee(emp);
    }
    file.close();
}
