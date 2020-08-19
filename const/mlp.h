#ifndef MLP_H
#define MLP_H


//DEFINES
#define inLength 12
#define hidLength 36
#define outputLength 1
#define learningRate 0.1
#define mlpThreshold 0.005
#define lim 0.5	//limite para considerar uma saída como classificada
#define maxtraincycles 10000
//#define resultsFile "const/mlpfiles/results"
//#define parametersFile "const/mlpfiles/parameters"
//#define trainFile "const/mlpfiles/traindataset"
//#define testFile "const/mlpfiles/testdataset"

#include <fstream>
#include<QString>
#include<QDir>

//CLASSES
class MLP{
    private:
        static std::ofstream results;
        static float matH[hidLength][inLength+1]; //pesos+bias de cada neuronio da camada H em uma linha
        static float matO[outputLength][hidLength+1]; //pesos+bias de cada neuronio da camada O em uma linha
        static float outputResult[outputLength]; //resultados obtidos em cada neuronio da camada O (output)
        static float hidResult[hidLength]; //resultados obtidos em cada neuronio da camada H (hidden)

        static float funcActiv(float z);
        static float derivFuncActiv(float z);
        static int countLines(std::ifstream    & file);
        static void backpropagation(float X[][inLength], float Y[][outputLength], int qtTrainCases);
    public:
        MLP();
        ~MLP();
        static void writeResult();
        static float* forward(float* inVector);
        static void trainMlp();
        static void testMlp();
        static void saveParameters();
        static void loadParameters();

        static float erroTotal;
};


#endif
