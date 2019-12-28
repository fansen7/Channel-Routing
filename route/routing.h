#ifndef ROUTING_H
#define ROUTING_H

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <sstream>

#define INT_MAX 2147483647
#define INT_MIN -2147483648
using namespace std;

struct Line
{
	int x1, y1;
	int x2, y2;
};

struct Trunk
{
	int start;
	int end;
	vector<int> vias;
};
class Routing {
	public:
		Routing(void);
		Routing(string);
		void Read(void);
		void Print(void);
        vector<vector<int>> getPinList();
		void LeftEdge(void);
		void SetTrunks(void);
		void PrintTracks(void);
		vector<vector<Line>> to_coordinate(void);
		int trackCount(void);
	private:
		string fileName_;
		vector<vector<int>> PinList_;
		vector<int> startList;
		vector<Trunk> Trunks;
		vector<vector<int>> Tracks;
		vector<vector<Line>> Lines;
        int MAXPinNumber;
		int getMax(const vector<int>&) const;
		int getMin(const vector<int>&) const;
		int PickFromStartList(const vector<int>&, int) const;
		int findTrack(int);
};

#endif // ROUTING_H
