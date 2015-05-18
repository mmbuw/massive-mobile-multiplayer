#include "InputHandler.hpp"

InputHandler::InputHandler(Game* gameToHandle) : gameToHandle_(gameToHandle), updateDeviceListThreadRunning_(true), inputValueThreadRunning_(true)
{
    if (getuid() != 0)
    {
        std::cout << "Error: you must be root in order to access the input devices." << std::endl;
        exit(EXIT_FAILURE);
    }

    updateDeviceListThread_ = std::thread(&InputHandler::updateDeviceList, this);
    inputValueThread_ = std::thread(&InputHandler::retrieveInputs, this);
}

InputHandler::~InputHandler()
{
    updateDeviceListThreadRunning_ = false;
    inputValueThreadRunning_ = false;
    updateDeviceListThread_.join();
    inputValueThread_.join();
}

//function of updateDeviceListThread_
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
                        currentInputDevicesMutex_.lock();
                        //devicesToAddMutex_.lock();

                        if (currentInputDevices_.find(idOfNextControllerToSave) == currentInputDevices_.end() &&
                            devicesToAdd_.find(idOfNextControllerToSave) == devicesToAdd_.end())
                        {
                            currentInputDevicesMutex_.unlock();
                            //devicesToAddMutex_.unlock();

                            AddDeviceQuery adq;
                            adq.deviceID = idOfNextControllerToSave;
                            adq.name = nameOfNextControllerToSave;
                            adq.eventString = std::string("/dev/input/").append(line.substr(eventStringIndex, endOfEventStringIndex-eventStringIndex));
                            
                            devicesToAddMutex_.lock();
                            devicesToAdd_.insert(std::make_pair(idOfNextControllerToSave, adq));
                            devicesToAddMutex_.unlock();
                        }
                        else
                        {
                            currentInputDevicesMutex_.unlock();
                            //devicesToAddMutex_.unlock();
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
            if (currentInputDevices_.size() != idsOfFoundDevices.size())
            {
                for (std::map<int, InputDevice*>::iterator it = currentInputDevices_.begin(); it != currentInputDevices_.end(); ++it)
                {
                    int deviceID(it->second->getDeviceId());

                    if (idsOfFoundDevices.find(deviceID) == idsOfFoundDevices.end())
                    {
                        devicesToRemoveMutex_.lock();
                        if (devicesToRemove_.find(deviceID) == devicesToRemove_.end())
                        {
                            devicesToRemove_.insert(deviceID);
                        }
                        devicesToRemoveMutex_.unlock();
                    }

                }
            }

            pclose(stream);
        }

    }
}

//function of inputValueThread_
void InputHandler::retrieveInputs()
{
    fd_set rfds;
    struct timeval tv;
    int retval;

    while (inputValueThreadRunning_)
    {
        //Clear the file descriptor set
        FD_ZERO(&rfds);
        int maxDescriptor = -1;
        std::unordered_map<int, InputDevice*> fileDescriptors;

        //Iterate over every InputDevice and add file descriptor to selector
        currentInputDevicesMutex_.lock();
        for (std::map<int, InputDevice*>::iterator it = currentInputDevices_.begin(); it != currentInputDevices_.end(); ++it)
        {
            int fileHandle(it->second->getDeviceFileHandle());
            FD_SET(fileHandle, &rfds);
            fileDescriptors[fileHandle] = it->second;

            if (fileHandle > maxDescriptor)
                maxDescriptor = fileHandle;

        }
        currentInputDevicesMutex_.unlock();

        //Set Timeout to 1 second (due to possibly joining and leaving players)
        tv.tv_sec = 1;
        tv.tv_usec = 0;

        //Blocking call: waits until at least one descriptor is ready to read
        retval = select(maxDescriptor+1, &rfds, NULL, NULL, &tv); // first parameter is highest descriptor number +1

        //If no timeout was triggered
        if (retval)
        {
            //Iterate over the range of file descriptors and check if they are ready to read
            for (int i = 1; i < maxDescriptor+1; ++i)
            {
                if (FD_ISSET(i, &rfds))
                {
                    InputDevice* inputDevice(fileDescriptors[i]);
                    Player* playerFigure(inputDevice->getPlayerInstance());

                    //read event from device
                    struct input_event ev;
                    size_t read_result = read(i, &ev, sizeof(ev));

                    //key events
                    if (ev.type == EV_KEY && ev.value >= 0 && ev.value <= 2 && (int) ev.code == 304)
                    {
                        inputDevice->setButtonA(true);
                    }
                    //relative events
                    else if (ev.type == EV_ABS && ev.value >= -1000 && ev.value <= 1000)
                    {
                        if ((int) ev.code == 0)
                        {
                            inputDevice->setValueX(ev.value);
                        }
                        else if ((int) ev.code == 1)
                        {
                            inputDevice->setValueY(ev.value);
                        }
                    }
                }
            }
        }


    }
}

void InputHandler::addToDevices(int deviceID, std::string const& name, std::string const& eventString)
{
    Player* playerFigure = gameToHandle_->addNewPlayer(name, rand()%100);
    InputDevice* newlyFoundInputDevice = new InputDevice(deviceID, name, eventString, playerFigure);
    currentInputDevicesMutex_.lock();
    currentInputDevices_.insert(std::make_pair(deviceID, newlyFoundInputDevice));
    currentInputDevicesMutex_.unlock();
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
        currentInputDevicesMutex_.lock();
        std::map<int, InputDevice*>::iterator nextIterator = currentInputDevices_.erase(iteratorToDelete);
        currentInputDevicesMutex_.unlock();
        return nextIterator;
    }
}

void InputHandler::processAddRemoveQueries()
{
    devicesToAddMutex_.lock();

    for (std::map<int, AddDeviceQuery>::iterator it = devicesToAdd_.begin(); it != devicesToAdd_.end(); ++it)
    {
        addToDevices(it->second.deviceID, it->second.name, it->second.eventString);
    }

    devicesToAdd_.clear();
    devicesToAddMutex_.unlock();

    devicesToRemoveMutex_.lock();

    for (std::set<int>::iterator it = devicesToRemove_.begin(); it != devicesToRemove_.end(); ++it)
    {
        removeDevice(*it);
    }

    devicesToRemove_.clear();
    devicesToRemoveMutex_.unlock();
}

void InputHandler::processDeviceInputs()
{
    for (std::map<int, InputDevice*>::iterator it = currentInputDevices_.begin(); it != currentInputDevices_.end(); ++it)
    {
        it->second->mapValuesAndApply();
    }
}