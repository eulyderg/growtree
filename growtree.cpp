//////////////////////////
// growtree by eulyderg //
//////////////////////////

#include <iostream>
#include <chrono>

const char BRANCHES[] = {'\\','|','/'};

const float LEAF_DECAY = 0.25F;
const float LEAF_GRADIENT = 0.0625F;

const uint8_t MAX_LAYERS = 15;
const uint8_t MAX_STEM_LENGTH = 255;

const uint8_t MARGIN = 5;
const uint8_t MAX_WIDTH = (MAX_LAYERS + MARGIN) * 2 + 1;
const uint8_t MAX_HEIGHT = MAX_LAYERS + 1;

char output[MAX_HEIGHT][MAX_WIDTH];

int8_t layershape[MAX_LAYERS][1<<MAX_LAYERS];
uint8_t layerpos[MAX_LAYERS][1<<MAX_LAYERS];

char leaf = '@';
uint8_t layers = 4;
uint8_t stem = 3;

int main(int argc, char *argv[])
{
    srand(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
    
    for (uint8_t i=1; i<argc; ++i) {
        std::string argumentName = std::string(argv[i]);
        try {
            if (argumentName == "-h" || argumentName == "--help") {
                std::cout << argv[0] << " usage:" << std::endl;
                std::cout << " -h --help    displays this help message." << std::endl;
                std::cout << " -l --leaf    sets leaf letter         [default @]" << std::endl;
                std::cout << " -s --stem    sets stem height         [default 3]" << std::endl;
                std::cout << " -b --branch  sets branch layers count [default 4]" << std::endl;
                exit(0);
            } else if (argumentName == "-l" || argumentName == "--leaf") {
                ++i;
                std::string argumentValue = argv[i];
                leaf = argumentValue[0];
            } else if (argumentName == "-s" || argumentName == "--stem") {
                ++i;
                int argumentValue = std::stoi(argv[i]);
                if (argumentValue > MAX_STEM_LENGTH) {
                    std::cout << "Maximum of 255 stem length exceeded." << std::endl;
                    exit(1);
                }
                stem = argumentValue;
            } else if (argumentName == "-b" || argumentName == "--branch") {
                ++i;
                int argumentValue = std::stoi(argv[i]);
                if (argumentValue > MAX_LAYERS) {
                    std::cout << "Maximum of 15 branch layers exceeded." << std::endl;
                    exit(1);
                }
                layers = argumentValue;
            }
        } catch (...) {
        }
    }
  
    const uint32_t horiz_offset = layers + MARGIN;
    const uint32_t output_width = horiz_offset + layers + MARGIN + 1;
    const uint32_t output_height = layers + 1;
  
    // set directions and positions of branches
    for (uint8_t i=0; i<layers; ++i) {
        for (uint32_t j=0; j<uint32_t(1<<i); ++j) {
            layershape[i][j] = (int8_t)(3 * (float)rand()/RAND_MAX) - 1;
            uint32_t _xoff = i>0 ? layerpos[i-1][j>>1] : horiz_offset;
            layerpos[i][j] = _xoff + layershape[i][j];
        }
    }
    
    // draw tree
    for (int16_t i=layers; i>=0; --i) {
        // fill with blank spaces
        for (uint8_t j=0; j<output_width; ++j) {
            output[i][j] = ' ';
        }
        output[i][output_width-1] = 0x00;
        // fill branches and leaves
        if (i < layers) {
            for (uint32_t j=0; j<uint32_t(1<<i); ++j) {
                // fill branch
                output[i][layerpos[i][j]] = BRANCHES[layershape[i][j]+1];
                // fill leaves
                for (int8_t x=-2; x<=2; ++x) {
                    for (int8_t y=0; y<=1; ++y) {
                        char currchar = output[i+y][layerpos[i][j]+x];
                        bool overwritingbranch = (x==0 && y==1 && (currchar==BRANCHES[0] || currchar==BRANCHES[1] || currchar==BRANCHES[2]));
                        bool incorner = (abs(x)==2 && y==1);
                        bool inbranch = (x==0 && y==0);
                        float W = (float)rand()/RAND_MAX;
                        float decay = (i+y)*LEAF_GRADIENT/layers + LEAF_DECAY;
                        
                        if (!incorner && !inbranch && W < decay && !overwritingbranch)
                        output[i+y][layerpos[i][j]+x] = leaf;
                    }
                }
            }
        }
    }

    // print tree
    for (int16_t i=output_height; i>=0; --i) { std::cout << output[i] << std::endl; }
    for (uint8_t i=0; i<stem; ++i) {
        for (uint8_t j=0; j<horiz_offset; j++) { std::cout << " "; }
        std::cout << BRANCHES[1] << std::endl;
    }
}