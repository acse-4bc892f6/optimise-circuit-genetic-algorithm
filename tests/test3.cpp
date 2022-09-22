/**
 * @file test1.cpp
 * @author Wan, Ian I
 * @version 0.2
 * @date 2022-03-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <iostream>
#include <cmath>
#include "../includes/Genetic_Algorithm.h"

int main(int argc, char *argv[])
{

    double exact = 375.495;
    double score = Genetic_Algorithm();

    if (std::fabs(score - exact) < 0.01)
        std::cout << "pass" << std::endl;
    else
        std::cout << "fail" << std::endl;
}
