#ifndef PEDS_H
#define PEDS_H
#include "Common.h"
#include "Files.h"
#include "Set.h"
#include "Physics.h"

namespace Peds
{
	class Ped : public Sets::Set
	{
	public:
		Ped();
		~Ped();


		Camera* GetCamera();
		void SetCamera(Camera* _camera);

		Physics::Body GetBody();
		Physics::Type GetBodyType();
		void SetBodyType(Physics::Type _type);

		void PushVelocity(glm::vec3 _velocity, bool _movementVelocity = false);
		void Update();
		float GetSpeed();
		void SetSpeed(float _speed);
		float GetRunningMultiplier();
		void SetRunningMultiplier(float _multiplier);
		void Run();

		void Simulate(GameData* _gameData);
	private:
		std::shared_mutex mutex;
		Camera* camera;

		Physics::Body body;
		glm::vec3 movementVelocity;
		float speed;
		float runningMultiplier;
		bool running;
	};

	Ped* Create();
	void Erase(Ped* _ped, bool _eraseModel = true);
	std::vector<Peds::Ped*>* GetPool();
	void Simulate(GameData* _gameData);
}

#endif // !PEDS_H