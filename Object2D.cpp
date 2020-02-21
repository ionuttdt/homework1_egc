// Duta Viorel-Ionut, 331CB
#include "Object2D.h"

#include <Core/Engine.h>
#include <math.h>
#include <GL/glu.h>

/*  Fucntie folosita la generarea stalpilor si a palariutelor.
*/
Mesh* Object2D::CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), glm::vec3(0.1f,0.1f,0.1f)),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}

/*	Generarea fundalului scenei.*/
Mesh* Object2D::CreateSquare2(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0),  glm::vec3(0,0.8f,0.8f))
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}

/*	Generarea aripii.*/
Mesh* Object2D::Wings(std::string name, glm::vec3 leftBottomCorner, float side, glm::vec3 color) {
	float angle = 3.14f * 2.f / 20;
	glm::vec3 corner = leftBottomCorner;
	std::vector<VertexFormat> vertices = {
		VertexFormat(corner + glm::vec3(-side/1.5, 0, 0), color),
	};
	std::vector<unsigned short> indices = { };

	for (int i = 0; i <= 8; i++) {

		float newX = -1.2*side * sin(angle * i);
		float newY = 1.2*side * cos(angle * i);
		vertices.push_back(VertexFormat(corner + glm::vec3(newY, newX, 0), color));

		indices.push_back(0);
		indices.push_back(i + 1);
		indices.push_back(i + 2);
	}

	Mesh* triangle = new Mesh(name);
	triangle->InitFromData(vertices, indices);
	return triangle;
}

/*	Generarea unui cerc cu 20 de puncte si raza radius.*/
Mesh* Object2D::Circle(std::string name, glm::vec3 leftBottomCorner, float radius, glm::vec3 color) {
	float angle = 3.14f * 2.f / 20;

	glm::vec3 corner = leftBottomCorner;
	std::vector<VertexFormat> vertices = {
		VertexFormat(corner + glm::vec3(0, 0, 0), color),
		VertexFormat(corner + glm::vec3(radius, 0, 0), color)
	};
	std::vector<unsigned short> indices = { };

	for (int i = 1; i <= 21; i++) {

		float newX = radius * sin(angle * i);
		float newY = -radius * cos(angle * i);
		vertices.push_back(VertexFormat(corner + glm::vec3(newY, newX, 0), color));

		indices.push_back(0);
		indices.push_back(i );
		indices.push_back(i + 1);
	}

	Mesh* circle = new Mesh(name);
	circle->InitFromData(vertices, indices);
	return circle;
}

/*	Functie care genereaza pasarea (doua cercuri si un triunghi).*/
Mesh* Object2D::Bird(std::string name, glm::vec3 leftBottomCorner, float radius) {

	float steps = 20.f;
	glm::vec3 corner = leftBottomCorner;
	float angle = 3.14f * 2.f / steps;

	std::vector<VertexFormat> vertices = {};
	std::vector<unsigned short> indices = {};

	vertices.push_back(VertexFormat(corner + glm::vec3(1.5 * radius, 0, 0), glm::vec3(1, 1, 0)));
	vertices.push_back(VertexFormat(corner + glm::vec3(radius * cos(M_PI / 8) +0.01, radius * sin(M_PI / 8) + 0.01, 0), glm::vec3(1, 1, 0)));
	vertices.push_back(VertexFormat(corner + glm::vec3(radius * cos(M_PI / 8) + 0.01, -radius * sin(M_PI / 8) + 0.01, 0), glm::vec3(1, 1, 0)));

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	vertices.push_back(VertexFormat(glm::vec3(radius / 2, radius / 2, 0) + glm::vec3(0, 0, 0), glm::vec3(0, 0, 0.5)));
	vertices.push_back(VertexFormat(glm::vec3(radius / 2, radius / 2, 0) + glm::vec3(-radius / 6, 0, 0), glm::vec3(0, 0, 0.5)));

	for (int i = 1; i <= steps; i++) {

		float newX = radius / 6 * sin(angle * i);
		float newY = -radius / 6 * cos(angle * i);

		vertices.push_back(VertexFormat(glm::vec3(radius / 2, radius / 2, 0) + glm::vec3(newY, newX, 0), glm::vec3(0, 0, 0.5)));
	
		indices.push_back(3);
		indices.push_back(i + 3);
		if (i != steps)
			indices.push_back(i + 4);
		else
			indices.push_back(4);

	}

	vertices.push_back(VertexFormat(corner + glm::vec3(0, 0, 0), glm::vec3(0.65, 0, 0)));
	vertices.push_back(VertexFormat(corner + glm::vec3(-radius, 0, 0), glm::vec3(0.65, 0, 0)));

	for (int i = 1; i <= steps; i++) {

		float newX = radius * sin(angle * i);
		float newY = -radius * cos(angle * i);

		vertices.push_back(VertexFormat(corner + glm::vec3(newY, newX, 0), glm::vec3(0.65, 0, 0)));

		indices.push_back(25);
		indices.push_back(i + 25);
		if (i != steps)
			indices.push_back(i + 26);
		else
			indices.push_back(26);

	}

	Mesh* bird = new Mesh(name);
	bird->InitFromData(vertices, indices);
	return bird;
}