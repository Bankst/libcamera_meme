#include "MainApp.h"
#include "ImGuiApp.h"
#include "imgui.h"
#include <chrono>
#include <string>

namespace matrixui {

using namespace std::chrono;

void MainApp::renderFrame() {
	renderMenubar();
}

void MainApp::renderMenubar() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			// close
			if (ImGui::MenuItem("Exit")) _wantsToClose = true;

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Widgets")) {
			auto widgetRenderStart = high_resolution_clock::now();
			// iterate widgets to fill menu

			for (auto it = _widgets.begin(); it != _widgets.end(); it++) {
				if (!it->second) {
					continue;
				}

				std::string widgetName = it->second->getWidgetName();
				bool visible = it->second->getVisibility();

				if (ImGui::MenuItem(widgetName.c_str(), NULL, &visible)) {
					it->second->setVisibility(visible);
				}
			}

			ImGui::EndMenu();
			_widgetMenuRenderTime = high_resolution_clock::now() - widgetRenderStart;
		} else {
			_widgetMenuRenderTime = 0ms;
		}

		ImGui::EndMainMenuBar();
	}
}

}
