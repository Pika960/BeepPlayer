#ifndef MAIN_H
#define MAIN_H
#endif

//includes
#include <string>
#include <vector>
#include <windows.h>

//typedefs
typedef struct Triplet
{
    float frequency;
    int playtime;
    int sleeptime;
} Triplet;

typedef std::vector<Triplet>     freq_vec;
typedef std::vector<freq_vec>    freq_vec_2d;
typedef std::vector<std::string> string_vec;

typedef struct DataSegment
{
    string_vec &titles;
    freq_vec_2d &frequencies;
} DataSegment;

//extern variables
extern bool      endRunningThread;
extern handle_t  threadHandle;
extern HWND      hwnd;

//prototypes
void loadFile(std::string const &filename);
