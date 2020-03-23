//includes
#include <cmath>
#include <fstream>
#include <sstream>
#include "main.h"
#include "resource.h"

//prototypes
void convertNotesToFrequencies(freq_vec_2d &frequencies);
void parseFile(std::string const &filename, string_vec &titles,
    freq_vec_2d &frequencies);
void playAll(string_vec &titles, freq_vec_2d &frequencies);

unsigned __stdcall threadFunction(void* data);

void convertNotesToFrequencies(freq_vec_2d &frequencies)
{
    float frequency;

    for (uint8_t i = 0; i < frequencies.size(); i++)
    {
        freq_vec* track = &frequencies[i];

        for (uint8_t j = 0; j < track->size(); j++)
        {
            frequency = 2.0f;
            frequency = pow(frequency, (track->at(j).frequency / 12.0f));
            frequency = frequency * 440;

            track->at(j).frequency = frequency;
        }
    }
}

void loadFile(std::string const &filename)
{
    if (threadHandle != NULL)
    {
        endRunningThread = true;

        WaitForSingleObject(threadHandle, INFINITE);
        CloseHandle(threadHandle);

        threadHandle     = NULL;
        endRunningThread = false;
    }

    if (threadHandle == NULL)
    {
        freq_vec_2d frequencies;
        string_vec  titles;

        parseFile(filename, titles, frequencies);

        DataSegment dg = (DataSegment){titles, frequencies};
        threadHandle   = (handle_t)_beginthreadex(0, 0, threadFunction,
            &dg, 0, 0);

        MSG msg;
        DWORD reason = WAIT_TIMEOUT;

        while (WAIT_OBJECT_0 != reason)
        {
            reason = MsgWaitForMultipleObjects(1, &threadHandle, FALSE,
                INFINITE, QS_ALLINPUT);

            switch (reason)
            {
                case(WAIT_OBJECT_0):
                {
                    // Thread finished
                } break;

                case(WAIT_OBJECT_0 + 1):
                {
                    // we have a message - peek and dispatch it
                    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
                    {
                        TranslateMessage(&msg);
                        DispatchMessage(&msg);
                    }
                } break;
            }
        }

        CloseHandle(threadHandle);
        threadHandle = NULL;
    }
}

void parseFile(std::string const &filename, string_vec &titles,
    freq_vec_2d &frequencies)
{
    int currPos = 0;

    std::ifstream infile(filename, std::ios::binary);
    std::string line;

    while (std::getline(infile, line))
    {
        if (currPos != 0)
        {
            infile.seekg(currPos);
            currPos = 0;
        }

        if (line.rfind("##", 0) == 0)
        {
            line    = line.substr(2);
            int pos = line.find_first_not_of(' ');

            if (pos == std::string::npos)
            {
                pos = 0;
            }

            std::string title = line.substr(pos);
            titles.push_back(title);

            freq_vec vec;

            while (std::getline(infile, line))
            {
                if (line.length() <= 1)
                {
                    currPos = infile.tellg();
                    continue;
                }

                if (line.rfind("##", 0) == 0)
                {
                    break;
                }

                std::istringstream iss(line);
                int a, b, c;

                if (!(iss >> a >> b >> c))
                {
                    break;
                }

                vec.push_back((Triplet){static_cast<float>(a), b, c});
            }

            if (!vec.empty())
            {
                frequencies.push_back(vec);
            }
        }
    }

    convertNotesToFrequencies(frequencies);
}

unsigned __stdcall threadFunction(void* data)
{
    DataSegment& dg = *(static_cast<DataSegment*>(data));

    for (uint8_t i = 0; i < dg.frequencies.size(); i++)
    {
        freq_vec* track = &dg.frequencies[i];

        std::string nowPlaying = "Now playing:\r\n" + dg.titles.at(i) + "\0";
        SetDlgItemText(hwnd, ID_EDIT1, nowPlaying.c_str());

        for (uint8_t j = 0; j < track->size(); j++)
        {
            if (endRunningThread)
            {
                goto endOfThread;
            }

            Beep(track->at(j).frequency, track->at(j).playtime);
            Sleep(track->at(j).sleeptime);
        }

        Sleep(200);
    }

    SetDlgItemText(hwnd, ID_EDIT1, "Status:\r\nPlayback finished.");

    endOfThread:
    return 0;
}
