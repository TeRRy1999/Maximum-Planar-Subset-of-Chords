#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <vector>

using namespace std;


class chord
{
public:
	chord():sp(0), ep(0){}
	chord(int i, int j):sp(i), ep(j){}

	void setval(int i, int j){
		sp = i;
		ep = j;
	}

	int getij(string index){
		if(index == "i")
			return sp;
		else
			return ep;
	}

	int getk(int j){
		if(j == sp)
			return ep;
		else
			return sp;
	}

private:
	int sp;
	int ep;
};

class MIS
{
public:
	MIS(){};
	MIS(const MIS &mis){
		chordset = mis.chordset;
	}

	MIS operator=(MIS mis){
		chordset = mis.chordset;
		return mis;
	}
	

	MIS operator+(MIS &mis){
		
		MIS tmp;
		tmp.getval().reserve(chordset.size() + mis.getval().size()); // preallocate memory
		tmp.getval().insert(tmp.getval().end(), chordset.begin(), chordset.end());
		tmp.getval().insert(tmp.getval().end(), mis.getval().begin(), mis.getval().end());

		return tmp;
	}

	vector < chord > &getval(){return chordset;}

private:
	vector < chord > chordset;
};

int formula(chord *indx[], int rani, int ranj)
{
	int pk = indx[ranj]->getk(ranj);
	if(pk == rani)
		return 3; // case 3
	else{
		if(pk > ranj || pk < rani)
			return 1; // case 1
		else
			return 2; // case 2
	}

}

void calmis(MIS **mis, chord **indx, int cleSize)
{
	int i, j;
	for(int k = 1; k < cleSize; k ++){
		for(i = 0, j = k; j < cleSize; i++, j++){
			switch (formula(indx, i, j)){
				case 1:{
					mis[i][j] = mis[i][j - 1];
					break;
				}
				case 2:{
					int pk = indx[j]->getk(j);
					int p1 = mis[i][j - 1].getval().size();
					int p2 = mis[i][pk - 1].getval().size() + 1 + mis[pk + 1][j - 1].getval().size();
					if(p1 > p2)
						mis[i][j] = mis[i][pk - 1];
					else{
						chord tmp(j, pk);
						mis[i][j] = mis[i][pk - 1] + mis[pk + 1][j - 1];
						// mis[i][j].getval().push_back(indx[j][pk]);
						mis[i][j].getval().push_back(tmp);
					}
					break;
				}
				case 3:{
					chord tmp(i, j);
					mis[i][j] = mis[i + 1][j - 1];
					mis[i][j].getval().push_back(indx[i][j]);
					// mis[i][j].getval().push_back(indx[i][j]);
					mis[i][j].getval().push_back(tmp);
					break;
				}
			};
		}
	}
}

chord ** readfile(string filename, chord *indx[], int &cleSize)
{
	ifstream infile(filename);
	infile >> cleSize;
	indx = new chord*[cleSize];
	infile.ignore();

	fill(indx, indx + cleSize, nullptr);
	string tmp;
	int pair = 0;
	while(getline(infile, tmp)){
		stringstream ss(tmp);
		int i, j;
		ss >> i;
		if(indx[i] == nullptr){
			ss >> j;
			indx[i] = new chord(i, j);
			indx[j] = indx[i];
		}
	}
	return indx;
}

void outfile(string filename, MIS **mis, int cleSize)
{
	ofstream out(filename);
	int size = mis[0][cleSize - 1].getval().size();
	out << size << endl;
	for (int i = 0; i < mis[0][cleSize - 1].getval().size(); i++){
		out << mis[0][cleSize - 1].getval()[i].getij("i") << " ";
		out << mis[0][cleSize - 1].getval()[i].getij("j") << " " << endl;
	}
}


int main(int argc, char* argv[])
{	
	int cleSize;
	chord **indx = readfile(argv[argc - 2], indx, cleSize);

	MIS **mis = new MIS*[cleSize];
	for(int i = 0; i < cleSize; i++)
		mis[i] = new MIS[cleSize];
	
	calmis(mis, indx, cleSize);
	outfile(argv[argc - 1], mis, cleSize);

}
