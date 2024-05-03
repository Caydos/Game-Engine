#include "Blocks.h"
#include <glm/gtx/matrix_decompose.hpp>
#include "Scene.h"

Blocks::Block::Block()
{
	this->inScene = false;
	this->type = nullptr;
	this->parent = nullptr;
	this->parent = nullptr;
	this->model = nullptr;
	//this->light = nullptr;
}
Blocks::Block::~Block() {}

Blocks::BlockType* Blocks::Block::GetType()
{
	return this->type;
}

void Blocks::Block::SetType(BlockType* _type)
{
	this->type = _type;
	std::cout << "Type set once" << std::endl;
	if (_type->IsLightEmitter() && this->light == nullptr)
	{

		this->light = Scene::Lights::CreateSpot();
		*this->light = _type->GetLight();
		// Transform the position
		glm::vec4 transformedLightPosition = glm::vec4(this->light->position, 1.0f) * *this->model;
		glm::vec3 newLightPosition = glm::vec3(transformedLightPosition);

		// Transform the direction
		glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(*this->model)));
		glm::vec3 newLightDirection = normalMatrix * this->light->direction;

		this->light->position = newLightPosition;
		this->light->direction = newLightDirection;
		Lighting::UpdateSpot(this->light);
		Scene::Lights::UpdateSpot(this->light);
	}
}

glm::mat4* Blocks::Block::GetParent()
{
	return this->parent;
}

void Blocks::Block::SetParent(glm::mat4* _parent)
{
	this->parent = _parent;
	this->ApplyTransformation();
}

void Blocks::Block::GenerateModel()
{
	this->model = new glm::mat4;
	*this->model = glm::mat4(1.0f);
}

glm::mat4 Blocks::Block::GetModel()
{
	return *this->model;
}

glm::mat4* Blocks::Block::GetModelAddress()
{
	return this->model;
}

void Blocks::Block::EraseModel()
{
	if (this->model != nullptr)
	{
		delete this->model;
	}
}

void Blocks::Block::ApplyTransformation()
{
	if (this->parent != nullptr)
	{
		*this->model = *parent;
	}
	else
	{
		*this->model = glm::mat4(1.0f);
	}

	*this->model = glm::translate(*this->model, this->position);
	*this->model = glm::rotate(*this->model, glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	*this->model = glm::rotate(*this->model, glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	*this->model = glm::rotate(*this->model, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	*this->model = glm::scale(*this->model, this->scale);


	this->CalculateBoundingBox();
	if (this->type != nullptr && this->type->IsLightEmitter() && this->light != nullptr)
	{
		//*this->light = this->type->GetLight();
		// Transform the position
		Lighting::Spot typeLight = this->type->GetLight();
		this->light->direction = typeLight.direction;
		this->light->position = typeLight.position;

		this->light->ambient = typeLight.ambient;
		this->light->diffuse = typeLight.diffuse;
		this->light->specular = typeLight.specular;

		this->light->constant = typeLight.constant;
		this->light->linear = typeLight.linear;
		this->light->quadratic= typeLight.quadratic;


		this->light->cutOff= typeLight.cutOff;
		this->light->outerCutOff= typeLight.outerCutOff;


		glm::vec4 transformedLightPosition = *this->model * glm::vec4(this->light->position, 1.0f);
		glm::vec3 newLightPosition = glm::vec3(transformedLightPosition);

		// Transform the direction
		glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(*this->model)));
		glm::vec3 newLightDirection = normalMatrix * this->light->direction;


		this->light->position = newLightPosition;
		this->light->direction = newLightDirection;


		Lighting::UpdateSpot(this->light);
		Scene::Lights::UpdateSpot(this->light);
		//std::cout << newLightPosition.x << " " << newLightPosition.y << " " << newLightPosition.z << std::endl;
		//std::cout << newLightDirection.x << " " << newLightDirection.y << " " << newLightDirection.z << std::endl << std::endl;
	}
}

glm::vec3 Blocks::Block::GetPosition() { return this->position; }
void Blocks::Block::SetPosition(float _x, float _y, float _z)
{
	this->position = glm::vec3(_x, _y, _z);
	ApplyTransformation();
}
void Blocks::Block::SetPosition(glm::vec3 _position)
{
	this->position = _position;
	ApplyTransformation();
}
void Blocks::Block::Move(glm::vec3 _position)
{
	this->position += _position;
	ApplyTransformation();
}
void Blocks::Block::Move(float _x, float _y, float _z)
{
	this->position.x += _x;
	this->position.y += _y;
	this->position.z += _z;
	ApplyTransformation();
}

glm::vec3 Blocks::Block::GetRotation() { return this->rotation; }
void Blocks::Block::SetRotation(float _x, float _y, float _z)
{
	this->rotation = glm::vec3(_x, _y, _z);
	ApplyTransformation();
}
void Blocks::Block::SetRotation(glm::vec3 _rotation)
{
	this->rotation = _rotation;
	ApplyTransformation();
}
void Blocks::Block::Rotate(glm::vec3 _rotation)
{
	this->rotation += _rotation;
	ApplyTransformation();
}
void Blocks::Block::Rotate(float _x, float _y, float _z)
{
	this->rotation.x += _x;
	this->rotation.y += _y;
	this->rotation.z += _z;
	ApplyTransformation();
}

glm::vec3 Blocks::Block::GetScale() { return this->scale; }
void Blocks::Block::SetScale(float _x, float _y, float _z)
{
	this->scale = glm::vec3(_x, _y, _z);
	ApplyTransformation();
}
void Blocks::Block::SetScale(float _scale)
{
	this->scale = glm::vec3(_scale, _scale, _scale);
	ApplyTransformation();
}
void Blocks::Block::SetScale(glm::vec3 _scale)
{
	this->scale = _scale;
	ApplyTransformation();
}
void Blocks::Block::Scale(glm::vec3 _scale)
{
	this->scale += _scale;
	ApplyTransformation();
}
void Blocks::Block::Scale(float _x, float _y, float _z)
{
	this->scale.x += _x;
	this->scale.y += _y;
	this->scale.z += _z;
	ApplyTransformation();
}

Bounds::Box Blocks::Block::GetBoundingBox()
{
	return this->boundingBox;
}

void Blocks::Block::CalculateBoundingBox()
{
	if (this->model == nullptr) { return; }
	float halfSideLength = 0.5f;

	std::vector<glm::vec3> vertices = {
		// Top vertices
		glm::vec3(-halfSideLength, halfSideLength, halfSideLength), // Top-front-left
		glm::vec3(halfSideLength, halfSideLength, halfSideLength), // Top-front-right
		glm::vec3(halfSideLength, halfSideLength, -halfSideLength), // Top-back-right
		glm::vec3(-halfSideLength, halfSideLength, -halfSideLength), // Top-back-left

		// Bottom vertices
		glm::vec3(-halfSideLength, -halfSideLength, halfSideLength), // Bottom-front-left
		glm::vec3(halfSideLength, -halfSideLength, halfSideLength), // Bottom-front-right
		glm::vec3(halfSideLength, -halfSideLength, -halfSideLength), // Bottom-back-right
		glm::vec3(-halfSideLength, -halfSideLength, -halfSideLength), // Bottom-back-left
	};
	glm::vec3 globalMin = glm::vec3(FLT_MAX);
	glm::vec3 globalMax = glm::vec3(-FLT_MAX);

	glm::mat4 tempModel(1.0f);
	if (this->parent != nullptr)
	{
		tempModel = *parent;
	}


	tempModel = glm::translate(tempModel, this->position);
	tempModel = glm::scale(tempModel, this->scale);


	glm::vec4 transformedVertex = *this->model * glm::vec4(vertices[0], 1.0);
	this->boundingBox.min = transformedVertex;
	this->boundingBox.max = transformedVertex;

	for (auto vertex : vertices)
	{
		glm::vec4 transformedVertex = *this->model * glm::vec4(vertex, 1.0);

		this->boundingBox.min.x = std::min(this->boundingBox.min.x, transformedVertex.x);
		this->boundingBox.min.y = std::min(this->boundingBox.min.y, transformedVertex.y);
		this->boundingBox.min.z = std::min(this->boundingBox.min.z, transformedVertex.z);

		this->boundingBox.max.x = std::max(this->boundingBox.max.x, transformedVertex.x);
		this->boundingBox.max.y = std::max(this->boundingBox.max.y, transformedVertex.y);
		this->boundingBox.max.z = std::max(this->boundingBox.max.z, transformedVertex.z);
	}

	//glm::vec3 scale;
	//glm::quat rotation;
	//glm::vec3 translation;
	//glm::vec3 skew;
	//glm::vec4 perspective;
	//glm::decompose(*this->model, scale, rotation, translation, skew, perspective);

	//boundingBox.min = translation - scale / 2.0f;
	//boundingBox.max = translation + scale / 2.0f;
}
