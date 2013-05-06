/** @file */
#include "navigation.h"



std::vector<node*> nodes;
node* lastNode;
line* currentLine;
std::queue<mapObject*> q;
pthread_t qrThread, pathThread, sonarThread;
int currentItem = 0;
bool obstruction = false;
bool orientation=true;
bool darklines=false;
#define THROP >= //<= for darklines, >= for lightlines
/**
* Plays notes to signify an action
*/
void notify(int type) {

	char song[100];
	int notes=0;
	switch(type)
	{
		case NEWITEM:
  			song[0]=36;
  			song[1]=16;
			notes=1;
		break;
		case FOUNDITEM:
  			song[0]=36;
  			song[1]=16;
			song[2]=72;
  			song[3]=16;
			notes=2;
		break;
		case NOTFOUND:
  			song[0]=80;
  			song[1]=64;
			notes=1;
			break;
		case OBSTACLE:
  			song[0]=22;
  			song[1]=64;
			notes=1;
		default:
		break;
	}

  writeSong(0, notes, (byte*)song);
  playSong (0);
}


/**
* Catches a sigterm signal and attempts to gracefully stop the robot
* @param var the error signal
*/
void myHandler(int var=0)
{
	std::cout<<"Caught quit signal"<<std::endl;
	drive(0,0); // stop the robot
	stopOI_MT (); // stop the connection
	exit(1);
}

/**
* Prints out all of the sensor values
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

/**
* Calculates the median of a vector of ints
* @param scores vector of ints
* @return a double which is the median value
*/
double calcMedian(std::vector<int> scores)
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

int calcMode(std::vector<int> vUserInput)
{
	std::vector<int> vOccurrence;
	vOccurrence.resize(vUserInput.size());
	std::set <int> occurSet;
	int mode;
	int ckOccur = 0;
	int maxOccur = 0;
	for( std::vector<int>::const_iterator iter = vUserInput.begin(); iter != vUserInput.end(); ++iter ) {
	//	sum = sum + *iter;//used for mean

		// vOccurrence will be used later to find the mode
		for( size_t i = 0; i < vUserInput.size(); ++ i) {
			if( *iter == vUserInput[i] )
				++vOccurrence[i];
		}
	}
	
	for( std::vector<int>::const_iterator iter = vOccurrence.begin(); iter != vOccurrence.end(); ++iter ) {
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

void* readSonar(void *ptr)
{
	while(1)
	{
		char tmp[100];
		if(fgets(tmp, 100, sonarFD) != NULL)
		{
			int reading = atoi(tmp);
			if(reading <= 7) // we are going to hit something
			{
				obstruction=true;
			}
			else
			{
				obstruction=false;
			}
		}
	}
}

/*
void printMap()
{
	int node;
	int dist;
	node* curnode;
	pnode = nodes[0]; //node 0
	std::cout<<"0 ";	
	if(pnode->connections[1]->items.size() > 0)
        {
        	node = pnode->connections[1]->items[0]->id;
                dist = pnode->connections[1]->items[0]->distance;
        }
	else
	{
		node = pnode->connections[1]->node1->id;
		dist - pnode->connections[1]->distance;
	}
	std::cout<<node<<" "<<dist<<" ";
	if(pnode->connections[2]->items.size() > 0)
        {
        	node = pnode->connections[2]->items[pnode->connections[2]->items.size()-1]->id;
                dist = pnode->connections[2]->distance - pnode->connections[2]->items[pnode->connections[2]->item.size()-1]->distance;
        }
	else
	{
		node = pnode->connections[2]->node0->id;
		dist - pnode->connections[2]->distance;
	}
	std::cout<<node<<" "<<dist<<" -1"<<std::endl;

	pnode = nodes[1]; //node 1
	std::cout<<"1 ";	
	if(pnode->connections[0]->items.size() > 0)
        {
        	node = pnode->connections[0]->items[pnode->connections[0]->items.size()-1]->id;
                dist = pnode->connections[0]->distance - pnode->connections[0]->items[pnode->connections[0]->item.size()-1]->distance;
        }
	else
	{
		node = pnode->connections[0]->node0->id;
		dist - pnode->connections[0]->distance;
	}
	std::cout<<node<<" "<<dist<<" ";
	if(pnode->connections[2]->items.size() > 0)
        {
        	node = pnode->connections[2]->items[0]->id;
                dist = pnode->connections[2]->items[0]->distance;
        }
	else
	{
		node = pnode->connections[2]->node1->id;
		dist - pnode->connections[2]->distance;
	}
	std::cout<<node<<" "<<dist<<" -1"<<std::endl;

	pnode = nodes[2]; //node 2
	std::cout<<"2 ";	
	if(pnode->connections[0]->items.size() > 0)
        {
        	node = pnode->connections[0]->items[0]->id;
                dist = pnode->connections[0]->items[0]->distance;
        }
	else
	{
		node = pnode->connections[0]->node1->id;
		dist - pnode->connections[0]->distance;
	}
	std::cout<<node<<" "<<dist<<" ";
	if(pnode->connections[1]->items.size() > 0)
        {
        	node = pnode->connections[1]->items[0]->id;
                dist = pnode->connections[1]->items[0]->distance;
        }
	else
	{
		node = pnode->connections[1]->node1->id;
		dist - pnode->connections[1]->distance;
	}
	std::cout<<node<<" "<<dist<<" ";
	if(pnode->connections[2]->items.size() > 0)
        {
        	node = pnode->connections[2]->items[pnode->connections[2]->items.size()-1]->id;
                dist = pnode->connections[2]->distance - pnode->connections[2]->items[pnode->connections[2]->item.size()-1]->distance;
        }
	else
	{
		node = pnode->connections[2]->node0->id;
		dist - pnode->connections[2]->distance;
	}
	std::cout<<node<<" "<<dist<<" -1"<<std::endl;
	
	int ii=0;
	line* pline;
	pline = pnode->connections[2];
	
	for(ii=0; ii < pline->items.size(); ii++)
	{
		std::cout<<pline->items[ii]->id<<" ";
		if(ii == 0)
		{
			node = pline->node0->id;
			dist = pline->items[ii]->distance;
		}
		else
		{
			node = pline->items[ii-1]->id;
			dist = pline->items[ii]->distance - pline->items[ii-1]->distance;
		}
		std::cout<<node<<" "<<dist<<" ";

		if(ii+1 == pline->items.size()) // last element
		{
			node = pline->node1->id;
			dist = pline->distance - pline->items[ii]->distance;
		}
		else
		{
			node = pline->items[ii+1]->id;
			dist = pline->items[ii+1]->distance - pline->items[ii]->distance;
		}
		std::cout<<node<<" "<<dist<<" -1"<<std::endl;
	}
	pline = pnode->connections[1];
	
	for(ii=0; ii < pline->items.size(); ii++)
	{
		std::cout<<pline->items[ii]->id<<" ";
		if(ii == 0)
		{
			node = pline->node0->id;
			dist = pline->items[ii]->distance;
		}
		else
		{
			node = pline->items[ii-1]->id;
			dist = pline->items[ii]->distance - pline->items[ii-1]->distance;
		}
		std::cout<<node<<" "<<dist<<" ";

		if(ii+1 == pline->items.size()) // last element
		{
			node = pline->node0->id;
			dist = pline->distance - pline->items[ii]->distance;
		}
		else
		{
			node = pline->items[ii+1]->id;
			dist = pline->items[ii+1]->distance - pline->items[ii]->distance;
		}
		std::cout<<node<<" "<<dist<<" -1"<<std::endl;
	}
	pnode = nodes[0];
	pline = pnode->connections[1];
	
	for(ii=0; ii < pline->items.size(); ii++)
	{
		std::cout<<pline->items[ii]->id<<" ";
		if(ii == 0)
		{
			node = pline->node0->id;
			dist = pline->items[ii]->distance;
		}
		else
		{
			node = pline->items[ii-1]->id;
			dist = pline->items[ii]->distance - pline->items[ii-1]->distance;
		}
		std::cout<<node<<" "<<dist<<" ";

		if(ii+1 == pline->items.size()) // last element
		{
			node = pline->node1->id;
			dist = pline->distance - pline->items[ii]->distance;
		}
		else
		{
			node = pline->items[ii+1]->id;
			dist = pline->items[ii+1]->distance - pline->items[ii]->distance;
		}
		std::cout<<node<<" "<<dist<<" -1"<<std::endl;
	}

}
*/

void processOrder()
{
	//get the order
	
}

void*  readPath(void *ptr)
{
	while(1)
	{
		if(shopping) // if we are suppose to be reading codes
		{
			char tmp[100];
			std::string result;
			if(fgets(tmp, 100, pathFD) != NULL)
			{
				result = tmp;
				int pos = result.find("|");
				std::string item = result.substr(0,pos);
				std::string quant = result.substr(pos+1);
				int id = atoi(item.c_str());
				int quantity = atoi(quant.c_str());
				//int id = atoi(tmp);
					
				std::cout<<"Item request detected: "<<result<<std::endl;
				getItem(id,quantity);
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
		//std::cout<<"Turning.."<<std::endl;
		if(count >= 45)
		{
		short r = readSensor(SENSOR_CLIFF_FRONT_RIGHT_SIGNAL);
		short l = readSensor(SENSOR_CLIFF_FRONT_LEFT_SIGNAL);
		if(l THROP lThresh)
			rc=1;
		if((r THROP rThresh) && (rc==1))
		{
			//std::cout<<"line found"<<std::endl;
			drive(0,0);
			//std::cin>>lc;
			//drive(50,-1);
			rc=0;
			//lines++;
			break;
		}
		}
		if (count >= 95)
			break;

	}
	drive(0,0);
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
		//std::cout<<"Turning.."<<std::endl;
		if(count <= -45)
		{
		short r = readSensor(SENSOR_CLIFF_FRONT_RIGHT_SIGNAL);
		short l = readSensor(SENSOR_CLIFF_FRONT_LEFT_SIGNAL);
		if(r THROP rThresh)
			rc=1;
		if((l THROP lThresh) && (rc==1))
		{
			//std::cout<<"line found"<<std::endl;
			drive(0,0);
			//std::cin>>lc;
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
		//std::cout<<"Turning.."<<std::endl;
		if(count <= -90)
		{
		short r = readSensor(SENSOR_CLIFF_FRONT_RIGHT_SIGNAL);
		short l = readSensor(SENSOR_CLIFF_FRONT_LEFT_SIGNAL);
		if(r THROP rThresh)
			rc=1;
		if((l THROP lThresh) && (rc==1))
		{
			//std::cout<<"line found"<<std::endl;
			drive(0,0);
			//std::cin>>lc;
			//drive(50,-1);
			rc=0;
			//lines++;
			break;
		}
		}
		if (count <= -185)
			break;
	}
	drive(0,0);
}
void  *readQR( void *ptr)
{
	int lastItem = 0;
	while(1)
	{
		char tmp[100];
		std::string result;
		if(fgets(tmp, 100, qrFD) != NULL)
		{
			result = tmp;
			int id = atoi(tmp);
			currentItem = id;
			if(id == lastItem)
				continue;
			lastItem = id;
			if(scanning) // if we are suppose to be reading codes
			{
				std::cout<<"QR decteded "<<result<<" Current distance: "<<currentDistance<<std::endl;
				if(currentLine->hasItem(id))
				{
					currentItem = id;
				}
				else//add item to current line
				{
					item* newItem = new item(id, currentLine, currentDistance, orientation);
					currentLine->items.push_back(newItem);
					notify(NEWITEM);
				}
			}
		}
	}
}

void recordPos(int id)
{
	node* newNode = new node(id);
	nodes.push_back(newNode);
}

void intersection(int path)
{
	driveDistance(50,0,90,1); //drive 30 mm forward
	//reset sensor
	getAngle();
	std::cout<<"got angle"<<std::endl;
	drive(50,-1);
	int count=0, current=0, rc=0, lc=0, lines=0;
	while(1)
	{
		usleep(10000);
		current = getAngle();
		count += current;
		std::cout<<"Turning.."<<std::endl;
		short r = readSensor(SENSOR_CLIFF_FRONT_RIGHT_SIGNAL);
		short l = readSensor(SENSOR_CLIFF_FRONT_LEFT_SIGNAL);
		if(r THROP rThresh)
			rc=1;
		if((l THROP lThresh) && (rc==1))
		{
			std::cout<<"line found"<<std::endl;
			drive(0,0);
			//std::cin>>lc;
			drive(50,-1);
			rc=0;
			lines++;
		}
		if (count <= -180)
			break;

	}
	drive(0,0);
	std::cout<<"done turning"<<std::endl;
	std::cout<<"lines: "<<lines<<std::endl;
	//std::cin>>lc;
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

void followLine(int dist=5000)
{
	std::cout<<"Following line"<<std::endl;
	int speed = 30; //originally 35
	drive(3*speed,0);
	int stop=0;
	int counter=0;
	while(stop< 2)
	{
/*		if(obstruction)
		{
			drive(0,0);
			continue;
		}
		else //drive straight
			drive(3*speed,0);
*/
		updatePosition();
		if(currentDistance >= dist)
		{
			drive(0,0);
			break;
		}
		counter++;
		usleep(20000);
		short r = readSensor(SENSOR_CLIFF_FRONT_RIGHT_SIGNAL);
		short l = readSensor(SENSOR_CLIFF_FRONT_LEFT_SIGNAL);
		//std::cout<<"r: "<<r<<" l: "<<l<<std::endl;
		if ((r THROP rThresh) && (l THROP lThresh)) //we are at an intersection
		{
			if(currentDistance > 100)
			{
				drive(0,0);
				usleep(200000);
				stop++;
				continue;
			}
		}
		else if(r THROP rThresh) //we have crossed the line
			directDrive(speed,-speed);
		else if(l THROP lThresh) //we have crossed the line
			directDrive(-speed,speed);
		else //drive straight
			drive(3*speed,0);
		stop=0;
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

void dPause(std::string message)
{
	std::cout <<message<<std::endl;
	//std::cout<<"x: "<<x<<" y: "<<y<<" heading: "<<heading<<std::endl;
	std::cout << "Distance: " << currentDistance << std::endl;
	std::string tmp;
	std::cin >> tmp;
}

void initalizeStore()
{
	currentDistance=0;
	//first we need to drive forward until we reach the the edge of the store
	followLine();
	//we are now at the edge of the store
	//we need to record our current posistion
	recordPos(homeEdge);
	lastNode=nodes[0];
	//now we want to make a right turn
	drive(50,0);
	usleep(3000000);
	drive(0,0);
	turnRight();
	currentLine =  new line(lastNode);
	lastNode->connections[1]=currentLine;
	//begin scanning items
	scanning=true;
	currentDistance=0;
	
	followLine();
	//we are at the top intersection
	//save our current position
	recordPos(topIntersection);
	lastNode=nodes[1];
	lastNode->connections[2] = currentLine;
	currentLine->node1=nodes[1];
	currentLine->distance = currentDistance;
	//now we want to drive straight
	myTurn(50,-1,-10,0);
	drive(50,0);
	usleep(3000000);
	currentDistance = 0; //reset distance count
	currentLine =  new line(lastNode);
	lastNode->connections[0]=currentLine;
	followLine();
	drive(50,0);
	usleep(3000000);
	drive(0,0);
	turnLeft();
	//we are at the bottom intersection
	//save our current position
	recordPos(botIntersection);
	lastNode=nodes[2];
	lastNode->connections[2] = currentLine;
	currentLine->node1=nodes[2];
	currentLine->distance = currentDistance;
	currentLine =  new line(lastNode);
	lastNode->connections[1]=currentLine;
	currentDistance =0;
	
	//now we want to turn left
	followLine();
	currentLine->node1=nodes[1];
	currentLine->distance = currentDistance;
	lastNode=nodes[1];
	lastNode->connections[1]=currentLine;
	//we are back at the top intersection
	//we want to spin 180 and go back down to scan other side of aisle
	turnAround();
	followLine();
	lastNode=nodes[2];
	currentLine = new line(lastNode);
	lastNode->connections[0]=currentLine;
	//we are back at the bottom intersection
	//we want to make a left turn
	drive(50,0);
	usleep(3000000);
	drive(0,0);
	myTurn(50,1,90,0);
	currentDistance =0;
	followLine();
	currentLine->distance = currentDistance;
	lastNode=nodes[0];
	currentLine->node1=nodes[0];
	currentDistance=0;
	lastNode->connections[2]=currentLine;
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
	//we are now in the home posistion
	//we want to spin 180
	turnAround();
	//we are done!
	notify(FOUNDITEM);
	store newStore;
	newStore.nodes=nodes;
//	saveStore(newStore, "store");
}

void setup()
{
	signal(SIGINT, myHandler);
	signal(SIGQUIT, myHandler);
	signal(SIGABRT, myHandler);
	signal(SIGTERM, myHandler);

	shopping=false;
	
	mkfifo("/dev/QRComms",777);
	mkfifo("/dev/pathComms",777);
	std::cout<<"Waiting for QR communication"<<std::endl;	
	qrFD = fopen("/dev/QRComms","r");
	std::cout<<"QR communication enabled"<<std::endl;
	std::cout<<"Waiting for Path communication"<<std::endl;	
	pathFD = fopen("/dev/pathComms","r");
	std::cout<<"Path communication enabled"<<std::endl;
//	sonarFD = fopen("/dev/ttyACM0","r");
	//make QR thread
	int qrRet, pathRet,sonarRet;
	qrRet = pthread_create(&qrThread, NULL, readQR, NULL);
	pathRet = pthread_create(&pathThread, NULL, readPath, NULL);
//	sonarRet = pthread_create(&sonarThread, NULL, readSonar, NULL);
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
	std::vector<int> left;
	std::vector<int> right;
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
	std::cout<<"Now put ontop of tape"<<std::endl;
	int tmp;
	std::cin>>tmp;
	//usleep(5000000);
	right.clear();
	left.clear();
	std::cout<<"Starting tape calibration"<<std::endl;
	for (ii=0; ii<20; ii++)
	{
	right.push_back(readSensor(SENSOR_CLIFF_FRONT_RIGHT_SIGNAL));
	left.push_back(readSensor(SENSOR_CLIFF_FRONT_LEFT_SIGNAL));
	
	usleep(100000);
	}
	leftTape = (short)calcMode(left);
	rightTape = (short)calcMode(right);
	printf("Left: %hu Right: %hu\n",leftTape,rightTape);
	std::cout<<"Remove tape calibration"<<std::endl;
	
	usleep(5000000);
	rThresh = (rightFloor + rightTape) / 2;
	lThresh = (leftFloor + leftTape) / 2;
	
	std::ofstream myfile;
 	myfile.open ("calibration.txt");
  	myfile << rThresh<<std::endl<<lThresh<<std::endl;
  	myfile.close();	
}

bool runBFS(mapObject *cur, int item, std::string path[]) {
	q.pop();
	line *curline;
	node* curnode;
	if(cur->whatAmI() == 'l') {
		curline = (line*)cur;
	}
	if(cur->whatAmI() == 'n') {
		curnode = (node*)cur;
	} 
	// make a corresponding point in the array for all of them
	if(cur->visited) {
	}
	else if (cur->whatAmI() == 'l' && curline->hasItem(item)) {
		return true;
	}
	else{
		//cur->visited = true;
		if (cur->whatAmI() == 'l') {
			char buf[10];
			sprintf(buf,"%d",curline->node0->id);
			path[curline->node0->id] = path[curline->id] + buf + " ";
			q.push(curline->node0);

			std::sprintf(buf,"%d",curline->node1->id);
			path[curline->node1->id] = path[curline->id] + buf + " ";
			q.push(curline->node1);
		} else {
			char buf[10];
			if(curnode->id != 0)
			{
			sprintf(buf,"%d",curnode->connections[0]->id);
			path[curnode->connections[0]->id] = path[curnode->id] + buf + " ";
			q.push(curnode->connections[0]);
			}
			sprintf(buf,"%d",curnode->connections[1]->id);
			path[curnode->connections[1]->id] = path[curnode->id] + buf + " ";
			q.push(curnode->connections[1]);

			sprintf(buf,"%d",curnode->connections[2]->id);
			path[curnode->connections[2]->id] = path[curnode->id] + buf + " ";
			q.push(curnode->connections[2]);

		}
	}
	return false;
	
}

std::string itemPath (mapObject *cur, int item)
{ 
	std::string path[7]; // should also be nodes+edges

	q.push(cur);
	char buf[10];
	sprintf(buf, "%d ",cur->id);
	path[cur->id] = buf;
	mapObject *last;
	while (q.size() != 0) {
		last = q.front();
		if(runBFS(last, item, path)) {
			break;
		}
	}
	std::queue<mapObject*> empty;
	std::swap(q,empty);
	return path[last->id];
}

void getItem(int item, int quantity)
{
	//item iTar;
	std::string path = itemPath(currentLine,item); // get path
	std::cout<<path<<std::endl;
	int index=2;
	while(1)
	{
		std::cout<<"Path loop index:"<<index<<" path size: "<<path.size()<<std::endl;
		if(index >= path.size()-2)
			break;
		if(currentLine->node1->id == atoi(&path[index]))
		{
			if(!orientation)
			{
				std::cout<<"Need to spin"<<std::endl;
				turnAround();
				orientation=true;
			}	
			followLine();
			lastNode=currentLine->node1;
			if(currentLine->id == 5) // we are on the middle line
			{
				if(lastNode->connections[0]->id == atoi(&path[index+2]))
				{
					std::cout<<"Need to turn left"<<std::endl;
			 		drive(50,0);
		         		usleep(3000000);
		         		drive(0,0);
					turnLeft();
					std::cout<<"Done turning"<<std::endl;
					currentLine=lastNode->connections[0];
					currentDistance=0;
				}
				if(lastNode->connections[2]->id == atoi(&path[index+2]))
				{
					std::cout<<"Need to turn right"<<std::endl;
			 		drive(50,0);
		         		usleep(3000000);
		         		drive(0,0);
					turnRight();
					std::cout<<"Done turning"<<std::endl;
					currentLine=lastNode->connections[2];
					currentDistance=0;
					orientation=false; //denote we are going backwards
				}
				
			}
			else
			{
			if(lastNode->connections[1]->id == atoi(&path[index+2]))
			{
				std::cout<<"Need to turn left"<<std::endl;
			 	drive(50,0);
		         	usleep(3000000);
		         	drive(0,0);
				turnLeft();
				std::cout<<"Done turning"<<std::endl;
				currentLine=lastNode->connections[1];
				currentDistance=0;
			}
			else if(lastNode->connections[0]->id == atoi(&path[index+2]))
			{
				std::cout<<"need to go straight"<<std::endl;
 				myTurn(50,-1,-10,0);
        			drive(50,0);
        			usleep(3000000);
		         	drive(0,0);
				currentLine=lastNode->connections[0];
				currentDistance=0;
			}
			}
		}
		else//need to spin arround
		{
			std::cout<<"point is behind us"<<std::endl;
			if(orientation)
			{
				turnAround();
				orientation=false; //denote we are going backwards
			}
			followLine();
			lastNode=currentLine->node0;
			if(currentLine->id == 5) // we are on the middle line
			{
				if(lastNode->connections[0]->id == atoi(&path[index+2]))
				{
					std::cout<<"Need to turn left"<<std::endl;
			 		drive(50,0);
		         		usleep(3000000);
		         		drive(0,0);
					turnLeft();
					std::cout<<"Done turning"<<std::endl;
					currentLine=lastNode->connections[0];
					currentDistance=0;
					orientation=true; //denote we are going backwards
				
				}
				if(lastNode->connections[2]->id == atoi(&path[index+2]))
				{
					std::cout<<"Need to turn right"<<std::endl;
			 		drive(50,0);
		         		usleep(3000000);
		         		drive(0,0);
					turnRight();
					std::cout<<"Done turning"<<std::endl;
					currentLine=lastNode->connections[2];
					currentDistance=0;
				}
			}
			else
			{		
			if(lastNode->connections[1]->id == atoi(&path[index+2]))
			{
			 	drive(50,0);
		         	usleep(3000000);
		         	drive(0,0);
				turnRight();
				currentLine=lastNode->connections[1];
				currentDistance=0;
				if(lastNode->id == 2)
					orientation=true;
			}
			else if(lastNode->connections[2]->id == atoi(&path[index+2]))
			{
 				myTurn(50,1,10,0);
        			drive(50,0);
        			usleep(3000000);
		         	drive(0,0);
				currentLine=lastNode->connections[2];
				currentDistance=0;
			}
			}
		}
		index+=2;
		std::cout<<"index incremented"<<std::endl;
	}
	std::cout<<"on final line"<<std::endl;
	//currentDistance=0;
	int ii=0;
	for(ii=0; ii<currentLine->items.size(); ii++)
	{
		if(currentLine->items[ii]->id==item)
		{
			//iTar=currentLine->items[ii];
			break;
		}
	}
	if(orientation)
	{
		followLine(currentLine->items[ii]->distance);
		if(!currentLine->items[ii]->orientation)
		{
			turnAround();
			orientation=false;
		}
	}
	else
	{
		followLine(currentLine->distance - currentLine->items[ii]->distance);
		if(currentLine->items[ii]->orientation)
		{
			turnAround();
			orientation=true;
		}
	}
	sleep(1);
	if(currentItem==item)
	{
		for(ii=0; ii < quantity; ii++)
		{
			notify(FOUNDITEM);
		}
		std::cout<<*currentLine->items[ii]<<std::endl;
	}
	else
	{
		notify(NOTFOUND);
	}
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
	std::string res;
	while(1)
	{
		std::cout<<"Would you like to callibrate the floor? (y/n)"<<std::endl;
		
		std::cin >> res;
		if(res == "y")
		{
			calibrateFloor();
			break;
		}
		else if(res == "n")
		{
			std::ifstream myfile ("calibration.txt");
			std::string line;
			getline (myfile,line);
			rThresh = atoi(line.c_str());
			getline (myfile,line);
			lThresh = atoi(line.c_str());
			break;
		}
	}
	while(1)
	{
		std::cout<<"Would you like to init the store? (y/n)"<<std::endl;
		
		std::cin >> res;
		if(res == "y")
		{
			initalizeStore();
			break;
		}
		else if(res == "n")
		{
			store newStore;
			restoreStore(newStore, "store");
			nodes=newStore.nodes;
			break;
		}
	}
		std::cout<<"Ready to begin"<<std::endl;
		std::cin>> res;
		followLine();
		drive(50,0);
		usleep(3000000);
		drive(0,0);
		turnRight();
		drive(0,0);
		currentLine=nodes[0]->connections[1];
		currentDistance=0;
		shopping=true;
		pthread_join( pathThread, NULL);
	drive(0,0);
}
