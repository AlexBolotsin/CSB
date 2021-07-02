#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

#define PI 3.14159265
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

    Vec2 operator+=(const float v)
    { x += v; y += v; return *this; }

    Vec2& operator+=(const Vec2& v)
    { x += v.x;  y += v.y; return *this; }

    Vec2& operator-=(const Vec2& v)
    { x -= v.x;  y -= v.y; return *this; }

    Vec2& operator*=(const float v)
    { x *= v; y *= v; return *this; }

    Vec2& operator*=(const Vec2& v)
    { x *= v.x;  y *= v.y; return *this; }

    Vec2 operator*(const Vec2& v) const
    { return Vec2(x*v.x, y*v.y); }

    Vec2 operator*(const float v) const
    { return Vec2(x*v, y*v); }

    Vec2& operator/=(const float v) {
        float r = 1.0f / v;
        *this *= r;
        return *this;
    }

    Vec2 operator-(Vec2& v)
    { x -= v.x; y -= v.y; return *this; }

    friend Vec2 operator*(const float v, const Vec2& vec )
    { return vec * v; }

    Vec2 operator/(const float v) const {
        float r = 1.0f / v;
        return *this * r;
    }

    float x = 0;
    float y = 0;
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

float dot_product(const Vec2& player, const Vec2& enemy) {
   return player.x * enemy.x + player.y* enemy.y;
}

int distance(int x0, int y0, int x1, int y1)
{
    return sqrt((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0));
}


inline
float length(const Vec2& v) {
    return std::sqrt( dot_product(v,v) );
}
Vec2 normalize(const Vec2& v) {
    return v / length(v);
}

inline
Vec2 rotate(const Vec2& v, float angle) {
    float radian = angle * PI / 180;
    double sinAngle = sin(radian);
    double cosAngle = cos(radian);

    return Vec2(v.x * cosAngle - v.y * sinAngle, v.y * cosAngle + v.x * sinAngle);
}

int main()
{
    const int kAngleToSteer = 1;
    const int kSlowingAngle = 90;
    const float kSlowingRadius = 600 * 4;

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
        int thrust = 100;

        if(nextCheckpointAngle <= -kAngleToSteer || nextCheckpointAngle >= kAngleToSteer)
        {
            Vec2 desiredDir = normalize(Vec2(nextCheckpointX - x, nextCheckpointY - y));
            Vec2 currentDir = normalize(rotate(desiredDir, -nextCheckpointAngle));
            Vec2 steeringDirection = normalize(desiredDir - currentDir) * 100.0f;

            nextCheckpointX += steeringDirection.x;
            nextCheckpointY += steeringDirection.y;

            // thrust dumping
            if( nextCheckpointAngle <= -kSlowingAngle || nextCheckpointAngle >= kSlowingAngle )
            {
                thrust = 0;
            }
            else if(nextCheckpointDist < kSlowingRadius)
            {
                thrust *= (kSlowingAngle - abs(nextCheckpointAngle)) / (float)kSlowingAngle;
            }
        }
        
        cout << nextCheckpointX << " " << nextCheckpointY << " ";
        if( false )
        {
            cout << "BOOST" << endl;
        }
        else
        {
            cout << thrust << endl;
        }
    }
}