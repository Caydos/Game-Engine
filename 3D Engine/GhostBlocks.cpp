#include "Blocks.h"
#include "DefaultVertices.h"

static unsigned int VAO;
static unsigned int vertexVBO;
static unsigned int instanceVBO;
static std::vector<glm::mat4> models;
static Colors::Color color = Colors::Color(1.0f, 1.0f, 1.0f, 0.3f);
static glm::vec3 scale;
static glm::vec3 position;
static bool ghostInitialized = false;



void Blocks::Ghost::Draw(GameData* _gameData)
{
	if (!ghostInitialized)
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &vertexVBO);
		glGenBuffers(1, &instanceVBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
		glBufferData(GL_ARRAY_BUFFER, 288 * sizeof(float), &cubeVertices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Texture attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// Lightning normal attribute
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(2);
		models.push_back(glm::mat4(1.0f));
		ghostInitialized = true;
	}

	Shaders::Shader* shader = _gameData->shaders[Shaders::WORLD_OBJECT];
	shader->use();
	shader->setBool("instanceUsage", true);

	shader->setVec4("color", glm::vec4(color.values[0], color.values[1], color.values[2], color.values[3]));
	shader->setBool("lightDependent", false);
	shader->setInt("mode", 0);
	shader->setFloat("opacity", 1.0f);

	glBindVertexArray(VAO);

	glDrawArraysInstanced(GL_TRIANGLES, 0, 36, models.size());

	glBindVertexArray(0);
	shader->setBool("instanceUsage", false);

}

void Blocks::Ghost::CalculateTransformations()
{
	for (int i = 0; i < models.size(); i++)
	{
		models[i] = glm::mat4(1.0f);
		models[i] = glm::translate(models[i], position);
		models[i] = glm::scale(models[i], scale);
	}
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, models.size() * sizeof(glm::mat4), &models[0], GL_STATIC_DRAW);

	for (unsigned int i = 0; i < 4; i++)
	{
		glEnableVertexAttribArray(i + 3); // 4 is an offset, assuming 0, 1, 2, 3 are used for the cube's vertex data
		glVertexAttribPointer(i + 3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i));
		glVertexAttribDivisor(i + 3, 1); // Tell OpenGL this is an instanced vertex attribute.
	}
}

void Blocks::Ghost::SetScale(glm::vec3 _scale)
{
	scale = _scale;
	CalculateTransformations();
}

void Blocks::Ghost::SetPosition(glm::vec3 _position)
{
	position = _position;
	CalculateTransformations();
}

