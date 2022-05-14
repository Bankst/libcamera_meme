#include <iostream>
#include <memory>
#include <opencv2/core/hal/interface.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "ImGuiWidget.h"
#include "MainApp.h"

#include "MatrixDebugWidget.h"
#include "OpenCVMatWidget.h"

using namespace matrixui;

MainApp app;

const uint width = 320;
const uint height = 240;
cv::Mat theImage = cv::Mat::zeros(height, width, CV_8UC3);

cv::Mat * getImg() {
	return &theImage;
}

int main() {
	std::cout << "Hello World!" << std::endl;

	// render something to our circle
	cv::circle(theImage, cv::Point(width/2, height/2), 15, cv::Scalar{0, 255, 0, 0});

	if (app.init() < 0) {
		std::cerr << "ImGuiApp init failed!" << std::endl;
		return -1;
	}

	CVMatProviderFunc getImgFunc = getImg;

	// cannot use auto, as the method expects the base class
	ImGuiWidgetPtr dbgWidgetPtr = std::make_shared<MatrixDebugWidget>();
	ImGuiWidgetPtr cvMatWidgetPtr = std::make_shared<OpenCVMatWidget>(getImg);

	app.addWidget(signalsWidgetPtr);
	app.addWidget(cvMatWidgetPtr);

	// Main loop
	while(!app.wantsToClose()) {
		// Do pre-GUI-render stuff here

		app.serviceLoop();

		// Do post-GUI-render stuff here

	}

	app.close();

	return 0;
}