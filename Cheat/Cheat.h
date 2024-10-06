#pragma once
#include <Windows.h>

#ifdef _DEBUG
#include "util/Console.h"
#endif

namespace Cheat {
	namespace Data {
		inline HMODULE hModule = NULL;

		void Unload();
	}

	void update() noexcept;
}