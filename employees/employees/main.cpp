#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <vector>

class Employee {
public:
    int id;
    std::string fullName;
    char gender;
    int yearOfBirth;
    std::string address;
    int salaryLevel;
    int yearOfEmployment;

    Employee(int id, std::string fullName, char gender, int yearOfBirth, std::string address, int salaryLevel, int yearOfEmployment)
        : id(id), fullName(fullName), gender(gender), yearOfBirth(yearOfBirth), address(address), salaryLevel(salaryLevel), yearOfEmployment(yearOfEmployment) {}
};

class Node {
public:
    Employee* data;
    Node* next;
    Node* prev;

    Node(Employee* emp) : data(emp), next(nullptr), prev(nullptr) {}
};

class DoublyLinkedList {
public:
    Node* head;
    Node* tail;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr) {}
    ~DoublyLinkedList();

    void addFront(Employee* emp);
    void addEnd(Employee* emp);
    void addSorted(Employee* emp);
    void remove(int id);
    void display();
    Node* findHighestSeniority();
    Node* findLowestSalary();
    int countSeniority(int year);
    int countByGender(char gender);
    int countLastName(const std::string& lastName);
    void sort();
    void clear();
};

DoublyLinkedList::~DoublyLinkedList() {
    clear();
}

void DoublyLinkedList::addFront(Employee* emp) {
    Node* newNode = new Node(emp);
    if (!head) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
}

void DoublyLinkedList::addEnd(Employee* emp) {
    Node* newNode = new Node(emp);
    if (!tail) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
}

void DoublyLinkedList::addSorted(Employee* emp) {
    if (!head || emp->id < head->data->id) {
        addFront(emp);
        return;
    }
    Node* newNode = new Node(emp);
    Node* current = head;
    while (current->next && current->next->data->id < emp->id) {
        current = current->next;
    }
    newNode->next = current->next;
    if (current->next) {
        current->next->prev = newNode;
    } else {
        tail = newNode;
    }
    newNode->prev = current;
    current->next = newNode;
}



int DoublyLinkedList::countSeniority(int years) {
    int count = 0;
    Node* current = head;
    while (current != nullptr) {
        if (2024 - current->data->yearOfEmployment >= years) { // Assuming the current year is 2024
            count++;
        }
        current = current->next;
    }
    return count;
}

// Remove employee by ID
void DoublyLinkedList::remove(int id) {
    Node* current = head;
    while (current != nullptr && current->data->id != id) {
        current = current->next;
    }
    if (current == nullptr) return; // Employee not found

    if (current->prev) {
        current->prev->next = current->next;
    } else {
        head = current->next; // Remove from front
    }
    if (current->next) {
        current->next->prev = current->prev;
    } else {
        tail = current->prev; // Remove from end
    }
    delete current->data; // Free the memory occupied by Employee
    delete current; // Free the memory occupied by Node
}

void DoublyLinkedList::sort() {
    if (head == nullptr || head->next == nullptr) return; // No need to sort if the list is empty or has one element

    Node* sorted = nullptr;
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->next;
        if (sorted == nullptr || sorted->data->id >= current->data->id) {
            current->next = sorted;
            if (sorted != nullptr) sorted->prev = current;
            sorted = current;
            sorted->prev = nullptr;
        } else {
            Node* temp = sorted;
            while (temp->next != nullptr && temp->next->data->id < current->data->id) {
                temp = temp->next;
            }
            current->next = temp->next;
            if (temp->next != nullptr) {
                temp->next->prev = current;
            }
            temp->next = current;
            current->prev = temp;
        }
        current = next;
    }
    head = sorted;
    // Update tail
    tail = head;
    if (tail != nullptr) {
        while (tail->next != nullptr) tail = tail->next;
    }
}


void DoublyLinkedList::display() {
    Node* current = head;
    while (current) {
        std::cout << "ID: " << current->data->id << ", Name: " << current->data->fullName << ", Gender: " << current->data->gender
                  << ", Year of Birth: " << current->data->yearOfBirth << ", Address: " << current->data->address
                  << ", Salary Level: " << current->data->salaryLevel << ", Year of Employment: " << current->data->yearOfEmployment << std::endl;
        current = current->next;
    }
}

Node* DoublyLinkedList::findHighestSeniority() {
    Node* current = head;
    Node* highest = nullptr;
    int minYear = std::numeric_limits<int>::max();
    while (current) {
        if (current->data->yearOfEmployment < minYear) {
            highest = current;
            minYear = current->data->yearOfEmployment;
        }
        current = current->next;
    }
    return highest;
}

Node* DoublyLinkedList::findLowestSalary() {
    Node* current = head;
    Node* lowest = nullptr;
    int minSalary = std::numeric_limits<int>::max();
    while (current) {
        if (current->data->salaryLevel < minSalary) {
            lowest = current;
            minSalary = current->data->salaryLevel;
        }
        current = current->next;
    }
    return lowest;
}



int DoublyLinkedList::countByGender(char gender) {
    int count = 0;
    Node* current = head;
    while (current) {
        if (current->data->gender == gender) {
            count++;
        }
        current = current->next;
    }
    return count;
}

int DoublyLinkedList::countLastName(const std::string& lastName) {
    int count = 0;
    Node* current = head;
    while (current) {
        std::size_t pos = current->data->fullName.find_last_of(" ");
        if (pos != std::string::npos && current->data->fullName.substr(pos + 1) == lastName) {
            count++;
        }
        current = current->next;
    }
    return count;
}

void DoublyLinkedList::clear() {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp->data;
        delete temp;
    }
    tail = nullptr;
}

void loadFromFile(DoublyLinkedList& list, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        int id, yearOfBirth, salaryLevel, yearOfEmployment;
        std::string fullName, address;
        char gender;
        if (!(iss >> id >> fullName >> gender >> yearOfBirth >> address >> salaryLevel >> yearOfEmployment)) {
            std::cerr << "Failed to parse line: " << line << std::endl;
            continue;
        }
        fullName = fullName.replace(fullName.begin(), fullName.end(), '_', ' ');
        address = address.replace(address.begin(), address.end(), '_', ' ');
        Employee* emp = new Employee(id, fullName, gender, yearOfBirth, address, salaryLevel, yearOfEmployment);
        list.addSorted(emp);
    }
    file.close();
}

void saveToFile(DoublyLinkedList& list, const std::string& filename) {
    std::ofstream file(filename);
    Node* current = list.head;
    while (current) {
        Employee* emp = current->data;
        file << emp->id << " " << emp->fullName << " " << emp->gender << " " << emp->yearOfBirth << " " << emp->address << " " << emp->salaryLevel << " " << emp->yearOfEmployment << "\n";
        current = current->next;
    }
    file.close();
}

void loadDataFromFile(const std::string& filename, DoublyLinkedList& list) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (getline(file, line)) {
        std::istringstream iss(line);
        int id, yearOfBirth, salaryLevel, yearOfEmployment;
        std::string fullName, address;
        char gender;
        iss >> id >> std::ws;
        getline(iss, fullName, ' ');
        iss >> gender >> yearOfBirth >> std::ws;
        getline(iss, address, ' ');
        iss >> salaryLevel >> yearOfEmployment;
        Employee* emp = new Employee(id, fullName, gender, yearOfBirth, address, salaryLevel, yearOfEmployment);
        list.addSorted(emp);
    }
    file.close();
}

void readEmployeeFromKeyboard(DoublyLinkedList& list) {
    int id, yearOfBirth, salaryLevel, yearOfEmployment;
    std::string fullName, address;
    char gender;

    std::cout << "Enter ID: ";
    std::cin >> id;
    std::cin.ignore(); // Ignore the newline character left in the input buffer

    std::cout << "Enter Full Name: ";
    getline(std::cin, fullName);

    std::cout << "Enter Gender (M/F): ";
    std::cin >> gender;

    std::cout << "Enter Year of Birth: ";
    std::cin >> yearOfBirth;

    std::cin.ignore(); // Ignore the newline character left in the input buffer

    std::cout << "Enter Address: ";
    getline(std::cin, address);

    std::cout << "Enter Salary Level: ";
    std::cin >> salaryLevel;

    std::cout << "Enter Year of Employment: ";
    std::cin >> yearOfEmployment;

    Employee* emp = new Employee(id, fullName, gender, yearOfBirth, address, salaryLevel, yearOfEmployment);
    list.addSorted(emp);
}


int main() {
    DoublyLinkedList employeeList;
    
        std::cout << "Do you want to load data from a file? (y/n): ";
        char choice;
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            std::string filename;
            std::cout << "Enter filename: ";
            std::cin >> filename;
            loadDataFromFile(filename, employeeList);
        } else {
            int numEmployees;
            std::cout << "Enter number of employees to input: ";
            std::cin >> numEmployees;
            for (int i = 0; i < numEmployees; ++i) {
                std::cout << "Entering data for employee " << i + 1 << ":" << std::endl;
                readEmployeeFromKeyboard(employeeList);
            }
        }

    
    
    //loadFromFile(employeeList, "/Users/nguyenlephong/Documents/Projects/c-learning/employees/employees/employees.txt");
    employeeList.display();
    
    // Adding new Employee
    Employee* newEmp = new Employee(3, "Jane Doe", 'F', 1992, "456 Lane", 2, 2012);
    employeeList.addSorted(newEmp);

    // Display all employees
    employeeList.display();

    // Find and display employee with the highest seniority
    Node* senior = employeeList.findHighestSeniority();
    if (senior) {
        std::cout << "Highest Seniority: " << senior->data->fullName << " - Year: " << senior->data->yearOfEmployment << std::endl;
    }

    // Find and display employee with the lowest salary
    Node* lowestSalary = employeeList.findLowestSalary();
    if (lowestSalary) {
        std::cout << "Lowest Salary: " << lowestSalary->data->fullName << " - Salary Level: " << lowestSalary->data->salaryLevel << std::endl;
    }

    // Count and display the number of employees with last name "Nguyen"
    int nguyenCount = employeeList.countLastName("Nguyen");
    std::cout << "Number of employees with last name 'Nguyen': " << nguyenCount << std::endl;

    saveToFile(employeeList, "/Users/nguyenlephong/Documents/Projects/c-learning/employees/employees/updated_employees.txt");

    return 0;
}
