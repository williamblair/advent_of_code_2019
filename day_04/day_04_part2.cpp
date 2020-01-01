/*
 * Day 04 - figure out a password meeting the specified criteria
 *   part 2 - additional criteria
 *
 * compile/run with:
 *   g++ -std=c++11 day_04_part2.cpp -o day_04_part2
 *   ./day_04_part2
 */

#include <iostream>
#include <vector>
#include <array>

// returns true if the given password meets criteria
bool check_criteria(std::array<int,6>& password)
{
    bool condition_met = false;
    
    // condition 1: contains AT LEAST one pair
    // of exactly 2 adjacent digits
    for (size_t i=0; i<password.size()-1; ++i)
    {
        if (password[i] == password[i+1])
        {
            // case 1: i == 0
            if (i == 0)
            {
                if (password[i+2] != password[i])
                {
                    condition_met = true;
                    break;
                }
            }
            // case 2: i == 4
            else if (i == password.size()-2)
            {
                if (password[i-1] != password[i])
                {
                    condition_met = true;
                    break;
                }
            }
            // case 3: i is in between
            else
            {
                if (password[i-1] != password[i] &&
                    password[i+2] != password[i])
                {
                    condition_met = true;
                    break;
                }
            }
        }
    }
    if (!condition_met)
    {
        return false;
    }

    // condition 1.5 now: the two adjacent matching digits
    // are NOT part of a larger group of matching digits

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
    std::array<int,6> test_input_1 = {1,1,2,2,3,3};
    std::cout << "Test 1 criteria check: " << check_criteria(test_input_1) 
              << std::endl;
    
    // doesn't meet criteria
    //  (444 is part of a larger group of 444)
    std::array<int,6> test_input_2 = {1,2,3,4,4,4};
    std::cout << "Test 2 criteria check: " << check_criteria(test_input_2) 
              << std::endl;

    // meets the criteria because even though 1111, the 2,2 makes up for it
    std::array<int,6> test_input_3 = {1,1,1,1,2,2};
    std::cout << "Test 3 criteria check: " << check_criteria(test_input_3) 
              << std::endl;

    // doesn't meet criteria
    std::array<int,6> test_input_4 = {1,2,3,3,3,4};
    std::cout << "Test 4 criteria check: " << check_criteria(test_input_4) 
              << std::endl;

#if 1
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

    // 2553 is too high
    std::cout << "Num passing: " << num_passing << std::endl;
#endif
    
    return 0;
}

