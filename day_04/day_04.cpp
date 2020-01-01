/*
 * Day 04 - figure out a password meeting the specified criteria
 *
 * compile/run with:
 *   g++ -std=c++11 day_04.cpp -o day_04
 *   ./day_04
 */

#include <iostream>
#include <vector>
#include <array>

// returns true if the given password meets criteria
bool check_criteria(std::array<int,6>& password)
{
    bool condition_met = false;
    
    // condition 1: two adjacent digits are the same
    //  assuming AT LEAST currently
    for (size_t i=0; i<password.size()-1; ++i)
    {
        if (password[i] == password[i+1])
        {
            condition_met = true;
            break;
        }
    }
    if (!condition_met)
    {
        return false;
    }

    // condition 2: going from left to right, the digits never decrease
    // (only ever increase or stay the same)
    condition_met = true;
    int prev_digit = password[0];
    for (size_t i=1; i<password.size(); ++i)
    {
        if (password[i] < prev_digit) {
            condition_met = false;
            break;
        }
        prev_digit = password[i];
    }
    if (!condition_met)
    {
        return false;
    } 
    
    // if we reached this point all conditions are met
    return true;
}

inline void convert_int_to_array(std::array<int,6>& password_array, 
                                 int password)
{
    size_t i = 0;
    int cur_password_val = password;
    for (int divisor = 100000; i<password_array.size(); divisor /= 10, ++i)
    {
        password_array[i] = cur_password_val / divisor;
        cur_password_val = cur_password_val % divisor;
    }
}

int main(void)
{
    // meets the criteria
    std::array<int,6> test_input_1 = {1,1,1,1,1,1};
    std::cout << "Test 1 criteria check: " << check_criteria(test_input_1) 
              << std::endl;
    
    // doesn't meet criteria
    std::array<int,6> test_input_2 = {2,2,3,4,5,0};
    std::cout << "Test 2 criteria check: " << check_criteria(test_input_2) 
              << std::endl;

    // doesn't meet criteria
    std::array<int,6> test_input_3 = {1,2,3,7,8,9};
    std::cout << "Test 3 criteria check: " << check_criteria(test_input_3) 
              << std::endl;

    int num_passing = 0;
    std::array<int,6> current_password_array;
    for (int cur_pass = 108457; cur_pass <= 562041; ++cur_pass)
    {
        convert_int_to_array(current_password_array, cur_pass);
        if (check_criteria(current_password_array))
        {
            ++num_passing;
        }
    }

    std::cout << "Num passing: " << num_passing << std::endl;
    
    return 0;
}

