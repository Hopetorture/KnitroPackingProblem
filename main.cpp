#include "KTRSolver.h"
#include "ExampleHelpers.h"
#include "Shape.h"
#include "VariableMappers.h"
#include "PackingProblem.h"

int main() {
  // Create a problem instance.
  SimpleVariableMapping mapper(generateTestInput());
  PackingProblem problem(mapper, 2);
    
  // Create a solver
  knitro::KTRSolver solver(&problem );

  int solveStatus = solver.solve();
  printSolutionResults(solver, solveStatus);
  

  return 0;
}
