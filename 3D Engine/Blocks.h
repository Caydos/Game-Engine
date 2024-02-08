#ifndef BLOCKS_H
#define BLOCKS_H
#include "Common.h"
#include "DefaultVertices.h"
#include "Texture.h"
//#include <glm/ext/matrix_float4x4.hpp>

#define BLOCKS_DIRECTORY "../Blocks/"
#define BLOCKS_FILE_EXTENSION ".json"
#define BLOCKS_TEXTURE_EXTENSION ".jpg"


namespace Blocks
{
	class BlockType
	{
	public:
		BlockType();
		~BlockType();

		void GenerateGraphicsBuffers();
		void RemoveGraphicsBuffers();

		void BindGraphicsBuffers();

		std::string GetName();
		void SetName(std::string _name);

		void SetVertices();
		void RegenerateMatrices();



		void InsertModel(glm::mat4* _model);

		void RemoveModel(glm::mat4* _model);





		Shaders::Shader* GetShader();
		void SetShader(Shaders::Shader* _shader);

		Texture* GetTexture();
		void SetTexture(Texture* _texture);

		Colors::Color GetColor();
		void SetColor(Colors::Color _color);


		glm::vec3 GetScale();
		void SetScale(glm::vec3 _scale);

		bool GetLightDependency();
		void SetLightDependency(bool _lightDependency);

		glm::vec3 GetDiffuse();
		void SetDiffuse(glm::vec3 _diffuse);

		glm::vec3 GetSpecular();
		void SetSpecular(glm::vec3 _specular);

		float GetShininess();
		void SetShininess(float _shininess);
		void Draw();


		void AskForRefresh();

	private:

		bool graphicsLoaded;
		unsigned int VAO;
		unsigned int vertexVBO;
		unsigned int instanceVBO;
		std::string name;

		float vertices[288];

		unsigned int unmappedBufferLastId;
		unsigned int graphicsBufferSize;
		std::vector<glm::mat4> models;
		std::vector<glm::mat4*> modelsAddresses;


		Shaders::Shader* shader;
		Texture* texture;
		Colors::Color color;

		glm::vec3 scale;
		bool lightDependent;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shininess;


		bool refreshInQueue;
	};

	class Block
	{
	public:
		Block();
		~Block();

		BlockType* GetType();
		void SetType(BlockType* _type);

		glm::mat4* GetParent();
		void SetParent(glm::mat4* _parent);

		void InsertInScene();
		void RemoveFromScene();

		glm::mat4 GetModel();
		void GenerateModel();
		void EraseModel();

		void ApplyTransformation();

		glm::vec3 GetPosition();
		void SetPosition(float _x, float _y, float _z);
		void SetPosition(glm::vec3 _position);

		void Move(glm::vec3 _position);
		void Move(float _x, float _y, float _z);

		glm::vec3 GetRotation();
		void SetRotation(float _x, float _y, float _z);
		void SetRotation(glm::vec3 _rotation);

		void Rotate(glm::vec3 _rotation);
		void Rotate(float _x, float _y, float _z);

		glm::vec3 GetScale();
		void SetScale(float _x, float _y, float _z);
		void SetScale(float _scale);
		void SetScale(glm::vec3 _scale);

		void Scale(glm::vec3 _scale);
		void Scale(float _x, float _y, float _z);

	private:
		bool inScene;

		BlockType* type;
		glm::mat4* parent;
		glm::mat4* model;


		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	};

	void Initialize();
	void Load(std::string _name);
	void MaterialCheck(Block* _block, const char* _materialName);
	void Refresh();
	void Draw();
	std::vector<Blocks::BlockType*> GetAll();

	namespace Ghost
	{
		void Draw(GameData* _gameData);
		void CalculateTransformations();
		void SetScale(glm::vec3 _scale);
		void SetPosition(glm::vec3 _position);
	}
}

#endif // !BLOCKS_H
