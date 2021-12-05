/*
 * 
 * Made By Luke Sequeira
 * December 2021
 * 
*/

#include "raylib.h"
#include "Electrostatics2D.hpp"
#include <iostream>
#include <vector>

//std::vector<SourceCharge*> Source;        MOVED TO electrostatics2d.h
//std::vector<MovingCharge> Test;           MOVED TO electrostatics2d.h

int main(){

    InitWindow(screenWidth, screenHeight, "Electric Field");
    SetTargetFPS(FPS);               // Set our game to run at 60 frames-per-second

    //create default charges
    Source.push_back(new PatternSource(10, screenWidth/2 - 100, screenHeight/2));
    Source.push_back(new PatternSource(-10, screenWidth/2 + 100, screenHeight/2));

    Test.push_back(MovingCharge(10, screenWidth/2, screenHeight/2 + 10, 1));

    const double deltaT = 100/(double)FPS; //the 100 is to speed up time.

    int spawnMode = 0; //0 means spawn test charges, 1 means spawn source charges

    while(!WindowShouldClose()){

        //Update

        //INPUT - Spawn charges on click
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            if(spawnMode == 0){
                Test.push_back(MovingCharge(10, GetMouseX(), GetMouseY(), 1));
            } else if(spawnMode == 1){
                Source.push_back(new PatternSource(10, GetMouseX(), GetMouseY()));
            }
        }

        //Spawn array of electric charges
        if(IsKeyPressed(KEY_E)){
            SpawnElectricField(20, 20);
        }

        //Switch spawn mode
        if (IsKeyPressed(KEY_KP_0))
        {
            spawnMode = 0;
        }
        if (IsKeyPressed(KEY_KP_1))
        {
            spawnMode = 1; std::cout << "hey";
        }
        

        //Calculate Electric Field Using the Principle of Superposition
        for (int i = 0; i < Test.size(); i++)
        {
            double m_Ex = 0;
            double m_Ey = 0;
            for (int j = 0; j < Source.size(); j++)
            {
                 m_Ex += Source[j]->electricFieldX(Test[i]);
                 m_Ey += Source[j]->electricFieldY(Test[i]);
            }
            Test[i].updateXY(deltaT, m_Ex, m_Ey);
        }
        
        //double m_E = s.electricField(m);
        //double m_theta = s.angularDifference(m);

        //m.update(deltaT, m_E, m_theta);

        //Render the charges
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("Made by Luke Sequeira", 190, 200, 20, LIGHTGRAY);
            for (int i = 0; i < Source.size(); i++)
            {
                Source[i]->render();
            }
            for (int i = 0; i < Test.size(); i++)
            {
                Test[i].render();
            }
            EndDrawing();

    }

    CloseWindow();

}