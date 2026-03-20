#include "Application.h"
#include <iostream>

using namespace std;

Application::Application() : flightManager(nullptr), isRunning(false) {
}

Application::~Application() {
    if (flightManager != nullptr) {
        delete flightManager;
    }
}

void Application::startup() {
    cout << "==================================================\n";
    cout << "      ITF-Airway Flight Seat Booking System       \n";
    cout << "==================================================\n";
    
    const string INPUT_FILE = "seats_input.txt";
    const string OUTPUT_FILE = "booking_output.txt";
    flightManager = new FlightManager(INPUT_FILE, OUTPUT_FILE);
    isRunning = true;
}

void Application::run() {
    while (isRunning) {
        cout << "\n> Enter action code ('Open', 'Close', or 'Exit' to quit): ";
        string action;
        cin >> action;

        if (action == "Exit" || action == "exit") {
            isRunning = false;
        } 
        else if (action == "Open" || action == "open") {
            processOpenCommand();
        } 
        else if (action == "Close" || action == "close") {
            processCloseCommand();
        } 
        else {
            cout << "Unknown action code. Valid commands: Open, Close, Exit.\n";
        }
    }
}

void Application::shutdown() {
    if (flightManager != nullptr && flightManager->isFlightActive()) {
        flightManager->closeFlight();
    }
    cout << "==================================================\n";
    cout << "             System safely terminated.            \n";
    cout << "==================================================\n";
}

void Application::processOpenCommand() {
    if (flightManager->isFlightActive()) {
        cout << "A flight check-in is already in progress. Please 'Close' it first.\n";
        return;
    }

    cout << "> Enter Flight Code: ";
    string fCode;
    cin >> fCode;

    flightManager->openFlight(fCode, MAX_SEATS);
    processBookingLoop();
}

void Application::processBookingLoop() {
    Flight* currentFlight = flightManager->getCurrentFlight();
    string outputFile = flightManager->getOutputFile();
    
    while (flightManager->isFlightActive()) {
        if (currentFlight->getAvailableSeats() == 0) {
            cout << "\n[!] The flight is currently FULL. Further bookings disabled.\n";
            cout << "> Type 'Close' to end check-in: ";
        } else {
            cout << "\n> Enter number of seats to book (or type 'Close' to finish): ";
        }
        
        string input;
        cin >> input;

        if (input == "Close" || input == "close") {
            flightManager->closeFlight();
            break;
        }

        bool isNumber = true;
        for (size_t i = 0; i < input.length(); ++i) {
            if (input[i] < '0' || input[i] > '9') {
                isNumber = false;
                break;
            }
        }

        if (!isNumber || input.empty()) {
            cout << "Invalid input. Please enter a positive integer or 'Close'.\n";
            continue;
        }

        int seatsToBook = stoi(input);
        currentFlight->bookSeats(seatsToBook, outputFile);
    }
}

void Application::processCloseCommand() {
    if (!flightManager->isFlightActive()) {
        handleInactiveClose();
    } else {
        cout << "Please type 'Close' during the booking prompt sequence.\n";
    }
}

void Application::handleInactiveClose() {
    cout << "No flight is active right now. Please 'Open' a flight first.\n";
    cout << "Would you like to review the output mappings? (Y/N): ";
    string viewOutput;
    cin >> viewOutput;
    if (viewOutput == "Y" || viewOutput == "y") {
        flightManager->displayFinalMapping();
    }
}
