#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <vector>

#include <iostream>

int main(void) 
{
	fd_set rfds;
	int retval;

	int handle = open("/dev/input/event4", O_RDWR);
	int handle2 = open("/dev/input/event5", O_RDWR);

	if (handle == -1 || handle2 == -1)
	{
		printf("Error opening device");
		return 1;
	}

	std::vector<int> fileDescriptors;
	fileDescriptors.push_back(handle);
	fileDescriptors.push_back(handle2);


	while (true)
	{
		FD_ZERO(&rfds);
		int maxDescriptor = -1;

		for (std::vector<int>::iterator it = fileDescriptors.begin(); it < fileDescriptors.end(); ++it)
		{
			FD_SET(*it, &rfds);

			if (*it > maxDescriptor)
				maxDescriptor = *it;
		}

		retval = select(maxDescriptor+1, &rfds, NULL, NULL, NULL); // first parameter is highest descriptor number +1

		for (std::vector<int>::iterator it = fileDescriptors.begin(); it < fileDescriptors.end(); ++it)
		{
			if (FD_ISSET(*it, &rfds))
			{
				std::cout << "Data is ready on file descriptor: " << *it << std::endl;

				struct input_event ev;
				size_t read_result = read(*it, &ev, sizeof(ev));

				std::cout << ev.type << ", " << ev.code << ", " << ev.value << std::endl;
				std::cout << std::endl;
			}
		}

	}

	return 0;
}