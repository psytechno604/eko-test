#include "Delivery.h"

#include <sstream>
#include <iostream>

#include <iomanip>
#include <stack>

Delivery::Delivery(std::string data)
{
	std::string token;
	std::istringstream tokenStream(data);
	while (std::getline(tokenStream, token, ','))
	{
		auto src = addNode(token.substr(0, 1));
		auto dest = addNode(token.substr(1, 1));
		auto weight = std::stoi(token.substr(2, token.length() - 2));

		auto entry = new AdjListPreloadUnit;
		entry->node = newAdjListNode(dest, weight);
		entry->src = src;

		buffer.push_back(entry);
	}

	createGraph(nodeNumber.size());

	for (auto u : buffer) {
		addEdge(u);
	}
}


Delivery::~Delivery()
{
	for (auto u : buffer) {
		delete u;
	}
	for (int i = 0; i < graph->V; i++) {
		auto next = graph->array[i].head;
		while (next != nullptr) {
			auto to_delete = next;
			next = next->next;
			delete to_delete;
		}
	}
}

int Delivery::size() {
	if (graph == nullptr) return -1;
	return graph->V;
}

//adds node (while reading input)
int Delivery::addNode(std::string nname) {
	auto it = nodeNumber.find(nname);
	if (it == nodeNumber.end()) {
		int index = nodeNumber.size();
		nodeName.insert(std::map<int, std::string>::value_type(index, nname));
		nodeNumber.insert(std::map<std::string, int>::value_type(nname, index));
		return index;
	}
	else {
		return it->second;
	}
}

// A utility function to create a new adjacency list node 
struct AdjListNode* Delivery::newAdjListNode(int dest, int weight)
{
	auto newNode = new AdjListNode;
	newNode->dest = dest;
	newNode->weight = weight;
	newNode->next = nullptr;
	newNode->used = false;
	return newNode;
}

// A utility function that creates a graph of V vertices 
void Delivery::createGraph(int V)
{
	graph = new Graph;
	graph->V = V;

	// Create an array of adjacency lists.  Size of array will be V 
	graph->array = new AdjList[V];

	// Initialize each adjacency list as empty by making head as nullptr 
	for (int i = 0; i < V; ++i)
		graph->array[i].head = nullptr;
}

// Adds an edge to an undirected graph 
void Delivery::addEdge(AdjListPreloadUnit * unit)
{
	// Add an edge from src to dest.  A new node is added to the adjacency 
	// list of src.  The node is added at the begining 

	unit->node->next = graph->array[unit->src].head;
	graph->array[unit->src].head = unit->node;
}

// A utility function to create a new Min Heap Node 
struct MinHeapNode* Delivery::newMinHeapNode(int v, int dist)
{
	auto minHeapNode = new MinHeapNode;
	minHeapNode->v = v;
	minHeapNode->dist = dist;
	return minHeapNode;
}

// A utility function to create a Min Heap 
struct MinHeap* Delivery::createMinHeap(int capacity)
{
	auto minHeap = new MinHeap;
	minHeap->pos = new int[capacity];
	minHeap->size = 0;
	minHeap->capacity = capacity;
	minHeap->array = new MinHeapNode * [capacity];
	return minHeap;
}


// A utility function to swap two nodes of min heap. Needed for min heapify 
void Delivery::swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
{
	auto t = *a;
	*a = *b;
	*b = t;
}

// A standard function to heapify at given idx 
// This function also updates position of nodes when they are swapped. 
// Position is needed for decreaseKey() 
void Delivery::minHeapify(struct MinHeap* minHeap, int idx)
{
	int smallest, left, right;
	smallest = idx;
	left = 2 * idx + 1;
	right = 2 * idx + 2;

	if (left < minHeap->size &&
		minHeap->array[left]->dist < minHeap->array[smallest]->dist)
		smallest = left;

	if (right < minHeap->size &&
		minHeap->array[right]->dist < minHeap->array[smallest]->dist)
		smallest = right;

	if (smallest != idx)
	{
		// The nodes to be swapped in min heap 
		auto smallestNode = minHeap->array[smallest];
		auto idxNode = minHeap->array[idx];

		// Swap positions 
		minHeap->pos[smallestNode->v] = idx;
		minHeap->pos[idxNode->v] = smallest;

		// Swap nodes 
		swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

		minHeapify(minHeap, smallest);
	}
}

// A utility function to check if the given minHeap is ampty or not 
int Delivery::isEmpty(struct MinHeap* minHeap)
{
	return minHeap->size == 0;
}

// Standard function to extract minimum node from heap 
struct MinHeapNode* Delivery::extractMin(struct MinHeap* minHeap)
{
	if (isEmpty(minHeap))
		return nullptr;

	// Store the root node 
	auto root = minHeap->array[0];

	// Replace root node with last node 
	auto lastNode = minHeap->array[minHeap->size - 1];
	minHeap->array[0] = lastNode;

	// Update position of last node 
	minHeap->pos[root->v] = minHeap->size - 1;
	minHeap->pos[lastNode->v] = 0;

	// Reduce heap size and heapify root 
	--minHeap->size;
	minHeapify(minHeap, 0);

	return root;
}

// Function to decreasy dist value of a given vertex v. This function 
// uses pos[] of min heap to get the current index of node in min heap 
void Delivery::decreaseKey(struct MinHeap* minHeap, int v, int dist)
{
	// Get the index of v in  heap array 
	int i = minHeap->pos[v];

	// Get the node and update its dist value 
	minHeap->array[i]->dist = dist;


	// Travel up while the complete tree is not hepified. 
	// This is a O(Logn) loop 
	while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist)
	{
		// Swap this node with its parent 
		minHeap->pos[minHeap->array[i]->v] = (i - 1) / 2;
		minHeap->pos[minHeap->array[(i - 1) / 2]->v] = i;
		swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
		// move to parent index 
		i = (i - 1) / 2;
	}
}

// A utility function to check if a given vertex 
// 'v' is in min heap or not 
bool Delivery::isInMinHeap(struct MinHeap* minHeap, int v)
{
	if (minHeap->pos[v] < minHeap->size)
		return true;
	return false;
}

// A utility function used to print the solution 
void Delivery::printArr(int dist[], int n)
{
	std::cout << "Vertex Distance from Source" << std::endl;
	for (int i = 0; i < n; ++i)
		std::cout << i << " " << dist[i] << std::endl;
}



int Delivery::deliveryCost(std::string route)
{
	std::string token;
	std::istringstream tokenStream(route);
	int from = -1;
	int cost = 0;
	
	while (std::getline(tokenStream, token, '-')) {
		int to = nodeNumber.at(token);
		if (from >= 0) {
			auto head = graph->array[from].head;
			while (head != nullptr && head->dest != to) {
				head = head->next;
			}
			if (head == nullptr)
				return -1;
			cost += head->weight;
		}
		from = to;
	}
	return cost;
}

int Delivery::numRoutes(std::string strSrc, std::string strDest, bool useSameRoute, int maxSteps, int maxCost) {
	int src = nodeNumber.at(strSrc);
	int dest = nodeNumber.at(strDest);
	int res = 0;
	int routeCost = 0;
	std::stack<AdjListNode*> routeStack; //using stack instead of recursion

	auto push = [&](AdjListNode * el) {
		el->used = true;
		routeStack.push(el);
		routeCost += el->weight;
		if (useSameRoute && el->dest == dest) {
			res++;			
		}
	};

	//pop element, decrease path cost
	auto pop = [&]() {
		auto top = routeStack.top();
		routeCost -= top->weight;
		top->used = false;
		routeStack.pop();
	};

	//can this node be put into path?
	auto isSuitable = [&](AdjListNode * node) {
		if (!node) return false;
		if (!useSameRoute && node->used) return false;
		if (node->weight >= maxCost - routeCost) return false;
		if (routeStack.size() == maxSteps - 1 && node->dest != dest) return false;
		return true;
	};

	//used to find next or substituting node
	auto getSuitableNode = [&](AdjListNode * node) {
		while (node && !isSuitable(node))
			node = node->next;
		return node;
	};

	//substitutes used nodes with their 'nexts' or goes down to root
	auto decrease = [&]() {
		AdjListNode* next = nullptr;
		while (!routeStack.empty() && next == nullptr) {
			auto top = routeStack.top();
			pop();
			next = getSuitableNode(top->next);
			if (!useSameRoute && next && next->dest == dest) {
				res++;
				next = getSuitableNode(next->next);
			}
		}
		if (next)
			push(next);
	};

	auto head = graph->array[src].head;

	if (!useSameRoute && head && head->dest == dest) {
		res++;
		if (head->next)
			head = getSuitableNode(head->next);
	}
	if (head != nullptr) {
		push(head);
	}

	//main loop
	while (!routeStack.empty()) {
		if (routeStack.size() < maxSteps) {
			auto top = routeStack.top();
			auto head = getSuitableNode(graph->array[routeStack.top()->dest].head);
			if (!useSameRoute && head && head->dest == dest) {
				res++;
				head = getSuitableNode(head->next);
			}
			if (head != nullptr) {
				push(head);
			}
			else {
				decrease();
			}
		}
		else {
			decrease();
		}
	}
	return res;
}

// The main function that calulates distances of shortest paths from src to all 
// vertices. It is a O(ELogV) function (Dijkstra algorithm)
int Delivery::cheapestRoute(std::string strSrc, std::string strDest) {
	int src = nodeNumber.at(strSrc);
	int dest = nodeNumber.at(strDest);

	int V = graph->V;// Get the number of vertices in graph 
	int* dist = new int[V];      // dist values used to pick minimum weight edge in cut 

	// minHeap represents set E 
	auto minHeap = createMinHeap(V);
	// Initialize min heap with all vertices. dist value of all vertices  
	for (int v = 0; v < V; ++v)
	{
		dist[v] = INT_MAX;
		minHeap->array[v] = newMinHeapNode(v, dist[v]);
		minHeap->pos[v] = v;
	}

	// Make dist value of src vertex as 0 so that it is extracted first 
	/*minHeap->array[src] = newMinHeapNode(src, dist[src]);
	minHeap->pos[src] = src;
	dist[src] = 0;

	decreaseKey(minHeap, src, dist[src]);*/

	// Initially size of min heap is equal to V 
	minHeap->size = V;

	bool firstStep = true;

	// In the followin loop, min heap contains all nodes 
	// whose shortest distance is not yet finalized. 
	auto minHeapNode = minHeap->array[src];
	while (!isEmpty(minHeap) && (
		minHeapNode->v != dest ||
		firstStep
		))
	{
		// Extract the vertex with minimum distance value 

		int u = minHeapNode->v; // Store the extracted vertex number 

		// Traverse through all adjacent vertices of u (the extracted 
		// vertex) and update their distance values 
		auto pCrawl = graph->array[u].head;
		while (pCrawl != nullptr)
		{
			int v = pCrawl->dest;

			// If shortest distance to v is not finalized yet, and distance to v 
			// through u is less than its previously calculated distance 
			int dist_u = u == src ? 0 : dist[u];
			if (isInMinHeap(minHeap, v) && dist_u != INT_MAX && pCrawl->weight + dist_u < dist[v])
			{
				dist[v] = dist_u + pCrawl->weight;

				// update distance value in min heap also 
				decreaseKey(minHeap, v, dist[v]);
			}
			pCrawl = pCrawl->next;
		}
		firstStep = false;
		if (!isEmpty(minHeap))
			minHeapNode = extractMin(minHeap);

	}

	// print the calculated shortest distances 
	//printArr(dist, V);
	int ret = dist[dest];
	delete [] dist;
	return ret;
}