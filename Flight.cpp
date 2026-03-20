#include "Flight.h"
#include <iostream>
#include <fstream>

using namespace std;

Flight::Flight(const string& code, int seats) 
    : flightCode(code), maxSeats(seats), availableSeats(seats), bookingCount(0) {
    
    seatStatus = new int[maxSeats];
    ticketCodes = new string[maxSeats];
    allocatedSeats = new int[maxSeats];

    for (int i = 0; i < maxSeats; ++i) {
        seatStatus[i] = 0; 
    }
}

Flight::~Flight() {
    delete[] seatStatus;
    delete[] ticketCodes;
    delete[] allocatedSeats;
}

bool Flight::loadInitialSeats(const string& filename) {
    ifstream inFile(filename);
    if (!inFile) {
        cerr << "Warning: Cannot open " << filename << ". Assuming all seats are empty." << endl;
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
    cout << "\n==================================================\n";
    cout << " Flight Code    : " << flightCode << "\n";
    cout << " Total Seats    : " << maxSeats << "\n";
    cout << " Available Seats: " << availableSeats << "\n";
    cout << "==================================================\n";
}

int Flight::getAvailableSeats() const {
    return availableSeats;
}

string Flight::getFlightCode() const {
    return flightCode;
}

void Flight::bookSeats(int count, const string& outputFilename) {
    if (count <= 0) {
        cout << "Error: Number of seats must be greater than 0.\n";
        return;
    }
    if (count > availableSeats) {
        cout << "Error: Not enough available seats. Only " << availableSeats << " seats left.\n";
        return;
    }

    string* tempTickets = new string[count];
    int* tempSeats = new int[count];

    for (int i = 0; i < count; ++i) {
        cout << " Enter Ticket Code for seat " << (i + 1) << " of " << count << ": ";
        cin >> tempTickets[i];
    }

    int foundCount = 0;
    for (int i = 0; i < maxSeats && foundCount < count; ++i) {
        if (seatStatus[i] == 0) {
            tempSeats[foundCount] = i + 1; 
            foundCount++;
        }
    }

    cout << " -> Found available seats: ";
    for (int i = 0; i < count; ++i) {
        cout << tempSeats[i] << (i < count - 1 ? ", " : "");
    }
    cout << "\n";

    cout << " Confirm selection of these seats? (Y/N): ";
    string confirm;
    cin >> confirm;

    if (confirm == "Y" || confirm == "y") {
        ofstream outFile(outputFilename, ios::app);
        if (!outFile) {
            cerr << "Error: Cannot open " << outputFilename << " for writing.\n";
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
            cout << " Success! Booking confirmed and mapped.\n";
        }
    } else {
        cout << " Booking cancelled by staff.\n";
    }

    delete[] tempTickets;
    delete[] tempSeats;
}
