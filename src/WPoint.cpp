//Hayden Coffey
//2020
#include "WPoint.hpp"

//Image dimensions
int32_t WPoint::_pixelWidth = 0;
int32_t WPoint::_pixelHeight = 0;
int32_t WPoint::_depth = 0;

//Initialize point with a random position
WPoint::WPoint()
{
    if (_pixelWidth)
    {
        _x = rand() % _pixelWidth;
        _y = rand() % _pixelHeight;
        _z = rand() % _depth;
    }
    else
    {
        _pixelIndex = _x = _y = _z = 0;
    }
}

//Set static values of image size
void WPoint::initPixelSize(int pixelWidth, int pixelHeight, int depth)
{
    _pixelWidth = pixelWidth;
    _pixelHeight = pixelHeight;
    _depth = depth;
}

//Set velocity for point
void WPoint::setVelocity(int x, int y, int z)
{
    _vx = x;
    _vy = y;
    _vz = z;
}

//Update position of point based on velocity
void WPoint::step()
{
    _x += _vx;
    _y += _vy;
    _z += _vz;

    if(_x >= _pixelWidth)
    {
        _x = _pixelWidth - 1;
        _vx *= -1;
    }
    else if(_x < 0)
    {
        _x = 0;
        _vx *= -1;
    }

    if(_y >= _pixelHeight)
    {
        _y = _pixelHeight - 1;
        _vy *= -1;
    }
    else if(_y < 0)
    {
        _y = 0;
        _vy *= -1;
    }

    if(_z >= _depth)
    {
        _z = _depth - 1;
        _vz *= -1;
    }
    else if(_z < 0)
    {
        _z = 0;
        _vz *= -1;
    }
}

//Accessors
int WPoint::getZ() const
{
    return _z;
}

int WPoint::getX() const
{
    return _x;
}

int WPoint::getY() const
{
    return _y;
}

int32_t WPoint::getPixelWidth() const
{
    return _pixelWidth;
}