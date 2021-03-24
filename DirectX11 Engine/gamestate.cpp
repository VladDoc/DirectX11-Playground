#include "gamestate.h"

#include "imgui/imgui.h"

const vec4 GameState::moonCoords = VecScale(Vec3Norm({-1.0f, 0.5f, 1.0f}), 10000.0f);

const vec4 GameState::lpCoords = Vec4(2.0f, 4.0f, 3.0f, 1.0f);

struct ObjWin
{
	bool show_moon = false;
	bool show_sp = false;
	bool show_pointS = false;
	bool show_plane = false;

	void renderImGui(GameState& s)
	{
		ImGui::Begin("Objects");
		ImGui::Checkbox("Show moon settings", &show_moon);
		ImGui::Checkbox("Show sp settings", &show_sp);
		ImGui::Checkbox("Show pointS settings", &show_pointS);
		ImGui::Checkbox("Show plane settings", &show_plane);

		if (show_moon)
		{
			ImGui::Begin("Moon");
			s.moon.renderImGui();
			ImGui::End();
		}

		if (show_sp)
		{
			ImGui::Begin("SP");
			s.sp.renderImGui();
			ImGui::End();
		}

		if (show_pointS)
		{
			ImGui::Begin("PointS");
			s.pointS.renderImGui();
			ImGui::End();
		}

		if (show_plane)
		{
			ImGui::Begin("Plane");
			s.plane.renderImGui();
			ImGui::End();
		}

		ImGui::End();
	}
};

struct LightWin
{
	bool show_moonL = false;
	bool show_pointL = false;

	void renderImGui(GameState& s)
	{
		ImGui::Begin("Lights");
		ImGui::Checkbox("Show moon lighting settings", &show_moonL);
		ImGui::Checkbox("Show point lighting settings", &show_pointL);

		if (show_moonL)
		{
			ImGui::Begin("Moon Light");
			s.moon_light.renderImGui();
			ImGui::End();
		}

		if (show_pointL)
		{
			ImGui::Begin("Point Light");
			s.pointL.renderImGui();
			ImGui::End();
		}
		ImGui::End();
	}
};

static bool show_demo_window = false;

static bool show_objects_win = false;
static bool show_lights_win = false;

static ObjWin objwin;
static LightWin lightwin;

void GameState::renderImGui()
{

	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	{

		ImGui::Begin("Settings");

		ImGui::Checkbox("Show object settings", &show_objects_win);
		ImGui::Checkbox("Show light settings", &show_lights_win);

		if (show_objects_win) objwin.renderImGui(*this);
		if (show_lights_win) lightwin.renderImGui(*this);

		if (ImGui::Button("Set Default Aspect")) {
			this->camera.setAspect(screenW, screenH);
		}
		
		this->camera.renderImGui();
		
		ImGui::ColorEdit4("SkyColor1", &this->skyColor1.m128_f32[0]);
		ImGui::ColorEdit4("SkyColor2", &this->skyColor2.m128_f32[0]);
		ImGui::ColorEdit4("FogColor",  &this->fogColor.m128_f32[0]);


		ImGui::Checkbox("Fog", &this->fog);
		ImGui::Checkbox("Textured Sky", &this->skymode);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
}

void GameState::updateCB(ConstantBuffer& cb) const
{
	cb.frametime  = this->frametime;
	cb.moon       = this->moon;
	cb.moon_light = this->moon_light;
	cb.plane      = this->plane;
	cb.pointL     = this->pointL;
	cb.pointS     = this->pointS;
	cb.screenH    = this->screenH;
	cb.screenW	  = this->screenW;
	cb.sp		  = this->sp;
	cb.time		  = this->time;
	cb.cameraPos  = this->camera.pos;
	cb.skyColor1  = this->skyColor1;
	cb.skyColor2  = this->skyColor2;
	cb.fogColor   = this->fogColor;
	cb.fog		  = this->fog;
	cb.skymode    = this->skymode;
	cb.invVP      = this->camera.getInvVP();
}