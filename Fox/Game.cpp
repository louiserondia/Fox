#include "pch.h"
#include "Game.h"
#include <iostream>

//Basic game functions
#pragma region gameFunctions											
void Start()
{
	InitWorldScenes();
	InitAnimationFrames();
	InitFox();
	InitFoxSteps();
	InitEnemy();
	InitBushes(g_World.currentScene);
	InitSnowStorm();
	InitScene();
	InitPlatforms();
}

void Draw()
{
	ClearBackground(g_White.r, g_White.g, g_White.b);
	DrawBackground();
	DrawEnemy();
	DrawSnowStorm();
	DrawBushes();
	DrawPlatforms();
	DrawSteps();
	DrawFox();
	DrawForeground();

	// debug : draw altitude vectors
	if (isT) {
		for (float col{}; col < g_WindowWidth; ++col) {
			SetColor(g_Black);
			DrawRect(col, g_World.scenes[g_World.currentScene].altitude[col], 1.f, 1.f);
		}
	}
}

void Update(float elapsedSec)
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	UpdateFox(elapsedSec, pStates);
	UpdateEnemy(elapsedSec);
	UpdateWorldScene();
	UpdateSnowStorm(elapsedSec);

	const float speed{ g_BarkCooldown < g_BarkCooldownMax ? 3.f : 1.f };

	g_SleepTime += elapsedSec;
	g_Time += elapsedSec * speed;

	g_CloudPos.x += cosf(g_Time) / 3;
}

void End()
{
	DeleteTexture(g_Fox.texture.texture);
	DeleteTexture(g_Enemy.texture);
	DeleteTexture(g_BushTexture);
	DeleteTexture(g_Cloud);
	DeleteTexture(g_Bark);
	DeleteTexture(g_World.scenes[0].bgTexture);
	DeleteTexture(g_World.scenes[0].fgTexture);
	DeleteTexture(g_World.scenes[1].bgTexture);
	DeleteTexture(g_World.scenes[1].fgTexture);
	DeleteTexture(g_World.scenes[2].bgTexture);
	DeleteTexture(g_World.scenes[3].bgTexture);
	DeleteTexture(g_World.scenes[4].bgTexture);
	DeleteTexture(g_MsgTex);
	DeleteTexture(g_KeyBindsTex);
	DeleteTexture(g_KeyBarkTex);
}
#pragma endregion gameFunctions

//Keyboard and mouse input handling
#pragma region inputHandling											

void OnKeyDownEvent(SDL_Keycode key)
{
	if (key == SDLK_t) {
		isT = !isT;
	}

	if (key >= SDLK_1 && key <= SDLK_7) {
		g_World.currentScene = key - SDLK_1;
		g_Fox.pos.x = 20.f;
		g_LastSpawn = g_SpawnPosLeft;
		InitFoxSteps();
		InitScene();
	}

	if (key == SDLK_b)
	{
		if (g_BarkCooldown > g_BarkCooldownMax && !g_Fox.isBarking)
		{
			g_BarkCooldown = 0.f;
			g_Fox.isBarking = true;
		}
	}
}

void OnKeyUpEvent(SDL_Keycode key)
{
}

void OnMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
}

void OnMouseDownEvent(const SDL_MouseButtonEvent& e)
{
}

void OnMouseUpEvent(const SDL_MouseButtonEvent& e)
{
}
#pragma endregion inputHandling

#pragma region ownDefinitions
// Define your own functions here

void	InitWorldScenes() {
	size_t size{ static_cast<size_t>(g_WindowWidth + 1) };
	g_World.scenes[0].altitude.resize(size);
	g_World.scenes[1].altitude.resize(size);
	g_World.scenes[2].altitude.resize(size);
	g_World.scenes[3].altitude.resize(size);
	g_World.scenes[4].altitude.resize(size);
	g_World.scenes[5].altitude.resize(size);
	g_World.scenes[6].altitude.resize(size);
	TextureFromFile("landscape0_bg.png", g_World.scenes[0].bgTexture);
	TextureFromFile("landscape0_fg.png", g_World.scenes[0].fgTexture);
	TextureFromFile("landscape1_bg.png", g_World.scenes[1].bgTexture);
	TextureFromFile("landscape1_fg.png", g_World.scenes[1].fgTexture);
	TextureFromFile("landscape2_bg.png", g_World.scenes[2].bgTexture);
	TextureFromFile("landscape3_bg.png", g_World.scenes[3].bgTexture);
	TextureFromFile("landscape4_bg.png", g_World.scenes[4].bgTexture);
	TextureFromFile("platform.png", g_BushTexture);
	TextureFromFile("cloud.png", g_Cloud);
	TextureFromFile("bark.png", g_Bark);
	TextureFromFile("message.png", g_MsgTex);
	TextureFromFile("keybinds.png", g_KeyBindsTex);
	TextureFromFile("keybark.png", g_KeyBarkTex);
	g_CloudPos = Point2f{ g_WindowWidth / 4 * 3 - 30.f, 50.f };

	const int half{ int(g_WindowWidth / 2) },
		third{ int(g_WindowWidth / 3) },
		third2{ int(g_WindowWidth / 3 * 2) },
		fourth{ int(g_WindowWidth / 4) },
		fourth3{ int(g_WindowWidth / 4 * 3) },
		sixth{ int(g_WindowWidth / 6) };
	const float gap{ (g_MinAltitude - g_MaxAltitude) };
	const float halfGap{ gap / 3 * 2 };

	for (int col{}; col < g_WindowWidth; ++col) {
		if (col > third && col < third2) {
			g_World.scenes[0].altitude[col] = g_MinAltitude - (gap / third * (col - third));
		}
		else if (col >= third2) {
			g_World.scenes[0].altitude[col] = g_MaxAltitude + (gap / third * (col - third2));
		}
		else {
			g_World.scenes[0].altitude[col] = g_MinAltitude;
		}
	}

	for (int col{}; col < g_WindowWidth; ++col) {
		if (col < third) {
			g_World.scenes[1].altitude[col] = g_MaxAltitude + (gap / third * col);
		}
		else {
			g_World.scenes[1].altitude[col] = g_MinAltitude;
		}
	}

	for (int col{}; col < g_WindowWidth; ++col) {
		g_World.scenes[2].altitude[col] = g_MinAltitude;
		g_World.scenes[4].altitude[col] = g_MinAltitude;
		g_World.scenes[5].altitude[col] = g_MinAltitude;
		g_World.scenes[6].altitude[col] = g_MinAltitude;
		if (col < sixth || col > half) {
			g_World.scenes[3].altitude[col] = g_MinAltitude;
		}
		//changer et faire entre quart et moitie
		else if (col > sixth && col < third) {
			g_World.scenes[3].altitude[col] = g_MinAltitude - (halfGap / (third - sixth) * (col - sixth));
		}
		else if (col > third && col < half) {
			g_World.scenes[3].altitude[col] = g_MinAltitude - (halfGap / (half - third) * (half - col));
		}
	}
}

void InitScene() {
	g_Time = -M_PI;
	InitBushes(g_World.currentScene);
	g_Enemy.isVisible = !(g_World.currentScene < 2 || g_World.currentScene > 5);
	g_Enemy.isBlinking = false;
	if (g_World.currentScene == 0)
		UpdateStormData(1.f, 500);
	else if (g_World.currentScene == 1)
		UpdateStormData(2.f, 500);
	else if (g_World.currentScene == 2)
		UpdateStormData(5.f, 1000);
	else if (g_World.currentScene == 3)
		UpdateStormData(7.f, 1000);
	else if (g_World.currentScene == 4)
		UpdateStormData(2.f, 1000);
	else if (g_World.currentScene == 5)
		UpdateStormData(4.f, 1000);
	else if (g_World.currentScene == 6)
		UpdateStormData(2.f, 500);
}

void InitFox() {
	if (!TextureFromFile("Fox Sprite Sheet.png", g_Fox.texture.texture)) {
		std::cout << "Couldn't load fox.png\n";
	}
	g_Fox.texture.frameWidth = g_Fox.texture.texture.width / g_Fox.texture.nrCols;
	g_Fox.texture.frameHeight = g_Fox.texture.texture.height / g_Fox.texture.nrRows;
	g_Fox.frame.rect = GetFrameRectf(0);
	g_Fox.frame.nrFrames = g_AnimFrames["idleRight"].nrFrames;
	g_Fox.frame.startIndex = GetIndex(g_AnimFrames["idleRight"].row, g_AnimFrames["idleRight"].col, g_Fox.texture.nrCols);
	g_Fox.pos = Point2f{ g_WindowWidth / 4, GetAltitude(g_WindowWidth / 4 + g_FoxDrawWidth / 2) - g_FoxDrawHeight };
	g_Fox.dir = Point2f{ 1.f, 0.f };
	g_BarkCooldown = g_BarkCooldownMax;
}

void	InitFoxSteps() {
	for (int i{}; i < g_NrSteps; ++i) {
		g_Steps[i].isOn = false;
		g_Steps[i].shape.center.y = g_Ground.top + (i & 1 ? 15.f : 0.f);
		g_Steps[i].index = i;
	}
}

void InitBushes(int index) {
	float offset{ 15.f };
	for (int idx{}; idx < g_MaxBushes; ++idx) {
		g_Bushes[idx].top = g_MinAltitude;
	}

	if (index < 2) {
		g_NumBushes = 0;
	}
	else if (index == 2) {
		g_NumBushes = 2;
		g_Bushes[0].left = 0.f;
		g_Bushes[0].width = 150.f;
		g_Bushes[0].top -= 80.f - offset;
		g_Bushes[0].height = 80.f;

		g_Bushes[1].left = g_WindowWidth / 3 * 2 - 30.f;
		g_Bushes[1].width = 150.f;
		g_Bushes[1].top -= 80.f - offset;
		g_Bushes[1].height = 80.f;
	}
	else if (index == 3) {
		g_NumBushes = 2;

		g_Bushes[0].left = 0.f;
		g_Bushes[0].width = 150.f;
		g_Bushes[0].top -= 80.f - offset;
		g_Bushes[0].height = 80.f;

		g_Bushes[1].left = g_WindowWidth / 3 - 45.f;
		g_Bushes[1].width = 110.f;
		g_Bushes[1].top = g_WindowHeight / 3 - 10.f;
		g_Bushes[1].height = 100.f;
	}
	else if (index == 4) {
		g_NumBushes = 3;

		g_Bushes[0].left = 0.f;
		g_Bushes[0].width = 150.f;
		g_Bushes[0].top -= 80.f - offset;
		g_Bushes[0].height = 80.f;

		g_Bushes[1].left = g_WindowWidth / 3 + 40.f;
		g_Bushes[1].width = 150.f;
		g_Bushes[1].top -= 80.f - offset;
		g_Bushes[1].height = 80.f;

		g_Bushes[2].left = g_WindowWidth / 3 * 2 + 60.f;
		g_Bushes[2].width = 100.f;
		g_Bushes[2].top -= 80.f - offset;
		g_Bushes[2].height = 80.f;
	}
	else if (index == 5) {
		g_NumBushes = 2;

		g_Bushes[0].left = 0.f;
		g_Bushes[0].width = 150.f;
		g_Bushes[0].top -= 80.f - offset;
		g_Bushes[0].height = 80.f;

		g_Bushes[1].left = g_WindowWidth / 3 + 215.f;
		g_Bushes[1].width = 145;
		g_Bushes[1].top -= 200.f;
		g_Bushes[1].height = 85.f;
	}
	//Rectf{ g_WindowWidth / 3 + 200.f, g_MinAltitude - 130.f, w, h }

}

void InitEnemy() {
	g_Enemy.triangle.p1 = Point2f{ g_WindowWidth / 2 - 20.f, 100.f }; // top
	//g_Enemy.triangle.p2 = Point2f{ g_WindowWidth / 4, g_WindowHeight }; // left
	//g_Enemy.triangle.p3 = Point2f{ g_WindowWidth / 4 * 3, g_WindowHeight}; // right
	g_Enemy.triangle.p2 = Point2f{ g_WindowWidth / 4, g_MinAltitude }; // left
	g_Enemy.triangle.p3 = Point2f{ g_WindowWidth / 4 * 3, g_MinAltitude }; // right
	if (!TextureFromFile("man.png", g_Enemy.texture)) {
		std::cout << "Couldn't load man.png\n";
	}

}

void InitAnimationFrames() {
	g_AnimFrames["idleRight"] = AnimationFrame{ 0, 0, 5 };
	g_AnimFrames["idleLeft"] = AnimationFrame{ 0, 5, 5 };
	g_AnimFrames["walkRight"] = AnimationFrame{ 2, 0, 8 };
	g_AnimFrames["walkLeft"] = AnimationFrame{ 2, 8, 8 };
	g_AnimFrames["jumpRight"] = AnimationFrame{ 3, 0, 11 };
	g_AnimFrames["jumpLeft"] = AnimationFrame{ 3, 11, 11 };
	g_AnimFrames["sleep"] = AnimationFrame{ 5, 0, 6 };
}

void InitSnowStorm()
{
	g_Snowflakes.reserve(g_MaxFlakes);

	for (int i{}; i < g_MaxFlakes; ++i)
	{
		SnowFlake flake{};

		flake.position.x = float(rand() % int(g_WindowWidth));
		flake.position.y = float(rand() % int(g_WindowHeight));
		flake.radius = float(rand() % 6 + 1);
		flake.speed = float(rand() % 50 + 50);

		g_Snowflakes.push_back(flake);
	}
}

void DrawBackground() {
	const float ratio{ (g_WindowWidth / 5) / g_KeyBindsTex.width };

	if (g_World.currentScene < 2)
	{
		SetColor(g_Orange);
		FillEllipse(g_Sun);
		if (g_World.currentScene == 0)
		{
			DrawTexture(g_KeyBindsTex, Rectf{ g_WindowWidth / 4 - 30.f, g_WindowHeight / 4 - 30.f, g_KeyBindsTex.width * ratio, g_KeyBindsTex.height * ratio });
		}
		else if (g_World.currentScene == 1)
		{
			DrawTexture(g_MsgTex, Rectf{ 100.f, 15.f, g_MsgTex.width * ratio, g_MsgTex.height * ratio });
		}
		DrawTexture(g_World.scenes[g_World.currentScene].bgTexture, Rectf{ 0.f, 0.f, g_WindowWidth, g_WindowHeight });
	}
	else if (g_World.currentScene >= 2 && g_Fox.isBarking)
	{
		SetColor(0.f, 0.f, 0.f, 0.2f - g_BarkTime * 0.5f);
		FillRect(Rectf{ 0.f, 0.f, g_WindowWidth, g_WindowHeight });
	}
	if (g_World.currentScene == 2)
	{
		DrawTexture(g_KeyBarkTex, Rectf{ 10.f, g_WindowHeight / 3, g_KeyBarkTex.width * ratio * 1.1f, g_KeyBarkTex.height * ratio * 1.1f });
		DrawTexture(g_World.scenes[g_World.currentScene].bgTexture, Rectf{ 0.f, -10.f, g_WindowWidth, g_WindowHeight });
	}
	else if (g_World.currentScene == 3)
	{
		DrawTexture(g_World.scenes[g_World.currentScene].bgTexture, Rectf{ 0.f, -5.f, g_WindowWidth, g_WindowHeight });
	}
	else if (g_World.currentScene > 3 && g_World.currentScene != 6)
	{
		DrawTexture(g_World.scenes[2].bgTexture, Rectf{ 0.f, -10.f, g_WindowWidth, g_WindowHeight });
	}
	else if (g_World.currentScene == 6)
	{
		const Ellipsef sun{ g_WindowWidth / 5 + 60, 150.f, 35.f, 35.f };

		DrawTexture(g_World.scenes[4].bgTexture, Rectf{ 0.f, -10.f, g_WindowWidth, g_WindowHeight });
		SetColor(g_Orange);
		FillEllipse(sun);
	}
}

void DrawForeground() {
	if (g_World.currentScene < 2) {
		DrawTexture(g_World.scenes[g_World.currentScene].fgTexture, Rectf{ 0.f, 0.f, g_WindowWidth, g_WindowHeight });
		if (!g_World.currentScene)
			DrawTexture(g_Cloud, g_CloudPos);
	}
}

void DrawEnemy() {
	if (g_Enemy.isVisible) {
		Rectf rect{
			g_WindowWidth / 2 - g_Enemy.texture.width / 3,
			0.f,
			g_Enemy.texture.width / 3 * 2,
			g_Enemy.texture.height / 3 * 2
		};

		if (!g_Enemy.isBlinking) {
			SetColor(g_Enemy.color);
			FillTriangle(g_Enemy.triangle);
		}

		DrawTexture(g_Enemy.texture, rect);
	}
}

void DrawBushes() {
	if (g_World.currentScene < 2) return;

	for (int idx{}; idx < g_NumBushes; ++idx) {
		SetColor(g_Orange);
		SetColor(g_White);
		//FillRect(g_Bushes[idx]);
		DrawTexture(g_BushTexture, g_Bushes[idx]);
	}
}

void DrawFox() {
	Rectf rect{ g_Fox.pos.x, g_Fox.pos.y, g_FoxDrawWidth, g_FoxDrawHeight };
	DrawTexture(g_Fox.texture.texture, rect, g_Fox.frame.rect);

	// debug draws
	SetColor(g_Black);
	const Point2f left{ g_Fox.pos.x + g_FoxDrawWidth / 3, g_Fox.pos.y + g_FoxDrawHeight };
	const Point2f right{ g_Fox.pos.x + g_FoxDrawWidth * 3 / 4, g_Fox.pos.y + g_FoxDrawHeight };

	if (g_Fox.isBarking)
	{
		rect.left += g_FoxDrawWidth;
		rect.top += g_FoxDrawHeight / 2;
		rect.width = 32.f;
		rect.height = 32.f;
		DrawTexture(g_Bark, rect);
	}

	//DrawRect(rect, 3.f); 
	//DrawLine(left, right, 3.f);
	//DrawLine(Point2f{ 0.f, g_MinAltitude }, Point2f{ g_WindowWidth, g_MinAltitude }, 3.f);
}

void DrawSnowStorm() {

	for (int i{}; i < g_NbFlakes; ++i)
	{
		SnowFlake& flake{ g_Snowflakes[i] };
		if (g_Enemy.isVisible && !g_Enemy.isBlinking && IsPointInTriangle(flake.position, g_Enemy.triangle))
		{
			SetColor(g_Enemy.color);
			SetColor(Color4f{ 1.f, 0.f, 0.f, .9f });
		}
		else
		{
			SetColor(g_SnowColor);
		}
		const Rectf rect{ flake.position.x, flake.position.y, flake.radius, flake.radius };
		FillRect(rect);
	}
}

void UpdateFox(float elapsedSec, const Uint8* pStates) {
	UpdateFoxAnimState(pStates);
	UpdateFoxJumpState(pStates);
	UpdateFoxDirection(pStates);
	UpdateFoxPos(elapsedSec);
	UpdateFoxFrame(elapsedSec);

	// should be when a key is pressed or released, whenever there's any event
	// so for now it's just updated all the time, is that ok ?

	//if (IsAnyMovementKeyPressed(pStates))  -> fix pour l'instant y a pas les release
	UpdateFoxFrameStartIndex();
	UpdateSteps(elapsedSec);

	UpdateBarking(elapsedSec);

	if (IsFoxInEnemyRange()) {
		g_Fox.pos.x = g_LastSpawn;
		g_Fox.pos.y = g_World.scenes[g_World.currentScene].altitude[g_Fox.pos.x] - g_FoxDrawHeight;
		g_DeathCounter++;

		const int r{ rand() % 3 };
		if (!r)
			std::cout << "yip yip yip " << "\n";
		else if (r == 1)
			std::cout << "wak wak wak " << "\n";
		else
			std::cout << "ay ay ay " << "\n";

		std::cout << "~ Death counter : " << g_DeathCounter << "\n";
	}

}

void UpdateFoxPos(float elapsedSec) {
	const float altitude{ GetAltitude(g_Fox.pos.x + g_FoxDrawWidth / 2) };

	if ((g_World.currentScene == 0 && g_Fox.pos.x < 15.f && g_Fox.vx < 0)
		|| (g_World.currentScene == 6 && g_Fox.pos.x > g_WindowWidth - 15.f && g_Fox.vx > 0))
	{
		g_Fox.vx *= 0;
	}

	g_Fox.pos.x += elapsedSec * g_Fox.vx;
	if (g_Fox.isJumping)
		g_Fox.pos.y += elapsedSec * g_Fox.vy;
	else if (!g_IsOnPlatform)
		g_Fox.pos.y = altitude - g_FoxDrawHeight;
	g_StepOffset += fabsf(elapsedSec * g_Fox.vx);

	if (g_Fox.isJumping) {
		g_Fox.vy += elapsedSec * g_Gravity;
	}
	if (g_Fox.animState == AnimState::Idle) {
		g_Fox.vx = 0.f;
	}
}

bool IsAnyMovementKeyPressed(const Uint8* pStates) {
	return pStates[MVT_LEFT] || pStates[MVT_RIGHT] || pStates[SDL_SCANCODE_SPACE];
}

void UpdateFoxFrameStartIndex() {
	std::string key{};

	switch (g_Fox.animState)
	{
	case AnimState::Sleep:
		key = "sleep";
		break;
	case AnimState::Jump:
		key = g_Fox.dir.x == 1.f ? "jumpRight" : "jumpLeft";
		break;
	case AnimState::Walk:
		key = g_Fox.dir.x == 1.f ? "walkRight" : "walkLeft";
		break;
	case AnimState::Idle:
	default:
		key = g_Fox.dir.x == 1.f ? "idleRight" : "idleLeft";
		break;
	}

	g_Fox.frame.nrFrames = g_AnimFrames[key].nrFrames;
	g_Fox.frame.startIndex = GetIndex(g_AnimFrames[key].row, g_AnimFrames[key].col, g_Fox.texture.nrCols);
}

void	UpdateFoxFrame(float elapsedSec) {
	const float frameRate{ 1.f / 8.f };

	g_FrameTime += elapsedSec;
	if (g_FrameTime >= frameRate) {
		g_FrameTime = 0.f;
		g_Fox.frame.index = (g_Fox.frame.index + 1) % g_Fox.frame.nrFrames;
		g_Fox.frame.rect = GetFrameRectf(g_Fox.frame.startIndex + g_Fox.frame.index);
	}
}

//void	UpdateFoxJumpState(const Uint8* pStates) {
//	const float altitude{ GetAltitude(g_Fox.pos.x + g_FoxDrawWidth / 2) };
//
//	if (!g_Fox.isJumping && pStates[SDL_SCANCODE_SPACE]) {
//		g_Fox.isJumping = true;
//		g_Fox.vy = g_JumpPower;
//		g_Fox.pos.y -= 2.f;
//	}
//	else if (g_Fox.isJumping && (g_Fox.pos.y > altitude - g_FoxDrawHeight )) {
//		g_Fox.isJumping = false;
//		g_Fox.vy = 0.f;
//		g_Fox.pos.y = GetAltitude(g_Fox.pos.x + g_FoxDrawWidth / 2) - g_FoxDrawHeight;
//	}
//}

void UpdateFoxJumpState(const Uint8* pStates)
{
	const float altitude{ GetAltitude(g_Fox.pos.x + g_FoxDrawWidth / 2) };
	bool foundPlatform{};
	// Start jump
	if (!g_Fox.isJumping && pStates[SDL_SCANCODE_SPACE])
	{
		g_Fox.isJumping = true;
		g_Fox.vy = g_JumpPower;
		g_Fox.pos.y -= 2.f;
	}
	else
	{
		Rectf foxRect
		{
			g_Fox.pos.x,
			g_Fox.pos.y,
			g_FoxDrawWidth,
			g_FoxDrawHeight
		};

		// Check platform landing
		for (const Rectf& platform : g_Platforms[g_World.currentScene])
		{
			float foxBottom = g_Fox.pos.y + g_FoxDrawHeight;

			float platformTop = platform.top;

			bool insideX =
				foxRect.left + foxRect.width > platform.left &&
				foxRect.left < platform.left + platform.width;

			bool landing = foxBottom >= platformTop && foxBottom <= platformTop + 5.f;

			// Landing on platform
			if (insideX && landing)
			{
				if (g_Fox.vy > 0.f)
				{
					g_Fox.isJumping = false;
					g_Fox.vy = 0.f;

					g_IsOnPlatform = true;
					g_Fox.animState = AnimState::Idle;
					g_SleepTime = 0.f;
					g_Fox.pos.y = platformTop - g_FoxDrawHeight;

				}
				return;
			}
		}
		g_IsOnPlatform = false;

		// Ground landing
		if (g_Fox.isJumping &&
			(g_Fox.pos.y > altitude - g_FoxDrawHeight))
		{
			g_Fox.isJumping = false;
			g_Fox.vy = 0.f;
			g_Fox.pos.y = altitude - g_FoxDrawHeight;
		}
	}
}

void	UpdateFoxAnimState(const Uint8* pStates) {
	const float altitude{ GetAltitude(g_Fox.pos.x + g_FoxDrawWidth / 2) };

	if ((pStates[MVT_RIGHT] || pStates[MVT_LEFT]) && g_Fox.animState != AnimState::Jump) {
		g_Fox.animState = AnimState::Walk;
	}
	else if (g_Fox.animState == AnimState::Walk && (!pStates[MVT_RIGHT] && !pStates[MVT_LEFT])) {
		g_Fox.animState = AnimState::Idle;
		g_SleepTime = 0.f;
	}
	if (!g_Fox.isJumping && pStates[SDL_SCANCODE_SPACE]) {
		g_Fox.animState = AnimState::Jump;
	}
	else if (g_Fox.isJumping && g_Fox.pos.y > altitude - g_FoxDrawHeight) {
		g_Fox.animState = AnimState::Idle;
		g_SleepTime = 0.f;
	}

	if (g_Fox.animState == AnimState::Idle && g_SleepTime > 5.f) {
		g_Fox.animState = AnimState::Sleep;
	}
}
void	UpdateFoxDirection(const Uint8* pStates) {
	if (pStates[MVT_RIGHT]) {
		g_Fox.vx = g_FoxSpeed * g_Fox.dir.x;
		g_Fox.dir.x = 1.f;
	}
	else if (pStates[MVT_LEFT]) {
		g_Fox.vx = g_FoxSpeed * g_Fox.dir.x;
		g_Fox.dir.x = -1.f;
	}
}

void	DrawSteps() {
	for (int i{}; i < g_NrSteps; ++i) {
		if (g_Steps[i].isOn) {
			SetColor(g_Steps[i].color);
			FillEllipse(g_Steps[i].shape);
		}
	}
}

void	UpdateSteps(float elapsedSec) {
	const float width{ g_Fox.frame.rect.width };
	const float altitude{ GetAltitude(g_Fox.pos.x + g_FoxDrawWidth / 2) };

	if (g_StepOffset > 15.f && !g_Fox.isJumping && !g_IsOnPlatform) {
		g_StepOffset = (g_StepIndex % 4 ? 0.f : -30.f);
		g_StepIndex = (g_StepIndex + 1) % g_NrSteps;

		g_Steps[g_StepIndex].shape.center.x = g_Fox.pos.x + (g_Fox.dir.x == -1 ? width * 2 : width);
		g_Steps[g_StepIndex].shape.center.y = altitude + (g_StepIndex & 1 ? (rand() % 7 - 3) + 7.f : (rand() % 7 - 3)) - 5.f;
		g_Steps[g_StepIndex].isOn = true;
		g_Steps[g_StepIndex].coolDown = 0.f;
		g_Steps[g_StepIndex].color = g_DarkGrey;

		const int last4{ (g_StepIndex + 1) % g_NrSteps }, last2{ (g_StepIndex + 3) % g_NrSteps };
		for (int index{}; index < g_NrSteps; ++index) {
			if (index == last4 || index == (last4 + 1) % g_NrSteps)
				g_Steps[index].color = g_LightGrey;
			else if (index == last2 || index == (last2 + 1) % g_NrSteps)
				g_Steps[index].color = g_MedGrey;
		}
	}

	for (int i{}; i < g_NrSteps; ++i) {
		g_Steps[i].coolDown += elapsedSec;

		if (g_Steps[i].coolDown > 5.f)
			g_Steps[i].isOn = false;
		else if (g_Steps[i].coolDown > 4.f)
			g_Steps[i].color = g_LightGrey;
		else if (g_Steps[i].coolDown > 3.f && g_Steps[i].color.a != g_LightGrey.a)
			g_Steps[i].color = g_MedGrey;
	}
}

void UpdateBarking(float elapsedSec)
{
	if (!g_Fox.isBarking)
	{
		g_BarkCooldown += elapsedSec;
		return;
	}

	const float max{ .5f };

	g_BarkTime += elapsedSec;
	if (g_BarkTime > max)
	{
		g_BarkTime = 0.f;
		g_Fox.isBarking = false;
	}
}

void	UpdateWorldScene() {
	if (g_Fox.pos.x + g_FoxDrawWidth / 2 <= 0.f) {
		g_World.currentScene -= 1;
		if (g_World.currentScene < 0)
			g_World.currentScene = g_World.nrScenes - 1;
		g_Fox.pos.x = g_WindowWidth - g_FoxDrawWidth / 2;
		g_LastSpawn = g_SpawnPosRight;
		InitFoxSteps();
		InitScene();
	}
	else if (g_Fox.pos.x + g_FoxDrawWidth / 2 >= g_WindowWidth) {
		g_World.currentScene += 1;
		g_World.currentScene %= g_World.nrScenes;
		g_Fox.pos.x = 20.f;
		g_LastSpawn = g_SpawnPosLeft;
		InitFoxSteps();
		InitScene();
	}
}

void UpdateEnemy(float elapsedSec) {
	UpdateEnemyBlinking(elapsedSec);
	UpdateEnemyTriangle(g_World.currentScene, elapsedSec);
}

void UpdateEnemyBlinking(float elapsedSec) {
	if (g_World.currentScene == 4) // blinking scene
	{
		if (!g_Enemy.isBlinking) {
			g_Enemy.blinkCooldown -= elapsedSec;
			if (g_Enemy.blinkCooldown < 0.f) {
				g_Enemy.isBlinking = true;
				g_Enemy.blinkCooldown = 5.f;
			}
		}
		else {
			g_Enemy.blinkTime += elapsedSec;
			if (g_Enemy.blinkTime > 2.f) {
				g_Enemy.blinkTime = 0.f;
				g_Enemy.isBlinking = false;
			}
		}
	}
}

void UpdateEnemyTriangle(int index, float elapsedSec) {

	if (!g_Enemy.isVisible) return;

	g_Enemy.triangle.p1.x = g_WindowWidth / 2;

	if (index == 2 || index == 5 || index == 6) {
		g_Enemy.triangle.p2.x = (g_WindowWidth / 3) * sinf(g_Time / 2) + g_WindowWidth / 3;
		g_Enemy.triangle.p3.x = (g_WindowWidth / 3) * sinf(g_Time / 2) + g_WindowWidth / 2;
	}
	if (index == 3) {
		//g_Enemy.triangle.p1.x = (g_WindowWidth / 4) * sinf(g_Time / 2) + g_WindowWidth / 2;
		g_Enemy.triangle.p2.x = (g_WindowWidth / 3) * sinf(g_Time / 2 + M_PI / 3);
		g_Enemy.triangle.p3.x = (g_WindowWidth / 3) * sinf(g_Time / 2 + M_PI / 3) + g_WindowWidth / 2;
	}
	if (index == 4) {
		g_Enemy.triangle.p2.x = 0.f;
		g_Enemy.triangle.p3.x = g_WindowWidth;
	}
}

void UpdateSnowStorm(float elapsedSec)
{
	for (int i{}; i < g_NbFlakes; ++i)
	{
		SnowFlake& flake{ g_Snowflakes[i] };

		//direction is left down
		flake.position.x -= flake.speed * g_StormSpeed * g_StormSpeed / 3 * elapsedSec;
		flake.position.y += flake.speed * g_StormSpeed * elapsedSec;

		float max{ g_WindowHeight };

		// if outside respawn at top/right
		if (g_World.currentScene != 6 && (flake.position.x < 0 || flake.position.y > max))
		{
			flake.position.x = float(rand() % int(g_WindowWidth + (300 * g_StormSpeed)));
			flake.position.y = float(-(rand() % 100));
		}
	}
}

void UpdateStormData(float speed, int nbFlakes)
{
	if (g_World.currentScene == 0) {
		g_StormSpeed = 1.f;
		g_NbFlakes = 750;
	}
	else if (g_World.currentScene == 1) {
		g_StormSpeed = 2.f;
		g_NbFlakes = 750;
	}
	else if (g_World.currentScene == 2) {
		g_StormSpeed = 3.f;
		g_NbFlakes = 1500;
	}
	else if (g_World.currentScene < 4) {
		g_StormSpeed = 4.f;
		g_NbFlakes = 1500;
	}
	else if (g_World.currentScene == 4) {
		g_StormSpeed = 3.f;
		g_NbFlakes = 750;
	}
	else if (g_World.currentScene >= 5) {
		g_StormSpeed = 4.f;
		g_NbFlakes = 1000;
	}
}


void	InitPlatforms()
{
	const float w{ 120.f }, h{ 30.f };
	for (int idx{}; idx < g_World.nrScenes; ++idx)
	{
		g_Platforms[idx] = std::vector<Rectf>{};
	}

	g_Platforms[5] = std::vector<Rectf>{
		Rectf{g_WindowWidth / 3, g_MinAltitude - 90.f, w, h},
		Rectf{g_WindowWidth / 3 + 230.f, g_MinAltitude - 130.f, w, h}
	};
}

std::vector<Rectf>& GetPlatformsFromLevel(int level)
{
	return g_Platforms[level];
}

void	DrawPlatforms()
{
	if (g_World.currentScene < 5) return;

	std::vector<Rectf>& rects{ g_Platforms[g_World.currentScene] };
	size_t max{ rects.size() };
	for (size_t idx{}; idx < max; ++idx)
	{
		SetColor(g_White);
		FillRect(rects[idx]);
	}
}


// utils

Rectf	GetFrameRectf(int index) {
	const int nrCols{ g_Fox.texture.nrCols };
	const float width{ g_Fox.texture.frameWidth }, height{ g_Fox.texture.frameHeight };

	return Rectf(GetCol(index, nrCols) * width, GetRow(index, nrCols) * height, width, height);
}

float GetAltitude(float index) {
	if (index < 0.f)
		index = 0.f;
	else if (index >= g_WindowWidth)
		index = g_WindowWidth;
	return g_World.scenes[g_World.currentScene].altitude[static_cast<unsigned int>(index)];
}

bool IsPointInTriangle(const Point2f& p, const Trianglef& triangle)
{
	if (isT) return false;

	auto Sign = [](const Point2f& p1, const Point2f& p2, const Point2f& p3)
		{
			return (p1.x - p3.x) * (p2.y - p3.y)
				- (p2.x - p3.x) * (p1.y - p3.y);
		};

	float d1 = Sign(p, triangle.p1, triangle.p2);
	float d2 = Sign(p, triangle.p2, triangle.p3);
	float d3 = Sign(p, triangle.p3, triangle.p1);

	bool hasNeg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	bool hasPos = (d1 > 0) || (d2 > 0) || (d3 > 0);

	return !(hasNeg && hasPos);
}

bool IsFoxInEnemyRange() {
	if (g_Enemy.isBlinking || !g_Enemy.isVisible) return false;

	const Point2f left{ g_Fox.pos.x + g_FoxDrawWidth / 3, g_Fox.pos.y + g_FoxDrawHeight };
	const Point2f right{ g_Fox.pos.x + g_FoxDrawWidth * 3 / 4, g_Fox.pos.y + g_FoxDrawHeight };
	bool isInLeft{ IsPointInTriangle(left, g_Enemy.triangle) },
		isInRight{ IsPointInTriangle(right, g_Enemy.triangle) };

	for (int idx{}; idx < g_NumBushes; ++idx) {
		if (isInLeft && IsPointInRect(left, g_Bushes[idx]))
			isInLeft = false;
		if (isInRight && IsPointInRect(right, g_Bushes[idx])) {
			isInRight = false;
		}
	}

	return isInLeft || isInRight;
}

void DrawTriangle(const Trianglef& t, float lineWidth)
{
	DrawTriangle(t.p1.x, t.p1.y, t.p2.x, t.p2.y, t.p3.x, t.p3.y, lineWidth);
}

void FillTriangle(const Trianglef& t)
{
	FillTriangle(t.p1.x, t.p1.y, t.p2.x, t.p2.y, t.p3.x, t.p3.y);
}

#pragma endregion ownDefinitions