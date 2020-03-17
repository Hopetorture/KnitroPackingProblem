#pragma once
#include "VariableMappers.h"
#include <set>

#define REDUCE_AREA_CALC_ERROR true

struct Plain2D{
	Plain2D() {
		x1 = 0.0; y1 = 0.0;
		x2 = 0.0; y2 = 10.0;
		x3 = 10.0; y3 = 10.0;
		x4 = 10.0; y4 = 0.0;
		
	}
	double x1,x2,x3,x4,
		   y1, y2, y3, y4;	
};
class PackingProblem : public knitro::KTRProblem {
	//2d container for packing
	Plain2D container;
	// should take reference to abstract interface instead of concrete class
	SimpleVariableMapping cfg;
public:	
	PackingProblem(SimpleVariableMapping &init_cfg, int constraints): 
		KTRProblem(init_cfg.variableCount(), constraints), cfg(init_cfg) {		
		KTRProblem::setObjType(knitro::KTREnums::ObjectiveType::ObjGeneral);
		KTRProblem::setObjGoal(knitro::KTREnums::ObjectiveGoal::Maximize);

		KTRProblem::setConTypes(knitro::KTREnums::ConstraintType::ConGeneral);		
		KTRProblem::setConFnTypes(knitro::KTREnums::FunctionType::Uncertain);

		KTRProblem::setConLoBnds(1.0); 
		KTRProblem::setConUpBnds(1.0);		

		std::vector<int> varTypes = this->cfg.getVarTypes();
		for (int i = 0; i < varTypes.size(); i++) 
		{
			KTRProblem::setVarTypes(i, varTypes[i]);
			if (varTypes[i] == knitro::KTREnums::VariableType::Continuous) {
				// should be reworked if container shape will change
				KTRProblem::setVarLoBnds(i, 0.0);
				KTRProblem::setVarUpBnds(i, 10.0);
			}
			else if (varTypes[i] == knitro::KTREnums::VariableType::Integer) 
			{
				KTRProblem::setVarLoBnds(i, 0);
				KTRProblem::setVarUpBnds(i, 1);
			}			
		}	

	};
	~PackingProblem() {};

	double evaluateFC(
		const std::vector<double>& i_x,
			  std::vector<double>& i_c,
			  std::vector<double>& objGrad,
			  std::vector<double>& jac)
	{
		ProblemConfiguratoin shapes = cfg.deserialize(i_x);
		for (auto &shape : shapes) 
		{
			shape->apply_shift()
		}
		
		double insideContainerConstraint = 1;
		for (auto &shape : shapes) {
			if ((!shape->arePointsAboveZero()) 
				|| (!shape->isShapeInPlane(container.x3, container.y3)))
			{
				insideContainerConstraint = -1;
			}						
		}

		i_c[0] = insideContainerConstraint;
		i_c[1] = calculateCollisions(shapes);

		return caclulateArea(shapes);
	}	

private:	
	int calculateCollisions(ProblemConfiguratoin &shapes)
	{
		std::set<std::set<int> > already_checked;
		for (int i = 0; i < shapes.size(); i++) 
		{
			for (int j = 0; j < shapes.size(); j++)			
			{
				if (i == j)
					continue;
				if (!shapes.at(i)->isEnabled() || !shapes.at(j)->isEnabled())
					continue;

				std::set<int> pair = { i, j };
				if (already_checked.find(pair) != already_checked.end())
					continue;
				already_checked.insert(pair);
				if (shapes.at(i)->collides(shapes.at(j))) 
				{
					return -1;
				}
			}
		}
		return 1;
	}

	double caclulateArea(ProblemConfiguratoin &shapes)
	{
		double result = 0.0;
		if (REDUCE_AREA_CALC_ERROR)
		{
			std::vector<double> area;
			for (auto &shape : shapes)
			{
				area.push_back(shape->area());
			}
			std::sort(area.begin(), area.end());
			
			for (auto a : area) 
			{
				result += a;
			}
		}
		else 
		{
			for (auto &shape : shapes)
			{
				result += shape->area();
			}
		}
	}
	
};