#include "sen.h"

std::vector<node*> nodes;

void constructMap()
{
        node* homeNode = new node(0);
        nodes.push_back(homeNode);
        line* line1 = new line(homeNode,3);
        homeNode->connections[1]=line1;
        node* topNode = new node(1);
        nodes.push_back(topNode);
        topNode->connections[2]=line1;
        line1->node1=topNode;
        line1->distance = 1000;


/*        std::ofstream ofs("line1");
        boost::archive::text_oarchive oa(ofs);
        oa << *line1;
*/
        line* line2 = new line(topNode,4);
        topNode->connections[0]=line2;
        node* botNode = new node(2);
        nodes.push_back(botNode);
        botNode->connections[2]=line2;
        line2->node1=botNode;
        line2->distance = 1000;

        line* line3 = new line(botNode,5);
        botNode->connections[1]=line3;
        topNode->connections[1]=line3;
        line3->node1=topNode;
        line3->distance = 500;

        line* line4 = new line(botNode,6);
        botNode->connections[0]=line4;
        homeNode->connections[2]=line4;
        line4->node1=homeNode;
        line4->distance = 500;


}


int main()
{

	//std::vector<int> grr;

	//node* t = new node(12);
	//line* t2 = new line(t,33);
/*	
	 node* homeNode = new node(0);
	 line* line1 = new line(homeNode,3);
	homeNode->connections[1]=line1;
	node* topNode = new node(1);
//        nodes.push_back(topNode);
        topNode->connections[2]=line1;
        line1->node1=topNode;
        line1->distance = 1000;
*/

	constructMap();

	 item* myItem = new item(1337,nodes[0]->connections[1], 200);

	std::ofstream ofs("tser3");
	boost::archive::text_oarchive oa(ofs);
    	oa << nodes;

	return 1;
}

