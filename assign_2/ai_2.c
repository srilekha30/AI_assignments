#include<stdio.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>

const double stepSize = 0.001;



double stof(const char*);
void assignRandomInner(double weights[][17],int length);
void assignRandoOuter(double weights[][17],int length);
void readFromFile(double featureX[][17],double testAnswers[]);
double stof(const char* s);
int forwardPropagate(double features[],double innerW[][17],double outerW[][17],int HidElements,double inputForInner[],double outputFromInner[],double inputForOuter[],double answer[]);
double updateInnerValues(double features[],double innerW[]);
void learn(double features[][17],int sizeOfTrainingSet,double innerW[][17],double outerW[][17],int HidElements,double inputForInner[],double outputFromInner[],double inputForOuter[],double finalAnswer[],double trainAnswers[],int sc,int lf);
double sigmoid(double x);
double sigmoidDerivative(double x);


//random weights generation
void assignRandomInner(double weights[][17],int length)
{
	srand ( time(NULL) );
	int i;
	int j;
	for(i=0;i<length;i++)
	{
		for(j=0;j<17;j++)
		{
			weights[i][j] = (rand()%99 + 1)/100.0;
		}
	}
}

void assignRandomOuter(double weights[][17],int length)
{
	srand ( time(NULL) );
	int i;
	int j;
	for(i=0;i<length;i++)
	{
		for(j=0;j<10;j++)
		{
			weights[i][j] = (rand()%99 + 1)/100.0;
		}
	}
}

double stof(const char* s){
  double rez = 0, fact = 1;
  if (*s == '-'){
    s++;
    fact = -1;
  };
  int point_seen;
  for (point_seen = 0; *s; s++){
    if (*s == '.'){
      point_seen = 1; 
      continue;
    };
    int d = *s - '0';
    if (d >= 0 && d <= 9){
      if (point_seen) fact /= 10.0f;
      rez = rez * 10.0f + (double)d;
    };
  };
  return rez * fact;
};



void readFromFileTrain(double featureX[][17],double testAnswers[])
{
	FILE* fp;
  	char buf[2024];
	fp = fopen("train.csv", "r");
 	if(fp == NULL)
 	{
 		printf("ERROR");
	 }
	 int inputFeature = 0;
	 int inputNo = 0;
	while (fgets(buf, sizeof(buf), fp) != NULL)
	{
		inputFeature = 0;
	    buf[strlen(buf) - 1] = '\0';
	    char temp[20];
	    int i,count=0;
	    int j=0;
	    for(i=0;i<strlen(buf);i++)
		{
			if(buf[i]==',')
			{
				temp[j] = '\0';
				count++;
				j=0;
				if(count!=2)
					featureX[inputNo][inputFeature++] = stof(temp);
				else{
					temp[j] = '\0';
					testAnswers[inputNo] = stof(temp);
				}
			}
			else
			{
				temp[j]=buf[i];
				j++;
			}
		}

		inputNo++;
	  }
	  fclose(fp);
}

void readFromFileTest(double featureX[][17],double testAnswers[])
{
	FILE* fp;
  	char buf[2024];
	fp = fopen("test.csv", "r");
 	if(fp == NULL)
 	{
 		printf("ERROR");
	 }
	 int inputFeature = 0;
	 int inputNo = 0;
	while (fgets(buf, sizeof(buf), fp) != NULL)
	{
		inputFeature = 0;
	    buf[strlen(buf) - 1] = '\0';
	    char temp[20];
	    int i,count=0;
	    int j=0;
	    for(i=0;i<strlen(buf);i++)
		{
			if(buf[i]==',')
			{
				temp[j] = '\0';
				count++;
				j=0;
				if(count!=2)
					featureX[inputNo][inputFeature++] = stof(temp);
				else{
					temp[j] = '\0';
					printf("%s",temp);
					testAnswers[inputNo] = stof(temp);

				}
			}
			else
			{
				temp[j]=buf[i];
				j++;

			}
		}

		inputNo++;
	  }
	  fclose(fp);
}


//updation of the errors 
void updateErrors(double finalAnswer[],double trainAnswers,double Error[],int lf)
{
	int i;	
	if (lf==1) {
		for(i=0;i<10;i++){
			if(i==trainAnswers-1)
				Error[i] = 1-finalAnswer[i];
			else
				Error[i]=0-finalAnswer[i];
		}
	}
	else {
		for(i=0;i<10;i++){
	        if(i+1==trainAnswers)
	            Error[i]=1.0/ finalAnswer[i];
	        else
	            Error[i]=1.0/(finalAnswer[i]-1) ;
    	}
	}
}

//modify
// Upation of weights
int updateWeights(double Error[],double outerW[][17],double outputFromInner[],double net_k[],int HidElements,double finalAnswers[] 
				,double innerW[][17],double X[],int sc) 
{
	
	int i, j;
	int eta=0.001;
	double delta1[17], delta2[9];
   
	for(i=0;i<10;i++){
		delta1[i] = -1*Error[i] * sigmoidDerivative(net_k[i]); 
    }
    for(i=0;i<HidElements;i++){
        delta2[i]=0;
        for(j=0;j<10;j++){
            delta2[i] += delta1[j] * outerW[i][j] * sigmoidDerivative(finalAnswers[i]);
        }
    }
	for(i=0;i<HidElements;i++){
		for(j=0;j<10;j++){
            double deltaW = eta * outputFromInner[i] * delta1[j];
            outerW[j][i] -= deltaW;
            if(sc==2 && abs(deltaW) < 0.01)
                return 1;
		}       
    }
	
	for(i=0;i<17;i++){
    	for(j=0;j<HidElements;j++){
			innerW[i][j] -= eta * delta2[j] * X[i];
		}		
    }
}	
	


//Learning Function


void learn(double features[100][17],int sizeOfTrainingSet,double innerW[][17],double outerW[][17],int HidElements,double inputForInner[],double outputFromInner[],double inputForOuter[],double finalAnswer[],double trainAnswers[],int sc,int lf)
{
	
	
	int i, j;
	double errors[17];
    for(i=0;i<sizeOfTrainingSet;i++){
        j=0;
        do{
		    forwardPropagate(features[i],innerW,outerW,HidElements,inputForInner,outputFromInner,inputForOuter,finalAnswer);	
			updateErrors(finalAnswer,trainAnswers[i],errors,lf);
		    if(updateWeights(errors,outerW,outputFromInner,inputForOuter,HidElements,finalAnswer,innerW,features[i],sc)==1){
                break;
            }
            j++;
            if (j==100 && sc==1)
                break;
	    }
        while(1);
    }
}

int identifyClass(double outputs[]){
	int i=0,max_ind=0;
	for(i=0;i<10;i++)
		if(outputs[max_ind]<outputs[i])
			max_ind=i;
	return max_ind;
}

void test(double test_set[][17], int size, double weights[][17], double hidden_weights[][17], int noOfNeurons, double hidden_inputs[], double hidden[], double outer[], double outputs[], double target[]){
   int i, count1=0;
    for(i=0;i<size;i++){
                forwardPropagate(test_set[i], weights, hidden_weights, noOfNeurons, hidden_inputs, hidden, outer, outputs);
                if( abs(target[i]-identifyClass(outputs)+1)==0){
                    count1++;
                    printf("%d",count1);
                }
            }
    printf("Accuracy:%f\n", 100.0*(count1/998.0));
}

//Forward Propagation
int forwardPropagate(double features[],double innerW[][17],double outerW[][17],int HidElements,double inputForInner[],double outputFromInner[],double inputForOuter[],double answer[])
{
	int i;
	//Hidden layer calculations.
	for(i=0;i<HidElements;i++)
	{
		inputForInner[i] = updateInnerValues(features,innerW[i]);
		
		outputFromInner[i]= sigmoid(inputForInner[i]);
	}
	
	/*Outer layer calculations*/
	for(i=0;i<10;i++)
	{
		int j;
		double ans = 0;
		for(j=0;j<HidElements;j++)
		{
			ans += (outputFromInner[j]*outerW[j][i]);
		}
		inputForOuter[i] = ans;
		answer[i] = sigmoid(ans);
	}
}


double updateInnerValues(double features[],double innerW[])
{
	int i;
	double answer = 0;
	
	for(i=0;i<17;i++)
	{
		answer += (features[i]*innerW[i]);
	}
	return answer;
}



double sigmoid(double x)
{
	return 1/(1+pow(2.718,-x));
}
double sigmoidDerivative(double sigmoidValue)
{
	return sigmoid(sigmoidValue )* (1-sigmoid(sigmoidValue));
}



int main()
{
	int HidElements;
	int Train_size = 2217;
	int Test_size = 1000;
	double featureX[2217][17];
	double trainAnswers[500];
	double innerW[20][17];
	double outerW[20][17];
	double inputFeatures[17];
	double inputForInner[20];
	double outputFromInner[20];
	double inputForOuter[20];
	double finalAnswer[20];
	double testX[1000][17];
	double target[1000];
	int i,j;
	printf("Which stopping criteria do you prefer?\n1:100 epochs\n2:Euclidian Norm");
	scanf("%d",&i);

	printf("Which loss function do you prefer?\n1:mean square\n2:cross entropy");
	scanf("%d",&j);
	
	printf("Please, Enter the number of hidden elements:\n");
	scanf("%d",&HidElements);
	readFromFileTrain(featureX,trainAnswers);
	readFromFileTest(testX,target);  
	
	assignRandomInner(innerW,HidElements);
	assignRandomOuter(outerW,HidElements);
	
	
//	features[100][13],int sizeOfTrainingSet,double innerW[][17],double outerW[][17],int HidElements,double inputForInner[],double outputFromInner[],double inputForOuter[],double finalAnswer[],double trainAnswers[]
	
	learn(featureX,Train_size,innerW,outerW,HidElements,inputForInner,outputFromInner,inputForOuter,finalAnswer,trainAnswers,i,j);
	test(testX, Test_size, innerW, outerW, HidElements, inputForInner, outputFromInner, inputForOuter, finalAnswer, target);
	return 0;
}
