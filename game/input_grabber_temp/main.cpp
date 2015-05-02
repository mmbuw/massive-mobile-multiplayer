#include "InputHandler.hpp"

int main()
{
    // MMM Input device recognizer and grabber
    InputHandler inputHandler;

    while (true)
    {
        inputHandler.updateDeviceList();
    }

    return 0;
}