#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>
#include <algorithm>

using namespace std;

struct Employee {
    int employeeID; // Renamed id to employeeID
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
        if (head == nullptr || head->employeeID > emp.employeeID) {
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
        while (curr->next != nullptr && curr->next->employeeID < emp.employeeID) {
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
        while (curr != nullptr && curr->employeeID != id) {
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
            cout << "ID: " << curr->employeeID << ", Name: " << curr->fullName
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
    cin >> emp.employeeID;
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

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Employee emp;

        ss >> emp.employeeID;
        ss.ignore(); // Ignore the comma
        getline(ss, emp.fullName, ',');
        ss >> emp.gender;
        ss.ignore(); // Ignore the comma
        ss >> emp.yearOfBirth;
        ss.ignore(); // Ignore the comma
        getline(ss, emp.address, ',');
        ss >> emp.salaryLevel;
        ss.ignore(); // Ignore the comma
        ss >> emp.yearOfEmployment;

        list.addEmployee(emp);
    }
    file.close();
}

int main() {
    EmployeeList list;

    // Load data from file or keyboard
    cout << "Do you want to load data from a file? (y/n): ";
    char choice;
    cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        string filename;
        cout << "Enter filename: ";
        cin >> filename;
        loadDataFromFile("./employees.txt", list);
    } else {
        int numEmployees;
        cout << "Enter number of employees to input: ";
        cin >> numEmployees;
        for (int i = 0; i < numEmployees; ++i) {
            cout << "Entering data for employee " << i + 1 << ":" << endl;
            readEmployeeFromKeyboard(list);
        }
    }

    // Display all employees
    cout << "\nList of Employees:" << endl;
    list.printEmployees();

    // Find employee with highest seniority
    Employee* senior = list.findHighestSeniority();
    if (senior) {
        cout << "\nEmployee with the highest seniority:" << endl;
        cout << "Name: " << senior->fullName << ", Year of Employment: " << senior->yearOfEmployment << endl;
    }

    // Find employee with the lowest salary
    Employee* lowest = list.findLowestSalary();
    if (lowest) {
        cout << "\nEmployee with the lowest salary:" << endl;
        cout << "Name: " << lowest->fullName << ", Salary Level: " << lowest->salaryLevel << endl;
    }

    // Count male employees
    int maleCount = list.countMaleEmployees();
    cout << "\nNumber of male employees: " << maleCount << endl;

    // Count employees with last name "Nguyễn"
    int nguyenCount = list.countLastNameNguyen();
    cout << "Number of employees with last name 'Nguyễn': " << nguyenCount << endl;

    return 0;
}
