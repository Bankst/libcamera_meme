#include "camera_runner.h"
#include <iostream>
#include <stdio.h>
#include <signal.h>

using namespace std::chrono_literals;

std::shared_ptr<CameraRunner> camRunnerPtr;

bool shouldLoop = true;

void sigintHandler(int sig_num) {
	shouldLoop = false;
}

int main() {
		// catch sigint to cleanup safely
		signal(SIGINT, sigintHandler);

    constexpr int width = 1920, height = 1080;

		CameraManagerUtils::instance->start();
		auto allCameras = CameraManagerUtils::instance->cameras();

		if (allCameras.size() == 0) {
			std::cout << "No cameras detected!" << std::endl;
			return 0;
		}

    std::cout << "Num cameras: " << allCameras.size() << std::endl;

		// starts immediately
    camRunnerPtr = std::make_unique<CameraRunner>(640, 480, allCameras[0]);


		while(true) {
			// speen!!!!
			std::this_thread::sleep_for(125ms);
		}

    camRunnerPtr->Stop();

    return 0;
}