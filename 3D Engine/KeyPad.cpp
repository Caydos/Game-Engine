#include "KeyPad.h"
#include "Texture.h"
#include "Sprite.h"

static bool initialized = false;
static std::shared_mutex mtx;
static bool displayKeyPad = false;
static Sprite keypadSprites[13];
static const glm::vec2 keypadBackSize(486, 525);
static const glm::vec2 keypadPosition(1920- keypadBackSize.x, 1080- keypadBackSize.y);
static const glm::vec2 keypadKeySize(133.5, 117.75);

bool displayed = false;
static const char* keyNames[] = {
	"Button 1.jpg",
	"Button 2.jpg",
	"Button 3.jpg",
	"Button 4.jpg",
	"Button 5.jpg",
	"Button 6.jpg",
	"Button 7.jpg",
	"Button 8.jpg",
	"Button 9.jpg",

	"Button ast.jpg",
	"Button 0.jpg",
	"Button #.jpg",
};

void KeyPad::Initialize(GameData* _gameData)
{
	GameData* gameData = GetGameData();
	keypadSprites[0].Load("../Textures/Keypad/Background.jpg", glm::vec3(keypadPosition, 0.0), glm::vec3(keypadBackSize, 0.0), 1);
	keypadSprites[0].BindShader(gameData->shaders[Shaders::UI]);

	const float padding = 22.5;
	for (size_t i = 0; i < 12; i++)
	{
		int row = i / 3;
		int column = i % 3;
		Texture* texture = new Texture;
		texture->LoadFromFile((std::string("../Textures/Keypad/") + keyNames[i]).c_str());
		keypadSprites[i + 1].SetTexture(texture);
		keypadSprites[i + 1].SetScale(glm::vec3(keypadKeySize, 0.0f));
		keypadSprites[i + 1].SetPosition(glm::vec3(padding + column * (keypadKeySize.x + padding) + keypadPosition.x, padding / 2 + row * (keypadKeySize.y + padding / 2) + keypadPosition.y, 0.0));
		keypadSprites[i + 1].BindShader(gameData->shaders[Shaders::UI]);
	}
	initialized = true;
}

void KeyPad::Tick(GameData* _gameData)
{
	if (!initialized) { Initialize(_gameData); }
	std::unique_lock<std::shared_mutex> lock(mtx);
	if (displayed)
	{
		for (size_t spriteId = 0; spriteId < 13; spriteId++)
		{
			keypadSprites[spriteId].Draw();
		}
		if (_gameData->window.IsKeyPressed(Keys::ESCAPE))
		{
			displayed = false;
			_gameData->window.Focus(true);
		}
	}
}

void KeyPad::CleanUp()
{
}

void KeyPad::Interaction()
{
	std::unique_lock<std::shared_mutex> lock(mtx);
	displayed = true;
	GetGameData()->window.Focus(false);
}
