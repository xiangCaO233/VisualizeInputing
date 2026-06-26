#include "app/Application.h"

#include <exception>
#include <iostream>

int main()
{
    try {
        vi::Application app;
        return app.run();
    } catch ( const std::exception& error ) {
        std::cerr << "visualizeinput failed: " << error.what() << '\n';
        return 1;
    }
}
