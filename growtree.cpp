//////////////////////////
// growtree by eulyderg //
//////////////////////////

#include <iostream>
#include <chrono>
#include <stdlib.h>
#include <cstdarg>
using namespace std;
using namespace std::chrono;

char Rdir() {
    return (char)(3*(float)rand()/RAND_MAX)-1;
}
char c2n(char c, int fallback) {
    if (c>='0'&&c<='9') { return c-'0'; }
    return fallback;
}
template<typename T>
bool scmp(T mystr, const T* compare, const int size) {
  for (unsigned int i=0; i<size; ++i)
    if (mystr==compare[i])
      return true;
  return false;
}


const string FLAGS_HELP[] = {"-h","--help","-?","/?"};
const char BRANCHES[] = {'\\','|','/'};

// params
char leaf = '@';
int layers = 4;
int stem = 3;
const float leafdecay = 0.25F;
const float leafgradient = 0.0625F;

/* legacy
const char legacy_layers = 4;
const char legacy_stem = 3;
const char legacy_horiz_offset = legacy_layers+3;
char legacy_output[legacy_layers+1][2*legacy_horiz_offset+1];
char legacy_legacy_layershape[legacy_layers][1<<legacy_layers];
char legacy_layerpos[legacy_layers][1<<legacy_layers];

// legacy behavior 
void generate_legacy(int argc, char *argv[])
{
  if (argc>=2) leaf = argv[1][0];
  for (unsigned char i=0;i<legacy_layers;++i) for (unsigned char j=0;j<(1<<i);++j) {
    legacy_legacy_layershape[i][j]=Rdir();
    legacy_layerpos[i][j]=(i>0?legacy_layerpos[i-1][j>>1]:legacy_horiz_offset)+legacy_legacy_layershape[i][j];
  }
  for (unsigned char i=0;i<legacy_layers+1;++i) for (unsigned char j=0;j<2*legacy_horiz_offset;++j) legacy_output[i][j]=' ';
  for (unsigned char i=0;i<legacy_layers;++i) for (unsigned char j=0;j<(1<<i);++j) {
      legacy_output[i][legacy_layerpos[i][j]]=branch[legacy_legacy_layershape[i][j]+1];
      for (char x=-2;x<=2;++x) for (unsigned char y=0;y<=1;++y)
          if (!(std::abs(x)==2&&y==1)&&!(x==0&&y==0)&&(float)rand()/RAND_MAX>leafdecay)
            legacy_output[i+y][legacy_layerpos[i][j]+x]=leaf;
  }
  for (char i=legacy_layers+1;i>=0;--i) std::cout << legacy_output[i] << std::endl;
  for (unsigned char i=0;i<legacy_stem;++i) {
    for (unsigned char j=0;j<legacy_horiz_offset;j++) std::cout << " ";
    std::cout << branch[1] << std::endl;
  }
}
*/

/* help function */
void help(int argc, char *argv[])
{
  cout << "Usage: " << argv[0] << " [X][Y][Z]" << endl;
  cout << "  X sets leaf letter (default @)" << endl;
  cout << "  Y sets layer count (@ for default=4)" << endl;
  cout << "  Z sets stem length (@ for default=3)" << endl;
  exit(0);
}


/* main behavior */
int main(int argc, char *argv[])
{
  srand(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
  /* legacy behavior flag
  if (argc>=3 && string(argv[2])==string("--legacy")) {
      generate_legacy(argc,argv);
      return 0;
  } else if (argc==2 && string(argv[1])==string("--legacy")) {
      generate_legacy(1,argv);
      return 0;
  }*/
  
  if (argc >= 2) {
    // help flag
    if (scmp(string(argv[1]),FLAGS_HELP,sizeof(FLAGS_HELP)/sizeof(*FLAGS_HELP)))
      help(argc, argv);
    
    if (argv[1][0]!='@') { leaf = argv[1][0]; }
    if (argv[1][1]!='@') { layers = c2n(argv[1][1],layers); }
    if (argv[1][2]!='@') { stem = c2n(argv[1][2],stem); }
  }
  
  const int margin = 5;
  const int horiz_offset = layers + margin;
  const int output_w = horiz_offset + layers + margin + 1;
  const int output_h = layers + 1;
  
  char output[output_h][output_w];
  /// TODO: dont waste half of the allocated memory on literally nothing
  int layershape[layers][1<<layers];
  int layerpos[layers][1<<layers];
  
  // set directions and positions of branches
  for (unsigned char i=0; i<layers; ++i) {
    for (unsigned int j=0; j<(1<<i); ++j) {
      layershape[i][j] = Rdir();
      int _xoff = i>0 ? layerpos[i-1][j>>1] : horiz_offset;
      layerpos[i][j] = _xoff+layershape[i][j];
    }
  }
  
  // draw tree
  for (char i=layers; i>=0; --i) {
    // fill with blank spaces
    for (unsigned char j=0; j<output_w; ++j)
      output[i][j] = ' ';
    output[i][output_w-1] = 0x00;
    
    // fill branches and leaves
    if (i < layers) {
      for (int j=0; j<(1<<i); ++j) {
        // fill branch
        output[i][layerpos[i][j]] = BRANCHES[layershape[i][j]+1];
        
        ///TODO: implement leaf grain
        // fill leaves
        for (char x=-2; x<=2; ++x) {
          for (char y=0; y<=1; ++y) {
            char currchar = output[i+y][layerpos[i][j]+x];
            bool overwritingbranch = (x==0 && y==1 && scmp(currchar,BRANCHES,sizeof(BRANCHES)/sizeof(*BRANCHES)));
            bool incorner = (abs(x)==2 && y==1);
            bool inbranch = (x==0 && y==0);
            float W = (float)rand()/RAND_MAX;
            float decay = (i+y)*leafgradient/layers + leafdecay;
            
            if (!incorner && !inbranch && W < decay && !overwritingbranch)
              output[i+y][layerpos[i][j]+x] = leaf;
          }
        }
      }
    }
  }

  // print tree
  for (char i=output_h; i>=0; --i) { std::cout << output[i] << std::endl; }
  for (char i=0; i<stem; ++i) {
    for (char j=0; j<horiz_offset; j++) { std::cout << " "; }
    std::cout << BRANCHES[1] << std::endl;
  }
  
  return 0;
}