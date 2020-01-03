/*
 * Day 06 - Orbit count
 *  part two - closest orbit trip
 * compile/run with:
 *   g++ -std=c++11 day_06_part2.cpp -o day_06_part2
 *   ./day_06_part2
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
{{"K","YOU"}},
{{"I","SAN"}}
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

    // get the total orbit list for each position
    for (std::array<std::string,2>& orbit_pair : input)
    {
        std::vector<std::string> orbit_list = get_orbit_list(direct_map, 
                                                             orbit_pair[1]);

        full_orbit_map.insert(std::pair<std::string,std::vector<std::string>>
                                        (orbit_pair[1], orbit_list));

    }

    // get a list of positions which both our start and target goal
    // have in common
    std::vector<std::string>& san_positions = full_orbit_map["SAN"];
    std::vector<std::string> common_positions;
    for (std::string position : full_orbit_map["YOU"])
    {
        // check if the position is also used by the SAN position
        if (std::find(san_positions.begin(),
                      san_positions.end(),
                      position) != 
            san_positions.end())
        {
            common_positions.push_back(position);
        }
    }

    // find the common position with the largest orbital list (means its
    // furthest down the chain)
    std::string closest_common_pos = "NOT FOUND";
    int max_orbital_length = INT_MIN;
    for (std::string position : common_positions)
    {
        int length = full_orbit_map[position].size();
        if (length > max_orbital_length)
        {
            max_orbital_length = length;
            closest_common_pos = position;
        }
    }

    // calculate the number of hops to reach the closest common pos from the
    // YOU position
    int you_to_closest = 0;
    std::vector<std::string>& you_list = full_orbit_map["YOU"];
    std::vector<std::string>::iterator closest_pos = 
        std::find(you_list.begin(),you_list.end(),closest_common_pos);

    // start at +1 because the list contains the first position twice,
    // so we need to skip it
    you_to_closest = closest_pos - (you_list.begin()+1);

    // calculate the number of hops from the closest to the goal SAN position
    int closest_to_san = 0;
    std::vector<std::string>& san_list = full_orbit_map["SAN"];
    std::vector<std::string>::iterator san_list_pos = 
        std::find(san_list.begin(),san_list.end(), closest_common_pos);

    // plus 1 again because same reason as above
    closest_to_san = san_list_pos - (san_list.begin()+1);

    // calculate the total number of orbits for the trip
    int total_hops = you_to_closest + closest_to_san;

    std::cout << "Closest pos: " << closest_common_pos << std::endl;
    std::cout << "Total orbital hops: " << total_hops << std::endl;

}   

int main(void)
{
    run_program(test_input);
    run_program(input_orbits);

    return 0;
}

