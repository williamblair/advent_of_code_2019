/*
 * Day 08 - image format
 *
 * compile/run with:
 *   g++ -std=c++11 day_08.cpp -o day_08
 *   ./day_08
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <climits>
#include <cassert>
#include <sstream>

#include "input.h"

std::vector<int> input_string_to_vector(std::string& input)
{
    std::vector<int> output(input.size());

    int val;
    for (size_t i=0; i<input.size(); ++i)
    {
        val = input[i] - '0';
        output[i] = val;
    }

    return output;
}

// vector of layers
std::vector<std::vector<int>> parse_image(std::vector<int>& input,
                                          const int width,
                                          const int height)
{
    std::vector<std::vector<int>> parsed_image;

    // figure out how many layers there are 
    const int pixels_per_layer = width*height;
    const int num_layers = input.size() / pixels_per_layer;
    assert(input.size()%pixels_per_layer == 0);

    // get each layer
    std::vector<int>::iterator in_iter = input.begin();
    for (size_t i=0; i<num_layers; ++i)
    {
        std::vector<int> pixels(pixels_per_layer);
        
        std::copy(in_iter, in_iter+pixels_per_layer, pixels.begin());
        in_iter += pixels_per_layer;

        parsed_image.push_back(pixels);
    }

    return parsed_image;
}

void print_image(std::vector<std::vector<int>>& image,
                 const int width,
                 const int height)
{
    for (size_t layer=0; layer<image.size(); ++layer)
    {
        std::cout << "Layer " << layer << std::endl;
        for (size_t row=0; row<height; ++row)
        {
            for (size_t col=0; col<width; ++col)
            {
                std::cout << image[layer][row*width + col];
            }
            std::cout << std::endl;
        }
    }
}

void run_program(std::string& input, const int width, const int height)
{
    std::vector<int> parsed_input = input_string_to_vector(input);

    std::vector<std::vector<int>> parsed_image = parse_image(parsed_input,
                                                             width,
                                                             height);

    // find the layer with the fewest 0 digits, and on that layer
    // what is the number of 1 digits multiplied by the number of
    // 2 digits
    size_t min_zeros = INT_MAX;
    int num_1s = 0;
    int num_2s = 0;
    for (std::vector<int>& layer : parsed_image)
    {
        // count the number of 0s, 1s, and twos
        int cur_num_zeros = 0;
        int cur_num_ones = 0;
        int cur_num_twos = 0;
        for (int val : layer)
        {
            if (val == 0) 
            {
                ++cur_num_zeros;
            }
            else if (val == 1)
            {
                ++cur_num_ones;
            }
            else if (val == 2)
            {
                ++cur_num_twos;
            }
        }

        if (cur_num_zeros < min_zeros)
        {
            min_zeros = cur_num_zeros;
            num_1s = cur_num_ones;
            num_2s = cur_num_twos;
        }
    }
    
    std::cout << "num 1s * num 2s: " << num_1s*num_2s << std::endl;

//    print_image(parsed_image, width, height);
}

int main(void)
{
    std::string test_image = "123456789012";
    run_program(test_image, 3,2);

    run_program(image_input, 25, 6);
    
    return 0;
}

