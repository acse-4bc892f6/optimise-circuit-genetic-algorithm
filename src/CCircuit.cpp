/**
 * @file    CCircuit.cpp
 * @author  Xiao Teng, Ian Wang, Yuna Nakamura, Beini Zhang, Jingyu Zhou
 * @brief   Encapsulate test functions
 * @version 0.2
 * @date    2022-03-25
 * 
 * @copyright Copyright (c) 2022
 */

#include <math.h>
#include "../includes/CCircuit.h"

/**
 * @brief   Traverse circuit and mark units that have been passed
 *
 * @param   unit_num        unit number that is marked
 */
void CCircuit::mark_units(int unit_num)
{
    // exit if the unit has already been to
    if (this->units[unit_num].mark)
        return;

    // mark the unit
    this->units[unit_num].mark = 1;

    // concentrate stream and tailing stream this unit is going to
    int conc_num = this->units[unit_num].conc_num;
    int tails_num = this->units[unit_num].tails_num;

    // check if this unit is going to the end stream or passing to another unit
    if (conc_num < num_units)
        this->mark_units(conc_num);
    else
        // return 1 if any unit reaches to concentrate end
        this->conc_toend = 1;

    if (tails_num < num_units)
        this->mark_units(tails_num);
    else
        // return 1 if any unit reaches to tailings end
        this->tails_toend = 1;
}

/**
 * @brief   check validity of circuit
 *
 * @return  bool    true if the vector is valid, false if the vector is invalid
 */
bool CCircuit::Check_Validity()
{
    bool valid = 1;

    // Set all the cells to unseen
    for (int i = 0; i < num_units; i++)
        this->units[i].mark = false;

    // Set no outlet streams
    this->conc_toend = false;
    this->tails_toend = false;

    // Traverse the circuit
    mark_units(this->start);

    int conc_end = num_units;
    int tails_end = num_units + 1;

    // Circuit is invalid if the start is concentrate end or tailings end
    if (this->start == conc_end || this->start == tails_end)
    {
        valid = 0;
        return valid;
    }

    // Circuit is invalid if not all units are marked
    for (int i = 0; i < num_units; i++)
    {
        // Circuit is invalid if any unit has not been to
        if (this->units[i].mark == 0)
        {
            valid = 0;
            break;
        }
        // Circuit is invalid if destination for concentrate and tails are the same
        if (this->units[i].conc_num == this->units[i].tails_num)
        {
            valid = 0;
            break;
        }
        // Circuit is invalid if destination for concentrate or tail of a unit is itself
        if (this->units[i].conc_num == i || this->units[i].tails_num == i)
        {
            valid = 0;
            break;
        }
        // Circuit is invalid if the concentrate end is not unit_num and tailings end is not unit_num+1
        if ((this->units[i].conc_num >= num_units && this->units[i].conc_num != conc_end) ||
            (this->units[i].tails_num >= num_units && this->units[i].tails_num != tails_end))
        {
            valid = 0;
            break;
        }
    }

    // Circuit is invalid if it does not have both concentrate and tails exit stream
    if (this->conc_toend != 1 || this->tails_toend != 1)
        valid = 0;

    return valid;
}

/**
 * @brief   Transfer data from circuit vector to a vector of units i.e. circuit
 *
 * @param   chromosome      Circuit vector
 */
void CCircuit::vector2units(std::vector<int> chromosome)
{
    this->start = chromosome[0];

    // for every two nums in vectors, get its concentrate destination and 
    // tailings destination
    for (int i = 0; i < num_units; i++)
    {
        this->units[i].conc_num = chromosome[i * 2 + 1];
        this->units[i].tails_num = chromosome[i * 2 + 2];
    }
}

/**
 * @brief   Transfer data from circuit vector stored as an integer array to a vector of units i.e. circuit
 *
 * @param   chromosome      Circuit vector stored as an integer array
 */
void CCircuit::vector2units(int *chromosome)
{
    this->start = chromosome[0];

    // for every two nums in vectors, get its concentrate destination and 
    // tailings destination
    for (int i = 0; i < num_units; i++)
    {
        this->units[i].conc_num = chromosome[i * 2 + 1];
        this->units[i].tails_num = chromosome[i * 2 + 2];
    }
}

/**
 * @brief   Score a circuit based on its performance
 *
 * @param   tolerance           Tolerance
 * @param   max_iterations      Maximum number of iterations
 * @return  double              Score
 */
double CCircuit::Evaluate_Circuit(double tolerance, int max_iterations)
{
  int iter = 0;
  bool converage;
  double performance = 0;
  /////////////////////////////////////////////////////////////////
  //1: Give an initial guess for the feed rate of both components ///
  // to every cell in the circuit                                 ///
  /////////////////////////////////////////////////////////////////
  for (int i = 0; i < num_units; i++)
  {
    this->units[i].flow_conc = initial_conc;
    this->units[i].flow_tails = initial_tails;
  }

  while (iter < max_iterations)
  {
    converage = true;

    /////////////////////////////////////////////////////////////////////
    //2. For each unit use the current guess of the feed flowrates    //
    //  to calculate the flowrate of each component in both the      //
    //  concentrate and tailings streams                            //
    //////////////////////////////////////////////////////////////////
    for (int n = 0; n < num_units; n++)
    {
      if ((this->units[n].flow_conc + this->units[n].flow_tails) / 3000 < 1e-10)
      {
        //To stop overflow errors 
        this->units[n].flow_conc = 1e-7;
        this->units[n].flow_tails = 1e-7;
      }
    ///////////////////////////////////////////////////////////////////////////
    //3. Calculate values. Store the current value of the feed to each cell  // 
    //  as an old feed value and set the current value                      //
    //   for all components to zero                                       ///
    /////////////////////////////////////////////////////////////////////////
      this->units[n].set_values();
    }

    //////////////////////////////////////////////////////////////////
    //4. Set the feed to the cell receiving the circuit feed      //
    //  equal to the flowrates of the circuit feed               ////
    /////////////////////////////////////////////////////////////////
    this->units[this->start].flow_conc = initial_conc;
    this->units[this->start].flow_tails = initial_tails;

    /////////////////////////////////////////////////////////////////
    //5. Go over each unit and add the concentrate and            //
    //  tailings flows to the appropriate unit feed               //
    //  based on the linkages in the circuit vector.               //
    /////////////////////////////////////////////////////////////////
      
    for (int n = 0; n < num_units; n++)
    {
      if (this->units[n].conc_num < num_units)
      {
        this->units[this->units[n].conc_num].flow_conc += this->units[n].conc_conc;
        this->units[this->units[n].conc_num].flow_tails += this->units[n].conc_tails;
      }

      if (this->units[n].tails_num < num_units)
      {
        this->units[this->units[n].tails_num].flow_conc += this->units[n].tails_conc;
        this->units[this->units[n].tails_num].flow_tails += this->units[n].tails_tails;
      }
    }

    iter++;

    ////////////////////////////////////////////////////////////////////////////////
    // 6. Check the difference between the newly calculated feed rate and       //
    // the old feed rate for each cell. If any of them have a relative change   //
    // that is above a given threshold (1.0e-6 might be appropriate)           //
    // then repeat from step 2.                                               //
    ///////////////////////////////////////////////////////////////////////////
    for (int n = 0; n < num_units; n++)
    {
      if (abs(this->units[n].flow_conc - this->units[n].flow_conc_old) > tolerance || \
          abs(this->units[n].flow_tails - this->units[n].flow_tails_old) > tolerance)
      {
        converage = false;
        break;
      }
    }
    if (converage)
      break;
  }
    /////////////////////////////////////////////////////////////////////////////////
    // 7. Based on the flowrates of the overall circuit concentrate stream,        //
    // calculate a performance value for the circuit. If there is no convergence   //
    // you may wish to use the worst possible performance as the performance value //
    /////////////////////////////////////////////////////////////////////////////////
  if (converage)
    for (int n = 0; n < num_units; n++)
    {
      if (this->units[n].conc_num > num_units - 1)
        performance += this->units[n].conc_conc * 100 - this->units[n].conc_tails * 500;
    }
  
  else
    return -50000;
  
  return performance;
}

/**
 * @brief   Constructor for CCircuit object from circuit vector
 *
 * @param   chromosome          circuit vector
 * @param   tolerance           error tolerance
 * @param   max_iterations      maximum number of iterations
 * @param   initial_conc        initial feed concentrate
 * @param   initial_tails       initial feed tailings
 */
CCircuit::CCircuit(std::vector<int> chromosome, double tolerance, int max_iterations, int initial_conc, int initial_tails)
{
    this->vector2units(chromosome);

    this->initial_conc = initial_conc;
    this->initial_tails = initial_tails;
}

/**
 * @brief   Constructor for CCircuit object from circuit vector stored as an integer array
 *
 * @param   chromosome          circuit vector stored as an integer array
 * @param   tolerance           error tolerance
 * @param   max_iterations      maximum number of iterations
 * @param   initial_conc        initial feed concentrate
 * @param   initial_tails       initial feed tailings
 */
CCircuit::CCircuit(int *chromosome, double tolerance, int max_iterations, int initial_conc, int initial_tails)
{
    this->vector2units(chromosome);

    this->initial_conc = initial_conc;
    this->initial_tails = initial_tails;
}
