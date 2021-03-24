#ifndef CONTROLS
#define CONTROLS

struct Controls
{
	bool forward = false;
	bool backward = false;
	bool strafeRight = false;
	bool strafeLeft = false;

	bool left = false;
	bool right = false;
	bool lookup = false;
	bool lookdown = false;
	bool rollLeft = false;
	bool rollRight = false;

	bool vsync = true;
	bool imgui = true;
};

#endif // !CONTROLS
