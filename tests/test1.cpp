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
#include "../includes/CUnit.h"
#include "../includes/CCircuit.h"

int main(int argc, char *argv[])
{

    // valid test case 1
    int vector0[2 * num_units + 1] = {0, 1, 2, 3, 4, 4, 5, 6, 7, 7, 8, 8, 9,
                                    10, 11, 10, 11, 10, 11, 10, 11};
    // valid test case 2
    int vector1[2 * num_units + 1] = { 0, 1, 11, 2, 11, 3, 11, 4, 11, 5, 11, 6, 11,
                                    7, 11, 8, 11, 9, 11, 10, 11 };
    // invalid test case 1 - final concentrate and tailings destinations are incorrect
    int vector2[2 * num_units + 1] = {4, 7, 8, 3, 0, 0, 5, 6, 7, 1, 2, 8, 9,
                                    11, 10, 11, 10, 11, 10, 11, 10};
    // valid test case - topologically the same as valid test case 1
    int vector3[2 * num_units + 1] = {7, 4, 3, 5, 9, 10, 6, 6, 8, 8, 1, 2, 4, 
                                    0, 2, 0, 11, 5, 7, 6, 2};

    CCircuit circuit0(vector0);
    CCircuit circuit1(vector1);
    CCircuit circuit2(vector2);
    CCircuit circuit3(vector3);

    std::cout << "Check validity of circuit0:" << std::endl;
    if (circuit0.Check_Validity())
        std::cout << "pass" << std::endl;
    else
        std::cout << "fail" << std::endl;

    std::cout << "Check validity of circuit1:" << std::endl;
    if (circuit1.Check_Validity())
        std::cout << "pass" << std::endl;
    else
        std::cout << "fail" << std::endl;

    std::cout << "Check validity of circuit2:" << std::endl;
    if (circuit2.Check_Validity())
        std::cout << "fail" << std::endl;
    else
        std::cout << "pass" << std::endl;

    std::cout << "Check validity of circuit3:" << std::endl;
    if (circuit3.Check_Validity())
        std::cout << "pass" << std::endl;
    else
        std::cout << "fail" << std::endl;

}
