#include "CommandProcessor.hpp"
#include <iomanip>
#include <limits>
#include <stdexcept>
#include <string>
#include "Date.hpp"
#include "Student.hpp"

namespace {
  struct StudentPrinter {
    std::ostream& out;
    void operator()(const gavrilova::SharedPtr< const gavrilova::student::Student >& s) const
    {
      out << "ID: " << s->id_ << ", ФИО: " << std::quoted(s->fullName_)
          << ", Группа: " << s->group_ << ", Средний балл: " << std::fixed << std::setprecision(2) << s->averageGrade_ << '\n';
    }
  };
  struct GradePrinter {
    std::ostream& out;
    void operator()(const std::pair< const gavrilova::date::Date, int >& grade) const
    {
      out << "  " << gavrilova::date::to_string(grade.first) << ": " << grade.second << '\n';
    }
  };
}

gavrilova::CommandProcessor::CommandProcessor(StudentDatabase& db):
  db_(db)
{
  using namespace std::placeholders;
  commandMap_.insert({"--help", std::bind(&CommandProcessor::handleHelp, this)});
  commandMap_.insert({"addstudent", std::bind(&CommandProcessor::handleAddStudent, this)});
  commandMap_.insert({"deletestudent", std::bind(&CommandProcessor::handleDeleteStudent, this)});
  commandMap_.insert({"infostudent", std::bind(&CommandProcessor::handleInfoStudent, this)});
  commandMap_.insert({"listgroup", std::bind(&CommandProcessor::handleListGroup, this)});
  commandMap_.insert({"listgrades", std::bind(&CommandProcessor::handleListGrades, this)});
  commandMap_.insert({"movegroup", std::bind(&CommandProcessor::handleMoveGroup, this)});
  commandMap_.insert({"addgrade", std::bind(&CommandProcessor::handleAddGrade, this)});
  commandMap_.insert({"changegrade", std::bind(&CommandProcessor::handleChangeGrade, this)});
  commandMap_.insert({"removegrade", std::bind(&CommandProcessor::handleRemoveGrade, this)});
  commandMap_.insert({"topstudentsall", std::bind(&CommandProcessor::handleTopStudentsAll, this)});
  commandMap_.insert({"riskstudentsall", std::bind(&CommandProcessor::handleRiskStudentsAll, this)});
  commandMap_.insert({"topstudentsgroup", std::bind(&CommandProcessor::handleTopStudentsGroup, this)});
  commandMap_.insert({"riskstudentsgroup", std::bind(&CommandProcessor::handleRiskStudentsGroup, this)});
  commandMap_.insert({"exportforgrades", std::bind(&CommandProcessor::handleExportForGrades, this)});
  commandMap_.insert({"loadgrades", std::bind(&CommandProcessor::handleLoadGrades, this)});
  commandMap_.insert({"groupstats", std::bind(&CommandProcessor::handleGroupStats, this)});
  commandMap_.insert({"avgmarkbydate", std::bind(&CommandProcessor::handleAvgMarkByDate, this)});
  commandMap_.insert({"clear", std::bind(&CommandProcessor::handleClear, this)});
  commandMap_.insert({"create_group", std::bind(&CommandProcessor::handleCreateGroup, this)});
  commandMap_.insert({"save", std::bind(&CommandProcessor::handleSave, this)});
  commandMap_.insert({"loadbase", std::bind(&CommandProcessor::handleLoad, this)});
}

void gavrilova::CommandProcessor::run()
{
  std::string command;
  while (std::cout << "> " && std::cin >> command) {
    auto it = commandMap_.find(command);
    if (it != commandMap_.end()) {
      try {
        it->second();
      } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        std::cout << "<INVALID COMMAND>\n";
      }
    } else {
      std::cout << "<INVALID COMMAND>\n";
    }
    if (std::cin.peek() != '\n' && std::cin.peek() != EOF) {
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}

void gavrilova::CommandProcessor::handleHelp()
{
  std::cout << "Список доступных команд:\n"
            << "--help\n"
            << "addstudent <\"ФИО\"> <группа>\n"
            << "deletestudent <ID>\n"
            << "infostudent <ID | \"ФИО\">\n"
            << "listgroup <группа>\n"
            << "listgrades <ID>\n"
            << "movegroup <ID> <новая_группа>\n"
            << "addgrade <ID> <оценка> <ДД-ММ-ГГГГ>\n"
            << "changegrade <ID> <оценка> <ДД-ММ-ГГГГ>\n"
            << "removegrade <ID> <ДД-ММ-ГГГГ>\n"
            << "topstudentsall <N>\n"
            << "riskstudentsall <порог>\n"
            << "topstudentsgroup <группа> <N>\n"
            << "riskstudentsgroup <группа> <порог>\n"
            << "exportforgrades <группа> <файл>\n"
            << "loadgrades <группа> <ДД-ММ-ГГГГ> <файл>\n"
            << "groupstats <группа> <старт_ДД-ММ-ГГГГ> <конец_ДД-ММ-ГГГГ>\n"
            << "avgmarkbydate <ДД-ММ-ГГГГ>\n"
            << "create_group <группа>\n"
            << "save <файл>\n"
            << "loadbase <файл>\n"
            << "clear\n";
}

void gavrilova::CommandProcessor::handleAddStudent()
{
  std::string fullName, groupName;
  std::cin >> std::quoted(fullName) >> groupName;
  auto result = db_.addStudent(fullName, groupName);
  if (result.first) {
    std::cout << "Студент добавлен с ID: " << result.second << '\n';
  } else {
    std::cout << "<INVALID COMMAND>\n";
  }
}

void gavrilova::CommandProcessor::handleDeleteStudent()
{
  StudentID id;
  std::cin >> id;
  if (std::cin.fail() || !db_.deleteStudent(id)) {
    std::cout << "<INVALID COMMAND>\n";
    std::cin.clear();
  } else {
    std::cout << "Студент с ID " << id << " удален.\n";
  }
}

void gavrilova::CommandProcessor::handleInfoStudent()
{
  std::string token;
  std::cin >> std::ws;
  std::getline(std::cin, token);

  StudentID id = 0;
  bool is_id = true;
  for (char c: token) {
    if (!std::isdigit(c)) {
      is_id = false;
      break;
    }
  }

  if (is_id) {
    id = std::stoul(token);
    auto student = db_.findStudentById(id);
    if (student) {
      StudentPrinter{std::cout}(student);
    } else {
      std::cout << "<INVALID COMMAND>\n";
    }
  } else {
    token.erase(0, token.find_first_not_of("\""));
    token.erase(token.find_last_not_of("\"") + 1);
    auto ids = db_.findStudentsByName(token);
    if (ids.empty()) {
      std::cout << "<INVALID COMMAND>\n";
    } else if (ids.size() == 1) {
      StudentPrinter{std::cout}(db_.findStudentById(ids.front()));
    } else {
      std::cout << "Найдено несколько студентов. Введите ID одного из них:\n";
      for (auto student_id: ids) {
        std::cout << student_id << " ";
      }
      std::cout << '\n';
    }
  }
}

void gavrilova::CommandProcessor::handleListGroup()
{
  std::string groupName;
  std::cin >> groupName;
  auto students = db_.getStudentsInGroup(groupName);
  if (students.empty() && !db_.groupExists(groupName)) {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  std::cout << "Студенты группы " << groupName << ":\n";
  for (const auto& s: students) {
    StudentPrinter{std::cout}(s);
  }
}

void gavrilova::CommandProcessor::handleListGrades()
{
  StudentID id;
  std::cin >> id;
  if (std::cin.fail()) {
    std::cout << "<INVALID COMMAND>\n";
    std::cin.clear();
    return;
  }
  auto student = db_.findStudentById(id);
  if (!student) {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  std::cout << "Оценки студента ID " << id << ":\n";
  student->grades_.traverse_lnr(GradePrinter{std::cout});
}

void gavrilova::CommandProcessor::handleMoveGroup()
{
  StudentID id;
  std::string newGroup;
  std::cin >> id >> newGroup;
  if (std::cin.fail() || !db_.moveStudentToGroup(id, newGroup)) {
    std::cout << "<INVALID COMMAND>\n";
    std::cin.clear();
  } else {
    std::cout << "Студент с ID " << id << " переведен в группу " << newGroup << ".\n";
  }
}

void gavrilova::CommandProcessor::handleAddGrade()
{
  StudentID id;
  int grade;
  date::Date date;
  std::cin >> id >> grade >> date;
  if (std::cin.fail() || !db_.addGrade(id, grade, date)) {
    std::cout << "<INVALID COMMAND>\n";
    std::cin.clear();
  } else {
    std::cout << "Оценка добавлена.\n";
  }
}

void gavrilova::CommandProcessor::handleChangeGrade()
{
  StudentID id;
  int grade;
  date::Date date;
  std::cin >> id >> grade >> date;
  if (std::cin.fail() || !db_.changeGrade(id, grade, date)) {
    std::cout << "<INVALID COMMAND>\n";
    std::cin.clear();
  } else {
    std::cout << "Оценка изменена.\n";
  }
}

void gavrilova::CommandProcessor::handleRemoveGrade()
{
  StudentID id;
  date::Date date;
  std::cin >> id >> date;
  if (std::cin.fail() || !db_.removeGradesByDate(id, date)) {
    std::cout << "<INVALID COMMAND>\n";
    std::cin.clear();
  } else {
    std::cout << "Оценка удалена.\n";
  }
}

void gavrilova::CommandProcessor::handleTopStudentsAll()
{
  size_t n;
  std::cin >> n;
  if (std::cin.fail() || n == 0) {
    std::cout << "<INVALID COMMAND>\n";
    std::cin.clear();
    return;
  }
  auto students = db_.getTopStudents(n);
  std::cout << "Топ-" << n << " студентов по среднему баллу:\n";
  for (const auto& s: students) {
    StudentPrinter{std::cout}(s);
  }
}

void gavrilova::CommandProcessor::handleRiskStudentsAll()
{
  double threshold;
  std::cin >> threshold;
  if (std::cin.fail() || threshold <= 0.0) {
    std::cout << "<INVALID COMMAND>\n";
    std::cin.clear();
    return;
  }
  auto students = db_.getRiskStudents(threshold);
  std::cout << "Студенты со средним баллом ниже " << threshold << ":\n";
  for (const auto& s: students) {
    StudentPrinter{std::cout}(s);
  }
}

void gavrilova::CommandProcessor::handleTopStudentsGroup()
{
  std::string groupName;
  size_t n;
  std::cin >> groupName >> n;
  if (std::cin.fail() || n == 0 || !db_.groupExists(groupName)) {
    std::cout << "<INVALID COMMAND>\n";
    std::cin.clear();
    return;
  }
  auto students = db_.getTopStudentsInGroup(groupName, n);
  std::cout << "Топ-" << n << " студентов группы " << groupName << ":\n";
  for (const auto& s: students) {
    StudentPrinter{std::cout}(s);
  }
}

void gavrilova::CommandProcessor::handleRiskStudentsGroup()
{
  std::string groupName;
  double threshold;
  std::cin >> groupName >> threshold;
  if (std::cin.fail() || threshold <= 0.0 || !db_.groupExists(groupName)) {
    std::cout << "<INVALID COMMAND>\n";
    std::cin.clear();
    return;
  }
  auto students = db_.getRiskStudentsInGroup(groupName, threshold);
  std::cout << "Студенты группы " << groupName << " со средним баллом ниже " << threshold << ":\n";
  for (const auto& s: students) {
    StudentPrinter{std::cout}(s);
  }
}

void gavrilova::CommandProcessor::handleExportForGrades()
{
  std::string groupName, filename;
  std::cin >> groupName >> filename;
  if (db_.exportGroupForGrading(groupName, filename)) {
    std::cout << "Шаблон для группы " << groupName << " экспортирован в " << filename << '\n';
  } else {
    std::cout << "<INVALID COMMAND>\n";
  }
}

void gavrilova::CommandProcessor::handleLoadGrades()
{
  std::string groupName, filename;
  date::Date date;
  std::cin >> groupName >> date >> filename;
  if (std::cin.fail()) {
    std::cout << "<INVALID COMMAND>\n";
    std::cin.clear();
    return;
  }
  if (db_.loadGradesFromFile(groupName, date, filename)) {
    std::cout << "Оценки для группы " << groupName << " из файла " << filename << " загружены.\n";
  } else {
    std::cout << "<INVALID COMMAND>\n";
  }
}

void gavrilova::CommandProcessor::handleGroupStats()
{
  std::string groupName;
  DateRange period;
  std::cin >> groupName >> period.start >> period.end;
  if (std::cin.fail()) {
    std::cout << "<INVALID COMMAND>\n";
    std::cin.clear();
    return;
  }
  auto result = db_.getGroupStatistics(groupName, period);
  if (!result.first) {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  auto stats = result.second;
  std::cout << "Статистика по группе " << groupName << " за период с " << date::to_string(period.start) << " по "
            << date::to_string(period.end) << ":\n";
  std::cout << " - Средний балл по группе: " << std::fixed << std::setprecision(2) << stats.groupAverage << '\n';
  std::cout << " - Средний балл по остальным группам: " << stats.allOtherGroupsAverage << '\n';
  std::cout << " - Распределение оценок:\n";
  struct GradeDistributionPrinter {
    std::ostream& out;
    void operator()(const std::pair< const int, int >& p) { out << "    Оценка " << p.first << ": " << p.second << " шт.\n"; }
  };
  stats.gradeDistribution.traverse_lnr(GradeDistributionPrinter{std::cout});

  std::cout << " - Лучшие студенты:\n";
  for (const auto& s: stats.topStudents) {
    StudentPrinter{std::cout}(s);
  }

  std::cout << " - Худшие студенты:\n";
  for (const auto& s: stats.bottomStudents) {
    StudentPrinter{std::cout}(s);
  }
}

void gavrilova::CommandProcessor::handleAvgMarkByDate()
{
  date::Date date;
  std::cin >> date;
  if (std::cin.fail()) {
    std::cout << "<INVALID COMMAND>\n";
    std::cin.clear();
    return;
  }
  auto result = db_.getAverageGradeByDate(date);
  if (result.first) {
    std::cout << "Средний балл за " << date::to_string(date) << ": " << std::fixed << std::setprecision(2) << result.second
              << '\n';
  } else {
    std::cout << "Нет данных за указанную дату.\n";
  }
}

void gavrilova::CommandProcessor::handleClear()
{
  char response;
  std::cout << "Вы уверены, что хотите очистить всю базу данных? (y/n): ";
  std::cin >> response;
  if (response == 'y' || response == 'Y') {
    db_.clear();
    std::cout << "База данных очищена.\n";
  } else {
    std::cout << "Отмена.\n";
  }
}

void gavrilova::CommandProcessor::handleCreateGroup()
{
  std::string groupName;
  std::cin >> groupName;
  if (db_.createGroup(groupName)) {
    std::cout << "Группа " << groupName << " создана.\n";
  } else {
    std::cout << "Группа " << groupName << " уже существует.\n";
  }
}

void gavrilova::CommandProcessor::handleSave()
{
  std::string filename;
  std::cin >> filename;
  if (db_.saveToFile(filename)) {
    std::cout << "База данных сохранена в файл " << filename << '\n';
  } else {
    std::cout << "Ошибка сохранения файла.\n";
  }
}

void gavrilova::CommandProcessor::handleLoad()
{
  std::string filename;
  std::cin >> filename;
  if (db_.loadFromFile(filename)) {
    std::cout << "База данных загружена из файла " << filename << '\n';
  } else {
    std::cout << "Ошибка загрузки файла.\n";
  }
}
