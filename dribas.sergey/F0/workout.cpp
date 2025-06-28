#include "workout.hpp"
#include <iostream>
#include <cmath>
#include <numbers>
#include <iomanip>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/optional.hpp>
#include <boost/algorithm/string.hpp>

#include "streamGuard.hpp"

namespace
{
  double toRad(double degree)
  {
    return degree * std::acos(-1.0) / 180.0;
  }

  double calculateDistance(double lat1, double lon1, double lat2, double lon2)
  {
    const double kEarthRadiusKm = 6371.0;
    double dLat = toRad(lat2 - lat1);
    double dLon = toRad(lon2 - lon1);
    double sinDLat2 = std::sin(dLat / 2);
    double sinDLon2 = std::sin(dLon / 2);
    double cosLat1 = std::cos(toRad(lat1));
    double cosLat2 = std::cos(toRad(lat2));
    double a = sinDLat2 * sinDLat2;
    a += cosLat1 * cosLat2 * sinDLon2 * sinDLon2;
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    return kEarthRadiusKm * c;
  }
}

namespace dribas
{
  workout parseGpx(std::istream& gpxStream)
  {
    workout result;
    boost::property_tree::ptree pt;
    boost::property_tree::read_xml(gpxStream, pt);

    result.name = pt.get< std::string >("gpx.trk.name", "Unknown workout");

    long long sumHeart = 0;
    int countHeart = 0;
    int maxHeart = 0;

    long long sumCadence = 0;
    int countCadence = 0;

    boost::posix_time::ptime startTimePt;
    boost::posix_time::ptime endTimePt;
    bool firstPoint = true;
    double prevLat = 0.0;
    double prevLon = 0.0;

    for (const auto& trkptV : pt.get_child("gpx.trk.trkseg")) {
      if (trkptV.first != "trkpt") {
        continue;
      }
      std::string timeStr = trkptV.second.get< std::string >("time");
      boost::algorithm::trim_if(timeStr, boost::is_any_of(".Z"));
      boost::replace_all(timeStr, "T", " ");
      boost::posix_time::ptime currentTimestamp = boost::posix_time::time_from_string(timeStr);
      if (firstPoint) {
        startTimePt = currentTimestamp;
      }
      endTimePt = currentTimestamp;

      double currentLat = trkptV.second.get<double>("<xmlattr>.lat");
      double currentLon = trkptV.second.get<double>("<xmlattr>.lon");

      if (!firstPoint) {
        result.distance += calculateDistance(prevLat, prevLon, currentLat, currentLon);
      } else {
        firstPoint = false;
      }
      prevLat = currentLat;
      prevLon = currentLon;

      if (auto hrStr = trkptV.second.get_optional<std::string>("extensions.ns3:TrackPointExtension.ns3:hr")) {
        int hr = std::stoi(*hrStr);
        sumHeart += hr;
        countHeart++;
        if (hr > maxHeart) maxHeart = hr;
      }

      if (auto cadStr = trkptV.second.get_optional<std::string>("extensions.ns3:TrackPointExtension.ns3:cad")) {
        int cad = std::stoi(*cadStr);
        sumCadence += cad;
        countCadence++;
      }
    }

    if (countHeart > 0) {
      result.avgHeart = static_cast< int >(sumHeart / countHeart);
      result.maxHeart = maxHeart;
    }

    if (countCadence > 0) {
      result.cadence = static_cast< int >(sumCadence / countCadence);
    }

    const boost::posix_time::ptime epoch(boost::gregorian::date(1970, 1, 1));
    result.timeStart = (startTimePt - epoch).total_seconds();
    result.timeEnd = (endTimePt - epoch).total_seconds();

    if (result.distance > 0 && result.timeEnd > result.timeStart) {
      double durationMinutes = static_cast< double >(result.timeEnd - result.timeStart) / 60.0;
      result.avgPaceMinPerKm = durationMinutes / result.distance;
    }

    return result;
  }

  std::istream& operator>>(std::istream& is, workout& w)
  {
    dribas::StreamGuard guard(is);
    is.unsetf(std::ios_base::skipws);
    w = parseGpx(is);
    return is;
  }

  std::ostream& operator<<(std::ostream& os, const workout& w)
  {
    dribas::StreamGuard guard(os);

    boost::posix_time::ptime startTime = boost::posix_time::from_time_t(w.timeStart);
    boost::posix_time::ptime endTime = boost::posix_time::from_time_t(w.timeEnd);

    long long duration = w.timeEnd - w.timeStart;
    long long hours = duration / 3600;
    long long minutes = (duration % 3600) / 60;
    long long seconds = duration % 60;

    int paceMin = static_cast< int >(w.avgPaceMinPerKm);
    int paceSec = static_cast< int >((w.avgPaceMinPerKm - paceMin) * 60);

    os << "Workout name: " << w.name << '\n';
    os << "Average heart rate: " << w.avgHeart << '\n';
    os << "Maximum heart rate: " << w.maxHeart << '\n';
    os << "Average cadence: " << w.cadence << '\n';
    os << "Distance: " << std::fixed << std::setprecision(2) << w.distance << " km\n";
    os << "Average pace: " << paceMin << ":" ;
    os << (paceSec < 10 ? "0" : "") << paceSec << " min/km\n";
    os << "Start time: " << boost::posix_time::to_simple_string(startTime) << '\n';
    os << "End time: " << boost::posix_time::to_simple_string(endTime) << '\n';
    os << "Duration: " << hours << "h " << minutes << "m " << seconds << "s\n";
    return os;
  }
}
