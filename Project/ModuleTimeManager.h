#ifndef __MODULE__TIME_MANAGER__
#define __MODULE__TIME_MANAGER__

#include "Module.h"
#include "Timer.h"
#include "Globals.h"


class ModuleTimeManager : public Module
{
public:
	ModuleTimeManager(Application* app, bool start_enabled = true);
	~ModuleTimeManager();

	bool Init(JSON_Object* nod);
	bool CleanUp();

	update_status PostUpdate(float dt);

	void ShowInfo();

	void PlayGame(bool play);
	void PauseGame(bool pause);
	void PlayOneFrame();
	bool IsGamePaused()const;

	void StartTime();
	void StartRealTime();
	void StopTime();
	void StopRealTime();

	void SetFrameCount(uint amount);

	uint GetFrameCount() const;
	float GetTime(); //Time since Startup [Game Clock]
	float GetTimeScale() const; //Scale at which time is passing [Game Clock]
	float GetDeltaTime() const; //Last frame time expressed in seconds [Game Clock]
	float GetRealTime(); //Real Time since Startup [Real Time Clock]
	float GetRealDeltaTime() const; //Last frame time expressed in seconds [Real Time Clock]
	bool GetPlayOneFrame() const;

private:
	uint frame_count;

	Timer time;
	float time_scale;
	float delta_time;
	Timer real_time_since_start;
	float real_dtime_since_start;

	bool show_info = false;
	bool play_frame;
	bool game_paused;
	bool in_game;
};


#endif // !__MODULE__TIME_MANAGER__
