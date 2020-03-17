#pragma once
#include "Shape.h"
// #include <math>

class Circle : public Shape
{	
public:
	Circle(Radius r, Center pos) {
		this->r = r;
		this->pos = pos;
		this->shifted_pos = pos;
		this->setEnabled(true);
	}
	Circle(Radius r) {
		this->r = r;
		this->pos = std::make_pair(0.0f, 0.0f);
		this->shifted_pos = std::make_pair(0.0f, 0.0f);
		this->setEnabled(true);
	}
	Circle(Radius r, bool state) {
		this->r = r;
		this->pos = std::make_pair(0.0f, 0.0f);
		this->shifted_pos = std::make_pair(0.0f, 0.0f);
		this->setEnabled(state);
	}
	~Circle();

	virtual BoundingCircle boundingCircle() const override
	{
		return BoundingCircle(this->shifted_pos.first , this->shifted_pos.second, this->r);
	}

	virtual bool arePointsAboveZero() const 
	{		
		double min_x = shifted_pos.first + (r * -1.0);		
		double min_y = shifted_pos.second + (r * -1.0);

		if (min_x < 0.0 || min_y < 0.0) {
			return false;
		}
		return true;
	}
	virtual bool isShapeInPlane(double x, double y) const override
	{
		double max_x = shifted_pos.first + r;
		double max_y = shifted_pos.second + r;
		if (max_x <= x || max_y <= y)
			return true;
		else
			return false;
	}
	

	virtual double area() const 
	{
		return M_PI * this->r;
	}
	void apply_shift() override 
	{
		this->shifted_pos.first = this->pos.first + this->x_shift;
		this->shifted_pos.second = this->pos.second + this->y_shift;
	}

private:
	Radius r;
	Center pos;
	Center shifted_pos;

	virtual bool exactCollisionDetection(std::shared_ptr<Shape> &other) const override 
	{
		return true;
	}
};

