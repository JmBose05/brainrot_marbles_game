#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include "raylib.h"
#include "raymath.h"
#include "GameMaps.h"

class Marble {
	public:
		static int num_marbles;
		static std::vector<Marble*> allMarbles;
		Vector2 position;
		Vector2 velocity;
		std::string name;
		Color color;
		float mass;
		float radius;
		
		Marble(float x_pos, float y_pos, float x_velo, float y_velo, Color col){
			position.x = x_pos;
			position.y = y_pos;
			velocity.x = x_velo;
			velocity.y = y_velo;
			radius = 20.0f;
			mass = 20.0f;
			color = col;
			num_marbles++;
			
			name = "marble" + std::to_string(num_marbles);
			allMarbles.push_back(this);
		}
		~Marble(){
			allMarbles.erase(
				std::remove(allMarbles.begin(), allMarbles.end(), this),
				allMarbles.end()
			);
		}

};

int Marble::num_marbles = 0;
std::vector<Marble*> Marble::allMarbles;
GameMap* currentMap = nullptr;

void UpdatePositions(){
	// check collisions with other marbles
	for(size_t i = 0; i < Marble::allMarbles.size(); i++){
		Marble* currM = Marble::allMarbles[i];
		for(size_t j = i + 1; j < Marble::allMarbles.size(); j++){
			Marble* otherM = Marble::allMarbles[j];
			if(currM != otherM){
				Vector2 collisionNormal = Vector2Subtract(currM->position, otherM->position);
				float distSq = Vector2LengthSqr(collisionNormal);
				float radiusSum = currM->radius + otherM->radius;
				if(distSq <= radiusSum * radiusSum){
					float dist = sqrtf(distSq);
					if (dist == 0) dist = 0.001f;

					Vector2 normalVect = Vector2Scale(collisionNormal, 1.0f / dist);
					float penetrationDepth = radiusSum - dist;
					
					Vector2 push = Vector2Scale(normalVect, penetrationDepth * 0.5f);
					currM->position = Vector2Add(currM->position, push);
					otherM->position = Vector2Subtract(otherM->position, push);

					Vector2 vRel = Vector2Subtract(currM->velocity, otherM->velocity);
					float velAlongNormal = Vector2DotProduct(vRel, normalVect);

					if(velAlongNormal > 0) return;

					float e = 1.0f;
					float j = -(1.0f + e) * velAlongNormal;
					j /= (1.0f / currM->mass + 1.0f / otherM->mass);

					Vector2 impulse = Vector2Scale(normalVect, j);
					
					currM->velocity = Vector2Add(currM->velocity, Vector2Scale(impulse, 1.0f / currM->mass));
					otherM->velocity = Vector2Subtract(otherM->velocity, Vector2Scale(impulse, 1.0f / otherM->mass));	
				}
			}
		}
	}
	
	// 2. UPDATE MOVEMENT & WALL BOUNCE
	for (Marble* currM : Marble::allMarbles) {
        	currM->position = Vector2Add(currM->position, currM->velocity);

		if (currentMap) {
			for (const auto& obstacle : currentMap->obstacles) {
				// Find the closest point on the rectangle to the circle's center
				Vector2 closestPoint = {
					Clamp(currM->position.x, obstacle.x, obstacle.x + obstacle.width),
					Clamp(currM->position.y, obstacle.y, obstacle.y + obstacle.height)
				};

				// Check if the circle is colliding with the rectangle
				if (CheckCollisionPointCircle(closestPoint, currM->position, currM->radius)) {
					Vector2 collisionNormal = Vector2Normalize(Vector2Subtract(currM->position, closestPoint));
					// If marble is exactly at the closest point (e.g. inside the rect), pick a default normal
					if (Vector2LengthSqr(collisionNormal) == 0) {
						collisionNormal = { 0, -1 };
					}

					// Reflect velocity
					currM->velocity = Vector2Reflect(currM->velocity, collisionNormal);

					// Resolve penetration to stop marbles from getting stuck
					float penetration = currM->radius - Vector2Distance(currM->position, closestPoint);
					currM->position = Vector2Add(currM->position, Vector2Scale(collisionNormal, penetration));
				}
			}
		}
	}	
}
Marble* CheckWin(){
	if (currentMap){
		for(Marble* m : Marble::allMarbles){
			for(const auto& winCondition : currentMap->winConditions){
				if(CheckCollisionPointRec(m->position, winCondition)){
					return m;
				}
			}
		}
	}	
	return nullptr;
}
int main (){
	const int num_marbles = 5;
	InitWindow(SCREENWIDTH, SCREENHEIGHT, "Brainrot Marbles");

	SetTargetFPS(60);

	Marble* my_marble = new Marble(20, 50, 5.0f, 5.0f, GREEN);
	Marble* my_marble1 = new Marble(20, 1150, 5.0f, 5.0f, RED);
	//Marble* my_marble2 = new Marble(600, 100, 5.0f, 7.0f, BLUE);
	//Marble* my_marble3 = new Marble(500, 350, 4.0f, 2.0f, YELLOW);
	//Marble* my_marble4 = new Marble(100, 30, 5.0f, 5.0f, MAROON);
	//Marble* my_marble5 = new Marble(200, 300, 3.0f, 1.0f, ORANGE);
	//Marble* my_marble6 = new Marble(300, 300, 5.0f, -5.0f, PURPLE);

	std::cout << "DEBUG: #marbles:" << Marble::num_marbles << std::endl;
	std::cout << "DEBUG: marbles: " << std::endl;
	for(Marble* m : Marble::allMarbles){
		std::cout << m->name << std::endl;
	}

	currentMap = &gameMaps["Race"];

	while(!WindowShouldClose()){
		if (IsKeyPressed(KEY_Q)) {
			break;
		}	

		UpdatePositions();
		Marble* winner = CheckWin();
		if(winner){
			std::cout << winner->name << " wins!" << std::endl;
			break;
		}

		BeginDrawing();
			ClearBackground(RAYWHITE);
			// Draw the map
			if (currentMap) {
				for (const auto& obstacle : currentMap->obstacles){
					DrawRectangleRec(obstacle, DARKGRAY);
				}
				for (const auto& winCondition : currentMap->winConditions){
					DrawRectangleRec(winCondition, GREEN);
				}
			}
			// Draw the marble
			for(Marble* m : Marble::allMarbles){
				DrawCircleV(m->position, m->radius, m->color);
			}
		EndDrawing();
	}
	while (!Marble::allMarbles.empty()) {
	    delete Marble::allMarbles.back();
	}
	CloseWindow();
	return 0;
}
