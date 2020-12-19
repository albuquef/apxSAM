#ifndef LEITOR_H
#define LEITOR_H

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <cfloat>
#include <cstdlib>
#include <math.h>
#include <sstream>
#include <vector>

using namespace std;

class Leitor
{
public:
    Leitor(const char *filename)
    {
        ifstream fileStream;
        fileStream.open(filename);
        string line;
        istringstream *iss;
        string word;

        copyFileName = filename;

        cout << "Reading file:" << filename << endl;
        char *pEnd = nullptr;
        int count_line = 0;
        int lin = 0;
        int col = 0;

        while (fileStream)
        {
            getline(fileStream, line);
            switch (count_line) {
            case 0:
                iss = new istringstream(line);
                (*iss) >> word;
                n = static_cast<int>(strtol(word.c_str(), &pEnd, 10));

                M = new int *[n];
                for (int i = 0 ; i < n; i++ )
                {
                    M[i] = new int[n];
                }

                break;
            default:
                iss = new istringstream(line);
                while( getline(*iss, word, ' ') )
                {
                    M[lin][col++] = static_cast<int>(strtol(word.c_str(), &pEnd, 10));
                }
                lin++; col = 0;
                break;
            }
            count_line++;
        }

        fileStream.close();
    }

    ~Leitor()
    {
        for (int i = 0; i < n; i++)
        {
            delete[] M[i];
        }
    }

    void show()
    {
        int i, j;
        cout << "Number of Nodes: " << n << endl;

        cout << "\n Adjacency Matrix \n";
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++) cout << "\t " << M[i][j];
            cout<< "\n";
        }
    }


public:
    int n;
    int **M;
    const char *copyFileName;
};

#endif
