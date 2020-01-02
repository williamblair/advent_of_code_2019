#ifndef _INTCODE_COMPUTER_H_INCLUDED_
#define _INTCODE_COMPUTER_H_INCLUDED_

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <functional>
#include <map>
#include <queue>

class Intcode_computer
{
public:
    Intcode_computer(std::vector<int>& input) :
        opcode_input(input)
    {}

    ~Intcode_computer()
    {}

    void send_input(int val)
    {
        input_queue.push(val);
    }

    void print_output()
    {
        for (int val : output_vec)
        {
            std::cout << val << ",";
        }
        std::cout << std::endl;
    }

    void run()
    {
        do
        {
            int opcode = opcode_input[opcode_index] % 100;

            if (opcodes_map.count(opcode) == 0)
            {
                std::cout << "unknown opcode: " << opcode << std::endl;
                break;
            }
            
            opcodes_map[opcode](opcode_input, opcode_index);

        } while (opcode_index != -1);
    }

private:

    std::vector<int>& opcode_input;
    int opcode_index = 0;

    std::queue<int> input_queue; // for the input function
    std::vector<int> output_vec; // for the output function

// opcode function map. Functions set opcode_index to -1 on error or 
// finish (in the case of the halt (99) opcode)
std::map<int,
         std::function<void(std::vector<int>& input, 
                            int& opcode_index)>> 
opcodes_map = 
{
    {1, [&](std::vector<int>& input,int& opcode_index)
    {
        // opcode 1: addition

        if (opcode_index + 3 >= input.size())
        {
            std::cout << "ADDITION ERROR: opcode index greater than input size" 
                      << std::endl;
            opcode_index = -1;
            return;
        }

        // get parameter modes
        std::vector<int> param_modes = parse_opcode(input[opcode_index]);
        if (param_modes.size() != 4) {
            std::cout << "ADDITION ERROR: bad opcode parse" << std::endl;
            opcode_index = -1;
            return;
        }

        // calculate locations
        const int result_index = input[opcode_index+3];
        const int input_1_index = input[opcode_index+1];
        const int input_2_index = input[opcode_index+2];

        // technically should bounds check here...

        // calculate and store the result, checking for immediate mode or
        // position mode based on parsed parameters
        input[result_index] = (param_modes[1] ? input_1_index : 
                                                input[input_1_index]) +
                              (param_modes[2] ? input_2_index : 
                                                input[input_2_index]);

        opcode_index += 4;
    }},
    
    {2, [&](std::vector<int>& input,int& opcode_index)
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

        // get parameter modes
        std::vector<int> param_modes = parse_opcode(input[opcode_index]);
        if (param_modes.size() != 4) {
            std::cout << "ADDITION ERROR: bad opcode parse" << std::endl;
            opcode_index = -1;
            return;
        }

        // calculate locations
        const int result_index = input[opcode_index+3];
        const int input_1_index = input[opcode_index+1];
        const int input_2_index = input[opcode_index+2];

        // technically should bounds check here...

        // calculate and store the result
        input[result_index] = (param_modes[1] ? input_1_index : 
                                                input[input_1_index]) *
                              (param_modes[2] ? input_2_index : 
                                                input[input_2_index]);

        opcode_index += 4;
    }},

    {3, [&](std::vector<int>& input, int& opcode_index)
    {
        // opcode 3: copy input value to given location
        
        if (opcode_index + 1 >= input.size())
        {
            
            std::cout << "COPY ERROR: opcode index greater than "
                      << " input size"
                      << std::endl;
            opcode_index = -1;
            return;
        }

        // get parameter modes
        std::vector<int> param_modes = parse_opcode(input[opcode_index]);
        if (param_modes.size() != 2) {
            std::cout << "COPY ERROR: bad opcode parse" << std::endl;
            opcode_index = -1;
            return;
        }

        // calculate locations
        const int result_index = input[opcode_index+1];

        // get input
        if (input_queue.empty()) {
            std::cout << "COPY ERROR: no input to read" << std::endl;
            opcode_index = -1;
            return;
        }
        int input_val = input_queue.front();
        input_queue.pop();

        // write the input to given location
        input[result_index] = input_val;

        opcode_index += 2;
    }},

    {4, [&](std::vector<int>& input, int& opcode_index)
    {
        // opcode 4: read from location and output it

        if (opcode_index + 1 >= input.size())
        {
            
            std::cout << "WRITE OUT ERROR: opcode index greater than "
                      << " input size"
                      << std::endl;
            opcode_index = -1;
            return;
        }
        
        // get parameter modes
        std::vector<int> param_modes = parse_opcode(input[opcode_index]);
        if (param_modes.size() != 2) {
            std::cout << "COPY ERROR: bad opcode parse" << std::endl;
            opcode_index = -1;
            return;
        }

        // calculate locations
        const int read_from_index = input[opcode_index+1];

        // get the value to output
        int output_val = param_modes[1] ? read_from_index : 
                                          input[read_from_index];

        // write the output
        output_vec.push_back(output_val);

        opcode_index += 2;
    }},

    {5, [&](std::vector<int>& input,int& opcode_index)
    {
        // opcode 5: jump if true

        if (opcode_index + 2 >= input.size())
        {
            std::cout << "JUMP IF TRUE ERROR: opcode index greater than "
                      << " input size"
                      << std::endl;
            opcode_index = -1;
            return;
        }

        // get parameter modes
        std::vector<int> param_modes = parse_opcode(input[opcode_index]);
        if (param_modes.size() != 3) {
            std::cout << "JUMP IF TRUE ERROR: bad opcode parse" << std::endl;
            opcode_index = -1;
            return;
        }

        // calculate locations
        int test_index = input[opcode_index+1];
        int jump_index = input[opcode_index+2];

        // if the first parameter is non-zero, set the instruction pointer
        // to the value of the second parameter
        if ((param_modes[1] ? test_index : input[test_index]) != 0)
        {
            opcode_index = param_modes[2] ? jump_index : input[jump_index];
            return;
        }

        opcode_index += 3;
    }},

    {6, [&](std::vector<int>& input,int& opcode_index)
    {
        // opcode 6: jump if false

        if (opcode_index + 2 >= input.size())
        {
            std::cout << "JUMP IF FALSE ERROR: opcode index greater than "
                      << " input size"
                      << std::endl;
            opcode_index = -1;
            return;
        }

        // get parameter modes
        std::vector<int> param_modes = parse_opcode(input[opcode_index]);
        if (param_modes.size() != 3) {
            std::cout << "JUMP IF FALSE ERROR: bad opcode parse" << std::endl;
            opcode_index = -1;
            return;
        }

        // calculate locations
        int test_index = input[opcode_index+1];
        int jump_index = input[opcode_index+2];

        // if the first parameter IS zero, set the instruction pointer
        // to the value of the second parameter
        if ((param_modes[1] ? test_index : input[test_index]) == 0)
        {
            opcode_index = param_modes[2] ? jump_index : input[jump_index];
            return;
        }

        opcode_index += 3;
    }},

    {7, [&](std::vector<int>& input,int& opcode_index)
    {
        // opcode 7: less than

        if (opcode_index + 3 >= input.size())
        {
            std::cout << "LESS THAN ERROR: opcode index greater than "
                      << " input size"
                      << std::endl;
            opcode_index = -1;
            return;
        }

        // get parameter modes
        std::vector<int> param_modes = parse_opcode(input[opcode_index]);
        if (param_modes.size() != 4) {
            std::cout << "LESS THAN ERROR: bad opcode parse" << std::endl;
            opcode_index = -1;
            return;
        }

        // calculate locations
        int test_index_1 = input[opcode_index+1];
        int test_index_2 = input[opcode_index+2];
        int store_index = input[opcode_index+3];

        // if the first parameter is less than the second parameter,
        // store 1 in the position given by the third parameter
        if ((param_modes[1] ? test_index_1 : input[test_index_1]) <
            (param_modes[2] ? test_index_2 : input[test_index_2]))
        {
            input[store_index] = 1;
        }
        // otherwise store 0
        else
        {
            input[store_index] = 0;
        }

        opcode_index += 4;
    }},

    {8, [&](std::vector<int>& input,int& opcode_index)
    {
        // opcode 7: equals

        if (opcode_index + 3 >= input.size())
        {
            std::cout << "EQUALS ERROR: opcode index greater than "
                      << " input size"
                      << std::endl;
            opcode_index = -1;
            return;
        }

        // get parameter modes
        std::vector<int> param_modes = parse_opcode(input[opcode_index]);
        if (param_modes.size() != 4) {
            std::cout << "EQUALS ERROR: bad opcode parse" << std::endl;
            opcode_index = -1;
            return;
        }

        // calculate locations
        int test_index_1 = input[opcode_index+1];
        int test_index_2 = input[opcode_index+2];
        int store_index = input[opcode_index+3];

        // if the first parameter equals the second parameter,
        // store 1 in the position given by the third parameter
        if ((param_modes[1] ? test_index_1 : input[test_index_1]) == 
            (param_modes[2] ? test_index_2 : input[test_index_2]))
        {
            input[store_index] = 1;
        }
        // otherwise store 0
        else
        {
            input[store_index] = 0;
        }

        opcode_index += 4;
    }},
    
    {99, [&](std::vector<int>& input,int& opcode_index)
    {
        // opcode 99: halt/end
        
        std::cout << "opcode 99 (halt) received" << std::endl;
        opcode_index = -1;
    }}
};

// returns a vector containing
// 1. the instruction
// 2 through end: the mode of each of the parameters
std::vector<int> parse_opcode(int opcode)
{
    std::vector<int> parsed_values;

    // get the instruction
    int instruction = opcode % 100; // 1s and 10s place
    parsed_values.push_back(instruction);

    // get the amount of parameters used by said instruction
    // (not including the instruction itself)
    static std::map<int,int> num_params_list = {
        {1,3},
        {2,3},
        {3,1},
        {4,1},
        {5,2},
        {6,2},
        {7,3},
        {8,3},
        {99,0}
    };

    int num_params = num_params_list[instruction];

    // get each of the parameters modes
    opcode /= 100;
    for (size_t i=0; i<num_params; ++i)
    {
        int next_mode = opcode % 10;
        opcode /= 10; 

        parsed_values.push_back(next_mode);
    }
    
    return parsed_values;
}

}; // end Intcode_computer class

#endif // _INTCODE_COMPUTER_H_INCLUDED_

