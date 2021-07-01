#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

const int map_width = 16000;
const int map_height = 9000;

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
struct Vec2
{
    Vec2(float _x, float _y) : x(_x), y(_y) {}
    Vec2(int _x, int _y)
    {
        x = (float)_x;
        y = (float)_y;
    }

    float x = 0;
    float y = 0;

    Vec2 operator*=(float&& value)
    {
        x *= value;
        y *= value;
        return *this;
    }

    Vec2 operator+=(float&& value)
    {
        x += value;
        y += value;
        return *this;
    }
};

Vec2 normalize(Vec2& vec)
{
    float length = sqrt(vec.x*vec.x+vec.y*vec.y);
    return { vec.x/length, vec.y/length };
}

Vec2 normalize(Vec2&& vec)
{
    float length = sqrt(vec.x*vec.x+vec.y*vec.y);
    return { vec.x/length, vec.y/length };
}

float dot_product(Vec2& player, Vec2& enemy) {
   return player.x * enemy.x + player.y* enemy.y;
}

float dot_product(Vec2&& player, Vec2&& enemy) {
   return player.x * enemy.x + player.y* enemy.y;
}

int distance(int x0, int y0, int x1, int y1)
{
    return sqrt((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0));
}

int prevOpponentX = 0, prevOpponentY = 0;
int prevX = 0, prevY = 0;

int main()
{

    // game loop
    while (1) {
        int x;
        int y;
        int nextCheckpointX; // x position of the next check point
        int nextCheckpointY; // y position of the next check point
        int nextCheckpointDist; // distance to the next checkpoint
        int nextCheckpointAngle; // angle between your pod orientation and the direction of the next checkpoint
        cin >> x >> y >> nextCheckpointX >> nextCheckpointY >> nextCheckpointDist >> nextCheckpointAngle; cin.ignore();
        int opponentX;
        int opponentY;
        cin >> opponentX >> opponentY; cin.ignore();
        int thrust = 0;

        int opponentDistance = distance(x, y, opponentX, opponentY);
        Vec2 player(x - prevX, y - prevY);
        Vec2 enemy(opponentX - prevOpponentX, opponentY - prevOpponentX);
        float dot = dot_product(normalize(player),normalize(enemy));
        
        cerr << "Current dot is " << dot << endl;
        cerr << "angle is " << abs((float)nextCheckpointAngle/90.f) << endl;
        cerr << "distance to opponent" << opponentDistance << endl;

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

        Vec2 center(map_width/2, map_height/2);

        Vec2 checkpointToCenterNorm = normalize(Vec2(x - nextCheckpointX, y - nextCheckpointY));
        checkpointToCenterNorm *= 1200;
        checkpointToCenterNorm.x += nextCheckpointX;
        checkpointToCenterNorm.y += nextCheckpointY;

        float fnextCheckpointDist = distance(x, y, checkpointToCenterNorm.x, checkpointToCenterNorm.y);
        cerr << "Distance " << nextCheckpointDist << endl;
        cerr << "Distance " << fnextCheckpointDist << endl;

        thrust = 100;
        float ftrhust = thrust;

        /*{
            if (nextCheckpointDist < 1500 && abs(dot) < 0.1f && opponentDistance < 1000)
            {
                float percentage = (float)nextCheckpointDist / 2500.f;
                ftrhust *= clamp(0.3f, 1.f, percentage*percentage, [](float x, float y){ return x > y; });
            }
        }*/

        //if (nextCheckpointDist < 5000 && abs(nextCheckpointAngle) > 90)
        {            
            float value = abs((float)nextCheckpointAngle/90.f);
            //cerr << "angle is " << value;
            float torgue = abs(clamp(0.25f, 1.0f, 1-(value*value), [](float x, float y){ return x > y; }));
            //cerr << "torgue " << torgue;
            ftrhust *= torgue;
        }

        {
            //ftrhust *= opponentDistance > 1500 ? 1.0f : 0.25f;
        }

        //thrust *= nextCheckpointAngle > 90 || nextCheckpointAngle < -90 ? 0 : 1;

        //thrust = nextCheckpointDist > 1000 ? 100 : (nextCheckpointDist > 500 ? 60 : (nextCheckpointDist > 300 ? 25 : 10));
        //thrust *= nextCheckpointAngle > 90 || nextCheckpointAngle < -90 ? 0 : 1;

        // You have to output the target position
        // followed by the power (0 <= thrust <= 100)
        // i.e.: "x y thrust"
        /*if ((nextCheckpointAngle < 5 && nextCheckpointAngle > -5) && nextCheckpointDist > 3000)
        {
            cout << nextCheckpointX << " " << nextCheckpointY << " BOOST" << endl;
        }
        else*/
        
        /*cerr << "Opponent distance " << opponentDistance << endl;
        if (opponentDistance < 1500 && opponentDistance > 1000)
        {
            cout << opponentX << " " << opponentY << " 100" << endl;
        }
        else if (opponentDistance < 1000)
        {
            cout << opponentX << " " << opponentY << " BOOST" << endl;
        }*/if (nextCheckpointAngle == 0 && nextCheckpointDist > 1000)
        {
            cerr << "boosted" << endl;
            cout << round(checkpointToCenterNorm.x) << " " << round(checkpointToCenterNorm.y) << " BOOST" << endl;
        }
        else
        {

            cerr << "TARGET :" << nextCheckpointX << " " << nextCheckpointY << endl;
            cerr << "Corrected TARGET :" << checkpointToCenterNorm.x << " " << checkpointToCenterNorm.y << endl;

            cout << round(checkpointToCenterNorm.x) << " " << round(checkpointToCenterNorm.y) << " " << round(ftrhust) << endl;
        }

        prevOpponentX = opponentX;
        prevOpponentY = opponentY;
        prevX = x;
        prevY = y;
    }
}