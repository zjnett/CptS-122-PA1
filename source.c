/****************************************************************************************
* Programmer: Zach Nett                                                                 *
* Class: CptS 122; Lab Section 5                                                        *
* Programming Assignment: PA1                                                           *
* Date: 1/24/19                                                                         *
* File: source.c																		*
*                                                                                       *
* Description: This program cleanses and analyzes 24 hours worth of Fitbit              *
*              data read in from a file.                                                *
*                                                                                       *
* Relevant Formulas: Refer to each function definition.                                 *
*                                                                                       *
* Format of record in input file (FitbitData.csv):									    *
*			Target: ,12cx7,,,,,,													    *
*			Patient,minute,calories,distance,floors,heart,steps,sleep_level			    *
*			12cx7,0:00:00,0.968900025,0,0,63,0,1									    *
*			...																		    *
*																					    *
* Format of output file (Results.csv):												    *
* Total Calories,Total Distance,Total Floors,Total Steps,Avg Heartrate,Max Steps,Sleep  *
* 2294.936577,6.624171,31,15415,70.710081,120,23:50:00:23:56:00						    *
* 12cx7,0:00:00,0.968900,0.000000,0,63,0,1											    *
* ...																				    *
****************************************************************************************/

#include "source.h"

/***************************************************************
* Function: findTargetPatient()                                *
* Date Created: 1/21/19                                        *
* Date Last Modified: 1/21/19                                  *
* Description: Processes file header and finds target patient. *
* Input parameters: An input file pointer, target string.      *
* Returns: Indirect string through character pointer.          *
* Preconditions: infile !=NULL, target is mutable, first       *
*				 string in file can be ignored.			       *
* Postconditions: A copy of the target ID string is returned   *
*				  indirectly through the character pointer.    *
***************************************************************/

void findTargetPatient(FILE *infile, char *target) {
	char buffer[100] = { '\0' };
	//Ignore first string, essentially a header
	fscanf(infile, "%s", buffer);
	//Read next string into buffer (contains target)
	fscanf(infile, "%s", buffer);
	//Use comma delimiter to reveal first target, assign to mutable char *target
	strcpy(target, strtok(buffer, ","));
	
}

/****************************************************************
* Function: processHeader()                                     *
* Date Created: 1/21/19                                         *
* Date Last Modified: 1/21/19                                   *
* Description: Reads header string/line into discarded buffer.  *
* Input parameters: An input file pointer.					    *
* Returns: Nothing.                                             *
* Preconditions: infile != NULL, findTargetPatient called	    *
*				 successfully.								    *
* Postconditions: fscanf static position pointer has moved past *
*				  the header in the input file.				    *
****************************************************************/

void processHeader(FILE *infile) {
	char buffer[100] = { '\0' };
	fscanf(infile, "%s", buffer);
}

/********************************************************************************
* Function: readLine()															*
* Date Created: 1/21/19															*
* Date Last Modified: 1/23/19													*
* Description: Reads in a line of data from input file, actively cleanses it,	*
*			   separates values from comma delimiters and copies it into		*
*			   an array of FitbitData structures.								*
*			   Note that data is ignored if it is not the target patient,		*
*			   invalid string are filled with '\0', doubles with -1.0, integers *
*			   with -1, and enumertion with 0.									*
* Input parameters: Immutable string target patient, array of FitbitData		*
*					structures, input file pointer, iteration tracking integer. *
* Returns: An integer (line has been read in successfully or not.)				*
* Preconditions: infile != NULL, findTargetPatient() and processHeader() have   *
*				 been successfully called. Internal helper functions exist.		*
* Postconditions: Line from file has either been successfully read in or not.   *
********************************************************************************/

int readLine(const char *targetPatient, FitbitData data[1500], FILE *infile, int iteration) {
	//Note that per line, strtok has to be called 8 times -- once for every piece of data
	char line[100] = { '\0' };
	fscanf(infile, "%s", line);
	int badCommaIndex = findBadComma(line);
	int ends = endsWithComma(line);
	char id[100] = { '\0' };
	char minute[25] = { '\0' };
	strcpy(id, strtok(line, ","));
	if (strcmp(targetPatient, id) == 0) { //line pertains to targetPatient
		//Patient, minute, calories, distance, floors, heart, steps, sleep_level
		//Check if minute data has been seen in the previous case (duplicate line)
		strcpy(minute, strtok(NULL, ","));
		if (iteration > 0) {
			if (strcmp(minute, data[iteration - 1].minute) == 0) {
				return 0;
			}
		}*
		//Patient ID structure fulfillment
		strcpy(data[iteration].patient, id);
		//Minute structure fulfillment
		if (badCommaIndex == 1) {
			strcpy(data[iteration].minute, "\0");
		} else {
			strcpy(data[iteration].minute, minute);
		}
		//Calories structure fulfillment
		if (badCommaIndex == 2) {
			data[iteration].calories = -1.0;
		} else {
			double calories = 0.0;
			sscanf(strtok(NULL, ","), "%lf", &calories);
			data[iteration].calories = calories;
		}
		//Distance structure fulfillment
		if (badCommaIndex == 3) {
			data[iteration].distance = -1.0;
		} else {
			double distance = 0.0;
			sscanf(strtok(NULL, ","), "%lf", &distance);
			data[iteration].distance = distance;
		}
		//Floors structure fulfillment
		if (badCommaIndex == 4) {
			data[iteration].floors = -1;
		} else {
			unsigned int floors = 0;
			floors = atoi(strtok(NULL, ","));
			data[iteration].floors = floors;
		}
		//Heart structure fulfillment
		if (badCommaIndex == 5) {
			data[iteration].heartRate = -1;
		} else {
			unsigned int heart = 0;
			heart = atoi(strtok(NULL, ","));
			data[iteration].heartRate = heart;
		}
		//Steps structure fulfillment
		if (badCommaIndex == 6) {
			//bad comma = element # + 1
			data[iteration].steps = -1;
		} else {
			unsigned int steps = 0;
			steps = atoi(strtok(NULL, ","));
			data[iteration].steps = steps;
		}
		//Sleep level structure fulfillment
		if (ends || badCommaIndex == 7) {
			data[iteration].sleepLevel = 0;
		} else {
			Sleep sleepLevel = NONE;
			sleepLevel = atoi(strtok(NULL, ","));
			data[iteration].sleepLevel = sleepLevel;
		}
		return 1;
	}
	return 0;
}

/*****************************************************************************************
* Function: printPatientLine															 *
* Date Created: 1/21/19																	 *
* Date Last Modified: 1/22/19															 *
* Description: Debug console print function to be run in conjunction with readLine().	 *
* Input parameters: An array of FitbitData structures, tracking integer iteration,       *
* Returns: Nothing.																		 *
* Preconditions: Data at index iteration in structure array has been processed with		 *
*				 readLine().														     *
* Postconditions: Patient information for corresponding iteration is printed to console. *
*****************************************************************************************/

void printPatientLine(FitbitData data[1500], int iteration, int skips) {
	printf("ITERATION %d, LINE %d\n", iteration, iteration + 3 + skips);
	printf("Patient ID: %s\n", data[iteration].patient);
	printf("Minute: %s\n", data[iteration].minute);
	printf("Calories: %lf\n", data[iteration].calories);
	printf("Distance: %lf\n", data[iteration].distance);
	printf("Floors: %u\n", data[iteration].floors);
	printf("Heartrate: %u\n", data[iteration].heartRate);
	printf("Steps: %u\n", data[iteration].steps);
	printf("Sleep Level: %d\n", data[iteration].sleepLevel);
}

/*************************************************************************************
* Function: findBadComma()															 *
* Date Created: 1/23/19																 *
* Date Last Modified: 1/23/19														 *
* Description: Processes line of input for a 'bad comma,' or two commas next to each *
*			   other, suggesting a missing data field.								 *
* Input parameters: A line string.													 *
* Returns: Integer index (counting from 1) of bad comma, or 0 for none present.      *
* Preconditions: Helper function for readLine().									 *
* Postconditions: An integer index is returned indicating the data field the bad	 *
*				  comma impacts, or returns 0 for no present bad comma.				 *
*************************************************************************************/

int findBadComma(char *line) {
	//if line does not contain ",,", return 0
	if (strstr(line, ",,") == NULL) {
		return 0;
	}
	int counter = 0;
	while (*line != '\0') {
		if (*line == ',' && *(line + 1) == ',')
			return ++counter;
		else if (*line == ',' && *(line + 1) != ',')
			counter++;
		line++;
	}
}

/*****************************************************************************************
* Function: endsWithComma()																 *
* Date Created: 1/23/19																	 *
* Date Last Modified: 1/23/19															 *
* Description: Checks if current line ends with a comma (indicating missing sleep data). *
* Input parameters: A line string.														 *
* Returns: Boolean integer 1 if line ends with a comma, 0 if not.                        *
* Preconditions: Helper function for readLine().                                         *
* Postconditions: Boolean integer is returned.										     *
*****************************************************************************************/

int endsWithComma(char *line) {
	while (*line != '\0') {
		if (*line == ',' && *(line + 1) == '\0')
			return 1;
		line++;
	}
	return 0;
}

/******************************************************************************************
* Function: calculateCaloriesBurned()													  *
* Date Created: 1/23/19																	  *
* Date Last Modified: 1/23/19															  *
* Description: Runs through Fitbit data and sums all valid calorie values.                *
* Input parameters: An array of FitbitData structures, integer iterations (ideally 1440). *
* Returns: A double-precision value sum of calories burned.                               *
* Preconditions: FitbitData structure array has been filled with valid data, iterations   *
*				 variable accurately represents indices of data in aforementioned		  *
*				 structure.																  *
* Postconditions: The total sum of valid calorie values has been returned.                *
******************************************************************************************/

double calculateCaloriesBurned(FitbitData data[1500], int iterations) {
	double total = 0.0;
	int i = 0;
	while (i < iterations) {
		if (data[i].calories >= 0.0) {
			total += data[i].calories;
		}
		i++;
	}
	return total;
}

/****************************************************************************************
* Function: calculateDistanceWalked()													*
* Date Created: 1/23/19																	*
* Date Last Modified: 1/23/19															*
* Description: Runs through Fitbit data and sums all valid distance values.             *
* Input parameters: Array of FitbitData structures, integer iterations (ideally 1440).	*
* Returns: A double-precision value sum of distance walked in miles.                    *     
* Preconditions: FitbitData structure array has been filled with valid data, iterations *
*				 variable accurately represents indices of data in aforementioned		*
*				 structure.																*
* Postconditions: The total sum of valid distance values has been returned.             *                           
****************************************************************************************/

double calculateDistanceWalked(FitbitData data[1500], int iterations) {
	double total = 0.0;
	int i = 0;
	while (i < iterations) {
		if (data[i].distance >= 0.0) {
			total += data[i].distance;
		}
		i++;
	}
	return total;
}

/****************************************************************************************
* Function: calculateFloorsWalked()														*
* Date Created: 1/23/19																	*
* Date Last Modified: 1/23/19															*
* Description: Runs through Fitbit data and sums all valid floor values.                *
* Input parameters: Array of FitbitData structures, integer iterations (ideally 1440).	*
* Returns: An integer value sum of floors walked.                                       *
* Preconditions: FitbitData structure array has been filled with valid data, iterations *
*				 variable accurately represents indices of data in aforementioned		*
*				 structure.																*
* Postconditions: The total sum of valid floor values has been returned.                * 
****************************************************************************************/

int calculateFloorsWalked(FitbitData data[1500], int iterations) {
	int total = 0, i = 0;
	while (i < iterations) {
		if (data[i].floors != 4294967295) { //maximum unsigned integer
			total += data[i].floors;
		}
		i++;
	}
	return total;
}

/****************************************************************************************
* Function: calculateStepsTaken()														*
* Date Created: 1/23/19																	*
* Date Last Modified: 1/23/19														    *
* Description: Runs through Fitbit data and sums all valid step values.                 *
* Input parameters: Array of FitbitData structures, integer iterations (ideally 1440).	*
* Returns: An integer value sum of steps taken.                                         *
* Preconditions: FitbitData structure array has been filled with valid data, iterations *
*				 variable accurately represents indices of data in aforementioned		*
*				 structure.																*
* Postconditions: The total sum of valid step values has been returned.                 *
****************************************************************************************/

int calculateStepsTaken(FitbitData data[1500], int iterations) {
	int total = 0, i = 0;
	while (i < iterations) {
		if (data[i].steps != 4294967295) {
			total += data[i].steps;
		}
		i++;
	}
	return total;
}

/****************************************************************************************
* Function: calculateAverageHeartRate()													*
* Date Created: 1/23/19																	*
* Date Last Modified: 1/23/19															*
* Description: Runs through all Fitbit data and sums all valid heartbeat data, then		*
*			   calculates its average by dividing it by the count of valid data.		*
* Input parameters: Array of FitbitData structures, integer iterations (ideally 1440).	*
* Returns: A double-precision value of the average heartrate in a 24 hour period.		*
* Preconditions: FitbitData structure array has been filled with valid data, iterations *
*				 variable accurately represents indices of data in aforementioned		*
*				 structure.																*
* Postconditions: The average heartrate double-precision value has been returned.       *
****************************************************************************************/

double calculateAverageHeartrate(FitbitData data[1500], int iterations) {
	double average = 0.0;
	int numValid = 0, i = 0;
	while (i < iterations) {
		if (data[i].heartRate != 4294967295) {
			average += data[i].heartRate;
			numValid++;
		}
		i++;
	}
	average = average / numValid;
	return average;
}

/****************************************************************************************
* Function: calculateMaxStepsInMinute()													*
* Date Created: 1/23/19																	*
* Date Last Modified: 1/23/19															*
* Description: Runs through Fitbit data and finds the maximum valid amount of steps.    *
* Input parameters: Array of FitbitData structures, integer iterations (ideally 1440).	*
* Returns: An integer value of the maximum amount of steps in one minute.               *
* Preconditions: FitbitData structure array has been filled with valid data, iterations *
*				 variable accurately represents indices of data in aforementioned		*
*				 structure.																*
* Postconditions: The maximum amount of steps in one minute has been returned.          *
****************************************************************************************/

int calculateMaxStepsInMinute(FitbitData data[1500], int iterations) {
	int max = data[0].steps;
	int i = 0;
	while (i < iterations) {
		if (data[i].steps > max) {
			max = data[i].steps;
		}
		i++;
	}
	return max;
}


/****************************************************************************************************************
* Function: findLongestRangePoorSleep()																			*
* Date Created: 1/23/19																							*
* Date Last Modified: 1/24/19																					*
* Description: Finds the longest consecutive range of poor sleep based on the sum of sleep levels in the range. *
*			   See problem description for more information.													*
* Input parameters: Array of FitbitData structures, integer iterations, mutable strings startTime and endTime.  *
* Returns: Starting and ending minute string values indirectly through startTime and endTime pointers.			*
* Preconditions: FitbitData structure array has been filled with valid data, iterations							*
*				 variable accurately represents indices of data in aforementioned								*
*				 structure, startTime and endTime strings are mutable.											*
* Postconditions: Starting and ending minute string values have been returned indirectly.                       *
****************************************************************************************************************/

void findLongestRangePoorSleep(FitbitData data[1500], int iterations, char *startTime, char *endTime) {
	int streak = 1, j = 0;
	int sum[100] = { 0 };
	char start[50][15] = { { '\0' } };
	char end[50][15] = { { '\0' } };
	for (int i = 0; i < iterations; i++) {
		if (data[i].sleepLevel > 1) {
			if (streak) {
				//continuation of streak
				sum[j] += data[i].sleepLevel;
			} else {
				//start of new streak
				streak = 1;
				strcpy(start[j], data[i].minute);
				sum[j] += data[i].sleepLevel;
			}
		} else if (data[i-1].sleepLevel > 1) {
			//end of streak
			strcpy(end[j], data[i-1].minute);
			streak = 0;
			j++;
		}
	}
	//Find max of sums
	int max = sum[0];
	j = 0;
	while (sum[j] != 0) {
		if (sum[j] > max)
			max = sum[j];
		j++;
	}
	//linear search for index of max
	j = 0;
	while (sum[j] != 0) {
		if (sum[j] == max)
			break;
		j++;
	}
	strcpy(startTime, start[j]);
	strcpy(endTime, end[j]);
}

/******************************************************************************************************
* Function: printResults()																			  *
* Date Created: 1/23/19																				  *
* Date Last Modified: 1/24/19																		  *
* Description: Prints all calculated and cleansed data to specified output file according to		  *
*			   specification (see problem description).												  *
* Input parameters: File pointer to output file, array of FitbitData structures, integer iterations,  *
*					total number of calories, distance (doubles), total number of floors, steps		  *
*					(integers), average heartbeat (double), maximum steps in one minute (integer),    *
*					starting and end minute/time values for worst consecutive sleep (strings).		  *
* Returns: Nothing.																					  *
* Preconditions: Output file has been opened for mode write. All analysis functions ran successfully. *
* Postconditions: Information has been successfully printed to output file.                           *
******************************************************************************************************/

void printResults(FILE *outfile, FitbitData data[1500], int iterations, double totalCalories, double totalDistance, unsigned int totalFloors, unsigned int totalSteps, double avgHeartrate, int maxSteps, char *startBadSleep, char *endBadSleep) {
	fprintf(outfile, "Total Calories,Total Distance,Total Floors,Total Steps,Avg Heartrate,Max Steps,Sleep\n");
	fprintf(outfile, "%lf,%lf,%u,%u,%lf,%d,%s:%s\n", totalCalories, totalDistance, totalFloors, totalSteps, avgHeartrate, maxSteps, startBadSleep, endBadSleep);
	for (int i = 0; i < iterations; i++) {
		fprintf(outfile, "%s,", data[i].patient);
		fprintf(outfile, "%s,", data[i].minute);
		fprintf(outfile, "%lf,", data[i].calories);
		fprintf(outfile, "%lf,", data[i].distance);
		fprintf(outfile, "%d,", data[i].floors);
		fprintf(outfile, "%d,", data[i].heartRate);
		fprintf(outfile, "%d,", data[i].steps);
		fprintf(outfile, "%d\n", data[i].sleepLevel);
	}
}