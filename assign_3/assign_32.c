#include<stdio.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>
int inputs[6675][195];
int testInput[3335][195];
//functions
void readTraining(int inputs[][195]);
void readTest(int testInput[][195]);
void findingProbabilities(int inputs[][195],double pro[10][192][5],int cp[]);
void testing(int testInput[][195],double pro[10][192][5]);

//reading data from file
void readTraining(int inputs[][195]){
    FILE* fp = fopen("shuffled.dat", "r");
    int i,j;

    while(getc(fp) != EOF){
        for(i=0;i<6670;i++){
            for(j=0;j<193;j++){
                fscanf(fp,"%d",&inputs[i][j]);

                        }
            }
        }
        fclose(fp);
}

//reading data from test file
void readTest(int testInput[][195]){
        FILE* fp = fopen("pp_tes.dat", "r");
        int i,j;
        while(getc(fp) != EOF){
        for(i=0;i<3333;i++){
            for(j=0;j<193;j++){
                fscanf(fp,"%d",&testInput[i][j]);
                        }
            }
        }
        fclose(fp);
}

//creating 3D matrix 
void findingProbabilities(int inputs[][195],double pro[10][192][5],int cp[]){
    int i,j,k;
    //calculating probabilities
    for(i=0;i<6670;i++){
        for(j=0;j<193;j++){
            pro[inputs[i][192]][j][inputs[i][j]]++;
        }
        cp[inputs[i][192]]++;
    }
    for(i=0;i<10;i++){
        for(j=0;j<192;j++){
            for(k=0;k<5;k++){
                pro[i][j][k]=pro[i][j][k]/667;
            }
        }
    }
}

//calculating probability with max class
void testing(int testInput[][195],double pro[10][192][5]){
    int i,j,k;
    double acc=0;
    double tpro[10];
    int ind=0;
    double max=tpro[0];
    for(i=0;i<3333;i++){
        for(j=0;j<10;j++){
            tpro[j]=0;
        }
        for(j=0;j<10;j++){
            for(k=0;k<192;k++){
                    tpro[j]+=log(pro[j][k][testInput[i][k]]);
            }
        }

        //storing index of maximum probability class
        for(j=1;j<10;j++){
            if(max<tpro[j]){
                max=tpro[j];
                ind=j;
            }
        }
        if(testInput[i][192]==ind) {
            acc++;
        }
    }
    /*printing the accuracy*/ printf("\nAccuracy : %lf \n",100-((acc/3333)*100));/*accuracy is calculated */
}
int main(){
    int i, j, k,cp[10];
    double pro[10][192][5];
    // reading train data
    readTraining(inputs);
    // reading test data
    readTest(testInput);
   
    // generating probabilities and storing in 3D MATRIX
    for(i=0;i<10;i++){
        for(j=0;j<192;j++){
            for(k=0;k<5;k++){
                pro[i][j][k]=0;
            }
        }
    }

    //storing class probability in a matrix...intializing with 0 
    for(k=0;k<10;k++){
        cp[k]=0;
    }

    findingProbabilities(inputs,pro,cp);
    testing(testInput,pro);
    return 0;
}