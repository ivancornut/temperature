#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "gnuplot_i.h"
#include <time.h>
// function to create csv readable by gnuplot using gnuplot_i library
void create_csv(double *big_array_1, double *big_array_2, int iteration, char * fileName);

void RemoveSpaces(char* source);

int main(int argc, char *argv[])
{
// file to read temperature
	FILE *read_file ;
// file to write current temperature for html page
	FILE *write_file ;


	double nb_of_seconds;
	time_t begin_time;
	time_t current_time;
	begin_time = time(NULL);

	time_t stamp_time;
	time(&stamp_time);	
	char fileName[100];
	char *buffer; 
	buffer = ctime(&stamp_time);
	RemoveSpaces(buffer);
	strcat(fileName,"temp_");
	strcat(fileName,buffer);
	strcat(fileName,".csv");

	int iterations = atoi(argv[1]);
	int sleep_time = atoi(argv[2]);
	char str1[100], str2[100], str3[100];
	
	double *temperature_storage = (double*) malloc(iterations * sizeof(double));
	memset(temperature_storage, 0, iterations * sizeof(double));

	double *time_storage = (double*) malloc(iterations * sizeof(double));
	memset(time_storage, 0, iterations * sizeof(double));
	int i = 0;

// number of iterations it will be running
	while (i < iterations)
	{

// opens the files "r" is for read and "w" is for write 
 		read_file=fopen("/sys/bus/w1/devices/28-000006744fe0/w1_slave", "r") ;
		write_file=fopen("/home/pi/public_html/temp.txt", "w") ;

// if the file containing data is empty
		if(read_file == NULL) 
		{
			perror("Error opening file");
     			return(-1);
   		}

// if the file containing data is filled 	
		if( fgets(str1,60,read_file) != NULL && fgets(str2,60,read_file) != NULL)
		{
			printf("%s  %s \n",str1, str2);
			str3[0] = str2[29];
			str3[1] = str2[30];
			str3[2] = '.';
			str3[3] = str2[31];
			str3[4] = str2[32];
			printf("%f \n", strtod(str3, NULL));			
			printf("Current temperature is : %s °C \n", str3);
			fprintf(write_file, "Current temperature is : %s \\°C \n", str3);
			temperature_storage[i] = strtod(str3, NULL);
			
			current_time = time(NULL);
			nb_of_seconds = (double) (current_time - begin_time);
			time_storage[i] = nb_of_seconds / 60; 
			
			create_csv(time_storage, temperature_storage, i, fileName);
		}

// close the files 
		fclose(read_file) ;
		fclose(write_file) ;

// wait 300 seconds for next iteration 
		sleep(sleep_time);		
		i++;
	}

	return 0 ;
}

void create_csv(double *big_array_1, double *big_array_2, int iteration, char * fileName)
{

	double adapt_array_1[iteration];
	double adapt_array_2[iteration];
	
	gnuplot_ctrl * h1;
	h1 = gnuplot_init();
	gnuplot_cmd(h1, "set terminal gif size 800,600 ");
	gnuplot_set_xlabel(h1,"t (in minutes)");
	gnuplot_set_ylabel(h1, "temp (in °C)");
	gnuplot_cmd(h1, "set output \'/home/pi/public_html/output_temp.gif\'");
	
	int i = 0;
	
	while ( i < iteration)
		{
			adapt_array_1[i] = big_array_1[i];
			adapt_array_2[i] = big_array_2[i];
			i++;
		}

	gnuplot_plot_xy(h1, adapt_array_1, adapt_array_2, iteration, "Temperature over time");
	gnuplot_close(h1);
	
	gnuplot_write_xy_csv(fileName, adapt_array_1, adapt_array_2, iteration,"Temperature over time" );
} 

void RemoveSpaces(char* source)
{
  char* i = source;
  char* j = source;
  while(*j != 0)
  {
    *i = *j++;
    if(*i != ' ' && *i != ':' && *i != '?' && *i != '\n')
      i++;
  }
  *i = 0;
}