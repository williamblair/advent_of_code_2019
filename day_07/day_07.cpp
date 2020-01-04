/*
 * Day 07 - Max thrust signal from amplifiers
 *
 * compile/run with:
 *   g++ -std=c++11 day_07.cpp -o day_07
 *   ./day_07
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <climits>

#include "Intcode_computer.h"
#include "input.h"

class Amplifier
{
public:

    Amplifier(std::vector<int>& program_input) :
        computer(program_input)
    {}
    ~Amplifier()
    {}

    // returns the resulting output signal,
    // to be sent as input to the next amplifier
    int run(int phase_input, int input_signal)
    {
        computer.send_input(phase_input); // first input
        computer.send_input(input_signal); // second input

        bool running = true;
        int output = 0;
        do
        {
            running = computer.run_next_opcode(output);
        } while (running);

        return output;
    }
    
private:
    Intcode_computer computer;
};


void run_program(std::vector<int>& input)
{

    int max_thruster_signal = INT_MIN;
    for (int phase1_input = 0; phase1_input <= 4; ++phase1_input)
    {
    for (int phase2_input = 0; phase2_input <= 4; ++phase2_input)
    {
        if (phase2_input == phase1_input)
        {
            continue;
        }
    for (int phase3_input = 0; phase3_input <= 4; ++phase3_input)
    {
        if (phase3_input == phase1_input ||
            phase3_input == phase2_input)
        {
            continue;
        }
    for (int phase4_input = 0; phase4_input <= 4; ++phase4_input)
    {
        if (phase4_input == phase1_input ||
            phase4_input == phase2_input ||
            phase4_input == phase3_input)
        {
            continue;
        }
    for (int phase5_input = 0; phase5_input <= 4; ++phase5_input)
    {
        if (phase5_input == phase1_input ||
            phase5_input == phase2_input ||
            phase5_input == phase3_input ||
            phase5_input == phase4_input)
        {
            continue;
        }

    Amplifier amp1(input);
    Amplifier amp2(input);
    Amplifier amp3(input);
    Amplifier amp4(input);
    Amplifier amp5(input);

    int next_amp_input = 0;
    next_amp_input = amp1.run(phase1_input, next_amp_input);
    next_amp_input = amp2.run(phase2_input, next_amp_input);
    next_amp_input = amp3.run(phase3_input, next_amp_input);
    next_amp_input = amp4.run(phase4_input, next_amp_input);
    next_amp_input = amp5.run(phase5_input, next_amp_input);

    if (next_amp_input > max_thruster_signal)
    {
        max_thruster_signal = next_amp_input;
    }

    } // phase5_input
    } // phase4_input
    } // phase3_input
    } // phase2_input
    } // phase1_input

    std::cout << "max thruster input: " << max_thruster_signal << std::endl;
}

// max thruster signal 43210, from phase setting sequence 4,3,2,1,0
std::vector<int> test_program_1 = {3,15,3,16,1002,16,10,16,1,16,15,15,4,15,99,0,0};

// max thruster signal 54321, from phase setting sequence 0,1,2,3,4
std::vector<int> test_program_2 = {3,23,3,24,1002,24,10,24,1002,23,-1,23,
101,5,23,23,1,24,23,23,4,23,99,0,0};

// max thruster signal 65210 from phase sequence 1,0,4,3,2
std::vector<int> test_program_3 = {3,31,3,32,1002,32,10,32,1001,31,-2,31,1007,31,0,33,
1002,33,7,33,1,33,31,31,1,32,31,31,4,31,99,0,0,0};

int main(void)
{
    run_program(test_program_1);
    run_program(test_program_2);
    run_program(test_program_3);
    run_program(input);

    return 0;
}

