#include "sen.h"

std::vector<node*> nodes;
std::queue<mapObject*> q;
node* lastNode;
line* currentLine;
void constructMap()
{
        node* homeNode = new node(0);
        nodes.push_back(homeNode);
        line* line1 = new line(homeNode);
        homeNode->connections[1]=line1;
        node* topNode = new node(1);
        nodes.push_back(topNode);
        topNode->connections[2]=line1;
        line1->node1=topNode;
        line1->distance = 2553;

        line* line2 = new line(topNode);
        topNode->connections[0]=line2;
        node* botNode = new node(2);
        nodes.push_back(botNode);
        botNode->connections[2]=line2;
        line2->node1=botNode;
        line2->distance = 4418;

        line* line3 = new line(botNode);
        botNode->connections[1]=line3;
        topNode->connections[1]=line3;
        line3->node1=topNode;
        line3->distance = 864;

        line* line4 = new line(botNode);
        botNode->connections[0]=line4;
        homeNode->connections[2]=line4;
        line4->node1=homeNode;
        line4->distance = 887;


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
                std::cout << "item not here in " << cur->id << std::endl;
                //cur->visited = true;
                if (cur->whatAmI() == 'l') {
                        char buf[10];
                        sprintf(buf,"%d",curline->node0->id);
                        path[curline->node0->id] = path[curline->id] + buf + " ";
                        q.push(curline->node0);

                        sprintf(buf,"%d",curline->node1->id);
                        path[curline->node1->id] = path[curline->id] + buf + " ";
                        q.push(curline->node1);
                        std::cout << "end of else of line" << std::endl;
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
                        std::cout << "end of else of node" << std::endl;

                }
        }
        return false;

}

std::string gotoItem (mapObject *cur, int item)
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
        //mapObject *final = q.front();
        return path[last->id];
}


int main()
{
	constructMap();

	 item* myItem = new item(13,nodes[0]->connections[1], 546);
	nodes[0]->connections[1]->items.push_back(myItem);
	std::cout<<*myItem<<std::endl;
	
	myItem = new item(24,nodes[0]->connections[1], 817);
	nodes[0]->connections[1]->items.push_back(myItem);
	std::cout<<*myItem<<std::endl;
	
	myItem = new item(23,nodes[0]->connections[1], 1134);
	nodes[0]->connections[1]->items.push_back(myItem);
	std::cout<<*myItem<<std::endl;

	myItem = new item(5,nodes[1]->connections[0], 2043);
	nodes[1]->connections[0]->items.push_back(myItem);
	std::cout<<*myItem<<std::endl;
	
	myItem = new item(33,nodes[1]->connections[0], 2240);
	nodes[1]->connections[0]->items.push_back(myItem);
	std::cout<<*myItem<<std::endl;
	
	myItem = new item(3,nodes[1]->connections[0], 2534);
	nodes[1]->connections[0]->items.push_back(myItem);
	std::cout<<*myItem<<std::endl;
	
	myItem = new item(2,nodes[1]->connections[1], 659);
	nodes[1]->connections[1]->items.push_back(myItem);
	std::cout<<*myItem<<std::endl;
	
	myItem = new item(4,nodes[1]->connections[1], 863);
	nodes[1]->connections[1]->items.push_back(myItem);
	std::cout<<*myItem<<std::endl;
	
	myItem = new item(1,nodes[1]->connections[1], 1004);
	nodes[1]->connections[1]->items.push_back(myItem);
	std::cout<<*myItem<<std::endl;
	
	myItem = new item(32,nodes[1]->connections[1], 1924, false);
	nodes[1]->connections[1]->items.push_back(myItem);
	std::cout<<*myItem<<std::endl;
	
	myItem = new item(35,nodes[1]->connections[1], 2077, false);
	nodes[1]->connections[1]->items.push_back(myItem);
	std::cout<<*myItem<<std::endl;
	
	myItem = new item(34,nodes[1]->connections[1], 2480, false);
	nodes[1]->connections[1]->items.push_back(myItem);
	std::cout<<*myItem<<std::endl;
	
	
	store newStore;
	newStore.nodes=nodes;

	saveStore(newStore, "save");

	store finalStore;
	
	restoreStore(finalStore,"save");

	nodes=finalStore.nodes;

	lastNode=finalStore.nodes[2];
        currentLine = lastNode->connections[0];

        std::cout << gotoItem(currentLine,36) << std::endl; // go to our item

	std::queue<mapObject*> empt;
	std::swap(q, empt);
	
	lastNode=finalStore.nodes[0];
        currentLine = lastNode->connections[1];

        std::cout << gotoItem(currentLine,44) << std::endl; // go to our item



	return 1;
}

