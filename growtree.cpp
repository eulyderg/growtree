//////////////////////////
// growtree by eulyderg //
//////////////////////////

#include <cstdlib>
#include <iostream>
#include <chrono>
#include <stdlib.h>
using namespace std;
using namespace std::chrono;

// params
char leaf = '@';
char layers = 4;
char stem = 3;
const float leafdecay = 0.25F;
const float leafgradient = 0.0625F;

const char branch[4] = "\\|/";

// legacy
const char legacy_layers = 4;
const char legacy_stem = 3;
const char legacy_horiz_offset = legacy_layers+3;
char legacy_output[legacy_layers+1][2*legacy_horiz_offset+1];
char legacy_legacy_layershape[legacy_layers][1<<legacy_layers];
char legacy_layerpos[legacy_layers][1<<legacy_layers];


// pick direction for branch
char Rdir() { return (char)(3*(float)rand()/RAND_MAX)-1; }
char c2n(char c, int fallback) {if (c>='0'&&c<='9') { return c-'0'; } return fallback; }

/* legacy behavior */
void legacy(int argc, char *argv[]) {
  if (argc>=2) leaf = argv[1][0];
  for (unsigned char i=0;i<legacy_layers;++i) for (unsigned char j=0;j<(1<<i);++j) {
    legacy_legacy_layershape[i][j]=Rdir();
    legacy_layerpos[i][j]=(i>0?legacy_layerpos[i-1][j>>1]:legacy_horiz_offset)+legacy_legacy_layershape[i][j];
  }
  for (unsigned char i=0;i<legacy_layers+1;++i) for (unsigned char j=0;j<2*legacy_horiz_offset;++j) legacy_output[i][j]=' ';
  for (unsigned char i=0;i<legacy_layers;++i) for (unsigned char j=0;j<(1<<i);++j) {
      legacy_output[i][legacy_layerpos[i][j]]=branch[legacy_legacy_layershape[i][j]+1];
      for (char x=-2;x<=2;++x) for (unsigned char y=0;y<=1;++y)
          if (!(std::abs(x)==2&y==1)&&!(x==0&y==0)&&(float)rand()/RAND_MAX>leafdecay)
            legacy_output[i+y][legacy_layerpos[i][j]+x]=leaf;
  }
  for (char i=legacy_layers+1;i>=0;--i) std::cout << legacy_output[i] << std::endl;
  for (unsigned char i=0;i<legacy_stem;++i) {
    for (unsigned char j=0;j<legacy_horiz_offset;j++) std::cout << " ";
    std::cout << branch[1] << std::endl;
  }
}

/* main routine */
int main(int argc, char *argv[])
{
  // set seed and leaf
  srand(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
  if (argc>=3) if (string(argv[2])==string("--legacy")) {legacy(argc,argv); return 0; }// legacy behavior flag
  if (argc>=2) {
    if (string(argv[1])==string("-h")||string(argv[1])==string("--help")||string(argv[1])==string("-?")||string(argv[1])==string("/?")) {
        std::cout << string("Usage: ")+string(argv[0])+string(" [X][Y][Z] [--legacy]\n  X sets leaf letter (default @)\n  Y sets layer count (@ for default=4)\n  Z sets stem length (@ for default=3)") << std::endl;
        return 0;
    }
    if (argv[1][0]!='@') { leaf = argv[1][0]; }
    if (argv[1][1]!='@') { layers = c2n(argv[1][1],layers); }
    if (argv[1][2]!='@') { stem = c2n(argv[1][2],stem); }
  }
  
  // set memory
  char horiz_offset = layers+3;
  char output[layers+1][2*horiz_offset+1];        // output lines buffer
  char layershape[layers][1<<layers];             // shape of each branch
  char layerpos[layers][1<<layers];               // position of each branch
  
  // set directions and positions of branches
  for (unsigned char i=0;i<layers;++i) for (unsigned int j=0;j<(1<<i);++j) {
    layershape[i][j]=Rdir();
    layerpos[i][j]=(i>0?layerpos[i-1][j>>1]:horiz_offset)+layershape[i][j];
  }
  
  // draw tree
  for (char i=0;i<=layers+1;++i) {output[i][2*horiz_offset]=0; for (unsigned char j=0;j<2*horiz_offset;++j) output[i][j]=' '; }
  for (char i=0;i<layers;++i) for (unsigned int j=0;j<(1<<i);++j) {
      output[i][layerpos[i][j]]=branch[layershape[i][j]+1];
      for (char x=-2;x<=2;++x)
        for (unsigned char y=0;y<=1;++y)
          if (!(std::abs(x)==2&y==1)&&!(x==0&y==0)&&(float)rand()/RAND_MAX<(i+y)*leafgradient/layers+leafdecay)
            output[i+y][layerpos[i][j]+x]=leaf;
  }

  // print tree
  for (char i=layers+1;i>=0;--i)
      std::cout << output[i] << std::endl;
  for (unsigned char i=0;i<stem;++i) {
    for (unsigned char j=0;j<horiz_offset;j++) 
      std::cout << " ";
    std::cout << branch[1] << std::endl;
  }
  
  return 0;
}
