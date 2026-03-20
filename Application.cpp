#include "Application.h"
#include <iostream>

Application::Application() : flightManager(nullptr), isRunning(false) {
}

Application::~Application() {
    if (flightManager != nullptr) {
        delete flightManager;
    }
}

void Application::startup() {
    std::cout << "==================================================\n";
    std::cout << "      ITF-Airway Flight Seat Booking System       \n";
    std::cout << "==================================================\n";
    
    const std::string INPUT_FILE = "seats_input.txt";
    const std::string OUTPUT_FILE = "booking_output.txt";
    flightManager = new FlightManager(INPUT_FILE, OUTPUT_FILE);
    isRunning = true;
}

void Application::run() {
    while (isRunning) {
        std::cout << "\n> Enter action code ('Open', 'Close', or 'Exit' to quit): ";
        std::string action;
        std::cin >> action;

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
            std::cout << "Unknown action code. Valid commands: Open, Close, Exit.\n";
        }
    }
}

void Application::shutdown() {
    if (flightManager != nullptr && flightManager->isFlightActive()) {
        flightManager->closeFlight();
    }
    std::cout << "==================================================\n";
    std::cout << "             System safely terminated.            \n";
    std::cout << "==================================================\n";
}

void Application::processOpenCommand() {
    if (flightManager->isFlightActive()) {
        std::cout << "A flight check-in is already in progress. Please 'Close' it first.\n";
        return;
    }

    std::cout << "> Enter Flight Code: ";
    std::string fCode;
    std::cin >> fCode;

    flightManager->openFlight(fCode, MAX_SEATS);
    processBookingLoop();
}

void Application::processBookingLoop() {
    Flight* currentFlight = flightManager->getCurrentFlight();
    std::string outputFile = flightManager->getOutputFile();
    
    while (flightManager->isFlightActive()) {
        if (currentFlight->getAvailableSeats() == 0) {
            std::cout << "\n[!] The flight is currently FULL. Further bookings disabled.\n";
            std::cout << "> Type 'Close' to end check-in: ";
        } else {
            std::cout << "\n> Enter number of seats to book (or type 'Close' to finish): ";
        }
        
        std::string input;
        std::cin >> input;

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
            std::cout << "Invalid input. Please enter a positive integer or 'Close'.\n";
            continue;
        }

        int seatsToBook = std::stoi(input);
        currentFlight->bookSeats(seatsToBook, outputFile);
    }
}

void Application::processCloseCommand() {
    if (!flightManager->isFlightActive()) {
        handleInactiveClose();
    } else {
        std::cout << "Please type 'Close' during the booking prompt sequence.\n";
    }
}

void Application::handleInactiveClose() {
    std::cout << "No flight is active right now. Please 'Open' a flight first.\n";
    std::cout << "Would you like to review the output mappings? (Y/N): ";
    std::string viewOutput;
    std::cin >> viewOutput;
    if (viewOutput == "Y" || viewOutput == "y") {
        flightManager->displayFinalMapping();
    }
}
