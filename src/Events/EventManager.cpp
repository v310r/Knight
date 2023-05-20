#include "EventManager.h"
#include <fstream>
#include <iostream>
#include <sstream>


EventManager::EventManager() : m_hasFocus(true)
{
	LoadBindings();
}

EventManager::~EventManager()
{
	for (auto& bindingObject : m_bindings)
	{
		delete bindingObject.second;
		bindingObject.second = nullptr;
	}
}

bool EventManager::AddBinding(Binding* binding)
{
	if (m_bindings.find(binding->GetName()) != m_bindings.end())
		return false;

	return m_bindings.emplace(binding->GetName(), binding).second;
}

bool EventManager::RemoveBinding(const std::string& name)
{
	auto iter = m_bindings.find(name);
	if (iter == m_bindings.end())
		return false;

	delete iter->second;
	m_bindings.erase(iter);
	return true;
}

void EventManager::SetFocus(const bool& focus)
{
}

bool EventManager::RemoveCallback(const StateType state, const std::string& name)
{
	auto i = m_callbacks.find(state);
	if (i == m_callbacks.end())
		return false;

	auto i2 = i->second.find(name);
	if (i2 == i->second.end())
		return false;

	i->second.erase(name);
	return true;
}

void EventManager::HandleEvent(const sf::Event& incomingEvent)
{
	for (auto& bindingObject : m_bindings)
	{
		Binding* bind = bindingObject.second;
		for (auto& e : bind->GetEvents())
		{
			EventType incomingType = static_cast<EventType>(incomingEvent.type);
			if (e.first != incomingType)
				continue;

			if (incomingType == EventType::KeyPressed || incomingType == EventType::KeyReleased)
			{
				if (e.second.GetCode() != incomingEvent.key.code)
					continue;

				if (bind->GetDetails().GetKeyCode() != -1)
				{
					bind->GetDetails().SetKeyCode(e.second.GetCode());
				}

				bind->SetCount(bind->GetCount() + 1);
				break;
			}
			else if (incomingType == EventType::MouseButtonPressed || incomingType == EventType::MouseButtonReleased)
			{
				if (e.second.GetCode() != incomingEvent.mouseButton.button)
					continue;

				bind->GetDetails().SetMousePos({ incomingEvent.mouseButton.x, incomingEvent.mouseButton.y });
				if (bind->GetDetails().GetKeyCode() != -1)
				{
					bind->GetDetails().SetKeyCode(e.second.GetCode());
				}

				bind->SetCount(bind->GetCount() + 1);
				break;
			}
			else
			{

				if (incomingType == EventType::MouseWheelMoved)
				{
					bind->GetDetails().SetMouseWheelDelta(incomingEvent.mouseWheel.delta);
				}
				else if (incomingType == EventType::WindowResized)
				{
					bind->GetDetails().SetSize(incomingEvent.size.width, incomingEvent.size.height);
				}
				else if (incomingType == EventType::TextEntered)
				{
					bind->GetDetails().SetTextEntered(incomingEvent.text.unicode);
				}
				bind->SetCount(bind->GetCount() + 1);
			}
		}
	}
}

void EventManager::Update()
{
	if (m_hasFocus == false)
	{
		return;
	}

	for (auto& bindingObject : m_bindings)
	{
		Binding* bind = bindingObject.second;
		for (auto& e : bind->GetEvents())
		{
			switch (e.first)
			{
				case EventType::Keyboard:
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(e.second.GetCode())))
					{
						if (bind->GetDetails().GetKeyCode() != 1)
						{
							bind->GetDetails().SetKeyCode(e.second.GetCode());
						}
						bind->SetCount(bind->GetCount() + 1);
					}
					break;
				}
				case EventType::Mouse:
				{
					if (sf::Mouse::isButtonPressed(sf::Mouse::Button(e.second.GetCode())))
					{
						if (bind->GetDetails().GetKeyCode() != 1)
						{
							bind->GetDetails().SetKeyCode(e.second.GetCode());
						}
						bind->SetCount(bind->GetCount() + 1);
					}
					break;
				}
				case EventType::Joystick:
				{
					// Up to expansion
					break;
				}
			}
		}

		if (bind->GetEvents().size() == bind->GetCount())
		{
			auto currentStateCallbacks = m_callbacks.find(m_currentState);
			if (currentStateCallbacks != m_callbacks.end())
			{
				auto callIter = currentStateCallbacks->second.find(bind->GetName());
				if (callIter != currentStateCallbacks->second.end())
				{
					callIter->second(&bind->GetDetails());
				}
			}

			auto globalStateCallbacks = m_callbacks.find(StateType::Global);
			if (globalStateCallbacks != m_callbacks.end())
			{
				auto callIter = globalStateCallbacks->second.find(bind->GetName());
				if (callIter != globalStateCallbacks->second.end())
				{
					callIter->second(&bind->GetDetails());
				}
			}
		}
		bind->SetCount(0);
		bind->GetDetails().Clear();
	}
}

void EventManager::LoadBindings()
{
	const std::string delimiter = ":";

	std::ifstream bindings;
	bindings.open("cfg/Input/bindings.cfg");
	if (bindings.is_open() == false)
	{
		std::cout << "Failed loading cfg file\n";
		return;
	}
	std::string line;
	while (std::getline(bindings, line))
	{
		std::stringstream keystream(line);
		std::string callbackName;
		keystream >> callbackName;
		Binding* bind = new Binding(callbackName);
		while (keystream.eof() == false)
		{
			std::string keyval;
			keystream >> keyval;
			const int start = 0;
			const int end = keyval.find(delimiter);
			if (end == std::string::npos)
			{
				delete bind;
				bind = nullptr;
				break;
			}
			EventType type = EventType(std::stoi(keyval.substr(start, end - start)));
			const int code = std::stoi(keyval.substr(end + delimiter.length(), 
				keyval.find(delimiter, end + delimiter.length())));
			EventInfo eventInfo;
			eventInfo.SetCode(code);

			bind->BindEvent(type, eventInfo);
		}

		if (AddBinding(bind) == false) delete bind;

		bind = nullptr;
	}
	bindings.close();
}
