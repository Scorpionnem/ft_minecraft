#include "App.hpp"

int main(void)
{
    App app;

    try
    {
        app.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}
