#include <fstream>
#include <cstring>
#include <algorithm> // For sorting
#include <limits>    // For numeric_limits
#include <string>    // For string manipulation
#include <iostream>  // For output
#include <iomanip>   // For formatting output
#include <sstream>

using namespace std;

struct Employee {
    int employeeID;
    string fullName;
    char gender;
    int birthYear;
    string address;
    int salaryLevel;
    int employmentYear;
    Employee* next;
    Employee* prev;
};

class EmployeeList {
private:
    Employee* head;
public:
    EmployeeList() {
        head = nullptr;
    }

    // Function to add an employee to the beginning of the list
    void addAtBeginning(Employee* emp) {
        emp->next = head;
        emp->prev = nullptr;
        if (head != nullptr)
            head->prev = emp;
        head = emp;
    }

    // Function to add an employee to the end of the list
    void addAtEnd(Employee* emp) {
        if (head == nullptr) {
            head = emp;
            return;
        }
        Employee* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = emp;
        emp->prev = temp;
    }

    // Function to add an employee at any position in the list
    void addAtPosition(int position, Employee* emp) {
        if (position <= 1) {
            addAtBeginning(emp);
            return;
        }
        Employee* temp = head;
        for (int i = 1; i < position - 1 && temp != nullptr; ++i) {
            temp = temp->next;
        }
        if (temp == nullptr) {
            cerr << "Position out of range." << endl;
            return;
        }
        emp->next = temp->next;
        emp->prev = temp;
        if (temp->next != nullptr)
            temp->next->prev = emp;
        temp->next = emp;
    }

    // Function to remove an employee from the list
    void removeEmployee(Employee* emp) {
        if (emp == nullptr)
            return;
        if (emp == head)
            head = emp->next;
        if (emp->next != nullptr)
            emp->next->prev = emp->prev;
        if (emp->prev != nullptr)
            emp->prev->next = emp->next;
        delete emp;
    }

   
    // Function to sort the list by employee ID
    void sortByEmployeeID() {
        if (!head || !head->next) // If the list is empty or has only one element, it's already sorted
            return;

        bool swapped;
        Employee* ptr1;
        Employee* lptr = nullptr;

        do {
            swapped = false;
            ptr1 = head;

            while (ptr1->next != lptr) {
                if (ptr1->employeeID > ptr1->next->employeeID) {
                    swap(ptr1, ptr1->next);
                    swapped = true;
                }
                ptr1 = ptr1->next;
            }
            lptr = ptr1;
        } while (swapped);
    }

    // Helper function to swap two employees
    void swap(Employee* a, Employee* b) {
        Employee temp = *a;
        a->employeeID = b->employeeID;
        a->fullName = b->fullName;
        a->gender = b->gender;
        a->birthYear = b->birthYear;
        a->address = b->address;
        a->salaryLevel = b->salaryLevel;
        a->employmentYear = b->employmentYear;

        b->employeeID = temp.employeeID;
        b->fullName = temp.fullName;
        b->gender = temp.gender;
        b->birthYear = temp.birthYear;
        b->address = temp.address;
        b->salaryLevel = temp.salaryLevel;
        b->employmentYear = temp.employmentYear;
    }

    // Function to add an employee while maintaining the sorted order
    void addMaintainingSort(Employee* emp) {
        if (!head || emp->employeeID < head->employeeID) {
            emp->next = head;
            if (head)
                head->prev = emp;
            head = emp;
        } else {
            Employee* temp = head;
            while (temp->next && temp->next->employeeID < emp->employeeID) {
                temp = temp->next;
            }
            emp->next = temp->next;
            if (temp->next)
                temp->next->prev = emp;
            temp->next = emp;
            emp->prev = temp;
        }
    }

    // Function to find the employee with the highest seniority
    Employee* findHighestSeniority() {
        if (!head)
            return nullptr;

        Employee* highest = head;
        Employee* current = head->next;

        while (current) {
            if (current->employmentYear < highest->employmentYear)
                highest = current;
            current = current->next;
        }

        return highest;
    }

    // Function to find the employee with the lowest salary level
    Employee* findLowestSalary() {
        if (!head)
            return nullptr;

        Employee* lowest = head;
        Employee* current = head->next;

        while (current) {
            if (current->salaryLevel < lowest->salaryLevel)
                lowest = current;
            current = current->next;
        }

        return lowest;
    }

    // Function to count the number of male employees
    int countMaleEmployees() {
        int count = 0;
        Employee* current = head;

        while (current) {
            if (current->gender == 'M')
                count++;
            current = current->next;
        }

        return count;
    }

    // Function to count employees with the last name "Nguyễn"
    int countNguyenEmployees() {
        int count = 0;
        Employee* current = head;

        while (current) {
            // Assuming the last name is the part after the last space in the full name
            size_t spacePos = current->fullName.find_last_of(" ");
            string lastName = current->fullName.substr(spacePos + 1);
            if (lastName == "Nguyễn")
                count++;
            current = current->next;
        }

        return count;
    }

    // Function to display employee statistics by seniority
    void displaySeniorityStatistics() {
        if (!head) {
            cout << "No employees in the list." << endl;
            return;
        }

        // Find the current year
        int currentYear = 2024; // Change this to the current year

        int minYear = numeric_limits<int>::max();
        int maxYear = numeric_limits<int>::min();
        Employee* current = head;

        while (current) {
            if (current->employmentYear < minYear)
                minYear = current->employmentYear;
            if (current->employmentYear > maxYear)
                maxYear = current->employmentYear;
            current = current->next;
        }

        cout << "Seniority Statistics:" << endl;
        cout << "---------------------" << endl;
        cout << "Total number of years: " << maxYear - minYear + 1 << endl;
        cout << "Minimum employment year: " << minYear << endl;
        cout << "Maximum employment year: " << maxYear << endl;
        cout << "Average employment year: " << fixed << setprecision(2) << (double)(maxYear - minYear + 1) / countEmployees() << endl;
    }

    // Function to count the total number of employees
    int countEmployees() {
        int count = 0;
        Employee* current = head;

        while (current) {
            count++;
            current = current->next;
        }

        return count;
    }

    // Function to read employee information from a text file
    void readFromFile(string filename) {
        ifstream file(filename);
         if (!file) {
             cerr << "Error opening file." << endl;
             return;
         }
         string line;
         while (getline(file, line)) {
             Employee* emp = new Employee;
             istringstream iss(line); // Changed to istringstream
             iss >> emp->employeeID >> emp->fullName >> emp->gender >> emp->birthYear >> emp->address >> emp->salaryLevel >> emp->employmentYear;
             emp->next = nullptr;
             emp->prev = nullptr;
             addAtEnd(emp); // Assuming addAtEnd function is defined elsewhere to add an employee at the end of the list
         }
         file.close();
    }

    // Function to write employee information to a text file
    void writeToFile(string filename) {
        ofstream file(filename);
        if (!file) {
            cerr << "Error opening file." << endl;
            return;
        }
        Employee* current = head;
        while (current) {
            file << current->employeeID << " " << current->fullName << " " << current->gender << " " << current->birthYear << " " << current->address << " " << current->salaryLevel << " " << current->employmentYear << endl;
            current = current->next;
        }
        file.close();
    }


    // Function to input employee information from keyboard
    Employee* inputFromKeyboard() {
        Employee* emp = new Employee;
        // Input employee information from keyboard
        return emp;
    }
};

int main() {
    // Create EmployeeList object
    EmployeeList empList;

    // Read employee information from file
    empList.readFromFile("employee_data.txt");

    
    // Display employee statistics by seniority
    empList.displaySeniorityStatistics();
    
    // Input employee information from keyboard
    Employee* emp = empList.inputFromKeyboard();

    // Add employee to the beginning of the list
    empList.addAtBeginning(emp);

    // Add employee to the end of the list
    empList.addAtEnd(emp);

    // Add employee at any position in the list
    empList.addAtPosition(2, emp);

    // Remove employee from the list
    empList.removeEmployee(emp);

    // Sort the list by employee ID
    empList.sortByEmployeeID();

    // Add employee while maintaining the sorted order
    empList.addMaintainingSort(emp);

    // Find employee with the highest seniority
    Employee* highestSeniority = empList.findHighestSeniority();

    // Find employee with the lowest salary level
    Employee* lowestSalary = empList.findLowestSalary();


    // Count the number of male employees
    int maleCount = empList.countMaleEmployees();

    // Count employees with the last name "Nguyễn"
    int nguyenCount = empList.countNguyenEmployees();

    // Write employee information to file
    empList.writeToFile("employee_data.txt");

    return 0;
}
