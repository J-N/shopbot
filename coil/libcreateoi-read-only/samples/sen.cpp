
#include <createoi.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <set>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <iostream>
#include <fstream>
#define MAX(a,b)	(a > b? a : b)
#define MIN(a,b)	(a < b? a : b)
#define SONG_LENGTH 8




using namespace std;

FILE* qrFD;
int x;
int y;
int heading;
short rThresh;
short lThresh;
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

string  readQR()
{
	char tmp[100];
	string result;
	if(fgets(tmp, 100, qrFD) != NULL)
	{
		result = tmp;
	}
	return result;
}

void recordPos(string qr)
{
	short pos = readSensor(SENSOR_DISTANCE);
	
}

void followLine()
{
	cout<<"Following line"<<endl;
	speed = 50;
	drive(3*speed,0);
	int stop=0;
	int counter=0;
	while(!stop)
	{
		counter++;
		usleep(100000);
		short r = readSensor(SENSOR_CLIFF_FRONT_RIGHT_SIGNAL);
		short l = readSensor(SENSOR_CLIFF_FRONT_LEFT_SIGNAL);
		if ((r <= rThresh) && (l <= lThresh)) //we are at an intersection
		{
			drive(0,0);
			stop=1;
		}
		else if(r <= rThresh) //we have crossed the line
			directDrive(speed,-speed);
		else if(l <= lThresh) //we have crossed the line
			directDrive(-speed,speed);
		else //drive straight
			drive(3*speed,0);
	}
}


void setup()
{
	mkfifo("/dev/QRComms");
	mkfifo("/dev/pathComms");
	
	qrFD = fopen("/dev/QRComms","r");
}
void calibrateFloor()
{
	int key;
	int not_done = 1;
	int speed = 0;//store user input.
	//int turn = 0;
	int velocity = 0; //values sent to the create
	int radius = 0;
	int charge;

        int  scans, ii;
	short leftFloor, leftTape, rightFloor, rightTape;
	scans = 20;
	printf("Begining Floor Calibration\n");
	vector<int> left;
	vector<int> right;
	for (ii=0; ii<scans; ii++)
	{
		right.push_back(readSensor(SENSOR_CLIFF_FRONT_RIGHT_SIGNAL));
		left.push_back(readSensor(SENSOR_CLIFF_FRONT_LEFT_SIGNAL));	
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
	rThresh = (rightFloor + rightTape) / 2;
	lThresh = (leftFloor + leftTape) / 2;
	
	ofstream myfile;
 	myfile.open ("calibration.txt");
  	myfile << rThresh<<endl<<lThresh<<endl
  	myfile.close();	
}
int main(int argv, char* argc[])
{
	if (argv < 2) {
	  fprintf(stderr, "Usage: drive DEVICE (e.g. /dev/ttyUSB0)\n");
	  exit(1);
	}
	startOI_MT (argc[1]);
	drive(0, 0);
	setup();
	while(1)
	{
		cout<<"Would you like to callibrate the floor? (y/n)"<<endl;
		string res;
		cin >> res;
		if(res == "y")
		{
			callibrateFloor();
			break;
		}
		else if(res == "n")
		{
			ifstream myfile ("calibration.txt");
			string line;
			getline (myfile,line);
			rThresh = atoi(line.c_str());
			getline (myfile,line);
			lThresh = atoi(line.c_str());
			break;
		}
	}
	cout << "r: "<<rThresh<<" l: "<<lThresh<<endl;
	//followLine();
		/*string qr = readQR();
		if (qr.size() != 0)
		{
			if (qr == "D") // we have detected a QR code
			{
				drive(0,0); // stop so we can analyze it
				
			}
			else // we have determined what number it is
			{
				recordPos(qr);
			}
		}*/
	drive(0,0);
}
