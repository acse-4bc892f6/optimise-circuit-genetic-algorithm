/**
 * @file    main.cpp
 * @author  Yang Bai
 * @brief   main file for running main function
 * @version 0.1
 * @date    2022-03-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <iostream>
#include "../includes/CCircuit.h"
#include "../includes/Genetic_Algorithm.h"

/**
 * @brief   Example of how to use Genertic Algorithm, 
 *          If you want to change parameters, go to Genetic_Algorithm.h
 * @return  int
 */
int main()
{
    double score;
    score = Genetic_Algorithm();
    std::cout << "The higest score is : " << score << std::endl;
    return 0;
}
