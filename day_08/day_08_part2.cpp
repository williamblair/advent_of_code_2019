/*
 * Day 08 - image format
 *  decode the image
 *
 * compile/run with:
 *   g++ -std=c++11 day_08_part2.cpp -o day_08_part2
 *   ./day_08_part2
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

    std::vector<int> rendered_image(width*height);

    // for each pixel, find the pixel closest to the front
    // layer that isn't transparent
    // the first layer is at the front and the last layer 
    // is at the back
    for (size_t row=0; row<height; ++row)
    {
        for (size_t col=0; col<width; ++col)
        {
            size_t pixel_index = row*width+col;
            // 2 is transparent
            int pixel_val = 2;
            for (size_t layer=0; 
                 pixel_val == 2 && layer<parsed_image.size(); 
                ++layer)
            {
                pixel_val = parsed_image[layer][pixel_index];
            }

            rendered_image[pixel_index] = pixel_val;
        }
    }

    // print the rendered image
    for (size_t row = 0; row<height; ++row)
    {
        for (size_t col=0; col<width; ++col)
        {
            int val = rendered_image[row*width + col];
            std::cout << ((val==1) ? '1' : ' ');
        }
        std::cout << std::endl;
    }

//    print_image(parsed_image, width, height);
}

int main(void)
{
//    std::string test_image = "0222112222120000";
//    run_program(test_image, 2,2);

    run_program(image_input, 25, 6);
    
    return 0;
}

