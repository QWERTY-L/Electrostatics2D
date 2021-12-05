/*
 * 
 * Made By Luke Sequeira
 * December 2021
 * 
*/

#include "raylib.h"
#include <cmath>
#include <iostream>
#include <vector>

#define FPS 120 //Defaults to 120 FPS for accuracy

#ifndef ELECTROSTATICS2D_HPP
#define ELECTROSTATICS2D_HPP

const int screenWidth = 800;
const int screenHeight = 450;
const int e = 1;                //e_0 [epsilon zero] from the 1/(4*pi*e_0) term

//Forward Declarations
class SourceCharge;
class MovingCharge;

class MovingCharge{
    private:
        double radius = 10;     //radius (should be overided if child class)
        double velocity_x = 0;    //the particle's velocity in pixels per second
        double velocity_y = 0;
        double mass;            //The particles' mass
    public:
        double charge;          //The particles' charge
        double x;               //x-position and y-position
        double y;
        Color colour = BLUE;     //the particle's colour when rendered

        MovingCharge(double charge_, double x_, double y_, double mass_){       //Constructor
            charge = charge_;
            x = x_;
            y = y_;
            mass = mass_;
        }

        void render(){                                                          //Render Function, should be overrided in child classes
            DrawCircle(x, y, radius, colour);
        }

        void update(double delta_t, double field, double angle){
            x += velocity_x*delta_t;
            y += velocity_y*delta_t; std::cout << velocity_x << std::endl;

            velocity_x += charge*field*cos(angle)*delta_t /mass;
            velocity_y += charge*field*sin(angle)*delta_t /mass;
        }

        void updateXY(double delta_t, double fieldX, double fieldY){
            x += velocity_x*delta_t;
            y += velocity_y*delta_t; //std::cout << velocity_x << std::endl;

            velocity_x += charge*fieldX*delta_t /mass;
            velocity_y += charge*fieldY*delta_t /mass;
        }
};

class SourceCharge{
    protected:
        double radius = 10;     //radius (should be overided if child class)
    public:
        double charge;          //The particles' charge in couloumbs
        double x;               //x-position and y-position
        double y;
        Color colour = RED;     //the particle's colour when rendered

        SourceCharge(double charge_, double x_, double y_){     //Constructor
            charge = charge_;
            x = x_;
            y = y_;
        }

        virtual void render(){                                  //Render Function, should be overrided in child classes
            DrawCircle(x, y, radius, colour);
        }

        double angularDifference(double x_pos, double y_pos){   //Finds the angular difference between two points in radians
            return atan2((y_pos - y), (x_pos - x));
        }

        double angularDifference(MovingCharge m){
            return angularDifference(m.x, m.y);
        }

        double electricField(double x_pos, double y_pos){       //Calculates Electric Field, Should be overided in child classes. Units is pixels / second^2 /couloumb
            return (1/(4 * PI * e)) * charge / (pow((x - x_pos), 2) + pow((y - y_pos), 2)); //For a point charge, E = 1/(4*pi*e_0) * (q/r^2)
        }

        double electricField(MovingCharge m){
            return electricField(m.x, m.y);
        }
        
        virtual double electricFieldX(MovingCharge m){
            return cos(angularDifference(m)) * electricField(m);
        }

        virtual double electricFieldY(MovingCharge m){
            return sin(angularDifference(m)) * electricField(m);
        }
};

std::vector<SourceCharge*> Source;
std::vector<MovingCharge> Test;

class PatternSource: public SourceCharge{               //Source Charge, but Positive is RED and Negative is YELLOW 
    public:
        Color positive = RED;
        Color negative = YELLOW;
        using SourceCharge::SourceCharge;
        void render(){
            if(charge > 0){
                DrawCircle(x, y, radius, positive);
            } else{
                DrawCircle(x, y, radius, negative);
            }
        }
};

void SpawnElectricField(int x_density, int y_density){  //Spawns array of Test Charges
    for (int i = 0; i < x_density; i++)
    {
        for (int j = 0; j < y_density; j++)
        {
            Test.push_back(MovingCharge(1, screenWidth*i/x_density, screenWidth*j/y_density, 1));
        }
        
    }
    
}

#endif // !ELECTROSTATICS2D_HPP