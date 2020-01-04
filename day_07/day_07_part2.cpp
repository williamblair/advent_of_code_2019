/*
 * Day 07 - Max thrust signal from amplifiers
 *  this time with a feedback loop
 *
 * compile/run with:
 *   g++ -std=c++11 day_07_part2.cpp -o day_07_part2
 *   ./day_07_part2
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

    void set_phase(int phase)
    {
        computer.send_input(phase); // first input
    }

    // returns the resulting output signal,
    // to be sent as input to the next amplifier
    bool run(int input_signal,int& output_signal)
    {
        computer.send_input(input_signal); // second input

        bool running = true;
        int computer_output = 0;
        try
        {
            do
            {
                running = computer.run_next_opcode(computer_output);
            } while (running);
        } 
        // will be caught when the computer reaches halt instruction
        // this is bad practice - exceptions should NOT manage program
        // control flow - they should be for unexpected errors instead!
        catch(std::runtime_error& e)
        {
//            std::cout << e.what() << std::endl;
            return false;
        }

        output_signal = computer_output;
        return true;
    }

    bool computer_is_running()
    {
        return computer.is_running();
    }
    
private:
    Intcode_computer computer;
};


void run_program(std::vector<int>& input)
{

    int max_thruster_signal = INT_MIN;
    for (int phase1_input = 5; phase1_input <= 9; ++phase1_input)
    {
    for (int phase2_input = 5; phase2_input <= 9; ++phase2_input)
    {
        if (phase2_input == phase1_input)
        {
            continue;
        }
    for (int phase3_input = 5; phase3_input <= 9; ++phase3_input)
    {
        if (phase3_input == phase1_input ||
            phase3_input == phase2_input)
        {
            continue;
        }
    for (int phase4_input = 5; phase4_input <= 9; ++phase4_input)
    {
        if (phase4_input == phase1_input ||
            phase4_input == phase2_input ||
            phase4_input == phase3_input)
        {
            continue;
        }
    for (int phase5_input = 5; phase5_input <= 9; ++phase5_input)
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
    
        // we only set the phase for the very first run
        amp1.set_phase(phase1_input);
        amp2.set_phase(phase2_input);
        amp3.set_phase(phase3_input);
        amp4.set_phase(phase4_input);
        amp5.set_phase(phase5_input);
    
        int next_amp_input = 0;
    
        bool feedback_loop_running = true;
        do
        {
            // first argument is input and second is output
            amp1.run(next_amp_input,next_amp_input);
            amp2.run(next_amp_input,next_amp_input);
            amp3.run(next_amp_input,next_amp_input);
            amp4.run(next_amp_input,next_amp_input);
            amp5.run(next_amp_input,next_amp_input);
    
            feedback_loop_running = amp5.computer_is_running();
            
        } while(feedback_loop_running);
        
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

// max thruster signal 139629729, from phase setting sequence 9,8,7,6,5
std::vector<int> test_program_1 = {3,26,1001,26,-4,26,3,27,1002,27,2,27,1,27,26,
27,4,27,1001,28,-1,28,1005,28,6,99,0,0,5};

// max thruster signal 18216, from phase setting sequence 9,7,8,5,6
std::vector<int> test_program_2 = {3,52,1001,52,-5,52,3,53,1,52,56,54,1007,54,5,55,1005,55,26,1001,54,
-5,54,1105,1,12,1,53,54,53,1008,54,0,55,1001,55,1,55,2,53,55,53,4,
53,1001,56,-1,56,1005,56,6,99,0,0,0,0,10};

int main(void)
{
    run_program(test_program_1);
    run_program(test_program_2);
    
    // answer - 14365052
    run_program(input);

    return 0;
}

