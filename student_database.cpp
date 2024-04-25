#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

struct Student {
    int id;
    std::string name;
    int age;
};

class StudentDatabase {
public:
    // Add a student to the database
    bool AddStudent(const Student &student) {
        if (students_.count(student.id) > 0) {
            return false; // Duplicate ID
        }
        students_[student.id] = std::make_shared<Student>(student);
        return true;
    }

    // Delete a student by ID
    bool DeleteStudent(int id) {
        auto it = students_.find(id);
        if (it == students_.end()) {
            return false; // Student not found
        }
        students_.erase(it);
        return true;
    }

    // Get student information by ID
    std::shared_ptr<Student> GetStudentInfo(int id) const {
        auto it = students_.find(id);
        if (it == students_.end()) {
            return nullptr; // Student not found
        }
        return it->second;
    }

private:
    std::unordered_map<int, std::shared_ptr<Student>> students_;
};

int main() {
    StudentDatabase db;

    Student student1{ 1, "Alice", 20 };
    Student student2{ 2, "Bob", 21 };

    db.AddStudent(student1);
    db.AddStudent(student2);

    auto student_info = db.GetStudentInfo(1);
    if (student_info) {
        std::cout << "ID: " << student_info->id << ", Name: " << student_info->name << ", Age: " << student_info->age << std::endl;
    }
    else {
        std::cout << "Student not found\n";
    }

    db.DeleteStudent(2);

    return 0;
}