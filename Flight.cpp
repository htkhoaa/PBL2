#include "Flight.h"
#include <iostream>
#include <fstream>

Flight::Flight(const std::string& code, int seats) 
    : flightCode(code), maxSeats(seats), availableSeats(seats), bookingCount(0) {
    
    seatStatus = new int[maxSeats];
    ticketCodes = new std::string[maxSeats];
    allocatedSeats = new int[maxSeats];

    for (int i = 0; i < maxSeats; ++i) {
        seatStatus[i] = 0; // 0 = empty
    }
}

Flight::~Flight() {
    delete[] seatStatus;
    delete[] ticketCodes;
    delete[] allocatedSeats;
}

bool Flight::loadInitialSeats(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Warning: Cannot open " << filename << ". Assuming all seats are empty." << std::endl;
        return false;
    }

    int takenCount = 0;
    for (int i = 0; i < maxSeats; ++i) {
        if (!(inFile >> seatStatus[i])) {
            break; 
        }
        if (seatStatus[i] == 1) { 
            takenCount++;
        }
    }
    availableSeats = maxSeats - takenCount;
    inFile.close();
    return true;
}

void Flight::displayFlightInfo() const {
    std::cout << "\n==================================================\n";
    std::cout << " Flight Code    : " << flightCode << "\n";
    std::cout << " Total Seats    : " << maxSeats << "\n";
    std::cout << " Available Seats: " << availableSeats << "\n";
    std::cout << "==================================================\n";
}

int Flight::getAvailableSeats() const {
    return availableSeats;
}

std::string Flight::getFlightCode() const {
    return flightCode;
}

void Flight::bookSeats(int count, const std::string& outputFilename) {
    if (count <= 0) {
        std::cout << "Error: Number of seats must be greater than 0.\n";
        return;
    }
    if (count > availableSeats) {
        std::cout << "Error: Not enough available seats. Only " << availableSeats << " seats left.\n";
        return;
    }

    std::string* tempTickets = new std::string[count];
    int* tempSeats = new int[count];

    for (int i = 0; i < count; ++i) {
        std::cout << " Enter Ticket Code for seat " << (i + 1) << " of " << count << ": ";
        std::cin >> tempTickets[i];
    }

    int foundCount = 0;
    for (int i = 0; i < maxSeats && foundCount < count; ++i) {
        if (seatStatus[i] == 0) {
            tempSeats[foundCount] = i + 1; 
            foundCount++;
        }
    }

    std::cout << " -> Found available seats: ";
    for (int i = 0; i < count; ++i) {
        std::cout << tempSeats[i] << (i < count - 1 ? ", " : "");
    }
    std::cout << "\n";

    std::cout << " Confirm selection of these seats? (Y/N): ";
    std::string confirm;
    std::cin >> confirm;

    if (confirm == "Y" || confirm == "y") {
        std::ofstream outFile(outputFilename, std::ios::app);
        if (!outFile) {
            std::cerr << "Error: Cannot open " << outputFilename << " for writing.\n";
        } else {
            for (int i = 0; i < count; ++i) {
                int seatIdx = tempSeats[i] - 1; 
                seatStatus[seatIdx] = 1; 
                
                ticketCodes[bookingCount] = tempTickets[i];
                allocatedSeats[bookingCount] = tempSeats[i];
                bookingCount++;
                availableSeats--;

                outFile << flightCode << " | " << tempTickets[i] << " | " << tempSeats[i] << "\n";
            }
            outFile.close();
            std::cout << " Success! Booking confirmed and mapped.\n";
        }
    } else {
        std::cout << " Booking cancelled by staff.\n";
    }

    delete[] tempTickets;
    delete[] tempSeats;
}
