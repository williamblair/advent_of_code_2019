/*
 * Day 02 - run a given opcode program where 1 = add, 2 = multiply, 99 = done/halt
 *
 * compile/run with:
 *   g++ -std=c++11 day_01.cpp -o day_01
 *   ./day_01
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <functional>
#include <map>

// given opcode program to run
std::vector<int> opcode_input = {1,0,0,3,1,1,2,3,1,3,4,3,1,5,0,3,2,1,6,19,1,9,19,23,1,6,23,27,1,10,27,31,1,5,31,35,2,6,35,39,1,5,39,43,1,5,43,47,2,47,6,51,1,51,5,55,1,13,55,59,2,9,59,63,1,5,63,67,2,67,9,71,1,5,71,75,2,10,75,79,1,6,79,83,1,13,83,87,1,10,87,91,1,91,5,95,2,95,10,99,2,9,99,103,1,103,6,107,1,107,10,111,2,111,10,115,1,115,6,119,2,119,9,123,1,123,6,127,2,127,10,131,1,131,6,135,2,6,135,139,1,139,5,143,1,9,143,147,1,13,147,151,1,2,151,155,1,10,155,0,99,2,14,0,0};

// opcode function map. Functions set opcode_index to -1 on error or 
// finish (in the case of the halt (99) opcode)
std::map<int,
         std::function<void(std::vector<int>& input, 
                            int& opcode_index)>> 
opcodes_map = 
{
    {1, [](std::vector<int>& input,int& opcode_index)
    {
        // opcode 1: addition

        if (opcode_index + 3 >= input.size())
        {
            std::cout << "ADDITION ERROR: opcode index greater than input size" 
                      << std::endl;
            opcode_index = -1;
            return;
        }

        // calculate locations
        const int result_index = input[opcode_index+3];
        const int input_1_index = input[opcode_index+1];
        const int input_2_index = input[opcode_index+2];

        // technically should bounds check here...

        // calculate and store the result
        input[result_index] = input[input_1_index] +
                              input[input_2_index];

        opcode_index += 4;
    }},
    
    {2, [](std::vector<int>& input,int& opcode_index)
    {
        // opcode 2: multiplication

        if (opcode_index + 3 >= input.size())
        {
            std::cout << "MULTIPLICATION ERROR: opcode index greater than "
                      << " input size"
                      << std::endl;
            opcode_index = -1;
            return;
        }

        // calculate locations
        const int result_index = input[opcode_index+3];
        const int input_1_index = input[opcode_index+1];
        const int input_2_index = input[opcode_index+2];

        // technically should bounds check here...

        // calculate and store the result
        input[result_index] = input[input_1_index] *
                              input[input_2_index];

        opcode_index += 4;
    }},
    
    {99, [](std::vector<int>& input,int& opcode_index)
    {
        // opcode 99: halt/end
        
        std::cout << "opcode 99 (halt) received" << std::endl;
        opcode_index = -1;
    }}
};


int main(void)
{

    //std::vector<int> test_program = {1,9,10,3,2,3,11,0,99,30,40,50};

    // 1. before running the program, replace position 1 with the value 12
    //    and replace position 2 with the value 2
    opcode_input[1] = 12;
    opcode_input[2] = 2;

    int opcode_index = 0;
    do
    {
        int opcode = opcode_input[opcode_index];
        if (opcodes_map.count(opcode) > 0)
        {
            opcodes_map[opcode](opcode_input, opcode_index);
        }
        else
        {
            std::cout << "Invalid opcode: " << opcode << std::endl;
            opcode_index = -1;
        }

    } while(opcode_index != -1);

    std::cout << "Program memory result:\n";
    for (int val : opcode_input)
    {
        std::cout << val << ",";
    }
    std::cout << std::endl;

    return 0;
}

