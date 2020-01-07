/*
 * Day 10 - Find the location with best line of sight to other
 * meteor locations. Part 2: find which meteor will be the 200th
 * to be destroyed by a clockwise rotating lazer
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
#include <cassert>

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
    int  row;
    int  col;
    int  num_visible_asteroids;
    bool is_destroyed;
    int  destroyed_number; // the order in which this one was destroyed
    float angle; // angle between the center point and itself
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
                asteroids.push_back({int(row),int(col),0,false,9999,9999.0F});
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

inline float get_angle_between_points(int x1, int y1, int x2, int y2)
{
    int dy = y2 - y1;
    int dx = x2 - x1;

    // check for vertical/horizontal lines
    if (dx == 0 && dy != 0)
    {
        if (dy > 0)
        {
            // 90 degrees
            return (M_PI / 2.0F) * 180.0F / M_PI;
        }
        else
        {
            // 270 degrees
            return (3.0F * M_PI / 2.0F) * 180.0F / M_PI;
        }
    }
    else if (dx != 0 && dy == 0)
    {
        if (dx > 0)
        {
            // 0 degrees
            return 0.0F;
        } 
        else
        {
            // 180 degrees
            return M_PI * 180.0F / M_PI;
        }
    }

    float result = atan(float(dy)/float(dx)) * 180.0F / M_PI;
    // quadrant 2
    if (x2 < x1 && y2 > y1)
    {
        result += 180.0F;
    }
    // quadrant 3
    else if (x2 < x1 && y2 < y1)
    {
        result += 180.0F;
    }
    // quadrant 4
    else if (x2 > x1 && y2 < y1)
    {
        result += 360.0F;
    }
    
    return result;
}

inline void test_get_angle()
{
    int x1, x2, y1, y2;
    x1 = 20; y1 = 18;
    x2 = 22; y2 = 18;
    float angle = get_angle_between_points(x1,y1,x2,y2);
    std::cout << "x1, y1, x2, y2, angle: (" << x1 << "," << y1 << "), (" << x2 << "," << y2 << ") " << angle << std::endl;

    x1 = 20; y1 = 18;
    x2 = 18; y2 = 18;
    angle = get_angle_between_points(x1,y1,x2,y2);
    std::cout << "x1, y1, x2, y2, angle: (" << x1 << "," << y1 << "), (" << x2 << "," << y2 << ") " << angle << std::endl;

    x1 = 20; y1 = 18;
    x2 = 20; y2 = 20;
    angle = get_angle_between_points(x1,y1,x2,y2);
    std::cout << "x1, y1, x2, y2, angle: (" << x1 << "," << y1 << "), (" << x2 << "," << y2 << ") " << angle << std::endl;

    x1 = 20; y1 = 18;
    x2 = 20; y2 = 16;
    angle = get_angle_between_points(x1,y1,x2,y2);
    std::cout << "x1, y1, x2, y2, angle: (" << x1 << "," << y1 << "), (" << x2 << "," << y2 << ") " << angle << std::endl;

    x1 = 20; y1 = 18;
    x2 = 22; y2 = 20;
    angle = get_angle_between_points(x1,y1,x2,y2);
    std::cout << "x1, y1, x2, y2, angle: (" << x1 << "," << y1 << "), (" << x2 << "," << y2 << ") " << angle << std::endl;

    x1 = 20; y1 = 18;
    x2 = 18; y2 = 20;
    angle = get_angle_between_points(x1,y1,x2,y2);
    std::cout << "x1, y1, x2, y2, angle: (" << x1 << "," << y1 << "), (" << x2 << "," << y2 << ") " << angle << std::endl;

    x1 = 20; y1 = 18;
    x2 = 18; y2 = 16;
    angle = get_angle_between_points(x1,y1,x2,y2);
    std::cout << "x1, y1, x2, y2, angle: (" << x1 << "," << y1 << "), (" << x2 << "," << y2 << ") " << angle << std::endl;

    x1 = 20; y1 = 18;
    x2 = 22; y2 = 16;
    angle = get_angle_between_points(x1,y1,x2,y2);
    std::cout << "x1, y1, x2, y2, angle: (" << x1 << "," << y1 << "), (" << x2 << "," << y2 << ") " << angle << std::endl;
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

    // from part 1, the location with the most visible (280)
    // is at row, col 18,20
    std::cout << "Most visible: " << most_visible << std::endl;
    std::cout << "Row, Col: " << row << "," << col << std::endl;

    const int center_row = row;
    const int center_col = col;

    // calculate the angle between each asteroid and the center point
    for (size_t i = 0; i < asteroids.size(); ++i)
    {
        // we don't want to check the current asteroid against itself
        if (asteroids[i].row == center_row &&
            asteroids[i].col == center_col)
        {
            continue;
        }

        // compute the angle between our center and the
        // current test asteroid
        asteroids[i].angle = 
            get_angle_between_points(center_col,
                                     input.size()-1-center_row,
                                     asteroids[i].col,
                                     input.size()-1-asteroids[i].row);
    }
    // now destroy the asteroids
    int num_asteroids_destroyed = 0;
    std::cout << "Asteroids size: " << asteroids.size() << std::endl;
    float current_lazer_angle = 90.0F; // 90 degrees; start straight up
    do
    {
        // find the minimum angle between the current lazer angle and each 
        // of the asteroids
        float min_angle = -9999.0F;
        for (size_t i=0; i<asteroids.size(); ++i)
        {
            // ignore if it's the center asteroid
            if (asteroids[i].row == center_row &&
                asteroids[i].col == center_col)
            {
                continue;
            }

            // ignore if the angle is greater than the current angle sense 
            // we're moving clockwise
//            if (asteroids[i].angle > current_lazer_angle &&
//                num_asteroids_destroyed != asteroids.size()-2)
//            {
//                continue;
//            }

            // ignore if the asteroid was already destroyed
            if (asteroids[i].is_destroyed)
            {
                continue;
            }

            // want the smallest difference, NOT the smallest angle
            if (current_lazer_angle - asteroids[i].angle < 
                current_lazer_angle - min_angle)
            {
                min_angle = asteroids[i].angle;
            }
        }
        std::cout << "Min angle: " << min_angle << std::endl;

        // create a list of asteroids with that angle (could be more than one)
        std::vector<size_t> candidate_indices;
        for (size_t i=0; i<asteroids.size(); ++i)
        {
            // ignore if it's the center asteroid
            if (asteroids[i].row == center_row &&
                asteroids[i].col == center_col)
            {
                continue;
            }

            // ignore if the angle is greater than the current angle sense 
            // we're moving clockwise, unless there's only one asteroid left
//            if (asteroids[i].angle > current_lazer_angle &&
//                num_asteroids_destroyed != asteroids.size()-2)
//            {
//                continue;
//            }

            // ignore if the asteroid was already destroyed
            if (asteroids[i].is_destroyed)
            {
                continue;
            }
            
            if (std::fabs(asteroids[i].angle - min_angle) < 0.0001F) // tolerance
            {
                candidate_indices.push_back(i);
                std::cout << "candidate index: " << i << std::endl;
            }
        }

        // we should have at least found the one with the matching min angle difference...
        assert(candidate_indices.size() > 0);

        size_t min_index = 0;
        float min_distance = 9999.0F;
        for (size_t index : candidate_indices)
        {
            float diff_y = std::fabs(center_row - asteroids[index].row);
            diff_y = diff_y * diff_y;
            
            float diff_x = std::fabs(center_col - asteroids[index].col);
            diff_x = diff_x * diff_x;
            
            float dist = sqrt(diff_x + diff_y);

            if (dist < min_distance)
            {
                min_distance = dist;
                min_index = index;
            }
        }
        std::cout << "Min index: " << min_index << std::endl;

        // finally, destroy the asteroid with the min distance
        asteroids[min_index].is_destroyed = true;
        asteroids[min_index].destroyed_number = ++num_asteroids_destroyed;
        std::cout << "Destroyed number: " << asteroids[min_index].destroyed_number << std::endl;

        // the current angle plus down a little bit
        std::cout << "current lazer angle before: " << current_lazer_angle << std::endl;
        current_lazer_angle = min_angle - 0.1F;
        std::cout << "current lazer angle after: " << current_lazer_angle << std::endl;
        if (current_lazer_angle < 0.0F)
        {
            current_lazer_angle += 360.0F;
        }

    }
    // minus 1 to exclude the center point 
    while (num_asteroids_destroyed != asteroids.size()-1);

    // finally, list all of the asteroids
#if 0
    for (Asteroid& asteroid : asteroids)
    {
        if (asteroid.row == center_row && 
            asteroid.col == center_col)
        {
            continue;
        }

        std::cout << "x, y, destroyed num: " << asteroid.col << ","
                  << input.size()-1-asteroid.row << ", " 
                  << asteroid.destroyed_number << std::endl;
    }
#endif

#if 1
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
                    if (asteroid.row == center_row &&
                        asteroid.col == center_col)
                    {
                        std::cout << "X";
                    }
                    else
                    {
                        if (asteroid.destroyed_number <= 9)
                            std::cout << asteroid.destroyed_number;
                        else
                            std::cout << ".";
                    }
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
#if 0
    run_problem(test_input_1);
    run_problem(test_input_2);
    run_problem(test_input_3);
    run_problem(test_input_4);
#endif
    run_problem(test_input_5);
//    run_problem(problem_input);
//    test_get_angle();

    return 0;
}

