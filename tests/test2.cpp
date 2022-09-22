/**
 * @file test2.cpp
 * @author Wan, Ian I
 * @version 0.2
 * @date 2022-03-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <cmath>
#include <iostream>
#include "../includes/CUnit.h"
#include "../includes/CCircuit.h"

int main(int argc, char * argv[])
{
      int vec1[2*num_units+1] = {0, 1, 2, 3, 4, 4, 5, 6, 7, 7, 8, 8, 9,
				10, 11, 10, 11, 10, 11, 10, 11}  ;

      int vec2[2*num_units+1] = {0, 1, 11, 2, 11, 3, 11, 4, 11, 5, 11, 6, 11,
				7, 11, 8, 11, 9, 11, 10, 11}  ;

      CCircuit circuit1(vec1);
      CCircuit circuit2(vec2);

      if (std::fabs(circuit1.Evaluate_Circuit(1e-8, 1000) + 979.269) < 0.01)
	        std::cout << "pass\n";
      else
	        std::cout << "fail\n";

      if (std::fabs(circuit2.Evaluate_Circuit(1e-8, 1000)-57.7668) < 0.01)
	        std::cout << "pass\n";
      else
	        std::cout << "fail\n";
}
