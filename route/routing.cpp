#include "routing.h"

Routing::Routing(void) {
	fileName_ = "";
	PinList_.resize(2);
    MAXPinNumber = 0;
}
Routing::Routing(string filename) {
	fileName_ = filename;
	PinList_.resize(2);
    MAXPinNumber = 0;
}
void Routing::Read(void) {
	fstream pinList(fileName_, ios::in);
	stringstream ss;
	int in;
	int line = 0;
	/*
	string in;
	while (pinList >> in)
	{
		PinList.push_back(in);
	}
	*/

	string readLine = "";
	while (getline(pinList, readLine)) {
		ss << readLine;
		while (ss >> in) {
			PinList_[line].push_back(in);
            if (in > MAXPinNumber) MAXPinNumber = in;
		}
		readLine = "";
		ss.str("");
		ss.clear();
		++line;
	}
}
void Routing::Print(void) {
	for (auto x : PinList_) {
		for (auto y : x) {
			cout << y << endl;
		}
		cout << endl;
	}
}
vector<vector<int>> Routing::getPinList(){
    return PinList_;
}
void Routing::SetTrunks(void) {
    Trunks.resize(MAXPinNumber + 1);
    startList.resize(MAXPinNumber + 1);

	for (int j = 0; j < PinList_[0].size(); ++j) {
		int via = PinList_[0][j];
		Trunks[via].vias.push_back(j);
		via = PinList_[1][j];
		Trunks[via].vias.push_back(j);
	}
	for (int i = 0; i < Trunks.size(); ++i) {
		if (Trunks[i].vias.empty()) {
			startList[i] = Trunks[i].start = Trunks[i].end = -1;
			continue;
		}
		Trunks[i].start = getMin(Trunks[i].vias);
		Trunks[i].end = getMax(Trunks[i].vias);
		startList[i] = Trunks[i].start;
	}
}
void Routing::PrintTracks(void) {
	for (int i = 0; i < Tracks.size(); i++) {
		cout << "Track " << i << " : ";
		for (auto& j : Tracks[i]) {
			cout << j << " ";
		}
		cout << endl;
	}
}
void Routing::LeftEdge(void) {
	set<int> scheduled;
	set<int> unscheduled;
	vector<int> startListCopy = startList;
	vector<int> track;
	int pos = 0;
    int realPinCount = MAXPinNumber;
	for (auto& a : startList) { // ��S���X�{��pin �R��
		if (a == -1) realPinCount--;
	}
    int MaxLength = PinList_[0].size();
    for (int i = 1; i < MAXPinNumber + 1; i++) {
		unscheduled.insert(i);
	}
	while (scheduled.size() != realPinCount) {
		int pin = PickFromStartList(startListCopy, pos);
		if (pin == -1) {
			pos = 0;
			Tracks.push_back(track);
			track.clear();
			continue;
		}
        int TrunkLength = Trunks[pin].end - Trunks[pin].start;
		if (TrunkLength + pos > MaxLength) {
			pos = 0;
			Tracks.push_back(track);
			track.clear();
			continue;
		}
		track.push_back(pin);
		scheduled.insert(pin);
		startListCopy[pin] = INT_MAX;
		pos = Trunks[pin].end + 1;

        if (pos > MaxLength) {
			pos = 0;
			Tracks.push_back(track);
			track.clear();
		}

	}

}

int Routing::getMax(const vector<int>& vias) const {
    int Maximum = vias[0];
	for (auto x : vias) {
        if (x > Maximum)
            Maximum = x;
	}
    return Maximum;
}
int Routing::getMin(const vector<int>& vias) const {
	int Min = vias[0];
	for (auto x : vias) {
		if (x < Min)
			Min = x;
	}
	return Min;
}
int Routing::PickFromStartList(const vector<int>& startlist, int lowerBound) const {
	int Min = INT_MAX; // index = pin �s�� 
	int idx = -1;
	for (int i = 1; i < startlist.size(); ++i) {
		if (startlist[i] < Min && startlist[i] >= lowerBound > 0) {
			Min = startlist[i];
			idx = i;
		}
	}
	return idx;
}
int Routing::findTrack(int pin) {
	for (int i = 0; i < Tracks.size(); ++i) {
		vector<int>::iterator result = find(Tracks[i].begin(), Tracks[i].end(), pin); //�d�� pin
		if (result != Tracks[i].end()) //���
			return i;
	}
	return -1;
}

vector<vector<Line>> Routing::to_coordinate(void) {
	Lines.resize(4);
	// Layer 1
	Line tmp = { 0, 0, 0, 0 }; // x1 , y1 , x2 , y2
	for (int i = 0; i < PinList_[0].size(); ++i) {
		int pin = PinList_[0][i];
		if (pin == 0)
			continue;
        tmp.x1 = i;
		tmp.y1 = 0;
		tmp.x2 = i;
        tmp.y2 = findTrack(pin) + 1;
		Lines[1].push_back(tmp);
	}
	// Layer 2
	for (int i = 0; i < Tracks.size(); ++i) {
		for (int j = 0; j < Tracks[i].size(); ++j) {
			int pin = Tracks[i][j];
			tmp.x1 = Trunks[pin].start;
			tmp.y1 = i + 1;
			tmp.x2 = Trunks[pin].end;
			tmp.y2 = i + 1;
			Lines[2].push_back(tmp);
		}
	}
	// Layer 3
	for (int i = 0; i < PinList_[1].size(); ++i) {
		int pin = PinList_[1][i];
		if (pin == 0)
			continue;
		tmp.x1 = i;
		tmp.y1 = findTrack(pin) + 1;
		tmp.x2 = i;
		tmp.y2 = Tracks.size() + 1;
		Lines[3].push_back(tmp);
	}
	return Lines;
}

int Routing::trackCount(void) {

	return Tracks.size();
}
/*
int main() {
    string filename = "sample.pin";
    Routing pinList(filename);
    pinList.Read();
    // pinList.Print();
    pinList.SetTrunks();
    pinList.LeftEdge();
    pinList.PrintTracks();
    pinList.to_coordinate();
    system("pause");
    return 0;

}

*/
