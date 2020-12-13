#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <time.h>

#include "quicksort-omp.h"
using namespace std;

const int lenArr = 5757; // 262144000 = 1000 MBytes = 1 GByte
int numthreads = 4;

string asciiToString(long long ascii);
void checkArrays(long long* arr1, long long* arr2);

void quickSort(long long* arr, int left, int right) 
{
	int i = left, j = right;
	long long tmp;
	long long pivot = arr[(left + right) / 2];

  	/* PARTITION PART */
	while (i <= j) {
		while (arr[i] < pivot)
			i++;
		while (arr[j] > pivot)
			j--;
		if (i <= j) {
			tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
			i++;
			j--;
		}
	}

	/* RECURSION PART */
	if (left < j){ quickSort(arr, left, j);  }
	if (i< right){ quickSort(arr, i, right); }
}


int main(){
	int minMum = 1;
	int maxNum = lenArr;
	int maxNumbersDisplayed = 30;

	double startTime, stopTime;

	long long* arr1;
	long long* arr2; 
	arr1 = (long long*) malloc(lenArr*sizeof(long long));
	arr2 = (long long*) malloc(lenArr*sizeof(long long));

	ifstream infile("sgb-words.txt");
	string key;
	long long value = 0;
	/*
		File Reading
	*/
	int i = 0;
	if (infile.is_open()) {
		while (infile >> key) {
			for (int x = 0; key[x] != '\0'; x++) {
               value *= 1000;
               value += int(key[x]);
           }
			// cout << value << endl;
			arr1[i] = value;
			// cout << arr1[i] << endl;
			arr2[i] = value;
			value = 0;
			i++;
			//cin.get();
		}
	}
	else std::cerr << "Cannot open file";
	

	//printf("--------------------------------------------------");
	//printf("Info about the program: \n");
	//printf("Datatype of the array: %p", typeof() );
	//printf("Positions in the array: %d", typeof(arr1) );
	printf("\nSize of the array (aprox.): %lu MBytes \n", (lenArr*sizeof(long long)/(1024*1024)) );
	printf("TOTAL MEMORY ALLOCATED:  3 x array = (aprox.): %lu MBytes \n\n", (lenArr*sizeof(long long)/(1024*1024))*3 );  
	//printf("-------------------------------------------------- \n\n");



	
	if( lenArr <= maxNumbersDisplayed ){
		printf("\n\nArray BEFORE sorting: \n");
		for( i = 0 ; i < lenArr; i++ ) 
		{
			printf("%lld \n", arr1[i]);
			cin.get();
		}
		printf("\n\n\n");
	}


	/*
		Sequential QuickSort
	*/
	printf("\nSorting with custom serial QuickSort..."); fflush(stdout);
	startTime = clock();
	quickSort(arr1, 0, lenArr-1);
	stopTime = clock();

	
	if( lenArr <= maxNumbersDisplayed ){
		for( i = 0 ; i < lenArr; i++ ) 
		{
			printf("%d \n", arr1[i]);
		}
	}
	printf("\nSorted in (aprox.): %f seconds \n\n", (double)(stopTime-startTime)/CLOCKS_PER_SEC);



	/*
		Parallel QuickSort
	*/
	printf("\nSorting with custom PARALLEL QuickSort... "); fflush(stdout);
	startTime = omp_get_wtime();
	quickSort_parallel(arr2, lenArr, numthreads);
	stopTime = omp_get_wtime();


	if( lenArr <= maxNumbersDisplayed ){
		for( i = 0 ; i < lenArr; i++ ) 
		{
			printf("%d \n", arr2[i]);
		}
	}
	printf("\nSorted in (aprox.): %f seconds \n\n", (stopTime-startTime));

    checkArrays(arr1, arr2);
    ofstream outfile("sortedStrings.txt");
    if (outfile.bad()) {
        cerr << "Can not open File" << endl;
        exit(-1);
    }

	// string s = "";
	//string srr[lenArr];
	for( i = 0 ; i < lenArr; i++ ) 
	{
        
        outfile << asciiToString(arr2[i]) << "\n";
        // cout << asciiToString(arr2[i]) << endl;
	}
	// for( i = 0 ; i < lenArr; i++ ) 
	// {
	// 	printf("%lld \n", arr2[i]);
		
	// }

	printf("\n\n");

	free(arr1);
	free(arr2);
    outfile.close();
	return 0;
}


string asciiToString(long long ascii) {
    string s = "";
	int a[5];
	int j = 0;
    while(ascii>1) {
        a[j] = ascii % 1000;
        ascii /= 1000;
        j++;
    }
    
    for (int k = 4; k >= 0 ; --k) {
        s += a[k];
    }
    return s;
}

void checkArrays(long long* arr1, long long* arr2) {
    printf("\nChecking if the results are correct...\n");

	bool correctResult;
	correctResult=true;
	int i = 0;
	while( (correctResult==true) && (i<lenArr) )
	{
		if(arr1[i]!=arr2[i]) { correctResult=false; }
		i++;
	}
	if(correctResult==true){
		printf("The result with 'custom serial QuickSort' is CORRECT\n");
	}else{
		printf("The result with 'custom serial QuickSort' is ¡¡INCORRECT!!\n");
	}

	correctResult = true;
	i = 0;
	while( (correctResult==true) && (i<lenArr) )
	{
		if(arr2[i]!=arr1[i]) { correctResult=false; }
		i++;
	}
	if(correctResult==true){
		printf("The result with 'custom PARALLEL QuickSort' is CORRECT\n\n");
	}else{
		printf("The result with 'custom PARALLEL QuickSort' is ¡¡INCORRECT!!\n");
	}
}