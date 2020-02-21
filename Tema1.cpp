#include "Tema1.h"

#include <vector>
#include <iostream>
#include <time.h>
#include <string>

#include <Core/Engine.h>
#include "Object2D.h"
#include "Transform2D.h"


using namespace std;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glm::vec3 corner = glm::vec3(0, 0, 0);

	// fundalul scenei
	Mesh* square = Object2D::CreateSquare2("square", corner, 900, glm::vec3(0, 0.2, 0.25), true);
	AddMeshToList(square);
	
	// obiect folosit pentru nori
	Mesh* circle = Object2D::Circle("circle", corner, 70, glm::vec3(0.9f, 0.9f, 0.9f));
	AddMeshToList(circle);

	// soarele din scena
	Mesh* sun = Object2D::Circle("sun", corner, 100, glm::vec3( 0.9f, 0.9f, 0));
	AddMeshToList(sun);

	// stalpul cu palariuta
	pillarSize = window->GetResolution().x / (numberPillar - 1);
	Mesh* square2 = Object2D::CreateSquare("square2", corner, pillarSide, glm::vec3(0, 0.5, 0.05), true);
	AddMeshToList(square2);

	Mesh* square3 = Object2D::CreateSquare("square3", corner, pillarSide , glm::vec3(0, 0.3, 0.05), true);
	AddMeshToList(square3);

	// pasarea si aripa
	squareSide = 26;
	Mesh* bird = Object2D::Bird("bird", corner, squareSide );
	AddMeshToList(bird);

	Mesh* wings = Object2D::Wings("triangle", corner, squareSide/2, glm::vec3(1, 0.3, 0.3));
	AddMeshToList(wings);

	// vector folosit pentru a genera stalpi de inaltime diferita
	srand(time(NULL));
	for (int i = 0; i < numberPillar; i++) 
		randNumber[i] = rand() % 6 + 1;

}

void Tema1::FrameStart()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{

	if (Collided)						// daca pasarea s-a lovit de un stalp, imaginea este inghetata
		deltaTimeSeconds = 0;
	
	if (P)								// daca P = true a fost apasat butonul de pauza
		deltaTimeSeconds = 0;

	// scorul este calculat in functie de pozitiile stalpilor
	int score = (gameScore - 200) / (densityP * pillarSize);

	// cresterea dificultatii in functie de scor
	if (score >= 10 && score < 20)
		difficultyP2 = 0.65;
	else if (score >= 20 && score < 50)
		difficultyP2 = 0.7;
	else if (score >= 50 && score < 100)
		difficultyP2 = 0.75;
	else if (score >= 200 && score < 500)
		difficultyP1 = 0.8;
	else if (score >= 500)
		difficultyP1 = 0.9;

	// afisarea scorului atunci cand creste
	if (scoreaux < score) {							
		scoreaux = score;
		cout << "Scor: " << score << "." << endl;
		scorePrint = false;
	}

	int BirdX = 200;						// pozitia pasarii pe axa Ox
	int BirdY = 0;

	// viteza cu care urca/coboara pasarea pe axa Oy
	birdVelocity += birdAcceleration * deltaTimeSeconds;
	birdPosition += birdVelocity * deltaTimeSeconds;
	BirdY = birdPosition;

	if (rotationB < 0.75f )						// ma asigur ca pasarea nu se invarte
		rotationB += rotationV * deltaTimeSeconds;

	if (birdPosition < squareSide) {			// conditie ca pasarea sa nu cada din scena
		birdPosition = squareSide;
		birdAcceleration = 0;
		birdVelocity = 0;
		rotationV = 0;
	}

	if (birdPosition >= 500 - squareSide ) {	// conditie ca pasarea sa nu iasa din fereastra
		birdPosition = 500 - squareSide ;
		birdAcceleration = -4 * gravity;
	}

	// incarcarea pasarii in scena:
	modelMatrix = glm::mat3(1);
	glm::mat3 modelMatrix2 = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(BirdX, birdPosition)
		* Transform2D::Rotate(rotationB);

	modelMatrix2 *= Transform2D::Translate(BirdX, birdPosition)
		* Transform2D::Rotate(rotationB);

	if (rotationB > 0)							// pasarea da din aripi la apasarea tastei space
		modelMatrix2 *= Transform2D::Scale(1, -1);

	RenderMesh2D(meshes["triangle"], shaders["VertexColor"], modelMatrix2);		// aripa
	RenderMesh2D(meshes["bird"], shaders["VertexColor"], modelMatrix);			// corpul pasarii

	// Mediul inconjurator
	gameScore += speedPillar * deltaTimeSeconds;
	pillarPosition += speedPillar * deltaTimeSeconds;		// viteza de deplasare a stalpilor
	
	// incarcarea stalpilor:
	for (int i = 1; i <= numberPillar; i++) {
		
		// punctele necesara determinarii pozitiilor stalpilor
		int Pillar1Xr = densityP * pillarSize * i + startPillard - pillarPosition + difficultyP2 * squareSide2;
		int Pillar1Xl = densityP * pillarSize * i + startPillard - pillarPosition;
		int Pillar1Yd = 0;
		int Pillar1Yu = pillarSize / 2 + ( difficultyP1 + randNumber[(i + randVar + 1) % 10] / 2) * pillarSide;
		int Pillar2Yd = Pillar1Yu + pillarSide - (difficultyP1 - 0.5) * pillarSide;
		int Pillar2Yu = window->GetResolution().y - squareSide;

		if (-pillarPosition + densityP*pillarSize * i + startPillard > -pillarSide) {

			// usa glisanta:
			if (BirdX + 0 < -pillarPosition + densityP * pillarSize * i + startPillard &&
				-pillarPosition + densityP * pillarSize * i + startPillard < window->GetResolution().x + 100) {
				slidingdiff =  1.5* (-pillarPosition + densityP * pillarSize * i + startPillard) / (1.85f * pillarSide*4) - 0.5f;
			}
			else
				slidingdiff = 0;


			// Ciupercuta:
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(densityP * pillarSize * i, 0)
				* Transform2D::Translate(startPillard, 0)
				* Transform2D::Translate(-pillarPosition - 0.1 * pillarSide, 0)
				* Transform2D::Translate(0, (slidingdiff + difficultyP1 +  randNumber[(i + randVar + 1) % 10] / 2) * pillarSide - 15)
				* Transform2D::Scale(difficultyP2 + 0.2, 0.3);
			RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);

			// stalpul de jos:
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(densityP * pillarSize * i, 0)
				* Transform2D::Translate(startPillard, 0)
				* Transform2D::Translate(-pillarPosition, 0)
				* Transform2D::Scale(difficultyP2, slidingdiff + difficultyP1 + randNumber[(i + randVar + 1) % 10] / 2);
			RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);

		}

		if (-pillarPosition + densityP*pillarSize * i + startPillard > -pillarSide) {
			// ciupercuta:
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(densityP * pillarSize * i, 0)
				* Transform2D::Translate(startPillard, 0)
				* Transform2D::Translate(-pillarPosition - 0.1 * pillarSide, 0)
				* Transform2D::Translate(0, Pillar1Yu + pillarSide - (difficultyP1 - 0.5) * pillarSide + 10)
				* Transform2D::Scale(difficultyP2 + 0.2, 0.3);
			RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);

			// stalpul de jos:
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(densityP * pillarSize * i, 0)
				* Transform2D::Translate(startPillard, 0)
				* Transform2D::Translate(-pillarPosition,0)
				* Transform2D::Translate(0, Pillar2Yd + 25)
				* Transform2D::Scale(difficultyP2, 5 - randNumber[(i + randVar + 1) % 10]/2 );
			RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);
		}

		/* verificarea coliziunii cu stalpii calculand 4 puncte esentiale pe pasare
		daca acele puncte se suprapun cu stalpii, se considera coliziune*/
		if (BirdX - 44 <= Pillar1Xr && BirdX + 25 >= Pillar1Xl && BirdY + 25 <= Pillar1Yu) {
			Collided = true;
		}
		else if (BirdX + 25 >= Pillar1Xl && BirdX <= Pillar1Xr && BirdY + 25 <= Pillar1Yu) {
			Collided = true;
		}
		if (BirdX - 44 <= Pillar1Xr && BirdX + 25 >= Pillar1Xl && BirdY + 12 >= Pillar2Yd) {
			Collided = true;
		}
		else if (BirdX + 25 >= Pillar1Xl && BirdX <= Pillar1Xr && BirdY + 12 >= Pillar2Yd) {
			Collided = true;
		}
	}

	// conditie necesara ca stalpii sa fie in dreapta la inceperea jocului
	if (-pillarPosition + densityP*pillarSize + startPillard < -pillarSide) {
		startPillard = 0;
		pillarPosition = pillarSide;
		randVar++;
	}

	// fundalul jocului:
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Scale(1, 0.85f);
	RenderMesh2D(meshes["circle"], shaders["VertexColor"], modelMatrix);
	modelMatrix *= Transform2D::Translate(300, 0);
	RenderMesh2D(meshes["circle"], shaders["VertexColor"], modelMatrix);
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(150, 0);
	modelMatrix *= Transform2D::Scale(1.85f, 1.5f);
	RenderMesh2D(meshes["circle"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(600, 0);
	RenderMesh2D(meshes["circle"], shaders["VertexColor"], modelMatrix);
	modelMatrix *= Transform2D::Translate(300, 0);
	RenderMesh2D(meshes["circle"], shaders["VertexColor"], modelMatrix);
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(750, 0);
	modelMatrix *= Transform2D::Scale(2, 2);
	RenderMesh2D(meshes["circle"], shaders["VertexColor"], modelMatrix);


	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(0, 500);
	RenderMesh2D(meshes["sun"], shaders["VertexColor"], modelMatrix);


	modelMatrix = glm::mat3(1);
	RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);
}

void Tema1::FrameEnd()
{

}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{

}

void Tema1::OnKeyPress(int key, int mods)
{
	// pasarea urca la apasarea tastei space
	if (key == GLFW_KEY_SPACE) {
		birdAcceleration = 0;
		birdVelocity = 2 * gravity;
		rotationB /= 2;
		rotationV = 2.5f;
	}

	// pun jocul pe pauza
	if (key == GLFW_KEY_P) {
		if (P == false)
			P = true;
		else
			P = false;
	}

	// schimb densitatea stalpilor
	if (key == GLFW_KEY_D) {
		if (densityP == 3)
			densityP = 2;
		else
			densityP = 3;

		pillarPosition = 0;
		startPillard = 300;
	}

	
	// restart Game
	if (key == GLFW_KEY_R) {
		gameScore = 0;
		Collided = false;

		birdPosition = 100;
		birdAcceleration = 0;
		birdVelocity = 0;
		speedPillar = 100;
		randVar = 0;
		pillarPosition = 0;
		startPillard = 300;
		rotationB = 0;
		scorePrint = true;
		difficultyP1 = 0.4;
		scoreaux = -1;
		cout << "===================== GAME OVER =====================" << endl;


	}
}

void Tema1::OnKeyRelease(int key, int mods)
{
	// pasarea coboara cand tasta space este eliberata
	if (key == GLFW_KEY_SPACE) {
		birdAcceleration = -4 * gravity;
		rotationB = 0;
		rotationV = -0.25f;
	}
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}