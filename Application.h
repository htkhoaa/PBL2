#ifndef APPLICATION_H
#define APPLICATION_H

#include "FlightManager.h"
#include <string>

class Application {
private:
    FlightManager* flightManager;
    bool isRunning;
    const int MAX_SEATS = 50;
    
    void processOpenCommand();
    void processCloseCommand();
    void processBookingLoop();
    void handleInactiveClose();

public:
    Application();
    ~Application();

    void startup();
    void run();
    void shutdown();
};

#endif // APPLICATION_H
