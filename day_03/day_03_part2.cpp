/*
 * Day 03 - find the closest intersection point of two wires from a common
 *   starting point. This time, instead of manhattan distance and closest
 *   intersection to center, we need the intersection that has the fewest 
 *   combined steps to get there from both wires
 *
 * compile/run with:
 *   g++ -std=c++11 day_03_part2.cpp -o day_03_part2
 *   ./day_03_part2
 */

#include <iostream>
#include <vector>
#include <array>
#include <sstream>
#include <string>
#include <cstring>
#include <climits>
#include <cmath>

// given wire layouts
std::array<std::vector<std::string>, 2> wire_layouts = {{
    {"R1003","D138","L341","U798","L922","U153","R721","D177","L297","D559","L414","U470","L589","D179","L267","D954","R739","D414","L865","U688","R541","U242","R32","D607","L480","D401","L521","U727","L295","D154","R905","D54","L353","U840","L187","U942","R313","D143","R927","D962","R739","U152","R6","D9","L807","D67","R425","D235","L598","D107","L838","D522","L882","U780","L942","D29","R933","U129","L556","D11","L859","D455","L156","U673","L54","D141","R862","U88","R362","U742","L511","D408","R825","U622","R650","D393","L882","D969","R866","D232","L423","U371","L744","U35","L196","D189","R803","U663","R41","U741","R742","U929","L311","U30","R357","D776","L929","U85","R415","U540","R921","U599","R651","U79","R608","D620","L978","D92","L491","D310","L830","U656","R244","U72","L35","U768","R666","U356","R82","U596","L798","D455","L280","D626","R586","U668","R331","D245","L140","U3","R283","U813","R620","U975","L795","U477","L100","D94","R353","D732","R694","U702","L305","U497","R900","U810","L412","D954","R584","D444","L531","D875","R49","D328","L955","U227","L370","D548","L351","U571","R373","U743","R105","D226","L755","U325","R496","D960","L415","U262","R197","D508","R725","U930","L722","D162","L996","D610","R346","U680","L75","U211","R953","U147","R114","D48","L305","D284","L630","U575","R142","D518","R704","D820","L617","D118","R67","D674","L90","D916","L483","D598","L424","U92","R188","U413","L702","D262","R720","D995","L759","D732","L259","D814","L342","U642","L875","U726","R265","D143","R754","D235","L535","U1","R211","D720","R943","D726","L398","U636","R994","U653","L401","U877","R577","D460","L730","U889","R166","D641","L693","U490","L78","D80","R535","U551","L866","U283","L336","U586","L913","U474","R158","D220","R278","U11","R421","D661","R719","D696","R188","D735","L799","U391","R331","U581","R689","D82","R375","D125","R613","D705","L927","U18","R399","D352","L411","D777","L733","D884","R791","U973","R772","D878","R327","U215","L298","D360","R426","D872","L99","U78","L745","U59","L641","U73","L294","D247","R944","U512","L396"},
    {"L1004","D252","L909","D935","R918","D981","L251","U486","R266","U613","L546","D815","L789","D692","L550","U633","R485","U955","R693","D784","R974","U529","R926","U550","L742","U88","R647","D572","R832","D345","R98","D122","R634","U943","L956","U551","R295","U122","L575","U378","R652","U97","R129","D872","R275","D492","L530","D328","R761","U738","R836","U884","R636","U776","L951","D977","R980","U526","L824","U125","R778","D818","R281","U929","R907","U234","L359","D521","R294","U137","L607","U421","L7","U582","R194","U979","L941","D999","R442","D330","L656","U410","R753","U704","R834","D61","R775","U750","R891","D989","R856","D944","R526","D44","R227","U938","R130","D280","L721","D171","R763","D677","L643","U931","L489","U250","L779","U552","R796","U220","R465","U700","L459","U766","R501","D16","R555","U257","R122","U452","L197","U905","L486","D726","L551","U487","R785","U470","L879","U149","L978","D708","R18","U211","L652","D141","L99","D190","L982","U556","R861","U745","L786","U674","R706","U986","R554","D39","R881","D626","R885","U907","R196","U532","L297","U232","L508","U283","L236","U613","L551","U647","R679","U760","L435","D475","R916","U669","R788","U922","R107","D503","R687","D282","L940","U835","L226","U421","L64","U245","R977","D958","L866","D328","R215","D532","R350","D199","R872","U373","R415","U463","L132","U225","L144","U786","R658","D535","R263","U263","R48","D420","L407","D177","L496","U521","R47","D356","L503","D557","R220","D879","L12","U853","R265","U983","L221","U235","R46","D906","L271","U448","L464","U258","R952","D976","L949","D526","L458","D753","L408","U222","R256","U885","R986","U622","R503","D5","L659","D553","R311","U783","L541","U17","R267","U767","L423","D501","R357","D160","L316","D912","R303","U648","L182","U342","L185","U743","L559","U816","R24","D203","R608","D370","R25","U883","L72","D816","L877","U990","R49","U331","L482","U37","L585","D327","R268","D106","L224","U401","L203","D734","L695","U910","L417","U105","R135","U876","L194","U723","L282","D966","R246","U447","R966","U346","L636","D9","L480","D35","R96"}
}};

typedef struct {
    int x1;
    int y1;
    int x2;
    int y2;
} Line;
std::vector<Line> get_lines(std::vector<std::string>& wire, int wire_num)
{
    std::vector<Line> lines;

    int cur_x = 0;
    int cur_y = 0;
    int old_x = 0;
    int old_y = 0;

    for (std::string& str : wire)
    {
        std::stringstream ss(str);
        
        // 1. Get the direction from the command
        char direction;
        ss >> direction;
        
        // 2. Get the distance from the command
        int distance;
        ss >> distance;

        // save previous position
        old_x = cur_x;
        old_y = cur_y;

        switch (direction)
        {
            case 'U': cur_y += distance; break;
            case 'D': cur_y -= distance; break;
            case 'L': cur_x -= distance; break;
            case 'R': cur_x += distance; break;
            default:
                std::cout << "Invalid direction: " << direction << std::endl;
                break;
        }

        // add a new line
        lines.push_back({old_x, old_y, cur_x, cur_y});
    }

    return lines;
}

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

#if 0
        std::cout << "Point intersection: ("
                  << X1 << "," << Y1 << "),(" << X2<<","<<Y2<<"), ("
                  << X3 << "," << Y3 << "),(" << X4<<","<<Y4<<") at "
                  << intersec_x << "," << intersec_y << ", t,u: "
                  << t << "," << u << std::endl;
#endif
    }
    else
    {
        return false;
    }

    return true;
}

inline int get_num_intersection_steps(std::vector<std::string>& wire, 
                                      int intersec_x, 
                                      int intersec_y)
{
    int num_steps = 0;
    int cur_x = 0;
    int cur_y = 0;
    for (std::string& str : wire)
    {
        std::stringstream ss(str);
        
        // 1. Get the direction from the command
        char direction;
        ss >> direction;
        
        // 2. Get the distance from the command
        int distance;
        ss >> distance;

        for (int i=0; i<distance; ++i)
        {
            switch (direction)
            {
                case 'U': cur_y++; break;
                case 'D': cur_y--; break;
                case 'L': cur_x--; break;
                case 'R': cur_x++; break;
                default:
                    std::cout << "Invalid direction: " << direction << std::endl;
                    break;
            }
            ++num_steps;

            // test if we hit intersection point
            if (cur_x == intersec_x && cur_y == intersec_y)
            {
//                std::cout << "Hit intersection" << std::endl;
                return num_steps;
            }
        }
    }

    return num_steps;
}

inline void run_problem(std::array<std::vector<std::string>,2>& wires)
{
    // 1. Get lines from both wires
    std::vector<Line> wire1_lines = get_lines(wires[0], 0);
    std::vector<Line> wire2_lines = get_lines(wires[1], 1);

    // 2. Check for intersections between the lines
    int intersection_x = 0;
    int intersection_y = 0;
    std::vector<std::pair<int,int>> intersections;
    for (Line& line1 : wire1_lines)
    {
        for (Line& line2 : wire2_lines)
        {
            if (get_intersection(line1, 
                                 line2, 
                                 intersection_x, 
                                 intersection_y))
            {
                if (intersection_x != 0 || intersection_y != 0)
                {
                    intersections.push_back(std::pair<int,int>
                                                     (intersection_x,
                                                      intersection_y));
                }
            }
        }
    }

    // 3. Get pair with minimum combined steps for both wires
    int min_steps = INT_MAX;
    for (std::pair<int,int>& intersection : intersections)
    {
        int wire1_steps = get_num_intersection_steps(wires[0], 
                                                     intersection.first, 
                                                     intersection.second);
        int wire2_steps = get_num_intersection_steps(wires[1], 
                                                     intersection.first, 
                                                     intersection.second);

        int distance = wire1_steps + wire2_steps;
        if (distance < min_steps) {
            min_steps = distance;
        }

    }
    std::cout << "Min steps: " << min_steps<< std::endl;
}

int main(void)
{
    // min distance is 6
    std::array<std::vector<std::string>,2> test_layouts_1 = {{
        {"R8", "U5", "L5", "D3"},
        {"U7", "R6", "D4", "L4"}
    }};

    // min distance is 159
    // min num steps is 610
    std::array<std::vector<std::string>,2> test_layouts_2 = {{
        {"R75","D30","R83","U83","L12","D49","R71","U7","L72"},
        {"U62","R66","U55","R34","D71","R55","D58","R83"}
    }};

    // min distance is 135
    // min num steps is 410
    std::array<std::vector<std::string>,2> test_layouts_3 = {{
        {"R98","U47","R26","D63","R33","U87","L62","D20","R33","U53","R51"},
        {"U98","R91","D20","R16","D67","R40","U7","R15","U6","R7"}
    }};


    // min distance is 2
    std::array<std::vector<std::string>,2> test_layouts_4 = {{
        {"U1","R1"},
        {"R2","D1","L1","U2"}
    }};
    
    run_problem(test_layouts_1);
    run_problem(test_layouts_2);
    run_problem(test_layouts_3);
//    run_problem(test_layouts_4);
    run_problem(wire_layouts);
    
    return 0;
}

