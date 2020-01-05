/*
 * Day 10 - Find the location with best line of sight to other
 * meteor locations
 *   
 *
 * compile/run with:
 *   g++ -std=c++11 day_10.cpp -o day_10
 *   ./day_10
 */

#include <iostream>
#include <vector>
#include <array>
#include <sstream>
#include <string>
#include <cstring>
#include <climits>
#include <cmath>

#include "input.h"

typedef struct {
    int x1;
    int y1;
    int x2;
    int y2;
} Line;

// returns true if there was an intersection
// https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection#Given_two_points_on_each_line
inline bool get_intersection(Line line1, Line line2, int& intersec_x, int& intersec_y)
{
#define X1 line1.x1
#define X2 line1.x2
#define X3 line2.x1
#define X4 line2.x2

#define Y1 line1.y1
#define Y2 line1.y2
#define Y3 line2.y1
#define Y4 line2.y2


    float t = (X1-X3)*(Y3-Y4) - (Y1-Y3)*(X3-X4);
    float u = (X1-X2)*(Y1-Y3) - (Y1-Y2)*(X1-X3);

    int divisor = (X1-X2)*(Y3-Y4) - (Y1-Y2)*(X3-X4);

    if (divisor == 0)
    {
        return false;
    }
    
    t /= (float)divisor;
    u /= (float)divisor;
    u = -u;

    if ((0.0F <= t && t <= 1.0F) && (0.0F <= u && u <= 1.0F))
    {
        intersec_x = int(float(X1) + t*float(X2-X1));
        intersec_y = int(float(Y1) + t*float(Y2-Y1));

        std::cout << "Point intersection: ("
                  << X1 << "," << Y1 << "),(" << X2<<","<<Y2<<"), ("
                  << X3 << "," << Y3 << "),(" << X4<<","<<Y4<<") at "
                  << intersec_x << "," << intersec_y << ", t,u: "
                  << t << "," << u << std::endl;
    }
    else
    {
        return false;
    }

    return true;
}

typedef struct {
    int row;
    int col;
    int num_visible_asteroids;
} Asteroid;

inline std::vector<Asteroid> get_asteroids(std::vector<std::string>& input)
{
    std::vector<Asteroid> asteroids;
    for (size_t row=0; row<input.size(); ++row)
    {
        for (size_t col=0; col<input[0].size(); ++col)
        {
            if (input[row][col] == '#')
            {
                asteroids.push_back({int(row),int(col),0});
            }
        }
    }

    return asteroids;
}

// https://stackoverflow.com/questions/7050186/find-if-point-lays-on-line-segment
inline bool get_point_intersection(Line line, int px, int py)
{
    int divisor_x = line.x2 - line.x1;
    int divisor_y = line.y2 - line.y1;

    if (divisor_x == 0)
    {
        // the line is vertical, so check if the x's match
        // and the y is in between the y points
        if (px != line.x2)
        {
            return false;
        }

        if (line.y2 < line.y1)
        {
            return (py >= line.y2 && py <= line.y1);
        }
        // we're assuming the lines don't fall on the same point
        else
        {
            return (py >= line.y1 && py <= line.y2);
        }
    }
    else if (divisor_y == 0)
    {
        // the line is horizontal, so check if the y's
        // match and the x is in between the two x points
        if (py != line.y2)
        {
            return false;
        }

        if (line.x2 < line.x1)
        {
            return (px >= line.x2 && px <= line.x1);
        }
        // we're assuming the lines don't fall on the same point
        else
        {
            return (px >= line.x1 && px <= line.x2);
        }
    }

    float xval = float(px - line.x1) / float(divisor_x);
    float yval = float(py - line.y1) / float(divisor_y);

    // check if they are NOT equal
    if (std::fabs(xval - yval) >= 0.001F)
    {
        return false;
    }
    
    // check if the point falls within the line segment
    if (line.x1 < line.x2)
    {
        if (px < line.x1 || px > line.x2) {
            return false;
        }
    }
    else
    {
        if (px < line.x2 || px > line.x1) {
            return false;
        }
    }
    if (line.y1 < line.y2)
    {
        if (py < line.y1 || py > line.y2) {
            return false;
        }
    }
    else
    {
        if (py < line.y2 || py > line.y1) {
            return false;
        }
    }
    
    return true;
}

inline void run_problem(std::vector<std::string>& input)
{
    // 1. get the list of asteroids on the map
    std::vector<Asteroid> asteroids = get_asteroids(input);

    // 2. Calculate the number of visible asteroids from each location
    for (Asteroid& asteroid : asteroids)
    {
        int num_visible = 0;
        for (Asteroid& test_asteroid : asteroids)
        {
            // we don't want to check the current asteroid against itself
            if (asteroid.row == test_asteroid.row &&
                asteroid.col == test_asteroid.col)
            {
                continue;
            }

            // create the line between the points
            // the rows are inverted so origin 0,0 is at bottom left
            Line cur_asteroid_line = {
                .x1 = asteroid.col,
                .y1 = int(input.size() - 1 - asteroid.row),
                .x2 = test_asteroid.col,
                .y2 = int(input.size() - 1 - test_asteroid.row)
            };

            // now we need to check for intersections with all of the
            // other points
            bool line_hits_other_astroid = false;
            for (Asteroid& point_asteroid : asteroids)
            {
                // check against the current asteroid
                if (point_asteroid.row == asteroid.row &&
                    point_asteroid.col == asteroid.col)
                {
                    continue;
                }
                // check against the asteroid we're trying to see
                if (point_asteroid.row == test_asteroid.row &&
                    point_asteroid.col == test_asteroid.col)
                {
                    continue;
                }

                // otherwise see if it falls on our line of sight
                line_hits_other_astroid = 
                    get_point_intersection(cur_asteroid_line, 
                                           point_asteroid.col,
                                           input.size()-1-point_asteroid.row);

                if (line_hits_other_astroid == true)
                {
                    break;
                }
            }

            // if we didn't find any intersections, then we have a clear line
            // of sight and we can increase the view count
            if (!line_hits_other_astroid) {
                asteroid.num_visible_asteroids++;
            }
        }
    }

#if 0
    // test print out num vis asteroids from each location
    for (size_t row = 0; row<input.size(); ++row)
    {
        for (size_t col=0; col<input[0].size(); ++col)
        {
            // find the asteroid for this location
            bool found_asteroid = false;
            for (Asteroid asteroid : asteroids)
            {
                if (asteroid.row == row && asteroid.col == col)
                {
                    std::cout << asteroid.num_visible_asteroids;
                    found_asteroid = true;
                    break;
                }
            }
            if (!found_asteroid)
            {
                std::cout << '.';
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
#endif

    // 3. Find the location with the most visible asteroids
    int most_visible = INT_MIN;
    int row = 0;
    int col = 0;
    for (Asteroid& asteroid : asteroids)
    {
        if (asteroid.num_visible_asteroids > most_visible) {
            most_visible = asteroid.num_visible_asteroids;
            row = asteroid.row;
            col = asteroid.col;
        }
    }

    std::cout << "Most visible: " << most_visible << std::endl;
}

std::vector<std::string> test_input_1 = {
".#..#",
".....",
"#####",
"....#",
"...##"
};

std::vector<std::string> test_input_2 = {
"......#.#.",
"#..#.#....",
"..#######.",
".#.#.###..",
".#..#.....",
"..#....#.#",
"#..#....#.",
".##.#..###",
"##...#..#.",
".#....####"
};

std::vector<std::string> test_input_3 = {
"#.#...#.#.",
".###....#.",
".#....#...",
"##.#.#.#.#",
"....#.#.#.",
".##..###.#",
"..#...##..",
"..##....##",
"......#...",
".####.###."
};

std::vector<std::string> test_input_4 = {
".#..#..###",
"####.###.#",
"....###.#.",
"..###.##.#",
"##.##.#.#.",
"....###..#",
"..#.#..#.#",
"#..#.#.###",
".##...##.#",
".....#.#.."
};

std::vector<std::string> test_input_5 = {
".#..##.###...#######",
"##.############..##.",
".#.######.########.#",
".###.#######.####.#.",
"#####.##.#.##.###.##",
"..#####..#.#########",
"####################",
"#.####....###.#.#.##",
"##.#################",
"#####.##.###..####..",
"..######..##.#######",
"####.##.####...##..#",
".#####..#.######.###",
"##...#.##########...",
"#.##########.#######",
".####.#.###.###.#.##",
"....##.##.###..#####",
".#.#.###########.###",
"#.#.#.#####.####.###",
"###.##.####.##.#..##"
};

int main(void)
{
    run_problem(test_input_1);
    run_problem(test_input_2);
    run_problem(test_input_3);
    run_problem(test_input_4);
    run_problem(test_input_5);
    run_problem(problem_input);

    return 0;
}

