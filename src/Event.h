#pragma once

class InputEventHandler : public RE::BSTEventSink<RE::InputEvent*>
{
public:
	using EventResult = RE::BSEventNotifyControl;

	static InputEventHandler* GetSingleton()
	{
		static InputEventHandler singleton;
		return std::addressof(singleton);
	}

	static void Enable()
	{
		auto ui = RE::BSInputDeviceManager::GetSingleton();
		if (ui) {
			ui->AddEventSink(GetSingleton());
		}
	}

	static void Disable()
	{
		auto ui = RE::BSInputDeviceManager::GetSingleton();
		if (ui) {
			ui->RemoveEventSink(GetSingleton());
		}
	}

	virtual EventResult ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>*) override;

private:
	InputEventHandler() = default;
	InputEventHandler(const InputEventHandler&) = delete;
	InputEventHandler(InputEventHandler&&) = delete;

	inline ~InputEventHandler() { Disable(); }

	InputEventHandler& operator=(const InputEventHandler&) = delete;
	InputEventHandler& operator=(InputEventHandler&&) = delete;
};

class MenuOpenCloseEventHandler : public RE::BSTEventSink<RE::MenuOpenCloseEvent>
{
public:
	using EventResult = RE::BSEventNotifyControl;

	static MenuOpenCloseEventHandler* GetSingleton()
	{
		static MenuOpenCloseEventHandler singleton;
		return std::addressof(singleton);
	}

	static void Enable()
	{
		auto ui = RE::UI::GetSingleton();
		if (ui) {
			ui->AddEventSink(GetSingleton());
		}
	}

	static void Disable()
	{
		auto ui = RE::UI::GetSingleton();
		if (ui) {
			ui->RemoveEventSink(GetSingleton());
		}
	}

	void RotateCamera();
	void ResetCamera();

	void OnInventoryOpen();
	void OnInventoryClose();

	virtual EventResult ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override;

private:
	MenuOpenCloseEventHandler() = default;
	MenuOpenCloseEventHandler(const MenuOpenCloseEventHandler&) = delete;
	MenuOpenCloseEventHandler(MenuOpenCloseEventHandler&&) = delete;

	inline ~MenuOpenCloseEventHandler() { Disable(); }

	MenuOpenCloseEventHandler& operator=(const MenuOpenCloseEventHandler&) = delete;
	MenuOpenCloseEventHandler& operator=(MenuOpenCloseEventHandler&&) = delete;

	bool			m_thirdForced;
	float			m_targetZoomOffset;
	RE::NiPoint2	m_freeRotation;
	RE::NiPoint3	m_posOffsetExpected;
};

