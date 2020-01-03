/*
 * Day 06 - Orbit count
 *
 * compile/run with:
 *   g++ -std=c++11 day_06.cpp -o day_06
 *   ./day_06
 */

#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <string>
#include <map>
#include <algorithm>
#include <climits>

#include "input.h"

std::vector<std::array<std::string,2>> test_input = {
{{"COM","B"}},
{{"B","C"}},
{{"C","D"}},
{{"D","E"}},
{{"E","F"}},
{{"B","G"}},
{{"G","H"}},
{{"D","I"}},
{{"E","J"}},
{{"J","K"}},
{{"K","L"}},
};

inline std::map<std::string,std::string> 
make_direct_orbit_map(std::vector<std::array<std::string,2>>& input)
{
    std::map<std::string,std::string> direct_map;
    for (std::array<std::string,2>& orbit_pair : input)
    {
        // the first entry orbits the second in the pair
        direct_map.insert(std::pair<std::string,std::string>(orbit_pair[1], 
                                                             orbit_pair[0]));
    }

    return direct_map;
}

inline std::vector<std::string> 
get_orbit_list(std::map<std::string,std::string>& orbit_map,
               std::string orbiter)
{
    std::vector<std::string> orbit_list;

    // get the first direct orbit of the entry, assuming its not
    // the lead/center of mass
    if (orbit_map.count(orbiter) == 0)
    {
        return orbit_list;
    }
    std::string direct_orbit = orbit_map[orbiter];
    orbit_list.push_back(direct_orbit);

    // work up the orbit list until we reach center of mass
    while (orbit_map.count(direct_orbit) > 0)
    {
        orbit_list.push_back(direct_orbit);
        direct_orbit = orbit_map[direct_orbit];
    }

    return orbit_list;
}

void run_program(std::vector<std::array<std::string,2>>& input)
{
    std::map<std::string,std::string> direct_map = 
        make_direct_orbit_map(input);

    std::map<std::string,std::vector<std::string>> full_orbit_map;

    // get a count of the total number of direct and indirect orbits
    int orbits_count = 0;
    for (std::array<std::string,2>& orbit_pair : input)
    {
        std::vector<std::string> orbit_list = get_orbit_list(direct_map, 
                                                             orbit_pair[1]);

        full_orbit_map.insert(std::pair<std::string,std::vector<std::string>>
                                        (orbit_pair[1], orbit_list));

        orbits_count += orbit_list.size();
    }

    std::cout << "Total orbits: " << orbits_count << std::endl;
}   

int main(void)
{
    run_program(test_input);
    run_program(input_orbits);

    return 0;
}

