#include <iostream>
#include <leitor.h>
#include <apsam.h>

#include <time.h>

using namespace std;

void sleep(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}



int main(int argc, char *argv[])
{

    if(argc < 2){
        cerr << "\nError - try: " << argv[0] << " input-instance-file" << endl;
        return 2;
    }


    Leitor r(argv[1]); // instance path
    APSAM apx(&r);
    apx.AlgGuloso();
    apx.AlgNiveis();
    apx.AlgOrdem();



    return 0;
}
