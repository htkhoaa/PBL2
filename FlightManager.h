#ifndef FLIGHT_MANAGER_H
#define FLIGHT_MANAGER_H

#include "Flight.h"
#include <string>

class FlightManager {
private:
    Flight* currentFlight;
    const std::string inputFile;
    const std::string outputFile;

public:
    FlightManager(const std::string& inFile, const std::string& outFile);
    ~FlightManager();

    bool isFlightActive() const;
    void openFlight(const std::string& fCode, int maxSeats);
    void closeFlight();
    
    Flight* getCurrentFlight() const;
    void displayFinalMapping() const;
    std::string getOutputFile() const;
};

#endif // FLIGHT_MANAGER_H
