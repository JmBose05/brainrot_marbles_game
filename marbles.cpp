#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include "raylib.h"
#include "raymath.h"

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
const int screenWidth = 800;
const int screenHeight = 800;

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

		// Wall Bounce X
		if (currM->position.x >= screenWidth - currM->radius) {
		    currM->position.x = screenWidth - currM->radius;
		    currM->velocity.x *= -1;
		}
		else if (currM->position.x <= currM->radius) {
		    currM->position.x = currM->radius;
		    currM->velocity.x *= -1;
		}

		// Wall Bounce Y
		if (currM->position.y >= screenHeight - currM->radius) {
		    currM->position.y = screenHeight - currM->radius;
		    currM->velocity.y *= -1;
		}
		else if (currM->position.y <= currM->radius) {
		    currM->position.y = currM->radius;
		    currM->velocity.y *= -1;
		}
	}	

}
int main (){
	const int num_marbles = 5;
	InitWindow(screenWidth, screenHeight, "Brainrot Marbles");

	SetTargetFPS(60);

	Marble* my_marble = new Marble(50, 50, -5.0f, 5.0f, GREEN);
	Marble* my_marble1 = new Marble(500, 300, 5.5f, 5.0f, RED);
	//Marble* my_marble2 = new Marble(600, 100, 5.0f, 7.0f, BLUE);
	//Marble* my_marble3 = new Marble(500, 350, 4.0f, 2.0f, YELLOW);
	//Marble* my_marble4 = new Marble(100, 30, 5.0f, 5.0f, MAROON);
	//Marble* my_marble5 = new Marble(200, 300, 3.0f, 1.0f, ORANGE);
	//Marble* my_marble6 = new Marble(300, 300, 5.0f, -5.0f, PURPLE);

	std::cout << "DEBUG: #marbles:" << num_marbles << std::endl;
	std::cout << "DEBUG: marbles:";
	for(Marble* m : Marble::allMarbles){
		std::cout << m->name << std::endl;
	}

	while(!WindowShouldClose()){
		if (IsKeyPressed(KEY_Q)) {
            		break;
        	}	
		UpdatePositions();
		
		BeginDrawing();
			ClearBackground(RAYWHITE);
			// Draw the marble (Position, Radius, Color)
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
