#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

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

    bool operator==(const Vec2& v) const
    { return (x == v.x) && ( y == v.y ); }

    bool operator!=(const Vec2& v) const
    { return (x != v.x) || (y != v.y); }

    float X() const
    { return x; }

    float Y() const
    { return y; }

private:
    float x = 0;
    float y = 0;
};

inline
float dot_product(const Vec2& v1, const Vec2& v2) {
   return v1.X() * v2.X() + v1.Y() * v2.Y();
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
    const float PI = 3.14159265f;
    float radian = angle * PI / 180.f;
    double sinAngle = sin(radian);
    double cosAngle = cos(radian);

    return Vec2(v.X() * cosAngle - v.Y() * sinAngle, v.Y() * cosAngle + v.X() * sinAngle);
}

struct Booster {
    void AddCheckpoint(const int x, const  int y, const int dist) {
        if(_pathComplete)
        {
            return;
        }

        const Vec2 newCheckpoint(x, y);
        if(checkpoints_.size() && checkpoints_.front() == newCheckpoint)
        {
            _pathComplete = true;
            return;
        }

        checkpoints_.push_back(newCheckpoint);

        if(_farthestDist < dist)
        {
            _farthestDist = dist;
        }
    }

    bool TryBoost(const float dist) {
        if(_boostAvailable && _pathComplete &&
            dist + kDistError > _farthestDist)
        {
            _boostAvailable = false;
            return true;
        }

        return false;
    }

private:
    const float kDistError = 2000;
    bool _boostAvailable = true;
    bool _pathComplete = false;
    float _farthestDist = 0.0;
    vector< Vec2 > checkpoints_;
};

int main()
{
    const int kAngleToSteer = 1;
    const int kSlowingAngle = 90;
    const float kSlowingRadius = 600 * 4;

    Booster booster;
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

        booster.AddCheckpoint(nextCheckpointX, nextCheckpointY, nextCheckpointDist);

        bool useBoost = false;

        if(nextCheckpointAngle <= -kAngleToSteer || nextCheckpointAngle >= kAngleToSteer)
        {
            Vec2 desiredDir = normalize(Vec2(nextCheckpointX - x, nextCheckpointY - y));
            Vec2 currentDir = normalize(rotate(desiredDir, -nextCheckpointAngle));
            Vec2 steeringDirection = normalize(desiredDir - currentDir) * 100.0f;

            nextCheckpointX += steeringDirection.X();
            nextCheckpointY += steeringDirection.Y();

            // thrust dumping
            if( nextCheckpointAngle <= -kSlowingAngle || nextCheckpointAngle >= kSlowingAngle )
            {
                thrust = 0;
            }
            else if(nextCheckpointDist < kSlowingRadius)
            {
                thrust *= (kSlowingAngle - abs(nextCheckpointAngle)) / (float)kSlowingAngle;
            }
        } else {
            if(booster.TryBoost(nextCheckpointDist))
            {
                useBoost = true;
            }
            else if(nextCheckpointDist < kSlowingRadius)
            {
                thrust *= nextCheckpointDist / kSlowingRadius;
            }
        }
        
        // output
        cout << nextCheckpointX << " " << nextCheckpointY << " ";
        if(useBoost)
        {
            cout << "BOOST" << endl;
        }
        else
        {
            cout << thrust << endl;
        }
    }
}