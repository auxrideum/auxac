#define HOOK __declspec(naked) void

typedef struct _stCameraAim
{
	float f1x, f1y, f1z;
	float pos1x, pos1y, pos1z;
	float pos2x, pos2y, pos2z;
	float f2x, f2y, f2z;

} stCameraAim;

class CPatchGameSA {
public:
	void Apply();

	static void GravityHook();
	static void KeyPress();
};

extern CPatchGameSA* pPatchGameSA;