#ifndef FLIGHT_H
#define FLIGHT_H

#include <string>

class Flight {
private:
    std::string flightCode;
    int maxSeats;
    int availableSeats;
    
    // Array representing physical seats. 0 = empty, 1 = taken. 
    int* seatStatus; 
    
    // Parallel arrays for bookings in memory.
    std::string* ticketCodes; 
    int* allocatedSeats;
    int bookingCount;

public:
    Flight(const std::string& code, int seats);
    ~Flight();

    bool loadInitialSeats(const std::string& filename);
    void displayFlightInfo() const;
    void bookSeats(int count, const std::string& outputFilename);
    int getAvailableSeats() const;
    std::string getFlightCode() const;
};

#endif // FLIGHT_H
