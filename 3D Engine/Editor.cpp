#include "Editor.h"
#include "Files.h"
#include "Colors.h"
#include "Scene.h"
#include "Clock.h"
#include "ObjectsMenu.h"
#include "LightMenu.h"
#include "Crosshair.h"
#include "Set.h"

static bool initialized = false;
static bool displayed = false;
static Clock inputClock;

void Editor::Initialize(GameData* _gameData)
{
	initialized = true;
}

bool Editor::IsDisplayed()
{
	return displayed;
}
void Editor::SetDisplay(bool _status)
{
	displayed = _status;
}

#pragma region Menu
static Editor::OutputFileType outputFileType;
static char filename[128] = ""; // Buffer for the filename
static bool showFilenameInput = false; // To toggle the visibility of input field
static float mainMenuBarHeight = 0;
static bool isLeftSideMenuOpen = true;


void Editor::Menu(GameData* _gameData)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Tool"))
		{
			if (ImGui::BeginMenu("Settings##ToolBar"))
			{
				if (ImGui::SliderFloat("Sensitivity##ToolBar", &_gameData->settings.sentivity, 0.01f, 1.5f))
				{
					_gameData->camera->MouseSensitivity = _gameData->settings.sentivity;
				}
				if (ImGui::MenuItem("Save##ToolBar"))
				{
					Settings::Save(_gameData);
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Close##ToolBar"))
			{
				_gameData->window.Close();
			}
			ImGui::EndMenu();

		}
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::BeginMenu("New"))
			{
				if (ImGui::MenuItem("Set"))
				{
					outputFileType = Editor::SET;
					showFilenameInput = true;
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Open"))
			{

				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
		mainMenuBarHeight = ImGui::GetItemRectSize().y;
	}

	glm::ivec2 windowDimensions = _gameData->window.GetDimensions();
	ImGui::SetNextWindowPos(ImVec2(0, mainMenuBarHeight));
	ImGui::SetNextWindowSize(ImVec2(windowDimensions.x * 0.2, windowDimensions.y - mainMenuBarHeight));

	if (ImGui::Begin("Configuration", &isLeftSideMenuOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysVerticalScrollbar))
	{
		if (ImGui::TreeNode("Scene Configuration"))
		{
			if (ImGui::TreeNode("Background Color"))
			{
				ImVec4 actualColor = Scene::GetClearColor().imguiValues;
				for (int i = 0; i < IM_ARRAYSIZE(Colors::DefaultColors); ++i)
				{
					ImGui::PushID(i);
					if (ImGui::ColorButton("##DefaultColors", Colors::DefaultColors[i]))
					{
						actualColor = Colors::DefaultColors[i];
						Scene::SetClearColor(Colors::Color(actualColor.x, actualColor.y, actualColor.z, actualColor.w));
					}
					ImGui::PopID();
					ImGui::SameLine();
				}
				ImGui::NewLine();
				if (ImGui::ColorPicker4("Custom Color", (float*)&actualColor))
				{
					Scene::SetClearColor(Colors::Color(actualColor.x, actualColor.y, actualColor.z, actualColor.w));
				}
				ImGui::TreePop();
			}

			LightMenu::Basic(_gameData);

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Camera Configuration"))
		{
			int cameraFov = Scene::World::GetCameraFov();
			ImGui::Text("FOV :");
			if (ImGui::SliderInt("##FOV", &cameraFov, 60, 90))
			{
				Scene::World::SetCameraFov(cameraFov);
			}
			Camera* cam = Scene::World::GetCamera();
			ImGui::Text("Speed :");
			ImGui::SliderFloat("##CamSpeed", &cam->MovementSpeed, 0.01f, 20.0f);

			if (ImGui::Button("Respawn"))
			{
				cam->Position = glm::vec3(.0f);
			}

			ImGui::TreePop();
		}
		Crosshairs::Menu(_gameData);

		ImGui::End();
	}

	Sets::Menu(_gameData);

	if (showFilenameInput)
	{
		ImGui::OpenPopup("Enter Filename");
	}

	if (ImGui::BeginPopupModal("Enter Filename", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::InputText("Filename", filename, IM_ARRAYSIZE(filename));
		if (ImGui::Button("OK"))
		{
			switch (outputFileType)
			{
			case Editor::SET:
			{
				Sets::Set* newSet = Sets::Create();
				newSet->SetName(filename);
				break;
			}
			default:
				break;
			}

			showFilenameInput = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			showFilenameInput = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

#pragma endregion


void Editor::Tick(GameData* _gameData)
{
	if (!initialized) { Initialize(_gameData); }
	if (displayed)
	{
		Menu(_gameData);
	}
	if (_gameData->window.IsKeyPressed(Keys::F1) && inputClock.GetElapsedTime() > 125)
	{
		_gameData->window.Focus(displayed);
		SetDisplay(!displayed);
		inputClock.Restart();
	}
	else if (_gameData->window.IsKeyPressed(Keys::ESCAPE) && inputClock.GetElapsedTime() > 125)
	{
		if (displayed)
		{
			SetDisplay(false);
			_gameData->window.Focus(true);
		}
		inputClock.Restart();
	}
}

void Editor::Close()
{
	//ObjectsMenu::CleanUp();
	LightMenu::CleanUp();
}