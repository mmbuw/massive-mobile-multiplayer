#include "InputHandler.hpp"

InputHandler::InputHandler(Game* gameToHandle) : gameToHandle_(gameToHandle), updateDeviceListThreadRunning_(true)
{
    if (getuid() != 0)
    {
        std::cout << "Error: you must be root in order to access the input devices." << std::endl;
        exit(EXIT_FAILURE);
    }

    updateDeviceListThread = std::thread(&InputHandler::updateDeviceList, this);
}

InputHandler::~InputHandler()
{
    updateDeviceListThreadRunning_ = false;
    updateDeviceListThread.join();
}

void InputHandler::updateDeviceList()
{
    while (updateDeviceListThreadRunning_)
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
            std::string nameOfNextControllerToSave("");

            while (!feof(stream))
            {
                if (fgets(buffer, max_buffer, stream) != NULL)
                {
                    std::string line(buffer);

                    if (idOfNextControllerToSave != -1)
                    {
                        int eventStringIndex(line.find("event"));
                        int endOfEventStringIndex(line.find(" ", eventStringIndex));

                        //only add devices which haven't been captured yet
                        if (currentInputDevices_.find(idOfNextControllerToSave) == currentInputDevices_.end())
                        {
                            vectorAccessMutex.lock();
                            if (devicesToAdd_.find(idOfNextControllerToSave) == devicesToAdd_.end())
                            {
                                AddDeviceQuery adq;
                                adq.deviceID = idOfNextControllerToSave;
                                adq.name = nameOfNextControllerToSave;
                                adq.eventString = std::string("/dev/input/").append(line.substr(eventStringIndex, endOfEventStringIndex-eventStringIndex));
                                
                                devicesToAdd_.insert(std::make_pair(idOfNextControllerToSave, adq));
                            }
                            vectorAccessMutex.unlock();
                        }

                        idOfNextControllerToSave = -1;
                        nameOfNextControllerToSave = "";
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

                            idOfNextControllerToSave = std::stoi(id);
                            idsOfFoundDevices.insert(idOfNextControllerToSave);

                            int openBracesIndex(line.find("("));
                            int closeBracesIndex(line.find(")"));
                            nameOfNextControllerToSave = line.substr(openBracesIndex+1, closeBracesIndex-openBracesIndex-1);
                        }
                    }

                }

            }

            //check for device removal
            if (!currentInputDevices_.size() == idsOfFoundDevices.size())
            {
                for (std::map<int, InputDevice*>::iterator it = currentInputDevices_.begin(); it != currentInputDevices_.end(); ++it)
                {
                    int deviceID(it->second->getDeviceId());

                    if (idsOfFoundDevices.find(deviceID) == idsOfFoundDevices.end())
                    {
                        vectorAccessMutex.lock();
                        if (devicesToRemove_.find(deviceID) == devicesToRemove_.end())
                        {
                            devicesToRemove_.insert(deviceID);
                        }
                        vectorAccessMutex.unlock();
                    }

                }
            }

            pclose(stream);
        }

    }
}

void InputHandler::addToDevices(int deviceID, std::string const& name, std::string const& eventString)
{
    Player* playerFigure = gameToHandle_->addNewPlayer(name, rand()%100);
    InputDevice* newlyFoundInputDevice = new InputDevice(deviceID, name, eventString, playerFigure);
    currentInputDevices_.insert(std::make_pair(deviceID, newlyFoundInputDevice));
    std::cout << "Added an input device with ID " << deviceID << " called " << name << " on " << eventString << std::endl;
}

std::map<int, InputDevice*>::iterator InputHandler::removeDevice(int deviceID)
{
    std::map<int, InputDevice*>::iterator iteratorToDelete = currentInputDevices_.find(deviceID);

    if (iteratorToDelete != currentInputDevices_.end())
    {
        //remove player figure
        gameToHandle_->removePlayer(iteratorToDelete->second->getPlayerInstance());
        
        //remove device
        delete iteratorToDelete->second;
        std::cout << "Removed input device " << deviceID << std::endl;
        return currentInputDevices_.erase(iteratorToDelete);
    }
}

void InputHandler::processAddRemoveQueries()
{
    vectorAccessMutex.lock();

    for (std::map<int, AddDeviceQuery>::iterator it = devicesToAdd_.begin(); it != devicesToAdd_.end(); ++it)
    {
        addToDevices(it->second.deviceID, it->second.name, it->second.eventString);
    }

    for (std::set<int>::iterator it = devicesToRemove_.begin(); it != devicesToRemove_.end(); ++it)
    {
        removeDevice(*it);
    }

    devicesToAdd_.clear();
    devicesToRemove_.clear();

    vectorAccessMutex.unlock();
}

void InputHandler::processDeviceInputs()
{
    for (std::map<int, InputDevice*>::iterator it = currentInputDevices_.begin(); it != currentInputDevices_.end(); ++it)
    {
        it->second->readValuesAndReact();
    }
}