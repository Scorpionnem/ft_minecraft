#include "App.hpp"

int main(int ac, char **av)
{
    App app;

    if (ac != 2)
    	return (1);

    try
    {
        app.run(av[1]);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}
