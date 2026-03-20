#include "Application.h"

int main() {
    Application app;
    app.startup();
    app.run();
    app.shutdown();
    return 0;
}
