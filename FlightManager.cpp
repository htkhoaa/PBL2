#include "FlightManager.h"
#include <iostream>
#include <fstream>
using namespace std;

FlightManager::FlightManager(const string& inFile, const string& outFile)
    : currentFlight(nullptr), inputFile(inFile), outputFile(outFile) {
}

FlightManager::~FlightManager() {
    if (currentFlight != nullptr) {
        delete currentFlight;
    }
}

bool FlightManager::isFlightActive() const {
    return currentFlight != nullptr;
}

void FlightManager::openFlight(const string& fCode, int maxSeats) {
    if (isFlightActive()) {
        cout << "A flight check-in is already in progress. Please 'Close' it first.\n";
        return;
    }
    
    currentFlight = new Flight(fCode, maxSeats);
    currentFlight->loadInitialSeats(inputFile);
    currentFlight->displayFlightInfo();     
}

void FlightManager::closeFlight() {
    if (!isFlightActive()) {
        cout << "No flight is active right now. Please 'Open' a flight first.\n";
        return;
    }
    cout << "\nClosing check-in for flight " << currentFlight->getFlightCode() << "...\n";
    displayFinalMapping();
    
    delete currentFlight;
    currentFlight = nullptr;
}

Flight* FlightManager::getCurrentFlight() const {
    return currentFlight;
}

string FlightManager::getOutputFile() const {
    return outputFile;
}

void FlightManager::displayFinalMapping() const {
    ifstream inFile(outputFile);
    if (!inFile) {
        cerr << "Error: Cannot open booking output file (" << outputFile << ").\n";
        return;
    }

    cout << "\n==================================================\n";
    cout << " FINAL BOOKING MAPPING (Content of " << outputFile << ")\n";
    cout << "==================================================\n";
    string line;
    while (getline(inFile, line)) {
        cout << " " << line << "\n";
    }
    cout << "==================================================\n";
    inFile.close();
}
