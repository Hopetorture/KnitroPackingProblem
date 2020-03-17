#pragma once
#include <vector>
#include "Shape.h"
#include "Circle.h"
#include "KTRSolver.h"
#include "KTRProblem.h"
#include "knitro.h"
#include <iostream>

using ProblemConfiguratoin = std::vector<std::shared_ptr<Shape> >;

ProblemConfiguratoin generateTestInput() {
	ProblemConfiguratoin res;
	res.push_back(std::make_shared<Circle>(4.95, false));
	res.push_back(std::make_shared<Circle>(0.5, true));
	res.push_back(std::make_shared<Circle>(2.5, false));
	return res;		
}
class SimpleVariableMapping 
{
	ProblemConfiguratoin problemCfg;
	std::vector<int> varTypes;
public:

	SimpleVariableMapping(ProblemConfiguratoin &problemCfg)
		: problemCfg(problemCfg)
	{
		for (int i = 0; i < this->problemCfg.size(); i++) {
			//knitro::KTREnums::ConstraintType
			//is enabled var
			this->varTypes.push_back(knitro::KTREnums::VariableType::Integer);
			// x and y shift variables. Add rotation later
			this->varTypes.push_back(knitro::KTREnums::VariableType::Continuous);
			this->varTypes.push_back(knitro::KTREnums::VariableType::Continuous);
		}
	}
	SimpleVariableMapping(SimpleVariableMapping &other) 
	{
		this->problemCfg = other.problemCfg;
		this->varTypes = other.varTypes;
	}
	SimpleVariableMapping & SimpleVariableMapping::operator=(const SimpleVariableMapping & other)
	{
		this->problemCfg = other.problemCfg;
		this->varTypes = other.varTypes;
		return *this;
	}

	std::vector<double> serialize(ProblemConfiguratoin &problemCfg) {
		std::vector<double> i_x;
		i_x.reserve(problemCfg.size() * 3);
		for (const auto &shape : problemCfg) 
		{			
			i_x.push_back(shape->isEnabled());
			i_x.push_back(shape->x_shift);
			i_x.push_back(shape->y_shift);
			// rotation could be added later
		}
		return i_x;
	}

	ProblemConfiguratoin deserialize(const std::vector<double> &i_x) {
		int current_shape = 0;
		for (int i = 0; i < i_x.size() - 3; i += 3) 
		{
			auto & shape = problemCfg.at(current_shape);
			shape->setEnabled(i_x.at(i));
			shape->x_shift = (i_x.at(i + 1));
			shape->y_shift = (i_x.at(i + 2));
			++current_shape;
		}
		return problemCfg;
	}
	int variableCount() {
		return int(varTypes.size());
	}
	int variableType(int idx) {
		return varTypes.at(idx);
	}
	std::vector<int> getVarTypes() {
		return this->varTypes;
	}
	auto getCfg() 
	{
		return problemCfg;
	}

};
