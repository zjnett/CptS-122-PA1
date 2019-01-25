/****************************************************************************************
* Programmer: Zach Nett                                                                 *
* Class: CptS 122; Lab Section 5                                                        *
* Programming Assignment: PA1                                                           *
* Date: 1/24/19                                                                         *
* File: source.h																		*
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

#ifndef SOURCE_H
#define SOURCE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***************************************************************************************************
* Provided enumeration and structure in documentation by Andrew O'Fallon; see problem description. *
***************************************************************************************************/
typedef enum sleep {
	NONE = 0, ASLEEP = 1, AWAKE = 2, REALLYAWAKE = 3
} Sleep;

typedef struct fitbit {
	char patient[10];
	char minute[9];
	double calories;
	double distance;
	unsigned int floors;
	unsigned int heartRate;
	unsigned int steps;
	Sleep sleepLevel;
} FitbitData;

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

void findTargetPatient(FILE *infile, char *target);

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

void processHeader(FILE *infile);

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

int readLine(const char *targetPatient, FitbitData data[1500], FILE *infile, int iteration);

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

void printPatientLine(FitbitData data[1500], int iteration, int skips);

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

int findBadComma(char *line);

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

int endsWithComma(char *line);

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

double calculateCaloriesBurned(FitbitData data[1500], int iterations);

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

double calculateDistanceWalked(FitbitData data[1500], int iterations);

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

int calculateFloorsWalked(FitbitData data[1500], int iterations);

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

int calculateStepsTaken(FitbitData data[1500], int iterations);

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

double calculateAverageHeartrate(FitbitData data[1500], int iterations);

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

int calculateMaxStepsInMinute(FitbitData data[1500], int iterations);

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

void findLongestRangePoorSleep(FitbitData data[1500], int iterations, char *startTime, char *endTime);

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

void printResults(FILE *outfile, FitbitData data[1500], int iterations, double totalCalories, double totalDistance, unsigned int totalFloors, unsigned int totalSteps, double avgHeartrate, int maxSteps, char *startBadSleep, char *endBadSleep);

#endif