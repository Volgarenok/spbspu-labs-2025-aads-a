#ifndef STUDENT_DATABASE_HPP
#define STUDENT_DATABASE_HPP

#include <string>
#include "Date.hpp"
#include "Student.hpp"
#include "Containers.hpp"
#include "SharedPointer.hpp"

namespace gavrilova {

  struct DateRange {
    date::Date start;
    date::Date end;
  };

  struct GroupStatistics {
    map< int, int > gradeDistribution;
    FwdList< SharedPtr< const student::Student > > topStudents;
    FwdList< SharedPtr< const student::Student > > bottomStudents;
    double groupAverage = 0.0;
    double allOtherGroupsAverage = 0.0;
  };

  class StudentDatabase {
  public:
    using Group = map< StudentID, SharedPtr< student::Student > >;

    explicit StudentDatabase(int id_digits = 4);

    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
    void clear();

    bool createGroup(const std::string& groupName);
    bool groupExists(const std::string& groupName) const;

    std::pair< bool, StudentID > addStudent(const std::string& fullName, const std::string& groupName);
    bool deleteStudent(StudentID id);
    bool moveStudentToGroup(StudentID id, const std::string& newGroupName);

    SharedPtr< const student::Student > findStudentById(StudentID id) const;
    FwdList< StudentID > findStudentsByName(const std::string& fullName) const;

    bool addGrade(StudentID id, int grade, const date::Date& date);
    bool changeGrade(StudentID id, int newGrade, const date::Date& date);
    bool removeGradesByDate(StudentID id, const date::Date& date);

    FwdList< SharedPtr< const student::Student > > getStudentsInGroup(const std::string& groupName) const;
    FwdList< SharedPtr< const student::Student > > getTopStudents(size_t n) const;
    FwdList< SharedPtr< const student::Student > > getRiskStudents(double threshold) const;
    FwdList< SharedPtr< const student::Student > > getTopStudentsInGroup(const std::string& groupName, size_t n) const;
    FwdList< SharedPtr< const student::Student > > getRiskStudentsInGroup(const std::string& groupName, double threshold) const;
    std::pair< bool, double > getAverageGradeByDate(const date::Date& date) const;

    bool loadGradesFromFile(const std::string& groupName, const date::Date& date, const std::string& filename);
    bool exportGroupForGrading(const std::string& groupName, const std::string& filename) const;
    std::pair< bool, GroupStatistics > getGroupStatistics(const std::string& groupName, const DateRange& period) const;

    SharedPtr< student::Student > findStudentById(StudentID id);
    void updateStudentAverageGrade(SharedPtr< student::Student >& student);

  private:
    map< StudentID, SharedPtr< student::Student > > students;
    map< std::string, Group > groups;
    map< std::string, set< StudentID > > nameToStudentIndex;
    map< date::Date, FwdList< std::pair< StudentID, int > > > dateToGradesIndex;
    StudentID nextId;
  };
}

#endif
