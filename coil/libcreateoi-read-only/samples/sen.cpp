
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
#include <signal.h>
#include <pthread.h>

#define MAX(a,b)	(a > b? a : b)
#define MIN(a,b)	(a < b? a : b)
#define SONG_LENGTH 8
#define PI 3.14159265

#define homeEdge -1
#define botIntersection -2
#define topIntersection -3

using namespace std;

FILE* qrFD;
float x=0;
float y=0;
float heading=0;
short rThresh;
short lThresh;
bool scanning;
int currentDistance;

void updatePosition();

class line;
class item;
class POI;

class item
{
	public: 
	int id;
	line* pLine;
	int distance; // distance from POI 0 on lineA
	item(int id, line* pLine, int distance)
	{
		this->id=id;
		this->pLine=pLine;
		this->distance=distance;
	}
};

class line
{
	public:
	POI* poi0;
	POI* poi1;
	int distance;
	vector <item*> items;
	line(POI* poi0)
	{
		this->poi0 = poi0;
		//this->poi1 = poi1;
	}
};


class POI
{
	public:
	int id;
	//float x;
	//float y;
	//float heading;
	//float distance;
	line* connections[3];
	//POI(int id, float x, float y, float heading);
	//POI(int id, float distance);
	POI(int id);
	
};

POI::POI(int id)
{
	//this->id=id; this->x=x; this->y=y; this->heading=heading;
	this->id=id;
	// this->distance=distance;
}

vector<POI*> POIs;
POI* lastPOI;
line* currentLine;
/* Plays some random notes on the create's speakers. */
void foundItem() {
  char song[4];
  song[0]=36;
  song[1]=16;
  song[2]=72;
  song[3]=16;

 /* int i;
  for (i =0; i< SONG_LENGTH; i++) {
    song[i*2] = 31 + floor(drand48() * (127-31));
    song[i*2 +1] = 8 + floor(drand48() * 10);
  }
*/
  writeSong(0, 2, (byte*)song);
  playSong (0);
}


void myHandler(int var=0)
{
	cout<<"Caught quit signal"<<endl;
	drive(0,0); // stop the robot
	stopOI_MT (); // stop the connection
	exit(1);
}

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

void  *readQR( void *ptr)
{
	while(1)
	{
		char tmp[100];
		string result;
		if(fgets(tmp, 100, qrFD) != NULL)
		{
			result = tmp;
			int id = atoi(tmp);
			if(scanning) // if we are suppose to be reading codes
			{
				foundItem();
				item* newItem = new item(id, currentLine, currentDistance);
				currentLine->items.push_back(newItem);
				cout<<"QR decteded "<<result<<" Current distance: "<<currentDistance<<endl;
			}
		}
	}
	//return result;
}
void turnLeft()
{
	int current;
	int count=0;
	int rc=0;
	drive(50,1);
	while(1)
	{
		usleep(10000);
		current = getAngle();
		count += current;
		//cout<<"Turning.."<<endl;
		if(count >= 45)
		{
		short r = readSensor(SENSOR_CLIFF_FRONT_RIGHT_SIGNAL);
		short l = readSensor(SENSOR_CLIFF_FRONT_LEFT_SIGNAL);
		if(l <= lThresh)
			rc=1;
		if((r <= rThresh) && (rc==1))
		{
			//cout<<"line found"<<endl;
			drive(0,0);
			//cin>>lc;
			//drive(50,-1);
			rc=0;
			//lines++;
			break;
		}
		}
		if (count >= 90)
			break;

	}
}

void turnRight()
{
	int current;
	int count=0;
	int rc=0;
	drive(50,-1);
	while(1)
	{
		usleep(10000);
		current = getAngle();
		count += current;
		//cout<<"Turning.."<<endl;
		if(count <= -45)
		{
		short r = readSensor(SENSOR_CLIFF_FRONT_RIGHT_SIGNAL);
		short l = readSensor(SENSOR_CLIFF_FRONT_LEFT_SIGNAL);
		if(r <= rThresh)
			rc=1;
		if((l <= lThresh) && (rc==1))
		{
			//cout<<"line found"<<endl;
			drive(0,0);
			//cin>>lc;
			//drive(50,-1);
			rc=0;
			//lines++;
			break;
		}
		}
		if (count <= -90)
			break;

	}
}
void turnAround()
{
	int current;
	int count=0;
	int rc=0;
	drive(50,-1);
	while(1)
	{
		usleep(10000);
		current = getAngle();
		count += current;
		//cout<<"Turning.."<<endl;
		if(count <= -90)
		{
		short r = readSensor(SENSOR_CLIFF_FRONT_RIGHT_SIGNAL);
		short l = readSensor(SENSOR_CLIFF_FRONT_LEFT_SIGNAL);
		if(r <= rThresh)
			rc=1;
		if((l <= lThresh) && (rc==1))
		{
			//cout<<"line found"<<endl;
			drive(0,0);
			//cin>>lc;
			//drive(50,-1);
			rc=0;
			//lines++;
			break;
		}
		}
		if (count <= -180)
			break;

	}
}

void recordPos(int id)
{
	//short pos = readSensor(SENSOR_DISTANCE);
	//POI* newPOI = new POI(id,x,y,heading);
	POI* newPOI = new POI(id);
	POIs.push_back(newPOI);
}

void intersection(int path)
{
	driveDistance(50,0,90,1); //drive 30 mm forward
	//reset sensor
	getAngle();
	cout<<"got angle"<<endl;
	drive(50,-1);
	int count=0, current=0, rc=0, lc=0, lines=0;
	while(1)
	{
		usleep(10000);
		current = getAngle();
		count += current;
		cout<<"Turning.."<<endl;
		short r = readSensor(SENSOR_CLIFF_FRONT_RIGHT_SIGNAL);
		short l = readSensor(SENSOR_CLIFF_FRONT_LEFT_SIGNAL);
		if(r <= rThresh)
			rc=1;
		if((l <= lThresh) && (rc==1))
		{
			cout<<"line found"<<endl;
			drive(0,0);
			//cin>>lc;
			drive(50,-1);
			rc=0;
			lines++;
		}
		if (count <= -180)
			break;

	}
	drive(0,0);
	cout<<"done turning"<<endl;
	cout<<"lines: "<<lines<<endl;
	//cin>>lc;
	if(path) // we want to make a turn
	{
		if(lines==2)
			turn(50,1,120,0);
		else
			turn(50,-1,-180,0);
			
	}
	else//we want the straight
	{
		if(lines==2)
			turn(50,-1,-140,0);
		else
			turn(50,-1,-90,0);
	}		
}

void followLine()
{
	cout<<"Following line"<<endl;
	int speed = 30;
	drive(3*speed,0);
	int stop=0;
	int counter=0;
	while(!stop)
	{
		updatePosition();
		counter++;
		usleep(10000);
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

void updatePosition()
{
	int da=getAngle();
	heading+=da;
	int dr=getDistance();
	currentDistance += dr;
	x+=dr*cos(da*PI/180);
	y+=dr*sin(da*PI/180);
}

void myTurn(int a, int b, int c, int d)
{
	//save current position
	updatePosition();
	
	//now call the real turn
	turn(a,b,c,d);
}

void dPause(string message)
{
	cout <<message<<endl;
	//cout<<"x: "<<x<<" y: "<<y<<" heading: "<<heading<<endl;
	cout << "Distance: " << currentDistance << endl;
	string tmp;
	cin >> tmp;
}

void initalizeStore()
{
	//first we need to drive forward until we reach the the edge of the store
	followLine();
	//dPause("found homeEdge");
	//we are now at the edge of the store
	//we need to record our current posistion
	recordPos(homeEdge);
	lastPOI=POIs[0];
	//now we want to make a right turn
	//intersection(1);
	drive(50,0);
	usleep(3000000);
	drive(0,0);
	//myTurn(50,-1,-80,0);
	turnRight();
	currentLine =  new line(lastPOI);
	lastPOI->connections[1]=currentLine;
	//begin scanning items
	scanning=true;
	
	
	followLine();
	//dPause("found top");
	//we are at the top intersection
	//save our current position
	recordPos(topIntersection);
	lastPOI=POIs[1];
	lastPOI->connections[2] = currentLine;
	currentLine->poi1=POIs[1];
	currentLine->distance = currentDistance;
	//now we want to drive straight
	//intersection(0);
	myTurn(50,-1,-10,0);
	drive(50,0);
	usleep(3000000);
	currentDistance = 0; //reset distance count
	currentLine =  new line(lastPOI);
	lastPOI->connections[0]=currentLine;
	followLine();
	//dPause("found bot");
	drive(50,0);
	usleep(3000000);
	drive(0,0);
	//myTurn(50,1,70,0);
	turnLeft();
	//we are at the bottom intersection
	//save our current position
	recordPos(botIntersection);
	lastPOI=POIs[2];
	lastPOI->connections[2] = currentLine;
	currentLine->poi1=POIs[2];
	currentLine->distance = currentDistance;
	currentDistance =0;
	currentLine =  new line(lastPOI);
	lastPOI->connections[1]=currentLine;
	//now we want to turn left
	//intersection(1);
	followLine();
	currentLine->poi1=POIs[1];
	currentLine->distance = currentDistance;
	lastPOI=POIs[1];
	lastPOI->connections[1]=currentLine;
	//dPause("back at top");
	//we are back at the top intersection
	//we want to spin 180 and go back down to scan other side of aisle
	//myTurn(50,-1,-180,0);
	turnAround();
	followLine();
	lastPOI=POIs[2];
	currentLine = new line(lastPOI);
	lastPOI->connections[0]=currentLine;
	//dPause("back at bot");
	//we are back at the bottom intersection
	//we want to make a left turn
	drive(50,0);
	usleep(3000000);
	drive(0,0);
	myTurn(50,1,90,0);
	currentDistance =0;
	//intersection(1);
	followLine();
	//dPause("back at homeEdge");
	currentLine->distance = currentDistance;
	lastPOI=POIs[0];
	currentLine->poi1=POIs[0];
	currentDistance=0;
	//we are back at the homeEdge
	//we want to disable scanning
	scanning=false;
	//we want to drive straight
	//intersection(0);
	myTurn(50,-1,-10,0);
	drive(50,0);
	usleep(2000000);
	drive(0,0);
	followLine();
	//dPause("back at home position");
	//we are now in the home posistion
	//we want to spin 180
	//myTurn(50,-1,-180,0);
	turnAround();
	//dPause("done");
	//we are done!
	foundItem();

}

void setup()
{
	signal(SIGINT, myHandler);
	signal(SIGQUIT, myHandler);
	signal(SIGABRT, myHandler);
	signal(SIGTERM, myHandler);
	
	mkfifo("/dev/QRComms",777);
	mkfifo("/dev/pathComms",777);
	cout<<"Waiting for QR communication"<<endl;	
	qrFD = fopen("/dev/QRComms","r");
	cout<<"QR communication enabled"<<endl;
	//make QR thread
	pthread_t qrThread;
	int qrRet;
	qrRet = pthread_create(&qrThread, NULL, readQR, NULL);
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
	//usleep(1000000);
	cout<<"Now put ontop of tape"<<endl;
	int tmp;
	cin>>tmp;
	//usleep(5000000);
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
  	myfile << rThresh<<endl<<lThresh<<endl;
  	myfile.close();	
}

bool constructPath (line* checkLine,  int item)
{
	if (find(checkLine->items.begin(), checkLine->items.end(), item) != checkLine->items.end())
	{
		//found line containing item
		return true;
	}
	else
		return false;
}


void getItem(int item)
{


}

int main(int argv, char* argc[])
{
	if (argv < 2) {
	  fprintf(stderr, "Usage: drive DEVICE (e.g. /dev/ttyUSB0)\n");
	  exit(1);
	}
	setup();
	startOI_MT (argc[1]);
	drive(0, 0);
	string res;
	while(1)
	{
		cout<<"Would you like to callibrate the floor? (y/n)"<<endl;
		
		cin >> res;
		if(res == "y")
		{
			calibrateFloor();
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
		cout<<"Ready to begin"<<endl;
		cin>> res;
	initalizeStore();
	getItem(22);
	drive(0,0);
}
