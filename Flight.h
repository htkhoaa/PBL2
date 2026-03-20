#include <string>
using namespace std;

class Flight {
private:
    string flightCode;
    int maxSeats;
    int availableSeats;
    
    int* seatStatus; 
    
    string* ticketCodes; 
    int* allocatedSeats;
    int bookingCount;

public:
    Flight(const string& code, int seats);
    ~Flight();

    bool loadInitialSeats(const string& filename);
    void displayFlightInfo() const;
    void bookSeats(int count, const string& outputFilename);
    int getAvailableSeats() const;
    string getFlightCode() const;
};
