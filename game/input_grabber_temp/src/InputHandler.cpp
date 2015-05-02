#include "InputHandler.hpp"

InputHandler::InputHandler()
{
    if (getuid() != 0)
    {
        std::cout << "Error: you must be root in order to access the input devices." << std::endl;
    }
}

void InputHandler::updateDeviceList()
{
    /* Capturing the stdout of a system command and returns it in a string */
    /* Source: https://www.jeremymorgan.com/tutorials/c-programming/how-to-capture-the-output-of-a-linux-command-in-c/ */

    std::string command("cat /proc/bus/input/devices | grep 'event\\|Name=\"MMM Controller'");
    std::set<int> idsOfFoundDevices;
    FILE* stream;

    int max_buffer = 1000;
    char buffer[max_buffer];

    stream = popen(command.c_str(), "r");

    if (stream)
    {
        int idOfNextControllerToSave(-1);

        while (!feof(stream))
        {
            if (fgets(buffer, max_buffer, stream) != NULL)
            {
                std::string line(buffer);

                if (idOfNextControllerToSave != -1)
                {
                    int eventStringIndex(line.find("event"));
                    int endOfEventStringIndex(line.find(" ", eventStringIndex));
                    addToDevicesIfNeeded(idOfNextControllerToSave, std::string("/dev/input/").append(line.substr(eventStringIndex, endOfEventStringIndex-eventStringIndex)));

                    idOfNextControllerToSave = -1;
                }
                else
                {
                    if (line.find("N: Name") == 0)
                    {
                        std::stringstream lineStream(line);
                        std::string id;
                        lineStream >> id;
                        lineStream >> id;
                        lineStream >> id;
                        lineStream >> id;
                        id = id.substr(0, id.size()-1);

                        idOfNextControllerToSave = std::stoi(id);
                        idsOfFoundDevices.insert(idOfNextControllerToSave);
                    }
                }

            }

        }

        //check for device removal
        if (!currentInputDevices_.size() == idsOfFoundDevices.size())
        {
            for (std::map<int, InputDevice*>::iterator it = currentInputDevices_.begin(); it != currentInputDevices_.end();)
            {
                int deviceID(it->second->getDeviceId());

                if (idsOfFoundDevices.find(deviceID) == idsOfFoundDevices.end())
                {
                    it = removeDevice(deviceID);
                    std::cout << "Removed input device " << deviceID << std::endl;
                }
                else
                {
                    ++it;
                }
            }
        }

        pclose(stream);
    }
}

void InputHandler::addToDevicesIfNeeded(int deviceID, std::string const& eventString)
{
    if (currentInputDevices_.find(deviceID) == currentInputDevices_.end())
    {
        InputDevice* newlyFoundInputDevice = new InputDevice(deviceID, eventString);
        currentInputDevices_.insert(std::make_pair(deviceID, newlyFoundInputDevice));
        std::cout << "Added an input device " << deviceID << " " << eventString << std::endl;
    }
}

std::map<int, InputDevice*>::iterator InputHandler::removeDevice(int deviceID)
{
    std::map<int, InputDevice*>::iterator iteratorToDelete = currentInputDevices_.find(deviceID);

    if (iteratorToDelete != currentInputDevices_.end())
    {
        delete iteratorToDelete->second;
        return currentInputDevices_.erase(iteratorToDelete);
    }
}

void InputHandler::processDeviceInputs()
{
    for (std::map<int, InputDevice*>::iterator it = currentInputDevices_.begin(); it != currentInputDevices_.end(); ++it)
    {
        it->second->readValuesAndReact();
    }
}