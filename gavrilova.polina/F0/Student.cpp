#include "Student.hpp"
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include "Date.hpp"

double gavrilova::student::calcAverage(const gavrilova::student::Student& s)
{
  const auto& grades = s.grades_;
  if (grades.empty()) {
    return 0.0;
  }
  double total = 0.0;
  grades.traverse_lnr([&total](const std::pair< const gavrilova::date::Date, int >& p) {
    total += p.second;
  });
  return total / grades.size();
}

std::istream& gavrilova::student::operator>>(std::istream& is, Student& s)
{
  std::string fullName;
  std::string group;

  std::istream::sentry sentry(is);
  if (!sentry) {
    return is;
  }

  is >> std::quoted(fullName) >> group;
  s.fullName_ = std::move(fullName);
  s.group_ = group;

  std::string tmp;
  std::getline(is, tmp);

  is.ignore(std::numeric_limits< std::streamsize >::max(), '\n');

  size_t n = 0;
  if (!(is >> tmp >> n)) {
    return is;
  }
  is.ignore(std::numeric_limits< std::streamsize >::max(), '\n');

  map< date::Date, int > grades;
  for (size_t i = 0; i < n; ++i) {
    gavrilova::date::Date d;
    int grade;
    if (is >> d >> grade) {
      grades.insert({d, grade});
    } else {
      is.setstate(std::ios::failbit);
      break;
    }
  }

  if (grades.size() == n) {
    is.clear();
    is.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
  s.grades_ = std::move(grades);

  return is;
}

std::ostream& gavrilova::student::operator<<(std::ostream& os, const Student& s)
{
  std::ostream::sentry sentry(os);
  if (!sentry) {
    return os;
  }
  os << std::quoted(s.fullName_) << " " << s.group_ << "\n";
  os << "Средний балл: " << calcAverage(s) << "\n";
  os << "Оценки " << s.grades_.size() << "\n";
  s.grades_.traverse_lnr([&os](const std::pair< const gavrilova::date::Date, int >& p) {
    os << p.first << " " << p.second << " ";
  });
  os << "\n";

  return os;
}
