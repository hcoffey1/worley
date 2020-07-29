#include "WPoint.hpp"

int32_t WPoint::_pixelCount = 0;
int32_t WPoint::_pixelWidth = 0;
int32_t WPoint::_pixelHeight = 0;
int32_t WPoint::_depth = 0;

WPoint::WPoint()
{
    if (_pixelCount)
    {
        _pixelIndex = rand() % _pixelCount;
        _x = _pixelIndex % _pixelWidth;
        _y = _pixelIndex / _pixelWidth;
        _z = rand() % _depth;
    }
    else
    {
        _pixelIndex = _x = _y = 0;
    }
}

void WPoint::initPixelSize(int pixelCount, int pixelWidth, int pixelHeight, int depth)
{
    _pixelCount = pixelCount;
    _pixelWidth = pixelWidth;
    _pixelHeight = pixelHeight;
    _depth = depth;
}

void WPoint::setVelocity(int x, int y, int z)
{
    _vx = x;
    _vy = y;
    _vz = z;
}

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