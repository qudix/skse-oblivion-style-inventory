#pragma once

class Inventory3DManagerPatch
{
public:
	void UpdateItem3D(RE::InventoryEntryData* a_objDesc)
	{
		if (!a_objDesc)
			return;

		auto obj = a_objDesc->GetObject();
		if (!obj)
			return;
		
		auto manager = RE::Inventory3DManager::GetSingleton();
		switch (obj->GetFormType())
		{
		case RE::FormType::Armor:
		case RE::FormType::Ammo:
		case RE::FormType::Weapon:
			manager->Clear3D();
			break;
		default:
			manager->UpdateMagic3D(obj, 0);
			break;
		}
	}

	static void Install()
	{
		REL::Relocation<uintptr_t> func{ REL::ID(50884) };
		auto& trampoline = SKSE::GetTrampoline();
		trampoline.write_branch<5>(func.address(), &UpdateItem3D);
	}
};
