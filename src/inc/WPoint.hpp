#ifndef __WPOINT__
#define __WPOINT__
#include <random>
#include <cstdint>

class WPoint
{
private:
    int _pixelIndex;
    int _vx, _vy, _vz;
    int _x, _y, _z;
    static int32_t _pixelWidth;
    static int32_t _pixelHeight;
    static int32_t _depth;

public:
    static void initPixelSize(int pixelWidth, int pixelHeight, int depth);

    WPoint();
    void setVelocity(int x, int y, int z);
    int getZ() const;
    int getX() const;
    int getY() const;
    int32_t getPixelWidth() const;
    void step();
};

#endif