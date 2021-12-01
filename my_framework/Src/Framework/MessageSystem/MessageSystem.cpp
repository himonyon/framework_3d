#include "../../../framework.h"
#include "../../../environment.h"

void MessageSystem::SendMessageToCom(noDel_ptr<Component> com, std::wstring str) {
	for (auto& m : com->messages) {
		if (m == str) return;
	}
	com->messages.emplace_back(str);
}

void MessageSystem::DeleteMessage(std::wstring str) {
	for (int i = 0; i < messages.size(); i++) {
		if (messages[i] == str) messages.erase(messages.begin() + i);
	}
}