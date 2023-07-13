#pragma once

#include "PlayerUpdate.h"

namespace papyrus
{
	inline unsigned int functions_counter = 0;

	/**
	 * \brief Returns debug mode state.
	 * \return                  - Debug mode state.
	 */
	static float TimedBlockingPeriod(RE::StaticFunctionTag*)
	{
		return PlayerUpdate::OnPlayerUpdate::GetSingleton().timedBlockingPeriod;
	}

	/**
	 * \brief Sets debug mode state.
	 * \param mode              - New debug mode state.
	 */
	static void SetTimedBlockingPeriod(RE::StaticFunctionTag*, float period)
	{
		PlayerUpdate::OnPlayerUpdate::GetSingleton().timedBlockingPeriod = period;
	}

	/**
	 * \brief Returns debug mode state.
	 * \return                  - Debug mode state.
	 */
	static bool PlayerTimedBlocking(RE::StaticFunctionTag*)
	{
		return PlayerUpdate::OnPlayerUpdate::GetSingleton().enableTimedBlocking;
	}

	/**
	 * \brief Sets debug mode state.
	 * \param mode              - New debug mode state.
	 */
	static void SetPlayerTimedBlocking(RE::StaticFunctionTag*, bool mode)
	{
		PlayerUpdate::OnPlayerUpdate::GetSingleton().enableTimedBlocking = mode;
	}

	/**
	 * \brief Register functions for Papyrus scripts.
	 * \param aVirtualMachine   - Papyrus virtual machine.
	 * \return                  - True, if everything went fine.
	 */
	inline bool RegisterFunctions(RE::BSScript::IVirtualMachine* aVirtualMachine)
	{
		aVirtualMachine->RegisterFunction("TimedBlockingPeriod", "DeadlyCombat", TimedBlockingPeriod);
		functions_counter++;
		aVirtualMachine->RegisterFunction("SetTimedBlockingPeriod", "DeadlyCombat", SetTimedBlockingPeriod);
		functions_counter++;
		aVirtualMachine->RegisterFunction("PlayerTimedBlocking", "DeadlyCombat", PlayerTimedBlocking);
		functions_counter++;
		aVirtualMachine->RegisterFunction("SetPlayerTimedBlocking", "DeadlyCombat", SetPlayerTimedBlocking);
		functions_counter++;

		// logger::info("Registered {} Papyrus functions.", functions_counter);
		return true;
	}
}
