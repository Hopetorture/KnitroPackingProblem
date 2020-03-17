#pragma once
#define _USE_MATH_DEFINES

#include <utility>
#include <cmath>
#include <math.h>
#include <memory>
#include <string>

using Radius = double;
using Center = std::pair<double, double>;

struct BoundingCircle {
	BoundingCircle(double x, double y, double r) {
		this->x = x;
		this->y = y;
		this->r = r;
	}
	double x, y, r;
};



class Shape
{
	bool enabled;
public:
	Shape() {
		x_shift = 0.0;
		y_shift = 0.0;
	}
	virtual ~Shape() {}
	virtual BoundingCircle boundingCircle() const = 0;
	bool isEnabled() const
	{
		return this->enabled;
	}
	void setEnabled(bool state)
	{
		this->enabled = state;
	}
	virtual bool arePointsAboveZero() const = 0;
	virtual bool isShapeInPlane(double x, double y) const = 0;		
	virtual double area() const = 0;
	virtual void apply_shift() = 0;

	double x_shift, y_shift;

private:
	virtual bool exactCollisionDetection(std::shared_ptr<Shape> &other)
	{
		return true; // returns true because it only gets called when bounding circles intersect already
	}

};
