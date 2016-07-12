#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gnuplot_i.h"
#include <time.h>
void create_csv(double *big_array, int iteration);
int main()
{
// file to read temperature
	FILE *fp ;
// file to write current temperature for html page
	FILE *fp2 ;
	char str1[100], str2[100], str3[100];
//	char str3[100]= "01.00";
//	printf("%s\n",str3);
	printf("%f \n", strtod(str3,NULL));
	int iterations = 2000;	
	double *storage = (double*) malloc(iterations * sizeof(double));
	memset(storage, 0, iterations * sizeof(double));
	int i = 0;
// number of iterations it will be running
	while (i < 2000)
	{
// opens the files "r" is for read and "w" is for write 
 		fp=fopen("/sys/bus/w1/devices/28-000006744fe0/w1_slave", "r") ;
		fp2=fopen("/home/pi/public_html/temp.txt", "w") ;
// if the file containing data is empty
		if(fp == NULL) 
		{
			perror("Error opening file");
     			return(-1);
   		}
// if the file containing data is filled 	
		if( fgets(str1,60,fp) != NULL && fgets(str2,60,fp) != NULL)
		{
			printf("%s  %s \n",str1, str2);
			str3[0] = str2[29];
			str3[1] = str2[30];
			str3[2] = '.';
			str3[3] = str2[31];
			str3[4] = str2[32];
			printf("%f \n", strtod(str3, NULL));			
			printf("Current temperature is : %s °C \n", str3);
			fprintf(fp2, "Current temperature is : %s \\°C \n", str3);
			storage[i] = strtod(str3, NULL);
			create_csv(storage,i);
		}
// close the files 
		fclose(fp) ;
		fclose(fp2) ;
// wait 300 seconds for next iteration 
		sleep(300);		
		i++;
	}
	return 0 ;
}

void create_csv(double *big_array_1, double *big_array_2, int iteration, int timestamp)
{
	time_t curtime;
	time(&curtime);	
	double adapt_array_1[iteration];
	double adapt_array_2[iteration];	
	char fileName[100];
	strcat(fileName,ctime(&curtime));
	strcat(fileName,".csv");

	int i = 0;
	while ( i < iteration)
		{
			adapt_array_1[i] = big_array_1[i];
			adapt_array_2[i] = big_array_2[i];
			i++;
		}
		gnuplot_write_xy_csv(fileName, adapt_array_1, adapt_array_2, iteration,"Temperature over time" );
} 
