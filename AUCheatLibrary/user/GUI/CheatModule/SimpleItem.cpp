#include "pch-il2cpp.h"
#include "SimpleItem.h"
#include "ImGUI/imgui.h"
#include <stdio.h>
#include <iostream>

static const ImVec4 RED_COLOR = ImVec4(1.0f, 0.1f, 0.1f, 1.0f);
static const ImVec4 GREEN_COLOR = ImVec4(0.1f, 1.0f, 0.1f, 1.0f);
SimpleItem::SimpleItem(char* name, void* func, isEnabledFunc checkFunc, bool hasState) : cheatFunc(func), checkEnabledFunc(checkFunc), hasState(hasState)
{ 
	this->name = name;
}

void SimpleItem::OnRender()
{
	ImGui::Text("%s", name);
	if (hasState) {
		ImGui::SameLine(200);
		if (state)
			ImGui::TextColored(GREEN_COLOR, "On");
		else
			ImGui::TextColored(RED_COLOR, "Off");
		ImGui::SameLine(230);
	}
	else {
		ImGui::SameLine(230);
	}
	
	std::cout << checkEnabledFunc() << std::endl;
	if (ImGui::Button(hasState ? "Enable" : "Activate") && checkEnabledFunc()) {
		printf("Button clicked.");
		if (hasState)
		{
			state = !state;
			((stateFunc)cheatFunc)(state);
		}
		else {
			((nostateFunc)cheatFunc)();
		}
	}
}

char* SimpleItem::GetName()
{
	return name;
}

