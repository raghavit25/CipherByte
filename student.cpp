#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Student {
public:
    int rollNo;
    char name[50];
    char department[50];
    float marks;

    void getData();
    void displayData();
};

void Student::getData() {
    cout << "Enter roll number: ";
    cin >> rollNo;
    cin.ignore();  
    cout << "Enter name: ";
    cin.getline(name, 50);
    cout << "Enter department: ";
    cin.getline(department, 50);
    cout << "Enter marks: ";
    cin >> marks;
}

void Student::displayData() {
    cout << "Roll Number: " << rollNo << endl;
    cout << "Name: " << name << endl;
    cout << "Department: " << department << endl;
    cout << "Marks: " << marks << endl;
}

void addStudent();
void displayAllStudents();
void searchStudent();
void modifyStudent();
void deleteStudent();

int main() {
    int choice;
    while (true) {
        cout << "\nStudent Record Management System\n";
        cout << "1. Add Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Search Student by Roll Number\n";
        cout << "4. Modify Student Record\n";
        cout << "5. Delete Student Record\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                displayAllStudents();
                break;
            case 3:
                searchStudent();
                break;
            case 4:
                modifyStudent();
                break;
            case 5:
                deleteStudent();
                break;
            case 6:
                exit(0);
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }
    return 0;
}

void addStudent() {
    Student student;
    student.getData();

    ofstream outFile("students.dat", ios::binary | ios::app);
    outFile.write(reinterpret_cast<char*>(&student), sizeof(Student));
    outFile.close();

    cout << "Student record added successfully.\n";
}

void displayAllStudents() {
    Student student;
    ifstream inFile("students.dat", ios::binary);

    if (!inFile) {
        cout << "File could not be opened! Press any key to continue...\n";
        return;
    }

    cout << "\nDisplaying all student records:\n";
    while (inFile.read(reinterpret_cast<char*>(&student), sizeof(Student))) {
        student.displayData();
        cout << "-------------------------------\n";
    }
    inFile.close();
}

void searchStudent() {
    int rollNo;
    cout << "Enter roll number to search: ";
    cin >> rollNo;

    Student student;
    ifstream inFile("students.dat", ios::binary);

    if (!inFile) {
        cout << "File could not be opened! Press any key to continue...\n";
        return;
    }

    bool found = false;
    while (inFile.read(reinterpret_cast<char*>(&student), sizeof(Student))) {
        if (student.rollNo == rollNo) {
            student.displayData();
            found = true;
            break;
        }
    }
    inFile.close();

    if (!found) {
        cout << "Student with roll number " << rollNo << " not found.\n";
    }
}

void modifyStudent() {
    int rollNo;
    cout << "Enter roll number to modify: ";
    cin >> rollNo;

    Student student;
    fstream file("students.dat", ios::binary | ios::in | ios::out);

    if (!file) {
        cout << "File could not be opened! Press any key to continue...\n";
        return;
    }

    bool found = false;
    while (file.read(reinterpret_cast<char*>(&student), sizeof(Student))) {
        if (student.rollNo == rollNo) {
            cout << "Current details are:\n";
            student.displayData();

            cout << "Enter new details:\n";
            student.getData();

            int pos = -1 * static_cast<int>(sizeof(student));
            file.seekp(pos, ios::cur);
            file.write(reinterpret_cast<char*>(&student), sizeof(Student));
            cout << "Student record updated successfully.\n";
            found = true;
            break;
        }
    }
    file.close();

    if (!found) {
        cout << "Student with roll number " << rollNo << " not found.\n";
    }
}

void deleteStudent() {
    int rollNo;
    cout << "Enter roll number to delete: ";
    cin >> rollNo;

    Student student;
    ifstream inFile("students.dat", ios::binary);
    ofstream outFile("temp.dat", ios::binary);

    if (!inFile || !outFile) {
        cout << "File could not be opened! Press any key to continue...\n";
        return;
    }

    bool found = false;
    while (inFile.read(reinterpret_cast<char*>(&student), sizeof(Student))) {
        if (student.rollNo == rollNo) {
            found = true;
            continue;
        }
        outFile.write(reinterpret_cast<char*>(&student), sizeof(Student));
    }
    inFile.close();
    outFile.close();

    if (found) {
        remove("students.dat");
        rename("temp.dat", "students.dat");
        cout << "Student record deleted successfully.\n";
    } else {
        cout << "Student with roll number " << rollNo << " not found.\n";
        remove("temp.dat");
    }
}
