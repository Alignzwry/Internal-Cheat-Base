#pragma once
#include <vector>

struct Hook {
	void** ppPointer = 0;
	void* pDetour = 0;
};


namespace hooks {
	bool init();
	void destroy();


	bool CreateHook(Hook hook);

	//
	inline std::vector<Hook> m_vHooks;
}