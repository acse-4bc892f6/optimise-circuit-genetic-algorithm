/**
 * @file    CCircuit.h
 * @author  Xiao Teng
 * @brief   Header file for the CCircuit class
 * @version 0.2
 * @date    2022-03-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once
#include "CUnit.h"

/** Number of units. This is constant and default value is 10 */
const int num_units = 10;

/**
* @brief    Circuit made up of units connected to each other, constructed from chromosome array or vector
*/
class CCircuit
{
public:

    // Constructor for CCircuit object from circuit vector
    CCircuit(std::vector<int> chromosome, double tolerance = 1e-6, \
            int max_iterations = 1000, int initial_conc = 10, \
            int initial_tails = 100);

    // Constructor for CCircuit object from circuit vector stored as an integer array
    CCircuit(int *chromosome, double tolerance = 1e-6, \
            int max_iterations = 1000, int initial_conc = 10, \
            int initial_tails = 100);

    // Check validity of circuit
    bool Check_Validity();
    
    // Score a circuit based on its performance
    double Evaluate_Circuit(double tolerance = 1e-6, int max_iterations = 1000);

private:

    /** Array of units of length num_units. Build up the circuit. */
    CUnit units[num_units];

    /** Feed unit number */
    int start;
    /** True if concentrate outlet is found */
    bool conc_toend;
    /** True if tailings outlet is found */
    bool tails_toend;

    /** tolerance for error in concentrate flow an tailings flow */
    double tolerance;
    /** Maximum number of iterations */
    int max_iterations;

    /** Initial value of concentrate outlet */
    double initial_conc;
    /** Initial value of tailings outlet */
    double initial_tails;
    
    // Transfer data from circuit vector to a vector of units i.e. circuit
    void vector2units(std::vector<int> chromosome);

    // Transfer data from circuit vector stored as an integer array to a vector of units i.e. circuit
    void vector2units(int *chromosome);

    // Traverse circuit and mark units that have been passed
    void mark_units(int unit_num);  
};
