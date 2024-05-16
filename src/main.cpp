#include <cstring>
#include <stdio.h>
#include <time.h>
#include <fstream>
#include <iostream>

#if defined(ABC_NAMESPACE)
namespace ABC_NAMESPACE
{
#elif defined(__cplusplus)
extern "C"
{
#endif

    // procedures to start and stop the ABC framework
    // (should be called before and after the ABC procedures are called)
    void Abc_Start();
    void Abc_Stop();
    

    // procedures to get the ABC framework and execute commands in it
    typedef struct Abc_Frame_t_ Abc_Frame_t;

    Abc_Frame_t *Abc_FrameGetGlobalFrame();
    int Cmd_CommandExecute(Abc_Frame_t *pAbc, const char *sCommand);

#if defined(ABC_NAMESPACE)
}
using namespace ABC_NAMESPACE;
#elif defined(__cplusplus)
}
#endif

void Abc_Do(Abc_Frame_t *pAbc, const char *sCommand){
    char Command[1000] = {0};
    std::string ss(sCommand);
    sprintf( Command,"%s" ,ss.c_str() );
    if ( Cmd_CommandExecute( pAbc, Command ) )
    {
        fprintf( stdout, "Cannot execute command \"%s\".\n", sCommand );
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <exNN.truth>\n", argv[0]);
        return 7414;
    }
    // variables
    Abc_Frame_t *pAbc;
    std::string pFileName(argv[1]);
    
    Abc_Start();
    pAbc = Abc_FrameGetGlobalFrame();
    std::string pFileName1 = "read_truth -x -f " + pFileName;

    Abc_Do(pAbc, pFileName1.c_str());
    Abc_Do(pAbc, "strash; collapse; balance; rewrite -l; rewrite -lz; rewrite -l");
    Abc_Do(pAbc, "strash; collapse; balance; rewrite -l; rewrite -lz; rewrite -l");
    Abc_Do(pAbc, "strash; balance; rewrite -l; rewrite -lz; rewrite -l; strash");
    for(int i = 0;i < 10;i++){
        Abc_Do(pAbc, "dsd; balance; rewrite -l; rewrite -lz; rewrite -l; strash");
    }
    Abc_Do(pAbc, "multi");
    
    for(int i = 0;i < 10;i++){
        Abc_Do(pAbc, "reorder");
    }
    // printf("HERE4\n");
    for(int i = 0; i <= 100; i++){
        Abc_Do(pAbc, "istrash; iresyn -l; dc2 -bl");
        Abc_Do(pAbc, "istrash; refactor -N 15 -lz");
        Abc_Do(pAbc, "istrash; orchestrate");
        Abc_Do(pAbc, "istrash; resub -lz");
        if(i % 10 == 0){
            Abc_Do(pAbc, "&get;");
            Abc_Do(pAbc, "&b; &deepsyn -T 60; &dc2;");
            Abc_Do(pAbc, "&put");
        }
    }
    
    // printf("HERE5\n");
    
    Abc_Do(pAbc, "strash; csweep; trim");
    std::string pFileName2 = "strash; write_aiger " + pFileName.substr(pFileName.find("ex")+2, pFileName.find("ex")+4) + ".aig";
    Abc_Do(pAbc, pFileName2.c_str());
    Abc_Stop();
    return 0;
    
}
