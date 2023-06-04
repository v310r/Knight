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
	for (auto& [bindingName, bindingObject] : m_bindings)
	{
		delete bindingObject;
		bindingObject = nullptr;
	}
}

bool EventManager::AddBinding(Binding* binding)
{
	if (m_bindings.find(binding->GetName()) != m_bindings.end())
	{
		return false;
	}

	return m_bindings.emplace(binding->GetName(), binding).second;
}

bool EventManager::RemoveBinding(const std::string& name)
{
	auto iter = m_bindings.find(name);
	if (iter == m_bindings.end())
	{
		return false;
	}

	auto& [bindingName, bindingObject] = *iter;

	delete bindingObject;
	bindingObject = nullptr;
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
	{
		return false;
	}

	auto& [stateType, callbackContainer] = *i;

	auto i2 = callbackContainer.find(name);
	if (i2 == callbackContainer.end())
	{
		return false;
	}

	callbackContainer.erase(name);
	return true;
}

void EventManager::HandleEvent(const sf::Event& incomingEvent)
{
	for (auto& [bindingName, bindingObject] : m_bindings)
	{
		Binding* bind = bindingObject;
		for (auto& [eventType, eventInfo] : bind->GetEvents())
		{
			const EventType incomingEventType = static_cast<EventType>(incomingEvent.type);
			if (eventType != incomingEventType)
			{
				continue;
			}

			if (incomingEventType == EventType::KeyPressed || incomingEventType == EventType::KeyReleased)
			{
				if (eventInfo.GetCode() != incomingEvent.key.code)
				{
					continue;
				}

				if (bind->GetDetails().GetKeyCode() != -1)
				{
					bind->GetDetails().SetKeyCode(eventInfo.GetCode());
				}

				bind->SetCount(bind->GetCount() + 1);
				break;
			}
			else if (incomingEventType == EventType::MouseButtonPressed || incomingEventType == EventType::MouseButtonReleased)
			{
				if (eventInfo.GetCode() != incomingEvent.mouseButton.button)
				{
					continue;
				}

				bind->GetDetails().SetMousePos({ incomingEvent.mouseButton.x, incomingEvent.mouseButton.y });
				if (bind->GetDetails().GetKeyCode() != -1)
				{
					bind->GetDetails().SetKeyCode(eventInfo.GetCode());
				}

				bind->SetCount(bind->GetCount() + 1);

				break;
			}
			else
			{

				if (incomingEventType == EventType::MouseWheelMoved)
				{
					bind->GetDetails().SetMouseWheelDelta(incomingEvent.mouseWheel.delta);
				}
				else if (incomingEventType == EventType::WindowResized)
				{
					bind->GetDetails().SetSize(incomingEvent.size.width, incomingEvent.size.height);
				}
				else if (incomingEventType == EventType::TextEntered)
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

	for (auto& [bindingName, bindingObject] : m_bindings)
	{
		Binding* bind = bindingObject;
		for (auto& [eventType, eventInfo] : bind->GetEvents())
		{
			switch (eventType)
			{
				case EventType::Keyboard:
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(eventInfo.GetCode())))
					{
						if (bind->GetDetails().GetKeyCode() != 1)
						{
							bind->GetDetails().SetKeyCode(eventInfo.GetCode());
						}
						bind->SetCount(bind->GetCount() + 1);
					}
					break;
				}
				case EventType::Mouse:
				{
					if (sf::Mouse::isButtonPressed(sf::Mouse::Button(eventInfo.GetCode())))
					{
						if (bind->GetDetails().GetKeyCode() != 1)
						{
							bind->GetDetails().SetKeyCode(eventInfo.GetCode());
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
		std::cerr << "Failed loading cfg file" << ", src: " << __FILE__ << std::endl;
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

		if (AddBinding(bind) == false)
		{
			delete bind;
		}

		bind = nullptr;
	}
}
