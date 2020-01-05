/*
 * Day 09 - BOOST program, part 2
 * 
 * compile/run with:
 *   g++ -std=c++11 day_09_part2.cpp -o day_09_part2
 *   ./day_09_part2
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <climits>

#include "Intcode_computer.h"
#include "input.h"

void run_program(std::vector<long int>& input)
{
    Intcode_computer computer(input);

    // takes in input 2 instead of 1 this time
    computer.send_input(2);
    computer.run();

    std::cout << "Output:" << std::endl;
    computer.print_output();
}

// no input, outputs itself (a Quine)
std::vector<long int> test_program_1 = {109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99};

// no input, outputs a 16 digit number
std::vector<long int> test_program_2 = {1102,34915192,34915192,7,4,7,99,0};

// no input, outputs the large middle number
std::vector<long int> test_program_3 = {104,1125899906842624,99};

int main(void)
{
//    run_program(test_program_1);
//    run_program(test_program_2);
//    run_program(test_program_3);
    run_program(problem_input);

    return 0;
}

