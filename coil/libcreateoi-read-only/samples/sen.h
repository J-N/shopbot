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
FILE* pathFD;
float x=0;
float y=0;
float heading=0;
short rThresh;
short lThresh;
bool scanning;
bool shopping;
int currentDistance;
int lineCounter=3;
void updatePosition();
void saveStore();
void getItem(int);

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
        friend std::ostream & operator<<(std::ostream &os, const item &it);
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version)
        {
                ar & id;
                ar & distance;
                ar & pLine;
                ar & orientation;
        }
        public:
        int id;
        line* pLine;
	bool orientation;
        int distance; // distance from node 0 on its line
	item()
	{
	}
        item(int id, line* pLine, int distance, bool myor=true)
        {
                this->id=id;
                this->pLine=pLine;
                this->distance=distance;
                this->orientation=myor;
	//	pLine->items.push_back(this);
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
                // iterate over vectors, ret true if match
		int ii=0;
		for(ii=0; ii<this->items.size(); ii++)
        	{
                	if(this->items[ii]->id==item)
                	{
                       		 //iTar=currentLine->items[ii];
                       	 	return true;
        	       	 }
        	}
		return false;	
        }


};



class node: public mapObject
{
	 friend class boost::serialization::access;
	 template<class Archive>
	void serialize(Archive & ar, const unsigned int /* file_version */){
	 ar & boost::serialization::base_object<mapObject>(*this);
	 ar & connections;
	 ar & num;
    	}

	public:
	int num;
	line* connections[3];
	node()
	{
	}
	node(int d)
        {
		this->num = d;
                this->id=d;
                this->visited=false;
                this->type = 'n';
        }

};

std::ostream & operator<<(std::ostream &os, const item &it)
{
    return os << "Item ID:" << it.id << " on line: " << it.pLine->id << " at distance "<< it.distance;
}


class store
{
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, unsigned int version)
        {
                ar & nodes;
        }

        public:
        std::vector<node*> nodes;
        store()
        {
        }

};

void saveStore(const store &s, const char * filename){
    // make an archive
    std::ofstream ofs(filename);
    boost::archive::text_oarchive oa(ofs);
    oa << s;
}

void
restoreStore(store &s, const char * filename)
{
    // open the archive
    std::ifstream ifs(filename);
    boost::archive::text_iarchive ia(ifs);

    // restore the schedule from the archive
    ia >> s;
}


