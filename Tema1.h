#pragma once

#include <Component/SimpleScene.h>
#include <Core/Engine.h>

class Tema1 : public SimpleScene
{
public:
	Tema1();
	~Tema1();

	void Init() override;
private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

protected:
	glm::mat3 modelMatrix;
	float squareSide = 100.f;
	float squareSide2 = 50.f;

	bool Collided = false;				// daca pasarea se loveste de stalp
	float gameScore = 0;				// punctajul jocului
	bool scorePrint = true;				// variabile folosite pentru a afisa scorul
	int scoreaux = -1;					

	// variabile folosite pentru generarea stalpilor
	int numberPillar = 10;				
	float pillarSize;
	float speedPillar = 100;
	float pillarPosition;
	float pillarSide = 80;			
	float startPillard = 300;
	int randVar = 0;

	// variabile folosite pentru generarea si miscarea pasarii
	float birdPosition = 100;
	float birdAcceleration = 0;
	float birdVelocity = 0;
	float rotationB = 0;
	float rotationV = 0;
	float gravity = 100.f;

	float randNumber[10];
	float difficultyP1 = 0.6;			// distanta stalp 0-1
	float difficultyP2 = 0.6f;			// grosime stalp  0-1
	float slidingdiff = 1.75f;			// variabila pentru deschiderea tuburilor
	float densityP= 3;					// cat de des apar 2-4

	bool P = false;						// pune jocul pe pauza
};