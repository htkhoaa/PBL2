#ifndef FLIGHT_MANAGER_H
#define FLIGHT_MANAGER_H

#include "Flight.h"
#include <string>

using namespace std;

class FlightManager {
private:
    Flight* currentFlight;
    const string inputFile;
    const string outputFile;

public:
    FlightManager(const string& inFile, const string& outFile);
    ~FlightManager();

    bool isFlightActive() const;
    void openFlight(const string& fCode, int maxSeats);
    void closeFlight();
    
    Flight* getCurrentFlight() const;
    void displayFinalMapping() const;
    string getOutputFile() const;
};


