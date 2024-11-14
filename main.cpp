#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
using namespace std;
class IVehicle
{
public:
  virtual bool drive(const int kilometers) = 0;
  virtual void refuel() = 0;
  virtual void printStatus() const = 0;
};
class AbstractCar : public IVehicle
{
protected:
  AbstractCar() : m_fuelConsumptionLitresPerKm(0.0), m_fuelLevel(0.0), m_fuelLevelMax(0.0), m_milageKm(0)
  {}
public:
  bool drive(const int kilometers) override
  {
    const double requiredFuelLitres = ((double)kilometers) / 100.0 * m_fuelConsumptionLitresPerKm;
    if (requiredFuelLitres > m_fuelLevel)
    {
      cout << "Not enough fuel to drive " << kilometers << " km" << endl;
      return false;
    }
    cout << m_name << ": Drive " << kilometers << " km" << endl;
    m_milageKm += kilometers;
    m_fuelLevel -= requiredFuelLitres;
    return true;
  }
  void refuel() override
  {
    cout << "Refuel " << m_name << endl;
    m_fuelLevel = m_fuelLevelMax;
  }
  void printStatus() const override
  {
    cout << m_name << " status: Mileage(km)=" << m_milageKm << ", Fuel level(l)=" << m_fuelLevel << endl;
  }
protected:
  double m_fuelConsumptionLitresPerKm;
  double m_fuelLevel;
  double m_fuelLevelMax;
  int m_milageKm;
  string m_name;
};
class Sedan : public AbstractCar
{
public:
  Sedan()
  {
    m_fuelConsumptionLitresPerKm = 7.0;
    m_fuelLevelMax = 50.0;
    m_name = "Sedan";
  }
};
class Suv : public AbstractCar
{
public:
  Suv()
  {
    m_fuelConsumptionLitresPerKm = 11.0;
    m_fuelLevelMax = 80.0;
    m_name = "Suv";
  }
};
class Bus : public AbstractCar
{
public:
  Bus()
  {
    m_fuelConsumptionLitresPerKm = 25.0;
    m_fuelLevelMax = 120.0;
    m_name = "Bus";
  }
};
class Bicycle : public IVehicle
{
public:
  Bicycle() : m_milageKm(0), m_name("Bicycle") {}
  bool drive(const int kilometers) override
  {
    m_milageKm += kilometers;
    return true;
  }
  void refuel() override {}
  void printStatus() const override
  {
    cout << m_name << " status: Mileage(km)=" << m_milageKm << endl;
  }
private:
  int m_milageKm;
  string m_name;
};
struct RoutePoint
{
  int xKm, yKm;
  string name;

  RoutePoint(const int xKm, const int yKm, const string& name) : xKm(xKm), yKm(yKm), name(name) {}
};
class Route
{
public:
  Route() {}
  void addPoint(const RoutePoint& point)
  {
    m_path.push_back(point);
  }
  void run(IVehicle* vehicle)
  {
    if (m_path.size() < 2)
    {
      cout << "Path is incomplete" << endl;
      return;
    }
    vehicle->printStatus();
    for (size_t i = 1, t = m_path.size(); i < t; ++i)
    {
      const RoutePoint& prevPoint = m_path[i - 1];
      const RoutePoint& currPoint = m_path[i];
      const int xDistance = currPoint.xKm - prevPoint.xKm;
      const int yDistance = currPoint.yKm - prevPoint.yKm;
      const int distance = (int)sqrt(xDistance * xDistance + yDistance * yDistance);

      for (int j = 0; j < 2; ++j) {
        if (!vehicle->drive(distance)) {
          if (j == 0)
          {
            vehicle->refuel();
            continue;
          }
          else
          {
            cout << "Unable to drive the rout" << endl;
            return;
          }
        }
        vehicle->printStatus();
        break;
      }
    }
  }

private:
  vector<RoutePoint> m_path;
};
int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    cout << "Not enough arguments. Specify a file path with the route description" << endl;
    return -1;
  }
  char* routeFilePath = argv[1];
  ifstream fileStream(routeFilePath);

  if (!fileStream.is_open())
  {
    cout << "Unable to open the specified file: " << routeFilePath << endl;
    return -1;
  }
  Route route;
  while (!fileStream.eof())
  {
    int x = 0, y = 0;
    string name;
    fileStream >> x >> y >> name;
    route.addPoint(RoutePoint(x, y, name));
  }
  IVehicle* vehicles[4] = {new Sedan, new Suv, new Bus, new Bicycle};
  for (int i = 0; i < 4; ++i)
  {
    IVehicle* vehicle = vehicles[i];
    route.run(vehicle);
    cout << endl;
    delete vehicles[i];
  }
  return 0;
}
