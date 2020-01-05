#ifndef _INTCODE_COMPUTER_H_INCLUDED_
#define _INTCODE_COMPUTER_H_INCLUDED_

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <functional>
#include <map>
#include <queue>
#include <exception>
#include <algorithm>
#include <sstream>

class Intcode_computer
{
public:
    Intcode_computer(std::vector<long int>& input)
    {
        #define INPUT_SCALE_SIZE 100
        opcode_input.assign(input.size()*INPUT_SCALE_SIZE,0);
        std::copy(input.begin(),input.end(),opcode_input.begin());
    }

    ~Intcode_computer()
    {}

    void send_input(long int val)
    {
        input_queue.push(val);
    }

    void print_output()
    {
        for (long int val : output_vec)
        {
            std::cout << val << ",";
        }
        std::cout << std::endl;
    }

    // returns false if output was provided
    bool run_next_opcode(long int& output)
    {
        long int opcode = opcode_input[opcode_index] % 100;

        if (opcodes_map.count(opcode) == 0)
        {
            std::stringstream error_str;
            error_str << "Opcode index: " << opcode_index << std::endl;
            error_str << "unknown opcode: " << opcode << std::endl;
            throw std::runtime_error(error_str.str());
        }
    
        long int prev_output_size = output_vec.size();        
        
        opcodes_map[opcode](opcode_input, opcode_index);

        if (output_vec.size() != prev_output_size)
        {
            output = output_vec[output_vec.size()-1];
            return false;
        }

        return true;
    }

    void run()
    {
        do
        {
            long int output;
            run_next_opcode(output);
        
        } while (opcode_index != -1);
    }

    bool is_running() const
    {
        return (opcode_index != -1);
    }

private:

    std::vector<long int> opcode_input;
    long int opcode_index = 0;

    long int relative_offset = 0; // for mode 2 params; relative address offsets

    std::queue<long int> input_queue; // for the input function
    std::vector<long int> output_vec; // for the output function

// opcode function map. Functions set opcode_index to -1 on error or 
// finish (in the case of the halt (99) opcode)
std::map<long int,
         std::function<void(std::vector<long int>& input, 
                            long int& opcode_index)>> 
opcodes_map = 
{
    {1, [&](std::vector<long int>& input,long int& opcode_index)
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
        std::vector<long int> param_modes = parse_opcode(input[opcode_index]);
        if (param_modes.size() != 4) {
            std::cout << "ADDITION ERROR: bad opcode parse" << std::endl;
            opcode_index = -1;
            return;
        }

        // calculate locations
        const long int result_index = input[opcode_index+3];
        const long int input_1_index = input[opcode_index+1];
        const long int input_2_index = input[opcode_index+2];

        // technically should bounds check here...

        // calculate and store the result, checking for immediate mode or
        // position mode based on parsed parameters
        input[result_index] = (param_modes[1] ? input_1_index : 
                                                input[input_1_index]) +
                              (param_modes[2] ? input_2_index : 
                                                input[input_2_index]);

        opcode_index += 4;
    }},
    
    {2, [&](std::vector<long int>& input,long int& opcode_index)
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
        std::vector<long int> param_modes = parse_opcode(input[opcode_index]);
        if (param_modes.size() != 4) {
            std::cout << "ADDITION ERROR: bad opcode parse" << std::endl;
            opcode_index = -1;
            return;
        }

        // calculate locations
        const long int result_index = input[opcode_index+3];
        const long int input_1_index = input[opcode_index+1];
        const long int input_2_index = input[opcode_index+2];

        // technically should bounds check here...

        // calculate and store the result
        input[result_index] = (param_modes[1] ? input_1_index : 
                                                input[input_1_index]) *
                              (param_modes[2] ? input_2_index : 
                                                input[input_2_index]);

        opcode_index += 4;
    }},

    {3, [&](std::vector<long int>& input,long int& opcode_index)
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
        std::vector<long int> param_modes = parse_opcode(input[opcode_index]);
        if (param_modes.size() != 2) {
            std::cout << "COPY ERROR: bad opcode parse" << std::endl;
            opcode_index = -1;
            return;
        }

        // calculate locations
        const long int result_index = input[opcode_index+1];

        // get input
        if (input_queue.empty()) {
            std::cout << "COPY ERROR: no input to read" << std::endl;
            opcode_index = -1;
            return;
        }
        long int input_val = input_queue.front();
        input_queue.pop();

        // write the input to given location
        input[result_index] = input_val;

        opcode_index += 2;
    }},

    {4, [&](std::vector<long int>& input,long int& opcode_index)
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
        std::vector<long int> param_modes = parse_opcode(input[opcode_index]);
        if (param_modes.size() != 2) {
            std::cout << "COPY ERROR: bad opcode parse" << std::endl;
            opcode_index = -1;
            return;
        }

        // calculate locations
        const long int read_from_index = input[opcode_index+1];

        // get the value to output
        long int output_val = param_modes[1] ? read_from_index : 
                                          input[read_from_index];

        // write the output
        output_vec.push_back(output_val);

        opcode_index += 2;
    }},

    {5, [&](std::vector<long int>& input,long int& opcode_index)
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
        std::vector<long int> param_modes = parse_opcode(input[opcode_index]);
        if (param_modes.size() != 3) {
            std::cout << "JUMP IF TRUE ERROR: bad opcode parse" << std::endl;
            opcode_index = -1;
            return;
        }

        // calculate locations
        long int test_index = input[opcode_index+1];
        long int jump_index = input[opcode_index+2];

        // if the first parameter is non-zero, set the instruction pointer
        // to the value of the second parameter
        if ((param_modes[1] ? test_index : input[test_index]) != 0)
        {
            opcode_index = param_modes[2] ? jump_index : input[jump_index];
            return;
        }

        opcode_index += 3;
    }},

    {6, [&](std::vector<long int>& input,long int& opcode_index)
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
        std::vector<long int> param_modes = parse_opcode(input[opcode_index]);
        if (param_modes.size() != 3) {
            std::cout << "JUMP IF FALSE ERROR: bad opcode parse" << std::endl;
            opcode_index = -1;
            return;
        }

        // calculate locations
        long int test_index = input[opcode_index+1];
        long int jump_index = input[opcode_index+2];

        // if the first parameter IS zero, set the instruction pointer
        // to the value of the second parameter
        if ((param_modes[1] ? test_index : input[test_index]) == 0)
        {
            opcode_index = param_modes[2] ? jump_index : input[jump_index];
            return;
        }

        opcode_index += 3;
    }},

    {7, [&](std::vector<long int>& input,long int& opcode_index)
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
        std::vector<long int> param_modes = parse_opcode(input[opcode_index]);
        if (param_modes.size() != 4) {
            std::cout << "LESS THAN ERROR: bad opcode parse" << std::endl;
            opcode_index = -1;
            return;
        }

        // calculate locations
        long int test_index_1 = input[opcode_index+1];
        long int test_index_2 = input[opcode_index+2];
        long int store_index = input[opcode_index+3];

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

    {8, [&](std::vector<long int>& input,long int& opcode_index)
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
        std::vector<long int> param_modes = parse_opcode(input[opcode_index]);
        if (param_modes.size() != 4) {
            std::cout << "EQUALS ERROR: bad opcode parse" << std::endl;
            opcode_index = -1;
            return;
        }

        // calculate locations
        long int test_index_1 = input[opcode_index+1];
        long int test_index_2 = input[opcode_index+2];
        long int store_index = input[opcode_index+3];

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

    {9, [&](std::vector<long int>& input,long int& opcode_index)
    {
        // opcode 9: adjust offset

        if (opcode_index + 1 >= input.size())
        {
            std::cout << "ADJUST OFFSET ERROR: opcode index greater than "
                      << " input size"
                      << std::endl;
            opcode_index = -1;
            return;
        }

        // get parameter modes
        std::vector<long int> param_modes = parse_opcode(input[opcode_index]);
        if (param_modes.size() != 2) {
            std::cout << "ADJUST OFFSET ERROR: bad opcode parse" << std::endl;
            opcode_index = -1;
            return;
        }

        // calculate locations
        long int test_index_1 = input[opcode_index+1];
        long int test_index_2 = input[opcode_index+2];
        long int store_index = input[opcode_index+3];

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

        opcode_index += 2;
    }},

    
    {99, [&](std::vector<long int>& input,long int& opcode_index)
    {
        // opcode 99: halt/end
        
     //   std::cout << "opcode 99 (halt) received" << std::endl;
        opcode_index = -1;
    }}
};

// returns a vector containing
// 1. the instruction
// 2 through end: the mode of each of the parameters
std::vector<long int> parse_opcode(long int opcode)
{
    std::vector<long int> parsed_values;

    // get the instruction
    long int instruction = opcode % 100; // 1s and 10s place
    parsed_values.push_back(instruction);

    // get the amount of parameters used by said instruction
    // (not including the instruction itself)
    static std::map<int,long int> num_params_list = {
        {1,3},
        {2,3},
        {3,1},
        {4,1},
        {5,2},
        {6,2},
        {7,3},
        {8,3},
        {9,1},
        {99,0}
    };

    long int num_params = num_params_list[instruction];

    // get each of the parameters modes
    opcode /= 100;
    for (size_t i=0; i<num_params; ++i)
    {
        long int next_mode = opcode % 10;
        opcode /= 10; 

        parsed_values.push_back(next_mode);
    }
    
    return parsed_values;
}
    
    // returns the appropriate value based on given parameter mode
    long int get_parametered_value(int param_mode,
                                   int input_index, 
                                   std::vector<long int>& input)
    {
        int result_val;
        switch (param_mode)
        {
            case 0:
                result_val = input[input_index];
                break;
            case 1:
                result_val = input_index;
                break;
            case 2:
                result_val = input[input_index+relative_offset];
                break;
            default:
                std::cout << "ERROR - unhandled param mode" << std::endl;
                result_val = -1;
                break;
        }
        return result_val;
    }

    // returns the appropriate address based on parameter mode
    long int* get_parametered_address(int param_mode,
                                      int input_index, 
                                      std::vector<long int>& input)
    {
        long int* result_addr;
        switch (param_mode)
        {
            case 0:
                result_addr = &input[input_index];
                break;
            case 1:
                result_addr = &input[input_index];
                break;
            case 2:
                result_addr = &input[input_index+relative_offset];
                break;
            default:
                std::cout << "ERROR - unhandled param mode" << std::endl;
                result_addr = NULL;
                break;
        }
        return result_addr;
    }

}; // end Intcode_computer class

#endif // _INTCODE_COMPUTER_H_INCLUDED_

