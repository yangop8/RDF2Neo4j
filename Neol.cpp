//============================================================================
// Name        : Neol.cpp
// Author      : Fan 
// Modefied    : Lei
// Copyright   : Your copyright notice
// Description : RDF to Nodes&Relationships
//============================================================================

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <time.h>
#include <map>
using namespace std;
struct property_list {
	int ID = 0;
	string property[100];
};
map<string, property_list> ID_Map;
string property_label[100];
int predicate_num = 100;
int main() {
	int entityID = 0;
	clock_t start, finish;
	double totaltime;
	start = clock();
	string line;
	for (int y = 0; y < predicate_num; y++) {
		property_label[y] = "-";
	}
	ofstream f1("entity_10M.csv");
	ofstream f2("relation_10M.csv");
	ofstream f3("entityHead_10M.csv");
	ifstream fin("/home/data/WatDiv/database/watdiv300M.nt");
	f2 << ":START_ID,:TYPE,:END_ID" << endl;
	int count = 0;
	while (getline(fin, line)) {
		if (count % 1000000 == 0) {
			cout << count << endl;
			finish = clock();
			totaltime = (double) (finish - start) / CLOCKS_PER_SEC;
			cout << "Finished in " << totaltime << "s" << endl;
		}
		count++;
		string triple[3];
		triple[0].assign(line, line.find_first_of("<"),
				line.find_first_of(">") + 1);
		line.erase(0, line.find_first_of(">") + 1);
		triple[1].assign(line, line.find_first_of("<"),
				line.find_first_of(">") + 1);
		line.erase(0, line.find_first_of(">") + 1);
		triple[2] = line;
		triple[0].erase(0, triple[0].find_first_of("<") + 1);
		triple[0].erase(triple[0].find_first_of(">"), triple[0].size() - 1);
		triple[1].erase(0, triple[1].find_first_of("<") + 1);
		triple[1].erase(triple[1].find_first_of(">"), triple[1].size() - 1);
		string::size_type property_check = triple[2].find("\"");
		if (property_check != triple[2].npos) {
			triple[2].erase(0, triple[2].find_first_of("\"") + 1);
			triple[2].erase(triple[2].find_first_of("\""),
					triple[2].size() - 1);
			int property_label_ID = 0;
			for (int i = 0; i < predicate_num; i++) {
				if (property_label[i] == triple[1]) {
					property_label_ID = i;
					break;
				}
				if (property_label[i] == "-") {
					property_label[i] = triple[1];
					property_label_ID = i;
					break;
				}
			}
			if (ID_Map.find(triple[0]) == ID_Map.end()) {
				ID_Map[triple[0]].ID = entityID;
				entityID++;
			}
			ID_Map[triple[0]].property[property_label_ID] = triple[2];

		} else {
			triple[2].erase(0, triple[2].find_first_of("<") + 1);
			triple[2].erase(triple[2].find_first_of(">"), triple[2].size() - 1);
			if (ID_Map.find(triple[0]) == ID_Map.end()) {
				ID_Map[triple[0]].ID = entityID;
				entityID++;
			}
			if (ID_Map.find(triple[2]) == ID_Map.end()) {
				ID_Map[triple[2]].ID = entityID;
				entityID++;
			}
			f2 << ID_Map[triple[0]].ID << "," << triple[1] << ","
					<< ID_Map[triple[2]].ID << endl;
		}
	}
	f2.close();
	fin.close();
	int property_num = 0;
	f1 << ":ID," << ":label";
	for (int j = 0; j < predicate_num; j++) {
		if (property_label[j] == "-") {
			break;
		}
		property_num = j + 1;
		f3 << "," << property_label[j];
	}
	f3 << endl;
	map<string, property_list>::iterator it;
	for (it = ID_Map.begin(); it != ID_Map.end(); it++) {
		f1 << it->second.ID << "," << it->first;
		for (int x = 0; x < property_num; x++) {
			f1 << "," << it->second.property[x];
		}
		f1 << endl;
	}
	f1.close();
	finish = clock();
	totaltime = (double) (finish - start) / CLOCKS_PER_SEC;
	cout << "Finished in " << totaltime << "s" << endl;
	return 0;
}
