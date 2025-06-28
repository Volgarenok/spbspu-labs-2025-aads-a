
#include "StudentDatabase.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace {
  template < typename T, typename Compare >
  void sortList(gavrilova::FwdList< T >& list, Compare comp)
  {
    if (list.size() < 2) {
      return;
    }

    gavrilova::FwdList< T > sortedList;
    while (!list.empty()) {
      T value = list.front();
      list.pop_front();

      if (sortedList.empty() || !comp(sortedList.front(), value)) {
        sortedList.push_front(value);
      } else {
        auto it_prev = sortedList.cbegin();
        auto it_curr = it_prev;
        ++it_curr;
        while (it_curr != sortedList.cend() && comp(*it_curr, value)) {
          ++it_prev;
          ++it_curr;
        }
        sortedList.insert(it_prev, value);
      }
    }
    list.swap(sortedList);
  }
}

gavrilova::StudentDatabase::StudentDatabase(int id_digits)
{
  nextId = static_cast< unsigned long >(std::pow(10, id_digits - 1)) + 1;
}

bool gavrilova::StudentDatabase::saveToFile(const std::string& filename) const
{
  std::ofstream out(filename);
  if (!out) {
    return false;
  }
  struct StudentWriter {
    std::ostream& out;
    void operator()(const std::pair< const StudentID, SharedPtr< student::Student > >& p) const
    {
      out << *p.second << '\n';
    }
  };
  students.traverse_lnr(StudentWriter{out});
  return true;
}

bool gavrilova::StudentDatabase::loadFromFile(const std::string& filename)
{
  std::ifstream in(filename);
  if (!in) {
    return false;
  }
  student::Student temp_student;
  while (in >> temp_student) {
    if (!groupExists(temp_student.group_)) {
      createGroup(temp_student.group_);
    }
    auto pair = addStudent(temp_student.fullName_, temp_student.group_);
    if (pair.first) {
      auto sp = findStudentById(pair.second);
      if (sp) {
        sp->grades_.swap(temp_student.grades_);
        updateStudentAverageGrade(sp);
      }
    }
    temp_student = student::Student();
  }
  return true;
}

void gavrilova::StudentDatabase::clear()
{
  students.clear();
  groups.clear();
  nameToStudentIndex.clear();
  dateToGradesIndex.clear();
}

bool gavrilova::StudentDatabase::createGroup(const std::string& groupName)
{
  if (groupExists(groupName)) {
    return false;
  }
  return groups.insert({groupName, Group{}}).second;
}

bool gavrilova::StudentDatabase::groupExists(const std::string& groupName) const
{
  return groups.find(groupName) != groups.end();
}

std::pair< bool, gavrilova::StudentID > gavrilova::StudentDatabase::addStudent(const std::string& fullName, const std::string& groupName)
{
  if (!groupExists(groupName)) {
    return {false, 0};
  }
  auto it_name = nameToStudentIndex.find(fullName);
  if (it_name != nameToStudentIndex.end()) {
    struct StudentExistsInGroupChecker {
      const StudentDatabase* db;
      const std::string& group;
      bool& exists;
      void operator()(const std::pair< const StudentID, StudentID >& id_pair) const
      {
        if (exists) return;
        auto student_it = db->students.find(id_pair.first);
        if (student_it != db->students.end() && student_it->second->group_ == group) {
          exists = true;
        }
      }
    };
    bool student_exists_in_group = false;
    it_name->second.traverse_lnr(StudentExistsInGroupChecker{this, groupName, student_exists_in_group});
    if (student_exists_in_group) {
      return {false, 0};
    }
  }

  auto student = gavrilova::make_shared< student::Student >(nextId, fullName, groupName);
  students.insert({nextId, student});
  groups.at(groupName).insert({nextId, student});

  auto name_set_it = nameToStudentIndex.find(fullName);
  if (name_set_it == nameToStudentIndex.end()) {
    gavrilova::set< StudentID > new_set;
    new_set.insert({nextId, nextId});
    nameToStudentIndex.insert({fullName, new_set});
  } else {
    name_set_it->second.insert({nextId, nextId});
  }
  return {true, nextId++};
}

bool gavrilova::StudentDatabase::deleteStudent(StudentID id)
{
  auto it_student = students.find(id);
  if (it_student == students.end()) {
    return false;
  }
  const auto& student = it_student->second;

  struct GradeRemoverFromIndex {
    StudentDatabase* db;
    StudentID studentId;
    void operator()(const std::pair< const date::Date, int >& gradeEntry) const
    {
      auto it_date = db->dateToGradesIndex.find(gradeEntry.first);
      if (it_date != db->dateToGradesIndex.end()) {
        struct IsStudentGrade {
          StudentID id_to_remove;
          bool operator()(const std::pair< StudentID, int >& p) const { return p.first == id_to_remove; }
        };
        it_date->second.remove_if(IsStudentGrade{studentId});
        if (it_date->second.empty()) {
          db->dateToGradesIndex.erase(gradeEntry.first);
        }
      }
    }
  };
  student->grades_.traverse_lnr(GradeRemoverFromIndex{this, id});

  groups.at(student->group_).erase(id);

  auto& name_set = nameToStudentIndex.at(student->fullName_);
  name_set.erase(id);
  if (name_set.empty()) {
    nameToStudentIndex.erase(student->fullName_);
  }
  students.erase(it_student);
  return true;
}

bool gavrilova::StudentDatabase::moveStudentToGroup(StudentID id, const std::string& newGroupName)
{
  auto student_ptr = findStudentById(id);
  if (!student_ptr || !groupExists(newGroupName) || student_ptr->group_ == newGroupName) {
    return false;
  }
  groups.at(student_ptr->group_).erase(id);
  groups.at(newGroupName).insert({id, student_ptr});
  student_ptr->group_ = newGroupName;
  return true;
}

gavrilova::SharedPtr< const gavrilova::student::Student > gavrilova::StudentDatabase::findStudentById(StudentID id) const
{
  auto it = students.find(id);
  if (it != students.end()) {
    return it->second;
  }
  return nullptr;
}

gavrilova::SharedPtr< gavrilova::student::Student > gavrilova::StudentDatabase::findStudentById(StudentID id)
{
  auto it = students.find(id);
  return (it != students.end()) ? it->second : nullptr;
}

gavrilova::FwdList< gavrilova::StudentID > gavrilova::StudentDatabase::findStudentsByName(const std::string& fullName) const
{
  FwdList< StudentID > result;
  auto it = nameToStudentIndex.find(fullName);
  if (it == nameToStudentIndex.end()) {
    return result;
  }
  struct IdCollector {
    FwdList< StudentID >& list;
    void operator()(const std::pair< const StudentID, StudentID >& p) const { list.push_front(p.first); }
  };
  it->second.traverse_rnl(IdCollector{result});
  return result;
}

bool gavrilova::StudentDatabase::addGrade(StudentID id, int grade, const date::Date& date)
{
  auto student_ptr = findStudentById(id);
  if (!student_ptr || student_ptr->grades_.find(date) != student_ptr->grades_.end()) {
    return false;
  }
  student_ptr->grades_.insert({date, grade});
  auto it_date = dateToGradesIndex.find(date);
  if (it_date == dateToGradesIndex.end()) {
    FwdList< std::pair< StudentID, int > > newList;
    newList.push_front({id, grade});
    dateToGradesIndex.insert({date, newList});
  } else {
    it_date->second.push_front({id, grade});
  }
  updateStudentAverageGrade(student_ptr);
  return true;
}

bool gavrilova::StudentDatabase::changeGrade(StudentID id, int newGrade, const date::Date& date)
{
  auto student_ptr = findStudentById(id);
  if (!student_ptr || student_ptr->grades_.find(date) == student_ptr->grades_.end()) {
    return false;
  }
  student_ptr->grades_.at(date) = newGrade;
  auto it_date = dateToGradesIndex.find(date);
  if (it_date != dateToGradesIndex.end()) {
    for (auto it = (it_date->second).begin(); it != (it_date->second).end(); ++it) {
      if (it->first == id) {
        it->second = newGrade;
        break;
      }
    }
  }
  updateStudentAverageGrade(student_ptr);
  return true;
}

bool gavrilova::StudentDatabase::removeGradesByDate(StudentID id, const date::Date& date)
{
  auto student_ptr = findStudentById(id);
  if (!student_ptr || student_ptr->grades_.find(date) == student_ptr->grades_.end()) {
    return false;
  }
  student_ptr->grades_.erase(date);
  auto it_date = dateToGradesIndex.find(date);
  if (it_date != dateToGradesIndex.end()) {
    struct IsStudentGrade {
      StudentID id_to_remove;
      bool operator()(const std::pair< StudentID, int >& p) const { return p.first == id_to_remove; }
    };
    it_date->second.remove_if(IsStudentGrade{id});
    if (it_date->second.empty()) {
      dateToGradesIndex.erase(it_date);
    }
  }
  updateStudentAverageGrade(student_ptr);
  return true;
}

void gavrilova::StudentDatabase::updateStudentAverageGrade(SharedPtr< student::Student >& student)
{
  if (student->grades_.empty()) {
    student->averageGrade_ = 0.0;
    return;
  }
  struct SumAccumulator {
    double& sum;
    void operator()(const std::pair< const date::Date, int >& grade_pair) const { sum += grade_pair.second; }
  };
  double sum = 0.0;
  student->grades_.traverse_lnr(SumAccumulator{sum});
  student->averageGrade_ = sum / student->grades_.size();
}

gavrilova::FwdList< gavrilova::SharedPtr< const gavrilova::student::Student > >
gavrilova::StudentDatabase::getStudentsInGroup(const std::string& groupName) const
{
  FwdList< SharedPtr< const student::Student > > result;
  auto it = groups.find(groupName);
  if (it == groups.end()) {
    return result;
  }

  struct StudentCollector {
    FwdList< SharedPtr< const student::Student > >& list;
    void operator()(const std::pair< const StudentID, SharedPtr< student::Student > >& p) const
    {
      list.push_front(p.second);
    }
  };
  
  it->second.traverse_rnl(StudentCollector{result});
  return result;
}

bool gavrilova::StudentDatabase::exportGroupForGrading(const std::string& groupName, const std::string& filename) const
{
  auto it = groups.find(groupName);
  if (it == groups.end()) {
    return false;
  }
  std::ofstream out(filename);
  if (!out) {
    return false;
  }
  out << "#GROUP:" << groupName << '\n';
  struct GroupExporter {
    std::ostream& out;
    void operator()(const std::pair< const StudentID, SharedPtr< student::Student > >& p) const
    {
      const auto& student = p.second;
      out << "ID:" << student->id_ << '\n';
      out << "ФИО:" << student->fullName_ << '\n';
      out << "Оценка:\n";
    }
  };
  it->second.traverse_lnr(GroupExporter{out});
  return true;
}

bool gavrilova::StudentDatabase::loadGradesFromFile(const std::string& groupName, const date::Date& date, const std::string& filename)
{
  if (!groupExists(groupName)) {
    return false;
  }
  std::ifstream in(filename);
  if (!in) {
    return false;
  }
  std::string line;
  StudentID currentId = 0;
  while (std::getline(in, line)) {
    if (line.rfind("ID:", 0) == 0) {
      currentId = std::stoul(line.substr(3));
    } else if (line.rfind("Оценка:", 0) == 0 && currentId != 0) {
      std::string value = line.substr(7);
      value.erase(0, value.find_first_not_of(" \t\n\r"));
      bool has_digit = false;
      for (char c: value) {
        if (::isdigit(c)) {
          has_digit = true;
          break;
        }
      }
      if (!value.empty() && has_digit) {
        int grade = std::stoi(value);
        addGrade(currentId, grade, date);
      }
    }
  }
  return true;
}

std::pair< bool, gavrilova::GroupStatistics >
gavrilova::StudentDatabase::getGroupStatistics(const std::string& groupName, const DateRange& period) const
{
  auto it = groups.find(groupName);
  if (it == groups.end()) return {false, {}};

  GroupStatistics stats;
  double groupGradesSum = 0;
  int groupGradesCount = 0;
  auto groupStudentsList = getStudentsInGroup(groupName);

  struct GroupStatsGradeCollector {
    const DateRange& period;
    GroupStatistics& stats;
    double& sum;
    int& count;
    void operator()(const SharedPtr< const student::Student >& student) const
    {
      struct GradeProcessor {
        const DateRange& p;
        GroupStatistics& s;
        double& sm;
        int& ct;
        void operator()(const std::pair< const date::Date, int >& grade) const
        {
          bool inPeriod = !(grade.first < p.start) && !(p.end < grade.first);
          if (inPeriod) {
            sm += grade.second;
            ct++;
            auto dist_it = s.gradeDistribution.find(grade.second);
            if (dist_it == s.gradeDistribution.end()) {
              s.gradeDistribution.insert({grade.second, 1});
            } else {
              dist_it->second++;
            }
          }
        }
      };
      student->grades_.traverse_lnr(GradeProcessor{period, stats, sum, count});
    }
  };
  for (const auto& student: groupStudentsList) {
    GroupStatsGradeCollector{period, stats, groupGradesSum, groupGradesCount}(student);
  }

  if (groupGradesCount > 0) {
    stats.groupAverage = groupGradesSum / groupGradesCount;
  }

  struct CompareTop {
    bool operator()(const SharedPtr< const student::Student >& a, const SharedPtr< const student::Student >& b) const
    {
      if (a->averageGrade_ == b->averageGrade_) return a->id_ < b->id_;
      return a->averageGrade_ > b->averageGrade_;
    }
  };
  struct CompareBottom {
    bool operator()(const SharedPtr< const student::Student >& a, const SharedPtr< const student::Student >& b) const
    {
      if (a->averageGrade_ == b->averageGrade_) return a->id_ < b->id_;
      return a->averageGrade_ < b->averageGrade_;
    }
  };

  FwdList< SharedPtr< const student::Student > > topList = groupStudentsList;
  sortList(topList, CompareTop{});
  auto it_top = topList.cbegin();
  for (size_t i = 0; i < 3 && it_top != topList.cend(); ++i, ++it_top) {
    stats.topStudents.push_front(*it_top);
  }
  stats.topStudents.reverse();

  FwdList< SharedPtr< const student::Student > > bottomList = groupStudentsList;
  sortList(bottomList, CompareBottom{});
  auto it_bottom = bottomList.cbegin();
  for (size_t i = 0; i < 3 && it_bottom != bottomList.cend(); ++i, ++it_bottom) {
    stats.bottomStudents.push_front(*it_bottom);
  }
  stats.bottomStudents.reverse();

  double otherGradesSum = 0;
  int otherGradesCount = 0;
  struct OtherGroupsCollector {
    const std::string& currentGroup;
    const DateRange& period;
    double& sum;
    int& count;
    void operator()(const std::pair< const std::string, Group >& group_pair) const
    {
      if (group_pair.first == currentGroup) return;
      struct StudentGradeCollector {
        const DateRange& p;
        double& s;
        int& c;
        void operator()(const std::pair< const StudentID, SharedPtr< student::Student > >& student_pair) const
        {
          struct GradeExtractor {
            const DateRange& period;
            double& sum;
            int& count;
            void operator()(const std::pair< const date::Date, int >& grade_pair) const
            {
              bool inPeriod = !(grade_pair.first < period.start) && !(period.end < grade_pair.first);
              if (inPeriod) {
                sum += grade_pair.second;
                count++;
              }
            }
          };
          student_pair.second->grades_.traverse_lnr(GradeExtractor{p, s, c});
        }
      };
      group_pair.second.traverse_lnr(StudentGradeCollector{period, sum, count});
    }
  };
  groups.traverse_lnr(OtherGroupsCollector{groupName, period, otherGradesSum, otherGradesCount});

  if (otherGradesCount > 0) {
    stats.allOtherGroupsAverage = otherGradesSum / otherGradesCount;
  }

  return {true, stats};
}

gavrilova::FwdList< gavrilova::SharedPtr< const gavrilova::student::Student > >
gavrilova::StudentDatabase::getTopStudents(size_t n) const
{
  FwdList< SharedPtr< const student::Student > > allStudents;

  struct StudentCollector {
    FwdList< SharedPtr< const student::Student > >& list;
    void operator()(const std::pair< const StudentID, SharedPtr< student::Student > >& p) const
    {
      list.push_front(p.second);
    }
  };
  students.traverse_lnr(StudentCollector{allStudents});

  struct CompareTop {
    bool operator()(const SharedPtr< const student::Student >& a, const SharedPtr< const student::Student >& b) const
    {
      if (a->averageGrade_ == b->averageGrade_) return a->id_ < b->id_;
      return a->averageGrade_ > b->averageGrade_;
    }
  };
  sortList(allStudents, CompareTop{});

  FwdList< SharedPtr< const student::Student > > topN;
  auto it = allStudents.cbegin();
  for (size_t i = 0; i < n && it != allStudents.cend(); ++i, ++it) {
    topN.push_front(*it);
  }
  topN.reverse();
  return topN;
}

gavrilova::FwdList< gavrilova::SharedPtr< const gavrilova::student::Student > >
gavrilova::StudentDatabase::getRiskStudents(double threshold) const
{
  FwdList< SharedPtr< const student::Student > > result;

  struct RiskStudentCollector {
    FwdList< SharedPtr< const student::Student > >& list;
    double threshold;
    void operator()(const std::pair< const StudentID, SharedPtr< student::Student > >& p) const
    {
      if (!p.second->grades_.empty() && p.second->averageGrade_ < threshold) {
        list.push_front(p.second);
      }
    }
  };
  students.traverse_rnl(RiskStudentCollector{result, threshold});
  return result;
}

gavrilova::FwdList< gavrilova::SharedPtr< const gavrilova::student::Student > >
gavrilova::StudentDatabase::getTopStudentsInGroup(const std::string& groupName, size_t n) const
{
  FwdList< SharedPtr< const student::Student > > studentsInGroup = getStudentsInGroup(groupName);
  struct CompareTop {
    bool operator()(const SharedPtr< const student::Student >& a, const SharedPtr< const student::Student >& b) const
    {
      if (a->averageGrade_ == b->averageGrade_) return a->id_ < b->id_;
      return a->averageGrade_ > b->averageGrade_;
    }
  };
  sortList(studentsInGroup, CompareTop{});
  FwdList< SharedPtr< const student::Student > > topN;
  auto it = studentsInGroup.cbegin();
  for (size_t i = 0; i < n && it != studentsInGroup.cend(); ++i, ++it) {
    topN.push_front(*it);
  }
  topN.reverse();
  return topN;
}

gavrilova::FwdList< gavrilova::SharedPtr< const gavrilova::student::Student > >
gavrilova::StudentDatabase::getRiskStudentsInGroup(const std::string& groupName, double threshold) const
{
  FwdList< SharedPtr< const student::Student > > studentsInGroup = getStudentsInGroup(groupName);
  FwdList< SharedPtr< const student::Student > > result;
  struct RiskStudentInGroupCollector {
    FwdList< SharedPtr< const student::Student > >& list;
    double threshold;
    void operator()(const SharedPtr< const student::Student >& s) const
    {
      if (!s->grades_.empty() && s->averageGrade_ < threshold) {
        list.push_front(s);
      }
    }
  };
  for (const auto& s: studentsInGroup) {
    RiskStudentInGroupCollector{result, threshold}(s);
  }
  result.reverse();
  return result;
}

std::pair< bool, double > gavrilova::StudentDatabase::getAverageGradeByDate(const date::Date& date) const
{
  auto it = dateToGradesIndex.find(date);
  if (it == dateToGradesIndex.end() || it->second.empty()) {
    return {false, 0.0};
  }
  const auto& grades = it->second;
  double sum = 0.0;
  for (const auto& p: grades) {
    sum += p.second;
  }
  return {true, sum / grades.size()};
}
