#include<stdio.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>

int inputs[6675][195];
int testInput[3335][195];
int m1[4450][195],m2[4450][195],m3[4450][195];


//functions
void readTraining(int inputs[][195]);
void readTest(int testInput[][195]);
void merge(double arr[],int index[], int l, int m, int r);
void mergeSort(double arr[],int index[], int l, int r);
double distance(int x[],int y[]);
int train();
void test(int testInput[][195],int inputs[][195],int k);



//reading the training data
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

//reading the data from test file
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

//merge sort algorithm
void merge(double arr[],int index[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* create temp arrays */
    int L[n1], R[n2],li[n1],ri[n2];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++){
        L[i] = arr[l + i];
	li[i]=index[l+i];
	}
    for (j = 0; j < n2; j++){
        R[j] = arr[m + 1+ j];
	ri[j]=index[m+1+j];
	}

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
		index[k]=li[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
		index[k]=ri[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        arr[k] = L[i];
	index[k]=li[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        arr[k] = R[j];
	index[k]=ri[j];
        j++;
        k++;
    }
}

/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
//merge sort
void mergeSort(double arr[],int index[], int l, int r)
{
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l+(r-l)/2;

        // Sort first and second halves
        mergeSort(arr,index, l, m);
        mergeSort(arr,index, m+1, r);

        merge(arr,index, l, m, r);
    }
}

//Euclidean  Distance
double distance(int x[],int y[]) {
  int i,count=0;
  float sum=0;
  for(i=0;i<192;i++) {
    count++;
     sum=sum+((x[i]-y[i])*(x[i]-y[i]));
    //printf("%d %d\n",x[i],y[i]);
  }
  sum=sqrt(sum);
  //printf("%f %d",sum,count);
  return sum;
}



//training the dataset and k value is found accordingly
int train(){
	int i,j,k=1,t=0;
	int a1[2225][195],a2[2225][195],a3[2225][195];
	double te[3][25];
	int mo,co=0;
	double dist[4450];
	int index[4450];

	//making 3 parts in the dataset
	for(i=0;i<6670;i++){
		for(j=0;j<193;j++){
			if(i>=0 && i<2224){
				a1[i][j]=inputs[i][j];
			}
			else if(i>=2224 && i<4447){
				a2[i-2224][j]=inputs[i][j];
			}
			else{
				a3[i-4447][j]=inputs[i][j];
			}
		}
	}

	//merging first and second part of the dataset
	for(i=0;i<4447;i++){
		for(j=0;j<193;j++){
			if(i<2224){
				m1[i][j]=a1[i][j];
			}
			else{
				m1[i][j]=a2[i-2224][j];
			}
		}
	}

	//merging third and second part of the dataset
	for(i=0;i<4447;i++){
		for(j=0;j<193;j++){
			if(i<2224){
				m2[i][j]=a2[i][j];
			}
			else{
				m2[i][j]=a3[i-2224][j];
			}
		}
	}

	//merging first and third part of the dataset
	for(i=0;i<4447;i++){
		for(j=0;j<193;j++){
			if(i<2224){
				m3[i][j]=a1[i][j];
			}
			else{
				m3[i][j]=a3[i-2224][j];
			}
		}
	}
	
	//initializing 3*25 matrix with 0's
	for(i=0;i<3;i++){
		for(j=0;j<25;j++){
			te[i][j]=0;
		}
	}	

	//calculating errors and storing in the 3*25 matrix
	for(k=1;k<=25;k++){
		for(t=0;t<3;t++){
			for(i=0;i<2224;i++){
				for(j=0;j<4447;j++){
					//distance matrix for distances between 2 instances
					dist[j]=0;
				}
				for(j=0;j<4447;j++){
					if(t==0){
						dist[j]=distance(a1[i],m2[j]);
						index[j]=j;
					}
					else if(t==1){
						dist[j]=distance(a2[i],m3[j]);
						index[j]=j;
					}
					else if(t==2){
						dist[j]=distance(a3[i],m1[j]);
						index[j]=j;
					}
				}
				//merging the distance array
				mergeSort(dist,index,0,4446);
				//first fold
				if(t==0){
					for(mo=0;mo<k;mo++){
						if(a1[i][192]!=m2[index[mo]][192]){
							te[t][k-1]++;
						}
					}				
				}
				//second fold
				else if(t==1){
					for(mo=0;mo<k;mo++){
						if(a2[i][192]!=m3[index[mo]][192]){
							te[t][k-1]++;
						}
					}
				}
				//third fold
				else if(t==2){
					for(mo=0;mo<k;mo++){
						if(a3[i][192]!=m1[index[mo]][192]){
							te[t][k-1]++;
						}
					}
				}
			}			
		}
	}
	double error[25];
	int va,z;
	//storing error probabilities in 3*25 matrix
	for(va=0;va<3;va++){
		for(z=0;z<25;z++) {
			if(va==0){
				te[va][z]=te[va][z]/2224;
			}
			else if(va==1){
				te[va][z]=te[va][z]/2223;
			}
			else{
				te[va][z]=te[va][z]/2223;
			}
		}
	}	
	for(z=0;z<25;z++) {
			error[z]=(te[0][z]+te[1][z]+te[2][z])/3;
	}

	//calculating which k value has minimum error
	int min=error[0],ind=0;
	for(z=1;z<25;z++) {
		if(min>error[z]){
			min=error[z];
			ind=z;
		}
	}	
	return ind+1;
}

//finding nearest neighbour and the most probable class
void test(int testInput[][195],int inputs[][195],int k){
	int i,j,v;
	double acc=0;
	double dist[6675];
	int index[6675];
	int vm[10];
	for(v=0;v<10;v++){
		vm[k]=0;
	}
	for(i=0;i<3333;i++){
		for(j=0;j<6670;j++){
			dist[j]=distance(testInput[i],inputs[j]);
			index[j]=j;
		}
		mergeSort(dist,index,0,6669);
		for(v=0;v<k;v++){
			vm[inputs[index[v]][192]]++;
		}
		int max=vm[0];
		int z,ind=0;
		for(z=1;z<10;z++) {
			if(max<vm[z]){
				max=vm[z];
				ind=z;
			}
		}
		if(testInput[i][192]==ind){
			acc++;
		}
	}
	//printf("%lf\n",acc);
	/*printing the accuracy*/ printf("\n\n\n\nAccuracy : %lf \n",100-((acc/3333)*100));/*accuracy is calculated */
}

int main(){
	int i, j, k;
	//reading from training file
	readTraining(inputs);
	//reading from test file
	readTest(testInput);
	//training and finding k value
	k=train(inputs);
	//printing k value
	printf("k value using 3-fold validation: %d \n",k);
	//reading test file and finding class for given text input
	test(testInput,inputs,k);
	return 0;
}