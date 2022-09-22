/**
 * @file      Genetic_Algorithm.h
 * @author    Galena Group, Yang Bai, Tengteng Huang, Xiao Teng
 * @brief     For genetic algorithm function decleartion
 * @version   0.5
 * @date      2022-03-25
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef Genetic
#define Genetic

// Relevent Parameters for Genetic_Algorithm function
#define NUM_PARENT 150
#define NUM_UNIT 10
#define TOLERANCE 0.001
#define MAX_ITERATIONS 500
#define NUM_CHILDREN 100
#define CROSSOVER_PRO 0.95
#define MUTATE_PRO 0.01
#define MAX_EVOLUTIONS 3000

// Compile switch, If you want to using the function, delete '//'
//#define Parallel  // If defined, using OpenMP for parallelization
//#define DO_TIMING // Doing Timing
//#define Print     // Doing Printing

// Produce child vectors from a list of parent vectors
double Genetic_Algorithm(void);

#endif
