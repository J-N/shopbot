#include <createoi.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//#include <vector>
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
#include <queue>


#define MAX(a,b)        (a > b? a : b)
#define MIN(a,b)        (a < b? a : b)
#define SONG_LENGTH 8
#define PI 3.14159265

#define homeEdge 0
#define botIntersection 2
#define topIntersection 1


#include <cstddef> // NULL
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>

#include <boost/archive/tmpdir.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/assume_abstract.hpp>


class node;
class line;
class item;
class mapObject;

FILE* qrFD;
float x=0;
float y=0;
float heading=0;
short rThresh;
short lThresh;
bool scanning;
int currentDistance;
int lineCounter=3;
void updatePosition();
void saveStore();

class mapObject
{
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version)
        {
                ar & type;
                ar & visited;
                ar & id;
        }
        public:
        char type;
        bool visited;
        int id;
        mapObject()
        {
        }
        char whatAmI()
        {
                return type;
        }


};

class item
{
        //friend std::ostream & operator<<(std::ostream &os, const item &it);
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version)
        {
                ar & id;
                ar & distance;
                ar & pLine;
        }
        public:
        int id;
        line* pLine;
        int distance; // distance from node 0 on its line
	item()
	{
	}
        item(int id, line* pLine, int distance)
        {
                this->id=id;
                this->pLine=pLine;
                this->distance=distance;
        }
};



class line: public mapObject
{
	 friend class boost::serialization::access;
	 template<class Archive>
	void serialize(Archive & ar, const unsigned int /* file_version */){
	 ar & boost::serialization::base_object<mapObject>(*this);
    	ar & node0;
                ar & node1;
                ar & distance;
               ar & items;

	}

	public:
	std::vector <item*> items;
	 node* node0;
        node* node1;
        int distance;
	line()
	{
	}
	line( node* node0)
	{
		this->node0 = node0;
                this->id = lineCounter++;
                this->visited = false;
                this->type = 'l';

	}
	bool hasItem(int item)
        {
                if(this->id == 3)
                        return true;
                else
                        return false;
                // iterate over vectors, ret true if match
        }


};



class node: public mapObject
{
	 friend class boost::serialization::access;
  	 int num;
	 template<class Archive>
	void serialize(Archive & ar, const unsigned int /* file_version */){
	 ar & boost::serialization::base_object<mapObject>(*this);
       	 ar & num;
	 ar & connections;
    	}

	public:
	line* connections[3];
	node()
	{
	}
	node( int d)
	{
		this->num = d;
	}
};


