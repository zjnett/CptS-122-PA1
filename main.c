/****************************************************************************************
* Programmer: Zach Nett                                                                 *
* Class: CptS 122; Lab Section 5                                                        *
* Programming Assignment: PA1                                                           *
* Date: 1/24/19                                                                         *
* File: main.c																	     	*
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

/************************************************************************
* NOTE ON EXPECTED FILE STRUCTURE:										*
* Patient,minute,calories,distance,floors,heart,steps,sleep_level		*
* > First line details patient IDs										*
* > Second line shows the corresponding categories as shown above		*
* > Third line until EOF is corresponding data in format of second line *
************************************************************************/

int main(void) {
	FitbitData data[1500] = { "", "", 0.0, 0.0, 0, 0, 0, 0 };
	char targetPatient[100];
	int skips = 0;
	// Opens file "FitbitData.csv" for mode read
	// Opens file "Results.csv" for mode write
	FILE *infile = NULL;
	FILE *outfile = NULL;
	infile = fopen("FitbitData.csv", "r");
	outfile = fopen("Results.csv", "w");

	// If file is opened successfully
	if (infile != NULL) {
		findTargetPatient(infile, targetPatient);
		processHeader(infile);
		int i = 0;
		for (; i < 1440; i++) { //ignoring duplicates, there are only 1440 minutes in a day, and thus iterations should span from 0-1439
			if (readLine(targetPatient, data, infile, i) == 0) {
				i--; //don't iterate i, but line should iterate (move past invalid case)
				skips++; //add 1 to skips so line count is consistent for debugging
			} else {
				//Debug console line print message.
				//printPatientLine(data, i, skips);
			}
		}
		//Following this for loop, we're assuming all data is now contained in the struct.
		//Start data analysis:
		double caloriesBurned = calculateCaloriesBurned(data, i);
		double distanceWalked = calculateDistanceWalked(data, i);
		unsigned int floorsWalked = calculateFloorsWalked(data, i);
		unsigned int stepsTaken = calculateStepsTaken(data, i);
		double averageHeartrate = calculateAverageHeartrate(data, i);
		int maxStepsInMinute = calculateMaxStepsInMinute(data, i);
		char startBadSleep[15] = { '\0' };
		char endBadSleep[15] = { '\0' };
		findLongestRangePoorSleep(data, i, startBadSleep, endBadSleep);

		//End data analysis, print results to screen/file:
		for (int i = 0; i < 1440; i++) {
			printf("%s,", data[i].patient);
			printf("%s,", data[i].minute);
			printf("%lf,", data[i].calories);
			printf("%lf,", data[i].distance);
			printf("%d,", data[i].floors);
			printf("%d,", data[i].heartRate);
			printf("%d,", data[i].steps);
			printf("%d\n", data[i].sleepLevel);
		}

		printf("Total calories burned: %lf\n", caloriesBurned);
		printf("Distance walked (mi): %lf\n", distanceWalked);
		printf("Floors walked: %u\n", floorsWalked);
		printf("Steps taken: %u\n", stepsTaken);
		printf("Average heartrate: %lf\n", averageHeartrate);
		printf("Max Steps in Minute: %d\n", maxStepsInMinute);
		printf("Duration of Worst Sleep: %s:%s\n", startBadSleep, endBadSleep);

		if (outfile != NULL) {
			printResults(outfile, data, i, caloriesBurned, distanceWalked, floorsWalked, stepsTaken, averageHeartrate, maxStepsInMinute, startBadSleep, endBadSleep);
			printf("Output written to Results.csv!\n");
		} else {
			printf("ERROR: Output file unable to be generated. Do you have enough hard disk space?\n");
		}

	} else {
		printf("ERROR: File unable to be read.\n");
	}

	//Closes input and output files FitbitData.csv and Results.csv respectively.
	fclose(infile);
	fclose(outfile);

	//End of main().
	return 0;
}