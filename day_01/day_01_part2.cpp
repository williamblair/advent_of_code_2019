/*
 * Day 01 - calculate the the sum of the fuel requirements for all modules
 * given. 
 *   This time, you need to account for the weight of the fuel needed
 *   for the weight of the module
 *
 * compile/run with:
 *   g++ -std=c++11 day_01_part2.cpp -o day_01_part2
 *   ./day_01_part2
 */

#include <iostream>
#include <vector>
#include <cmath>

// given input modules
std::vector<int> modules = {
116860,
130144,
79347,
120725,
137692,
139037,
72089,
133224,
102168,
100679,
122298,
132969,
109196,
85162,
66316,
68461,
87608,
108091,
71061,
85477,
97748,
105766,
141169,
94553,
98932,
134376,
69822,
104858,
102584,
59682,
52092,
105784,
144100,
83695,
130436,
105447,
133102,
82770,
68684,
103878,
136774,
71462,
96828,
74743,
127523,
124145,
148013,
103862,
80052,
74095,
130394,
125589,
137576,
111299,
69311,
63144,
119014,
136084,
94348,
109511,
102493,
117791,
76202,
138442,
72724,
104579,
80285,
56847,
145460,
132255,
58264,
60460,
98995,
63343,
51207,
133619,
126155,
130707,
105010,
104589,
128527,
67715,
71823,
82517,
74115,
135483,
82230,
127410,
128969,
140127,
59133,
145973,
109430,
103608,
113203,
133402,
123971,
71761,
114178,
52940
};


// when doing the initial call current_sum should be set to 0
// as it's the running recursive fuel amount total
int calc_module_fuel_usage(int mass, int current_sum)
{
    // 1. calculate the fuel and add it to the total
    int fuel_amount = (mass / 3) - 2;
    
    // 2. if the amount isn't negative add it to the total
    if (fuel_amount > 0)
    {
        current_sum += fuel_amount;

        // 3. take the fuel amount and use that as input mass
        //    for more fuel usage
        current_sum = calc_module_fuel_usage(fuel_amount, current_sum);
    }

    return current_sum;
}

int main(void)
{
    int fuel_sum = 0;

    for (size_t i=0; i<modules.size(); ++i)
    {
        // recursive function; starts with a sum of 0
        fuel_sum += calc_module_fuel_usage(modules[i], 0);
    }

    std::cout << "Total fuel usage: " << fuel_sum << std::endl;

    return 0;
}

