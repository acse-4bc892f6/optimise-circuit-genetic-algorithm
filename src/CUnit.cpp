/**
 * @file    CUnit.cpp
 * @author  Xiao Teng
 * @brief   Calculation of products (Gormanium) and wastes
 * @version 0.2
 * @date    2022-03-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "../includes/CUnit.h"

/**
 * @brief Calculate physical parameters for each iteration
 * 
 */
void CUnit::set_values()
{
    // calculate physical parameters
    double tau = (V * phi * rho) / (this->flow_conc + this->flow_tails);
    double R_conc = K_conc * tau / (1 + K_conc * tau);
    double R_tails = K_tails * tau / (1 + K_tails * tau);

    // calculate the valuable matters and tailings matters in 
    // concentrate and tailings streams
    this->conc_conc = this->flow_conc * R_conc;
    this->conc_tails = this->flow_tails * R_tails;
    this->tails_conc = this->flow_conc * (1 - R_conc);
    this->tails_tails = this->flow_tails * (1 - R_tails);

    // reset the feed flow values and store the old fow values
    this->flow_conc_old = this->flow_conc;
    this->flow_tails_old = this->flow_tails;
    this->flow_conc = 0;
    this->flow_tails = 0;
}
