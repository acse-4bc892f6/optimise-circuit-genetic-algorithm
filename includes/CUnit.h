/**
 * @file    CUnit.h
 * @author  Xiao Teng
 * @brief   Header file for the CUnit class
 * @version 0.2
 * @date    2022-03-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once
#include <vector>

/** the rate constant tailings (waste) */
const double K_tails = 0.0005;
/** the rate constant of concentrate (gormanium) */
const double K_conc = 0.005;
/** solids (gormanium + waste) density */
const double rho = 3000;
/** the total solids (gormanium + waste) content of the feed by volume */
const double phi = 0.1;
/** volume of each cell/unit */
const double V = 10;

/**
* @brief  Units that make up a circuit
*/
class CUnit
{
public:
  /** index of the unit to which this unit’s concentrate stream is connected */
  int conc_num;
  /** index of the unit to which this unit’s tailings stream is connected */
  int tails_num;
  /** A Boolean that is changed to true if the unit has been seen */
  bool mark = false;

  /** the mass flow rate of solid (gormanium) */
  double flow_conc = 0;
  /** the mass flow rate of solid  (waste) */
  double flow_tails = 0;

  /** the mass flow rate of solid (gormanium) from previous iteration */
  double flow_conc_old = 0;
  /** the mass flow rate of solid (waste) from previous iteration */
  double flow_tails_old = 0;

  /** concentrate in concentrate stream */
  double conc_conc;
  /** tailings in concentrate stream */
  double conc_tails;
  /** concentrate in tailings stream */
  double tails_conc;   
  /** tailings in tailings stream */
  double tails_tails; 
  
  // Calculate physical parameters for each iteration
  void set_values();
};
