#include "Event.h"

constexpr auto MATH_PI = 3.14159265358979323846f;

bool m_inMenu = false;
uint32_t m_camStateId;

// Input

auto InputEventHandler::ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>*)
	-> EventResult
{
	if (!a_event)
		return EventResult::kContinue;

	auto camera = RE::PlayerCamera::GetSingleton();
	if (camera) {
		auto& state = camera->currentState;
		if (state) {
			if (!m_inMenu && (state->id != RE::CameraState::kTween)) {
				m_camStateId = state->id;
			}
		}
	}

	return EventResult::kContinue;
}

// Menu

void MenuOpenCloseEventHandler::RotateCamera()
{
	auto player = RE::PlayerCharacter::GetSingleton();
	auto camera = RE::PlayerCamera::GetSingleton();

	if (m_camStateId < RE::CameraState::kThirdPerson) {
		m_thirdForced = true;
	}

	auto thirdState = (RE::ThirdPersonState*)camera->cameraStates[RE::CameraState::kThirdPerson].get();
	camera->SetState(thirdState);
	camera->UpdateThirdPerson(player->IsWeaponDrawn());

	m_targetZoomOffset = thirdState->targetZoomOffset;
	m_freeRotation = thirdState->freeRotation;
	m_posOffsetExpected = thirdState->posOffsetExpected;

	thirdState->targetZoomOffset = -0.1f;
	thirdState->freeRotation.x = MATH_PI;
	thirdState->posOffsetExpected = thirdState->posOffsetActual = RE::NiPoint3(-50, -10, -50);

	camera->Update();
}

void MenuOpenCloseEventHandler::ResetCamera()
{
	auto camera = RE::PlayerCamera::GetSingleton();
	auto thirdState = (RE::ThirdPersonState*)camera->cameraStates[RE::CameraState::kThirdPerson].get();

	thirdState->targetZoomOffset = m_targetZoomOffset;
	thirdState->freeRotation = m_freeRotation;
	thirdState->posOffsetExpected = thirdState->posOffsetActual = m_posOffsetExpected;

	if (m_thirdForced) {
		auto cameraState = (RE::TESCameraState*)camera->cameraStates[m_camStateId].get();
		camera->SetState(cameraState);
	}

	camera->Update();
}

void MenuOpenCloseEventHandler::OnInventoryOpen()
{
	auto camera = RE::PlayerCamera::GetSingleton();
	if (!camera) {
		return;
	}

	m_thirdForced = false;
	RotateCamera();
}

void MenuOpenCloseEventHandler::OnInventoryClose()
{
	ResetCamera();
	m_inMenu = false;
}

auto MenuOpenCloseEventHandler::ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*)
	-> EventResult
{
	auto uiStr = RE::InterfaceStrings::GetSingleton();
	if (uiStr) {
		auto& name = a_event->menuName;
		if (name == uiStr->inventoryMenu ||
			name == uiStr->barterMenu ||
			name == uiStr->containerMenu)
		{
			m_inMenu = true;
			if (a_event->opening)
				OnInventoryOpen();
			else
				OnInventoryClose();
		}
	}
	return EventResult::kContinue;
}
