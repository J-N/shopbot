
#include <createoi.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <set>
#include <iostream>
#define MAX(a,b)	(a > b? a : b)
#define MIN(a,b)	(a < b? a : b)
#define SONG_LENGTH 8


using namespace std;

/* Plays some random notes on the create's speakers. */
/*void horn() {
  char song[SONG_LENGTH * 2];
  int i;
  for (i =0; i< SONG_LENGTH; i++) {
    song[i*2] = 31 + floor(drand48() * (127-31));
    song[i*2 +1] = 8 + floor(drand48() * 10);
  }
  writeSong(0, SONG_LENGTH, song);
  playSong (0);
}
*/
void printSensors()
{
        int* sensors = getAllSensors();
        int i;

        for (i = 0; i < 36; i++)
        {
                printf ("Sensor %d:\t%d\n", i, sensors[i]);
        }
        free (sensors);
}



double calcMedian(vector<int> scores)
{
  double median;
  size_t size = scores.size();

  sort(scores.begin(), scores.end());

  if (size  % 2 == 0)
  {
      median = (scores[size / 2 - 1] + scores[size / 2]) / 2;
  }
  else 
  {
      median = scores[size / 2];
  }

  return median;
}

int calcMode(vector<int> vUserInput)
{
	vector<int> vOccurrence;
	vOccurrence.resize(vUserInput.size());
	set <int> occurSet;
	int mode;
	int ckOccur = 0;
	int maxOccur = 0;
	for( vector<int>::const_iterator iter = vUserInput.begin(); iter != vUserInput.end(); ++iter ) {
	//	sum = sum + *iter;//used for mean

		// vOccurrence will be used later to find the mode
		for( size_t i = 0; i < vUserInput.size(); ++ i) {
			if( *iter == vUserInput[i] )
				++vOccurrence[i];
		}
	}
	
	for( vector<int>::const_iterator iter = vOccurrence.begin(); iter != vOccurrence.end(); ++iter ) {
		if( *iter == 1 )//if the vector is filled with 1's it means each number has occurred only once, therefore no mode.
			ckOccur = ckOccur + 1;
		else if( *iter >maxOccur )
			maxOccur = *iter;
	}

	for( size_t i = 0; i < vOccurrence.size(); ++i) {
			if( vOccurrence[i] == maxOccur ) {
				mode = vUserInput[i];
				occurSet.insert(mode);
			}
	}
	return *occurSet.begin();
}



int main(int argv, char* argc[])
{
	int key;
	int not_done = 1;


	int speed = 0;//store user input.
	//int turn = 0;
	
	int velocity = 0; //values sent to the create
	int radius = 0;

	int charge;

	if (argv < 2) {
	  fprintf(stderr, "Usage: drive DEVICE (e.g. /dev/ttyUSB0)\n");
	  exit(1);
	}
	
	 startOI_MT (argc[1]);
	drive(0, 0);
        int  scans, ii;
	short leftFloor, leftTape, rightFloor, rightTape;
	scans = 20;
	printf("Begining Initilization\n");
	vector<int> left;
	vector<int> right;
	for (ii=0; ii<scans; ii++)
	{
	//int* sensors = getAllSensors();
	right.push_back(readSensor(SENSOR_CLIFF_FRONT_RIGHT_SIGNAL));
	left.push_back(readSensor(SENSOR_CLIFF_FRONT_LEFT_SIGNAL));
//	printf("front left cliff sensor %hu\n",(short)readSensor(SENSOR_CLIFF_FRONT_LEFT_SIGNAL));
//	printf("front right cliff sensor %hu\n",(short)readSensor(SENSOR_CLIFF_FRONT_RIGHT_SIGNAL));
	
	usleep(100000);
	}
	
	leftFloor = (short)calcMode(left);
	rightFloor = (short)calcMode(right);
	
	printf("Left: %hu Right: %hu\n",leftFloor,rightFloor);
	usleep(1000000);
	cout<<"Now put ontop of tape"<<endl;
	usleep(5000000);
	right.clear();
	left.clear();
	cout<<"Starting tape calibration"<<endl;
	for (ii=0; ii<20; ii++)
	{
	right.push_back(readSensor(SENSOR_CLIFF_FRONT_RIGHT_SIGNAL));
	left.push_back(readSensor(SENSOR_CLIFF_FRONT_LEFT_SIGNAL));
	
	usleep(100000);
	}
	leftTape = (short)calcMode(left);
	rightTape = (short)calcMode(right);
	printf("Left: %hu Right: %hu\n",leftTape,rightTape);
	cout<<"Remove tape calibration"<<endl;
	
	usleep(5000000);
	cout<<"Moving forward"<<endl;
	speed = 50;
	drive(3*speed,0);
	int stop=0;
	short rThresh = (rightFloor + rightTape) / 2;
	short lThresh = (leftFloor + leftTape) / 2;
	int counter=0;
	while(!stop)
	{
		counter++;
		usleep(100000);
		short r = readSensor(SENSOR_CLIFF_FRONT_RIGHT_SIGNAL);
		short l = readSensor(SENSOR_CLIFF_FRONT_LEFT_SIGNAL);
		//if((r <= rThresh) || (l <= lThresh)) //we have crossed the line
		//	stop=1;
		if(r <= rThresh) //we have crossed the line
			//directDrive(110,5);
			directDrive(speed,-speed);
		else if(l <= lThresh) //we have crossed the line
			directDrive(-speed,speed);
			//directDrive(5,110);
		else
			drive(3*speed,0);
		if (counter > 340)
		{
			turn (speed,-1,-180,0);
			//stop=1;
			counter=0;
		}
	}
	drive(0,0);
}
