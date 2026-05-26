#pragma once
#include <utils.h>
#include <map>
using namespace utils;

#pragma region gameInformation
// TODO: Set your name and group in the title here
std::string g_WindowTitle{ "Gameplay Prototyping - Fox - Rondia, Louise - 1DAE12" };

// Change the window dimensions here
float g_WindowWidth{ 1080 };
float g_WindowHeight{ 720 };
#pragma endregion gameInformation

#pragma region ownDeclarations
// Declare your own global variables here

#define MVT_RIGHT SDL_SCANCODE_D
#define MVT_LEFT SDL_SCANCODE_A

const Color4f g_White{ 1.f, 1.f, 1.f, 1.f };
const Color4f g_LightGrey{ 0.f, 0.f, 0.f, 0.1f };
const Color4f g_MedGrey{ 0.f, 0.f, 0.f, 0.2f };
const Color4f g_DarkGrey{ 0.f, 0.f, 0.f, 0.5f };
const Color4f g_Black{ 0.f, 0.f, 0.f, 1.f };
const Color4f g_Orange{ 1.f, 0.7f, 0.2f, 1.f };
const Color4f g_SnowColor{ 215.f / 255.f, 225.f / 255.f, 1.f, 1.f };

struct Trianglef {
	Point2f p1{};
	Point2f p2{};
	Point2f p3{};
};

struct Scene {
	Texture bgTexture{};
	Texture fgTexture{};
	std::vector<float> altitude{};
};

Texture g_BushTexture{};

Texture g_Cloud{};
Point2f	g_CloudPos{};
int		g_CloudDir{ 1 };

Texture g_Bark{};

Ellipsef	g_Sun{ g_WindowWidth / 2, g_WindowHeight / 3 + 25.f, 35.f, 35.f };
//Ellipsef	g_SmallSun{ g_WindowWidth / 2, g_WindowHeight / 3 + 25.f, 15.f, 15.f };

std::map<int, std::vector<Rectf>>	g_Platforms{};
bool g_IsOnPlatform{};

struct World {
	static const int nrScenes{ 7 };
	Scene	scenes[nrScenes]{};
	int		currentScene{ 0 };
};

enum class AnimState {
	Idle,
	Walk,
	Jump,
	Sleep,
};

struct FoxTexture {
	Texture		texture{};
	const int	nrCols{ 22 };
	const int	nrRows{ 7 };
	float		frameWidth{};
	float		frameHeight{};
};

struct AnimationFrame {
	int row{};
	int col{};
	int nrFrames{};
};

struct Frame {
	Rectf		rect{};
	int			index{};
	int			startIndex{};
	int			nrFrames{};
};

struct Fox {
	Point2f		pos{};
	Point2f		dir{};
	Frame		frame{};
	FoxTexture	texture{};
	AnimState	animState{};
	bool		isJumping{};
	bool		isBarking{};
	float		vx{};
	float		vy{};
};

struct Step {
	Ellipsef	shape{ 0.f, 0.f, 5.f, 3.f };
	Color4f		color{ g_DarkGrey };
	bool		isOn{};
	float		coolDown{};
	int			index{};
};

struct Enemy {
	Trianglef	triangle{};
	bool		isVisible{};
	Color4f		color{ 1.f, 0.f, 0.f, 0.1f };
	Texture		texture{};
	float		blinkTime{};
	float		blinkCooldown{4.f};
	bool		isBlinking{};
};

struct SnowFlake
{
	Point2f position{};
	float radius{};
	float speed{};
};

float		g_Time{};
float		g_FrameTime{};
float		g_SleepTime{};
float		g_BarkTime{};
float		g_BarkCooldown{};
const float	g_BarkCooldownMax{ 2.5f };
const float g_Gravity{ 800.f };
const float g_JumpPower{ -400.f };
const float g_FoxSpeed{ 200.f };
const float g_FoxDrawWidth{ 100.f };
const float g_FoxDrawHeight{ 100.f };

Texture		g_MsgTex{};
Texture		g_KeyBindsTex{};
Texture		g_KeyBarkTex{};

const int	g_NrSteps{ 8 };
Step		g_Steps[g_NrSteps]{};
float		g_StepOffset{};
int			g_StepIndex{};

const float g_MaxAltitude{ 200.f };
const float g_MinAltitude{ g_WindowHeight - 200.f };

World		g_World{};
Fox			g_Fox{};
Enemy		g_Enemy{};
Rectf		g_Ground{ 0.f, g_MinAltitude, g_WindowWidth, g_WindowHeight };
const int	g_MaxBushes{ 5 };
Rectf		g_Bushes[g_MaxBushes]{};
int			g_NumBushes{};

float		g_LastSpawn{};
const float g_SpawnPosLeft{};
const float g_SpawnPosRight{g_WindowWidth - 50.f};

std::vector<SnowFlake> g_Snowflakes{};
float		g_StormSpeed{ 1.f };
int			g_NbFlakes{ 1000 };
const int	g_MaxFlakes{ 3000 };

int			g_DeathCounter{};

std::vector<float>	g_Altitude(static_cast<unsigned int>(g_WindowWidth));

std::map<std::string, AnimationFrame> g_AnimFrames{};

bool isT{};

// Declare your own functions here

void	InitWorldScenes();
void	InitScene();
void	UpdateWorldScene();
void	DrawBackground();
void	DrawForeground();

void	InitBushes(int index);
void	DrawBushes();

void	InitEnemy();
void	DrawEnemy();
void	UpdateEnemy(float elapsedSec);
void	UpdateEnemyBlinking(float elapsedSec);
void	UpdateEnemyTriangle(int index, float elapsedSec);

void	DrawSteps();
void	UpdateSteps(float elapsedSec);

void	UpdateBarking(float elapsedSec);

void	InitFox();
void	InitFoxSteps();
void	DrawFox();
void	UpdateFox(float elaspedSec, const Uint8* pStates);
void	UpdateFoxPos(float elapsedSec);
void	UpdateFoxFrame(float elapsedSec);
void	UpdateFoxFrameStartIndex();
void	UpdateFoxAnimState(const Uint8* pStates);
void	UpdateFoxDirection(const Uint8* pStates);
void	UpdateFoxJumpState(const Uint8* pStates);

Rectf	GetFrameRectf(int index);
float	GetAltitude(float index);
bool	IsPointInTriangle(const Point2f& p, const Trianglef& triangle);

void	InitAnimationFrames();
bool	IsAnyMovementKeyPressed(const Uint8* pStates);

void	InitSnowStorm();
void	DrawSnowStorm();
void	UpdateSnowStorm(float elapsedSec);
void	UpdateStormData(float speed, int nbFlakes);

void	InitPlatforms();
std::vector<Rectf>	&GetPlatformsFromLevel(int level);
void	DrawPlatforms();


// utils

bool IsFoxInEnemyRange();
void DrawTriangle(const Trianglef& t, float lineWidth = 1);
void FillTriangle(const Trianglef& t);

#pragma endregion ownDeclarations

#pragma region gameFunctions											
void Start();
void Draw();
void Update(float elapsedSec);
void End();
#pragma endregion gameFunctions

#pragma region inputHandling											
void OnKeyDownEvent(SDL_Keycode key);
void OnKeyUpEvent(SDL_Keycode key);
void OnMouseMotionEvent(const SDL_MouseMotionEvent& e);
void OnMouseDownEvent(const SDL_MouseButtonEvent& e);
void OnMouseUpEvent(const SDL_MouseButtonEvent& e);
#pragma endregion inputHandling


// platformes blanches, un peut plus larges
// buisson a l entree comme dhab et un sur la 2e platforme
// visuel pour expliquer les touches
// dessin fond 
// bark attire le regard
// outro scene scene [6]