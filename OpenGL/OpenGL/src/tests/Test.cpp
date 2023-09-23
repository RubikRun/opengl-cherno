#include "Test.h"
#include "imgui/imgui.h"

namespace test {

TestMenu::TestMenu(Test*& currentTestPointer)
	: currentTest(currentTestPointer)
{

}

void TestMenu::onImGuiRender() {
	for (auto& test : tests) {
		if (ImGui::Button(test.first.c_str())) {
			currentTest = test.second();
		}
	}
}

} // namespace test