#include "apsam.h"

#include <graph.h>
#include <sys/time.h>



APSAM::APSAM(Leitor *r) : rd(r)
{
    n = rd->n;
//    timeT = 0;
    Inicializar();
    saveSol = false;
}

double get_wall_time(){
    struct timeval time;


    if(gettimeofday(&time,nullptr)){
        // HANDLE ERROR
        return 0;
    }else{

        return static_cast<double>(time.tv_sec) + static_cast<double>(time.tv_usec*0.000001); //microsegundos

    }

}




void APSAM::Inicializar()
{

//    unsigned int  col = static_cast<unsigned int>(n);
//    unsigned int  lin = static_cast<unsigned int>(n);


//    **Maux= static_cast<bool **>(malloc(col * sizeof(bool*)));
//    for(unsigned int i = 0; i < col; i++) Maux[i] = static_cast<bool *>(malloc(lin * sizeof(bool)));

//    **Mbest= static_cast<bool **>(malloc(col * sizeof(bool*)));
//    for(unsigned int i = 0; i < col; i++) Mbest[i] = static_cast<bool *>(malloc(lin * sizeof(bool)));


}


void APSAM::solRandom()
{

    int i,j;
    for(int k=0; k<n*(n-1);k++) {
        i = rand()%n;
        j = rand()%n;

        if(i != j && rd->M[i][j] == 1)
            addArc(i,j);

    }

}

void APSAM::AlgGuloso()
{

    SAMSol S;
    SAMSol T;

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            S.M[i][j] = 0;
            T.M[i][j] = 0;
            Maux[i][j] = 0; // mesmo valor de S
        }
    }


    S.valor = 0;
    T.valor = 0;

    double cpu0,cpu1;

    cpu0 = get_wall_time();
    for(int i = 0;i<n;i++)
        for(int j=0;j<n;j++)
            if(i!=j && rd->M[i][j]==1){
                if(addArc(i,j)){
                    S.M[i][j]=1;
                    S.valor++;
                }else{
                    T.M[i][j]=1;
                    T.valor++;
                }
            }

    cpu1 = get_wall_time();




    Graph gS(n);
    Graph gT(n);



    for (int i=0; i < n; i++)
        for (int j=0; j< n; j++) {
            if(S.M[i][j] == 1 && rd->M[i][j] == 1)
                gS.addEdge(i,j);
        }


    for (int i=0; i < n; i++)
        for (int j=0; j< n; j++) {
            if(T.M[i][j] == 1 && rd->M[i][j] == 1)
                gT.addEdge(i,j);
        }



    cout << "---------------------------------------------" << endl;
    cout << endl << "Algoritmo Guloso: " << endl;

    if(!gS.isCyclic()){
        cout << "Conjunto S é ACICLICO !!" << endl;
        cout << "Solucao = " << S.valor << endl;
    }else{
        cout << "ciclico" << endl;
    }

    if(!gT.isCyclic()){
        cout << endl << "Conjunto T é ACICLICO !!" << endl;
        cout << "Solucao = " << T.valor << endl;
    }else{
        cout << "ciclico" << endl;
    }


    cout << "CPU time: " << cpu1 - cpu0 << endl;


    if( S.valor > T.valor){
        copySolution(S);

    }else{
       copySolution(T);
    }


    if(saveSol == true)
        saveSolution("APG", cpu1 - cpu0);





}

void APSAM::AlgNiveis()
{


    cout << "---------------------------------------------" << endl;
    cout << endl << "Algoritmo de Niveis: " << endl;

    SAMSol A;
    SAMSol C;

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            C.M[i][j] = rd->M[i][j];
            A.M[i][j] = 0;
        }
    }

    A.valor = 0;
    C.valor = 0;


    vector<int> vetRandom;

    for(int i =0; i<n; i++)
        vetRandom.push_back(i);

//    cout << "myvector contains:";
//    for (vector<int>::iterator it=vetRandom.begin(); it!=vetRandom.end(); ++it)
//        cout << ' ' << *it;
//    cout<< "\n";

    random_shuffle(vetRandom.begin(),vetRandom.end());

//    cout << "myvector shuffle contains:";
//    for (vector<int>::iterator it=vetRandom.begin(); it!=vetRandom.end(); ++it)
//        cout << ' ' << *it;
//    cout<< "\n";



    double cpu0,cpu1;

    cpu0 = get_wall_time();
    unsigned int pos=0;
    for(int i =0; i<n; i++){

        //cout << pos+1 <<  " -> " << vetRandom.at(pos)+1 << endl;

        int random_pos = static_cast<int>(vetRandom.at(pos));

        if(inArcs(C,random_pos) > outArcs(C,random_pos)){ // se tivermos Mbestis arcos de entrada que de saida
            for(int j=0;j<n;j++){
                if(C.M[j][random_pos] == 1){
                    A.M[j][random_pos] = 1;                             // adicionamos os arcos de entrada na soluca

                }if(C.M[random_pos][j] == 1){
                    C.M[random_pos][j] = 0;                           // descartamos os vertices de saida
                }
            }

        }if(inArcs(C,random_pos) == outArcs(C,random_pos)){ // se for igual
            for(int j=0;j<n;j++){
                if(C.M[random_pos][j] == 1){
                    A.M[random_pos][j] = 1;                             // adicionamos os arcos de saida na solucao
                }if(C.M[j][random_pos] == 1){
                    C.M[j][random_pos] = 0;                           // descartamos os vertices de entrada
                }
            }
        }


        pos++;

    }


    cpu1 = get_wall_time();

    Graph gS(n);


    for (int i=0; i < n; i++)
        for (int j=0; j< n; j++) {
            if(A.M[i][j] == 1 && rd->M[i][j] == 1){
                gS.addEdge(i,j);
                A.valor++;
            }
        }


    if(!gS.isCyclic()){
        cout << "Conjunto Solucao é ACICLICO !!" << endl;
        cout << "Solucao = " << A.valor << endl;
    }else{
        cout << "ciclico"<< endl;
        cout << "Solucao = " << A.valor << endl;
    }



    cout << "CPU time: " << cpu1 - cpu0 << endl;


    copySolution(A);

    if(saveSol == true)
        saveSolution("APN", cpu1 - cpu0);






}

void APSAM::AlgOrdem()
{

    cout << "---------------------------------------------" << endl;
    cout << endl << "Algoritmo de Ordenacao: " << endl;

    SAMSol Af; // forward
    SAMSol Ab; // backward

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            Af.M[i][j] = 0;
            Ab.M[i][j] = 0;
        }
    }

    Af.valor = 0;
    Ab.valor = 0;



    vector<int >vetPesos;

    for(int i =0; i< 2*n; i++)
        vetPesos.push_back(i);

    srand(3);
    random_shuffle(vetPesos.begin(),vetPesos.end());


    double cpu0,cpu1;

    cpu0 = get_wall_time();

    for(unsigned int i =0; i< static_cast<unsigned int>(n); i++)
        for(unsigned int j =0; j< static_cast<unsigned int>(n); j++){
            if(vetPesos.at(i) < vetPesos.at(j)){
                if(rd->M[i][j] == 1){
                    Af.M[i][j] = 1;
                    Af.valor++;
                }
            }if(vetPesos.at(i) > vetPesos.at(j)){
                if(rd->M[i][j] == 1){
                    Ab.M[i][j] = 1;
                    Ab.valor++;
                }

            }

        }

     cpu1 = get_wall_time();


    Graph gF(n);
    Graph gB(n);


    for (int i=0; i < n; i++)
        for (int j=0; j< n; j++) {
            if(Af.M[i][j] == 1 && rd->M[i][j] == 1)
                gF.addEdge(i,j);
        }


    for (int i=0; i < n; i++)
        for (int j=0; j< n; j++) {
            if(Ab.M[i][j] == 1 && rd->M[i][j] == 1)
                gB.addEdge(i,j);
        }



    if(!gF.isCyclic()){
        cout << "Conjunto Aforward é ACICLICO !!" << endl;
        cout << "Solucao = " << Af.valor << endl;
    }



    if(!gB.isCyclic()){
        cout << endl << "Conjunto Abackward é ACICLICO !!" << endl;
        cout << "Solucao = " << Ab.valor << endl;

    }



    if(Af.valor > Ab.valor){
        copySolution(Af);

    }else{
       copySolution(Ab);
    }


    cout << "CPU time: " << cpu1 - cpu0 << endl;



    if(saveSol == true)
        saveSolution("APO", cpu1 - cpu0);



}

int APSAM::inArcs(SAMSol sol, int i)
{
    int in=0;


    for(int k=0;k<n;k++)
        if(sol.M[k][i] == 1){
              in++;
              //cout << "entrando " << k+1 << i+1 << endl;
        }


    return in;


}

int APSAM::outArcs(SAMSol sol, int i)
{


    int out=0;


    for(int k=0;k<n;k++)
        if( i!=k && sol.M[i][k] == 1){
              out++;
             // cout << "saindo " << i+1 << k+1 << endl;
        }


    return out;

}



int APSAM::FA()
{
    int bestSol = 0;
    for (int i = 0; i < n; i++) {
        for (int j =0; j < n; j++) {
            if(Mbest[i][j] == 1 && rd->M[i][j] == 1)
               bestSol++;

        }
    }


    return bestSol;

}

void APSAM::copySolution(SAMSol sol)
{


    for (int i = 0; i < n; i++)
        for (int j =0; j < n; j++)
            Mbest[i][j] = 0;


    best = 0;

    best = sol.valor;
    for (int i = 0; i < n; i++)
        for (int j =0; j < n; j++)
            if(sol.M[i][j] == 1 && rd->M[i][j] == 1)
                Mbest[i][j] = 1;


}

void APSAM::saveSolution(string alg, double time)
{


    string arqv;
    string s;
    s = static_cast<string>(rd->copyFileName);

    arqv = s + "_output";





    ofstream outputTable;

    string t;
    t = "SAM_APX_output";


    outputTable.open(t+".ods",ios:: app);
    if(outputTable.is_open()){

        outputTable << arqv << ";";  // instancia
        outputTable << alg << ";"; // FORMULACAO
        outputTable << best << ";";
        outputTable << time << ";";
        outputTable << " \n ";

    }

    outputTable.close();





}

bool APSAM::isFeasible()
{

    Graph g(n);

    for (int i=0; i < n; i++)
        for (int j=0; j< n; j++) {
            if(Maux[i][j] == 1 && rd->M[i][j] == 1)
                g.addEdge(i,j);
        }

    return !g.isCyclic(); // se nao for ciclico =>  aciclico

}



bool APSAM::addArc(int i, int j)
{

    Maux[i][j] = 1;
    if(!isFeasible()){
        Maux[i][j] = 0;  // Mbestux = S (alg guloso)
        return false;
    }else
        return true;



}

bool APSAM::remArc(int i, int j)
{
    if(Mbest[i][j] == 1){

        Mbest[i][j] = 0;
        return true;


    }else {
        return false;
    }
}




bool APSAM::N1()
{


    bool Mbest_copy[101][101];

    for (int i =0; i < n; i++) {
        for (int j =0 ; j < n; j++) {
            Mbest_copy[i][j] = Mbest[i][j];
        }
    }

    best = FA();
    //int old_best = FA();
    int new_best = 0;
    bool melhorou =false;



    int k =0;
    while(k < 100) {


        int no1 = rand()%n;
        int no2 = rand()%n;

        if(Mbest[no1][no2] == 1){


            Mbest[no1][no2] = 0;



            for (int i =0; i < 100; i++) {
                int no3 = rand()%n;
                if(addArc(no2,no3)){
                    new_best = FA();
//                    cout << "new best " << new_best << endl;
                }
            }

            if(new_best > best){
                melhorou = true;
                best = new_best;
            }else {

            }
        }


         k++;


    }


    if(melhorou){
        return true;
    }else {
        for (int i =0; i < n; i++)
            for (int j =0 ; j < n; j++){
               Mbest[i][j] = Mbest_copy[i][j];
             }
        return false;

    }



}

int APSAM::maxDG()
{
    for (int i =0; i < n; i++) {
        for (int j =0 ; j < n; j++) {

        }
    }
    return 0;
}












