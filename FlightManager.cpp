#include "FlightManager.h"
#include <iostream>
#include <fstream>

FlightManager::FlightManager(const std::string& inFile, const std::string& outFile)
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

void FlightManager::openFlight(const std::string& fCode, int maxSeats) {
    if (isFlightActive()) {
        std::cout << "A flight check-in is already in progress. Please 'Close' it first.\n";
        return;
    }
    
    currentFlight = new Flight(fCode, maxSeats);
    currentFlight->loadInitialSeats(inputFile);
    currentFlight->displayFlightInfo();
}

void FlightManager::closeFlight() {
    if (!isFlightActive()) {
        std::cout << "No flight is active right now. Please 'Open' a flight first.\n";
        return;
    }
    std::cout << "\nClosing check-in for flight " << currentFlight->getFlightCode() << "...\n";
    displayFinalMapping();
    
    delete currentFlight;
    currentFlight = nullptr;
}

Flight* FlightManager::getCurrentFlight() const {
    return currentFlight;
}

std::string FlightManager::getOutputFile() const {
    return outputFile;
}

void FlightManager::displayFinalMapping() const {
    std::ifstream inFile(outputFile);
    if (!inFile) {
        std::cerr << "Error: Cannot open booking output file (" << outputFile << ").\n";
        return;
    }

    std::cout << "\n==================================================\n";
    std::cout << " FINAL BOOKING MAPPING (Content of " << outputFile << ")\n";
    std::cout << "==================================================\n";
    std::string line;
    while (std::getline(inFile, line)) {
        std::cout << " " << line << "\n";
    }
    std::cout << "==================================================\n";
    inFile.close();
}
