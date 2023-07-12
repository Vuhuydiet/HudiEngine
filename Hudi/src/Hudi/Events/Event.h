#pragma once

#include "hdpch.h"
#include "Hudi/Core/Core.h"

#include "Hudi/Source/KeyCodes.h"
#include "Hudi/Source/MouseCodes.h"

#include <glm/glm.hpp>

namespace Hudi {

	enum EventType 
	{
		FIRST_EVENT			= 0, //SDL_FIRSTEVENT,
		QUIT				= 256, //SDL_QUIT,
		WINDOW_EVENT		= 512, //SDL_WINDOWEVENT,
		KEY_DOWN			= 768, //SDL_KEYDOWN,
		KEY_UP				= 769, //SDL_KEYUP,
		MOUSE_BUTTON_DOWN	= 1025, //SDL_MOUSEBUTTONDOWN,
		MOUSE_BUTTON_UP		= 1026, //SDL_MOUSEBUTTONUP,
		MOUSE_MOTION		= 1024, //SDL_MOUSEMOTION,
		MOUSE_WHEEL			= 1027 //SDL_MOUSEWHEEL
	};

	enum WindowEvent
	{
		WINDOWEVENT_RESIZED = 5, //SDL_WINDOWEVENT_RESIZED,
		WINDOWEVENT_SIZE_CHANGED = 6, //SDL_WINDOWEVENT_SIZE_CHANGED,
		WINDOWEVENT_MOVED = 4 //SDL_WINDOWEVENT_MOVED
	};

	class Event
	{
	public:
		EventType type() const;// { return (EventType)m_Event.type; }
		WindowEvent WindowType() const;// { return (WindowEvent)m_Event.window.event; }

	public:
		KeyCode GetKeyCode() const;// { return m_Event.key.keysym.sym; }
		int GetWheelYOffset() const;// { return m_Event.wheel.y; }

		const void* GetEventHandle() const { return m_Event; }
	public:
		bool handled = false;
	private:
		void* m_Event = nullptr;
		friend class EventManager;
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event);

		bool Dispatch(EventType type, const std::function<void(Event&)>& fn);

	private:
		Event& m_Event;
	};

#ifndef HD__MEMBER_VARS_DEFINE
	
#endif
	class EventManager
	{
	public:
		static void Init();
		static void ShutDown();
		static void SetInstance(EventManager* instance) { s_Instance = instance; }
		static EventManager* GetInstance() { return s_Instance; }

		static void Clear();
		static void Reset();
		static void ClearEventQueue();

		static void SetCallBackFn(std::function<void(Event&)> fn);

		static void OnUpdate();
		
		static bool Quit();
		
		static bool KeyJustDown(KeyCode key);
		static bool KeyJustUp(KeyCode key);
		static bool KeyDown(KeyCode key);
		static bool KeyUp(KeyCode key);
		
		static bool MouseJustDown(MouseCode button);
		static bool MouseJustUp(MouseCode button);
		static bool MouseDown(MouseCode button);
		static bool MouseUp(MouseCode button);

		static glm::vec2 MousePosition();
		static glm::vec2 MouseDeltaPos();// { return m_CurrentMousePos - m_PreviousMousePos; }

	private:
		std::function<void(Event&)> s_CallBackFn = nullptr;

		bool s_QuitEvent = false;

		std::unordered_set<KeyCode> s_KeyJustDown;
		std::unordered_set<KeyCode> s_KeyJustUp;
		std::unordered_set<KeyCode> s_KeyDownEvent;

		std::unordered_set<MouseCode> s_ButtonJustDown;
		std::unordered_set<MouseCode> s_ButtonJustUp;
		std::unordered_set<MouseCode> s_MouseButtonDownEvent;

		glm::vec2 s_CurrentMousePos = { 0, 0 };
		glm::vec2 s_PreviousMousePos = { 0, 0 };

		EventManager() = default;
		static EventManager* s_Instance;
	};

	/*class HUDI_API Event
	{
		friend class EventPathcher;

	public:
		virtual EventType GetEventType() const { return EventType::None; }

		virtual const char* GetName() const { return "None"; }
		virtual std::string ToString() const { return GetName(); }

	protected:
		SDL_Event m_Event;

	private:
		friend int PollEvent(Event* e);
	};

	 std::ostream& operator<< (std::ostream& os, const Event& e)
	{
		os << e.ToString();
	}*/

}