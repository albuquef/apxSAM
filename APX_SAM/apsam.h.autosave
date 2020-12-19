#ifndef APSAM_H
#define APSAM_H

#include <leitor.h>
#include <vector>
#include <algorithm> // random_shuffle


#define N 100


typedef struct
{
    bool M[N+1][N+1];
  int valor;

}SAMSol;



class APSAM
{
public:
    APSAM(Leitor *r);
    void Inicializar();
    void solRandom();
    void AlgGuloso();
    void AlgNiveis();
    void AlgOrdem();
    int  inArcs(SAMSol sol,int i);
    int  outArcs(SAMSol sol,int i);
    bool isFeasible();
    int FA();
    void copySolution(SAMSol sol);
    void saveSolution(string alg, double time);
    bool addArc(int i, int j);
    bool remArc(int i, int j);
    bool N1();
    int maxDG();
    int n;
    bool Mbest[N+1][N+1];
    bool Maux[N+1][N+1];
    int best;
    double timeT;
    SAMSol solBest;


private:
    const Leitor *rd;
    bool saveSol;


};

#endif // APSAM_H
