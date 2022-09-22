/**
 * @file      Genetic_Algorithm.cpp
 * @author    Galena Group, Yang Bai, Tengteng Huang, Xiao Teng
 * @brief     For genetic algorithm implementation
 * @version   0.5
 * @date      2022-03-25
 *
 * @copyright Copyright (c) 2022
 *
 */

// Headfile
#include <stdio.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>

#include "../includes/CCircuit.h"
#include "../includes/Genetic_Algorithm.h"

using namespace std;

#ifdef Parallel
#include <omp.h>
#endif

/**
 * @brief   Fill in parent vector by randomly generating numbers
 *
 * @param   parent      Empty vector for loading parent
 * @param   unit_num    Number of circuit units
 */
void create_parent(vector<int> *parent, int unit_num)
{
    int i;
    int concentrate_channel = 0;
    int tailings_channel = 0;
    bool valid = false;
    vector<int> temp(2 * unit_num + 1);

    while (valid == false)
    {
        i = rand() % unit_num;
        temp[0] = i;
        for (int i = 0; i < unit_num; i++)
        {
            while ((concentrate_channel = rand() % (unit_num + 2)) == i)
                ;
            temp[i * 2 + 1] = (concentrate_channel);
            while ((tailings_channel = rand() % (unit_num + 2)) == i || concentrate_channel == tailings_channel)
                ;
            temp[i * 2 + 2] = (tailings_channel);
        }

        CCircuit circuit(temp);

        if (circuit.Check_Validity() && (circuit.Evaluate_Circuit() > 50))
            valid = true;
    }

    for (int i = 0; i < 2 * unit_num + 1; i++)
        parent->push_back(temp[i]);
}

/**
 * @brief   Create a set of parent vectors
 *
 * @param   parent_set      Empty vector for loading multiple parents
 * @param   unit_num        Number of circuit units
 * @param   set_num         Number of Parents
 */
void create_chromosome_set(vector<vector<int> > *parent_set, int unit_num, int set_num)
{
    vector<int> temp[set_num];

    for (int i = 0; i < set_num; i++)
    {
        create_parent(&temp[i], unit_num);
        parent_set->push_back(temp[i]);
    }
}

/**
 * @brief   Calculate fitness value for parent set
 *
 * @param   score               Empty vector for loading fitness value
 * @param   parent_set          Parent vector that already load
 * @param   tolerance           Error tolerance
 * @param   max_iterations      Maximum number of iterations
 */
void calculate_fitness_value(vector<double> *score, vector<vector<int> > parent_set, double tolerance, int max_iterations)
{
    double temp = 0.0;

    for (int i = 0; i < parent_set.size(); i++)
    {
        CCircuit *circuit = new CCircuit(parent_set[i]);
        temp = circuit->Evaluate_Circuit();
        score->push_back(temp);
    }
}

/**
 * @brief   Select best parent in parent set and put it into child set
 *
 * @param   child_set       Vector for loading child vector
 * @param   score           Vector for fitness value
 * @param   parent_set      Vector for Parents set
 */
double best_parent2child(vector<vector<int> > &child_set, vector<double> score, vector<vector<int> > parent_set)
{
    int max_num = 0;
    double max_value = 0;
    child_set.resize(NUM_CHILDREN);

    #ifdef Parallel
      #pragma omp parallel for
        for (int i = 0; i < score.size(); i++)
        {
            if (score[i] > max_value)
            {
                max_num = i;
                max_value = score[i];
            }
        }
    #endif

    #ifndef Parallel
      for (int i = 0; i < score.size(); i++)
      {
          if (score[i] > max_value)
          {
              max_num = i;
              max_value = score[i];
          }
      }
    #endif

    child_set[0]=parent_set[max_num];
    return max_value;
}

/**
 * @brief   Randomly select a parent
 *
 * @param   parent_set      Vector for Parent set
 * @param   score           Vector for fitness value
 * @return  int             Parent number in the parents set
 */
int select_parent(vector<vector<int> > parent_set, vector<double> score)
{
    double over_score = 0.0;
    double probability = 0.0;
    vector<double> sum_probability_map;
    double sum_probability = 0.0;

    #ifdef Parallel
      #pragma omp parallel reduction(+:over_score)
        for (int i = 0; i < score.size(); i++)
        {
            over_score += score[i];
        }
    #endif

    #ifndef Parallel
      for (int i = 0; i < score.size(); i++)
      {
          over_score += score[i];
      }
    #endif

    for (int i = 0; i < score.size(); i++)
    {
        sum_probability = sum_probability + score[i] / over_score;
        sum_probability_map.push_back(sum_probability);
    }
    probability = (rand() % 100) / 100.0;
    for (int i = 0; i < sum_probability_map.size(); i++)
    {
        if (probability <= sum_probability_map[i])
        {
            return i;
        }
    }
    return score.size() / 2;
}

/**
 * @brief   Judge whether the event occurs by given probability
 *
 * @param   x       Probability
 * @return  int     whether the event occurs by given probability
 */
static int get_rand(double x)
{
    double p = rand() % 100 / 100.0;

    if (p >= x)
        return 1;

    return 0;
}

/**
 * @brief   Crossover: Swap a portion of one parent vector with a portion of another parent vector 
 *
 * @param   father      One parent vector
 * @param   mother      Another parent vector
 */
void crossover(vector<int> &father, vector<int> &mother)
{
    int random = 0;
    int temp;

    if ((get_rand(CROSSOVER_PRO)) == 1)
        return;

    random = rand() % (2*NUM_UNIT+1);

    for (int i = 0; i < random; i++)
    {
        temp = father[i];
        father[i] = mother[i];
        mother[i] = temp;
    }
}

/**
 * @brief   Mutate: Random changes in the numbers in the vector
 *
 * @param   before      Vector to mutate
 */
void mutate(vector<int> &before)
{
    if ((get_rand(MUTATE_PRO)) == 1)
        return;

    int random_unit_num = rand() % (2*NUM_UNIT+1);
    int random = rand() % (NUM_UNIT+2);

    before[random_unit_num] = random;
}

/**
 * @brief   Produce child vectors from a list of parent vectors
 * 
 * @return  double  highest score
 */
double Genetic_Algorithm(void)
{
  srand((unsigned)time(0));

  #ifdef Print
      ofstream outfile;
      outfile.open("data.txt");
  #endif

  // variable 
  vector< vector<int> > parent_set;
  vector<double>        fitness_score;
  vector< vector<int> > child_set;
  child_set.resize(NUM_CHILDREN);
  int father_num, mother_num = 0;
  vector<int> father, mother;
  vector <double> emp;
  int k =0;
  double the_max_value = 0;
  double finalsocre;

   #ifdef DO_TIMING

    #ifdef Parallel
      double start = omp_get_wtime();
    #endif

    #ifndef Parallel
      clock_t  start = clock();
    #endif

  #endif

  // Step 1: Start with the vectors representing the initial random collection of valid circuits.
  create_chromosome_set(&parent_set, NUM_UNIT, NUM_PARENT);

  while(k<MAX_EVOLUTIONS)
  {

    // Step 2: Calculate the fitness value for each of these vectors.
    calculate_fitness_value(&fitness_score, parent_set, TOLERANCE, MAX_ITERATIONS);

    // Step 3: Find best parent and put it into child_set
    the_max_value=best_parent2child(child_set, fitness_score, parent_set);

    for(int i=1;i<NUM_CHILDREN;i++)
    {
      child_set[i] = child_set[0];
    }
     
    int child_num=1;

    while(NUM_CHILDREN>child_num)
    {
      // Step 4: Select a pair of the parent vectors with a probability that depends on the fitness value
      father_num = select_parent(parent_set, fitness_score);
      mother_num = select_parent(parent_set, fitness_score);
      while(father_num == mother_num)
      {
        mother_num = select_parent(parent_set, fitness_score);
      }
      father = parent_set[father_num];
      mother = parent_set[mother_num];

      // Step 5: Crossover
      crossover(father, mother);

      // Step 6: Mutate
      mutate(father);
      mutate(mother);

      // Step 7: Check validity
      CCircuit *Cfather = new CCircuit(father);
      CCircuit *Cmother = new CCircuit(mother);

      if (Cfather->Check_Validity() && Cfather->Evaluate_Circuit() > 0)
      {
        // Step 8: Add father to child list
        child_set[child_num] = father;
        child_num++;
      }

      if (Cmother->Check_Validity() && Cmother->Evaluate_Circuit() > 0)
      {
        // Step 8: Add mother to child list
        child_set[child_num] = mother;
        child_num++;
      }

      delete Cfather;
      delete Cmother;
    }

    #ifdef Print
      for(int i=0;i <(1+2*NUM_UNIT) ; i++)
      {
        outfile << child_set[0][i]<<" ";
      }
      outfile << fitness_score[0]<<endl;
    #endif

    parent_set = child_set;
    finalsocre = fitness_score[0];
    fitness_score = emp;
    k++;
    cout<<"k = "<<k<<" "<<"the max value = "<<the_max_value<<endl;
  } 
   #ifdef DO_TIMING

    #ifdef Parallel
      double end = omp_get_wtime();
      cout << " Runtime: " << (double)(end - start) << " s" << endl;
    #endif

    #ifndef Parallel
      clock_t  end = clock();
      cout << " Runtime: " << (double)(end - start) / CLOCKS_PER_SEC << " s" << endl;
    #endif

  #endif

  #ifdef Print
      outfile.close();
  #endif
  return finalsocre ;
}
