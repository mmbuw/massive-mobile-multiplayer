#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <linux/input.h>

#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <unordered_map>
#include <set>
#include <thread>
#include <atomic>
#include <mutex>
#include <ctime>

#include "Game.hpp"
#include "InputDevice.hpp"

struct AddDeviceQuery
{
	int deviceID;
	std::string name;
	std::string eventString;
};

class InputHandler
{
	public:
		InputHandler(Game* gameToHandle);
		~InputHandler();

		void updateDeviceList();
		void retrieveInputs();

		void addToDevices(int deviceID, std::string const& name, std::string const& eventString);
		std::map<int, InputDevice*>::iterator removeDevice(int deviceID);

		//Thread save functions to be called every frame
		void processAddRemoveQueries();
		void processDeviceInputs();

	private:
		Game* gameToHandle_;
		std::mutex currentInputDevicesMutex_;
		std::map<int, InputDevice*> currentInputDevices_;
		
		std::thread updateDeviceListThread_;
		std::atomic<bool> updateDeviceListThreadRunning_;

		std::mutex devicesToAddMutex_;
		std::map<int, AddDeviceQuery> devicesToAdd_;
		std::mutex devicesToRemoveMutex_;
		std::set<int> devicesToRemove_;

		std::thread inputValueThread_;
		std::atomic<bool> inputValueThreadRunning_;

};

#endif //INPUT_HANDLER_HPP