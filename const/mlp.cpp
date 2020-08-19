#include "mlp.h"
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<time.h>
#include<random>

//#include"const/mlp.h"

using namespace std;

//inicializando as variaveis
float MLP::erroTotal = 0;
std::ofstream MLP::results;
float MLP::matH[hidLength][inLength+1];
float MLP::matO[outputLength][hidLength+1];
float MLP::outputResult[outputLength];
float MLP::hidResult[hidLength];

MLP::MLP(){

    //preencher pesos e biases com numeros aleatorios
    int i, j;
    srand(time(0));
    for(i=0;i<hidLength;i++){
        for(j=0;j<(inLength+1);j++){
            MLP::matH[i][j] = 2.0f * ((float)rand() / (2.0f * (float)RAND_MAX)) - 0.5f;
        }
    }
    for(i=0;i<outputLength;i++){
        for(j=0;j<(hidLength+1);j++){
            MLP::matO[i][j] = 2.0f * ((float)rand() / (2.0f * (float)RAND_MAX)) - 0.5f;
        }
    }
    //path para o arquivo results
    QString QresultsFilePath = QDir().filePath("../const/mlpfiles/results");
    std::string resultsFile = QresultsFilePath.toUtf8().constData();
    results.open(resultsFile);
    if(!results.is_open()){
        cout << "error opening results file" << endl;
    }
}

MLP::~MLP(){
    results.close();
}

void MLP::trainMlp(){
    //path para o arquivo traindataset
    QString QtrainFilePath = QDir().filePath("../const/mlpfiles/traindataset");
    std::string trainFile = QtrainFilePath.toUtf8().constData();
    int i, j;
    std::ifstream trainDataset;
    //acessando arquivo contendo dataset
    trainDataset.open(trainFile);
    if(!trainDataset.is_open()){
        cout << "erro ao abrir trainDataset" << endl;
    }else{
        //Contando quantidade de linhas do dataset = quantidade de "casos teste" para o MLP

        int qtTrainCases = countLines(trainDataset);

        //matrizes de entradas e de saidas para treinar o MLP
        float X[qtTrainCases][inLength], Y[qtTrainCases][outputLength];

        //Preenchendo matrizes com dados do dataset
        for(i=0;i<qtTrainCases;i++){
            for(j=0;j<inLength;j++){
                trainDataset >> X[i][j];
            }
            for(j=0;j<outputLength;j++){
                trainDataset >> Y[i][j];
            }
        }
        trainDataset.close();

        backpropagation(X, Y, qtTrainCases);
    }
}

void MLP::testMlp(){
    //path para o arquivo testdataset
    QString QtestFilePath = QDir().filePath("../const/mlpfiles/testdataset");
    std::string testFile = QtestFilePath.toUtf8().constData();
    int i, j;
    std::ifstream testDataset;
    //testando MLP
    float vec[inLength];

    testDataset.open(testFile);
    if(!testDataset.is_open()){
        cout << "error opening test dataset" << endl;
    }else{
        int qtTestCases = countLines(testDataset);

        for(i=0;i<qtTestCases;i++){
            //cout << "digite as entradas:" << endl;
            for(j=0;j<inLength;j++){
                testDataset >> vec[j];
            }
            forward(vec);
            writeResult();
        }
        testDataset.close();
    }
}

int MLP::countLines(ifstream& file){
    string line;
    int qtLines = 0;

    while(!file.eof()){
        getline(file, line);
        if(line.size() > 1){
            qtLines++;
        }
    }
    file.clear();
    file.seekg(0, std::ios_base::beg);
    return qtLines;
}

void MLP::writeResult(){
    int i;
    //std::cout << "resultado = [";
    for(i=0;i<outputLength;i++){
        if(i==outputLength-1){
            //printf("%.2f]\n", outputResult[i]);
            results << outputResult[i];
            results << endl;
        }else{
            //printf("%.2f, ", outputResult[i]);
            results << outputResult[i];
            results << " ";
        }
    }
}

float MLP::funcActiv(float z){
    //sigmoide
    return (1.0/(1.0 + expf(-z)));
}

float MLP::derivFuncActiv(float z){
    //derivada sigmoide
    return (z*(1.0 - z));
}

float* MLP::forward(float* inVector){
    int i, j;
    float HidTotal = 0, OutTotal = 0;
    float* output;

    output = (float*)malloc(outputLength*sizeof(float));

    //Calcular resultados dos neuronios da camada hidden
    for(i=0;i<hidLength;i++){
        HidTotal = 0;
        for(j=0;j<inLength;j++){
            HidTotal += matH[i][j] * inVector[j]; // + w*x
        }
        HidTotal += matH[i][inLength]; // + b
        hidResult[i] = funcActiv(HidTotal); // resultado final do neuronio
    }

    //Calcular resultados dos neuronios da camada output
    bool foundOne = false;
    for(i=0;i<outputLength;i++){
        OutTotal = 0;
        for(j=0;j<hidLength;j++){
            OutTotal += matO[i][j] * hidResult[j]; // + w*x
        }
        OutTotal += matO[i][hidLength]; // + b
        outputResult[i] = funcActiv(OutTotal);
        output[i] = outputResult[i];

        //vetor a ser retornado
        /*comentando para evitar arredondamento
        if(outputResult[i] >= lim && !foundOne){
            output[i] = 1;
            foundOne = true;
        }else if(outputResult[i] < (1-lim)){
            output[i] = 0;
        }else{
            output[i] = -1;
        }*/
    }
    return output;

}
void MLP::backpropagation(float X[][inLength], float Y[][outputLength], int qtTrainCases){
    int i, j, k;
    float erro, deltaO[outputLength], deltaH[hidLength];
    float entradas[inLength];
    double cycles = 0;
    erroTotal = 2*mlpThreshold;

    while(erroTotal > mlpThreshold && cycles < maxtraincycles){
        cycles++;

        erroTotal = 0; //erro eh recalculado a cada processamento do dataset para treino do MLP

        for(i=0;i<qtTrainCases;i++){

            for(j=0;j<inLength;j++){
                entradas[j] = X[i][j];
            }

            forward(entradas);

            //calculando delta de cada neuronio da camada de saida
            for(j=0;j<outputLength;j++){
                erro = Y[i][j] - outputResult[j];
                erroTotal += pow(erro, 2);
                deltaO[j] = -2 * erro * derivFuncActiv(outputResult[j]);
            }

            //calculando delta de cada neuronio da camada hiddden
            float sum;
            for(j=0;j<hidLength;j++){
                sum = 0;
                for(k=0;k<outputLength;k++){
                    sum += deltaO[k] * matO[j][k];
                }
                deltaH[j] = sum * derivFuncActiv(hidResult[j]);
            }

            //atualizando pesos e bias da camada hidden
            for(j=0;j<hidLength;j++){
                for(k=0;k<inLength;k++){
                    matH[j][k] = matH[j][k] - learningRate * deltaH[j] * entradas[k];
                }
                matH[j][inLength] = matH[j][inLength] - learningRate * deltaH[j];
            }

            //atualizando pesos e bias da camada de saida
            for(j=0;j<outputLength;j++){
                for(k=0;k<hidLength;k++){
                    //atualiza pesos
                    matO[j][k] = matO[j][k] - learningRate * deltaO[j] * hidResult[k];
                }
                //atualiza bias
                matO[j][hidLength] = matO[j][hidLength] - learningRate * deltaO[j];
            }

        }
        erroTotal = erroTotal/qtTrainCases;
        //cout << "cycles = " << cycles << std::endl;
    }
    std::cout << "erro = " << erroTotal << std::endl;
    cout << "ciclos = " << cycles << endl;
    //printf("erro = %f\n", erroTotal);
}

void MLP::saveParameters(){
    //path para o arquivo parameters
    QString QparametersFilePath = QDir().filePath("../const/mlpfiles/parameters");
    std::string parametersFile = QparametersFilePath.toUtf8().constData();
    //atualiza arquivo 'parameters' caso seja obtido um erro menor
    int i, j;
    fstream parameters;
    parameters.open(parametersFile, ios::in);
    if(!parameters.is_open()){
        cout << "error opening parameters file" << endl;
    }else{
        float savedError;
        string errostr;

        parameters >> errostr;
        parameters >> savedError;
        //parameters.seekg(0, ios::beg); //posiciona cursor no comeco do arquivo
        parameters.close();
        if(erroTotal < savedError){ //confere se o erro obtido eh menor que o erro salvo
            parameters.open(parametersFile, ios::out);
            if(!parameters.is_open()){
                cout << "parameters couldn't be updated" << endl;
            }else{
                parameters << "erro ";
                parameters << erroTotal;
                parameters << endl;
                for(i=0;i<hidLength;i++){
                    for(j=0;j<(inLength+1);j++){
                        parameters << matH[i][j];
                        if(j!=inLength){
                            parameters << " ";
                        }
                        //matH[i][j] = 2.0f * ((float)rand() / (2.0f * (float)RAND_MAX)) - 0.5f;
                    }
                    parameters << endl;
                }
                for(i=0;i<outputLength;i++){
                    for(j=0;j<(hidLength+1);j++){
                        parameters << matO[i][j];
                        if(j!=hidLength){
                            parameters << " ";
                        }
                        //matO[i][j] = 2.0f * ((float)rand() / (2.0f * (float)RAND_MAX)) - 0.5f;
                    }
                    parameters << endl;
                }
                parameters.close();
            }
        }
    }
}

void MLP::loadParameters(){
    //path para o arquivo parameters
    QString QparametersFilePath = QDir().filePath("../const/mlpfiles/parameters");
    std::string parametersFile = QparametersFilePath.toUtf8().constData();
    //carrega parametros salvos nas variaveis correspondentes
    int i, j;
    ifstream parameters;
    parameters.open(parametersFile);
    if(!parameters.is_open()){
        cout << "error opening parameters file" << endl;
    }else{
        string errostr;
        parameters >> errostr;
        parameters >> erroTotal;
        cout << "erro = " << erroTotal << endl;
        for(i=0;i<hidLength;i++){
            for(j=0;j<(inLength+1);j++){
                parameters >> matH[i][j];
                //matH[i][j] = 2.0f * ((float)rand() / (2.0f * (float)RAND_MAX)) - 0.5f;
            }
        }
        for(i=0;i<outputLength;i++){
            for(j=0;j<(hidLength+1);j++){
                parameters >> matO[i][j];
                //matO[i][j] = 2.0f * ((float)rand() / (2.0f * (float)RAND_MAX)) - 0.5f;
            }
        }
        parameters.close();
    }
}
