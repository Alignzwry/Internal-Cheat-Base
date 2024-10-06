#pragma once


namespace Features::Aimbot {
	inline bool enabled = false;
	inline float fov = 30.f;
	inline float smoothing = 4.f;
	inline int key = -1;

	void begin() noexcept;
	void validateTarget(void* target) noexcept;
	void end() noexcept;
}