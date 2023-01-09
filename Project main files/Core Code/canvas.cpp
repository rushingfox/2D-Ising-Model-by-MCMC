#include "canvas.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <queue>
#include <vector>
using namespace std;

canvas::canvas(int size, double J, double H, double KbT, int PorNorR)
{
	this->size = size;
	this->Jfactor = J;
	this->Hfactor = H;
	this->KbT = KbT;
	this->RealTemperature = KbT / Kb;
	this->GenerationWay = PorNorR;
	
	grid = new int* [size];
	for (int i = 0; i < size; i++)
	{
		grid[i] = new int[size];
	}

	switch (this->GenerationWay)
	{
	case 0:this->RandomGenerate(); break;
	case 1:this->AllPositiveGenerate(); break;
	case -1:this->AllNegativeGenerate(); break;
	default:
		cout << "The canvas has not been generated successfully, please use 1 0 or -1 as input parameter" << endl;
		abort();
		break;
	}
}

canvas::~canvas()
{
	for (int i = 0; i < size; i++)
	{
		delete[]grid[i];
	}
	delete[]grid;
}

//The second part of the hamiltonian could be calculated just when generating the grid randomly, thus saving some time
//But the first part could only be calculated in another loop.
void canvas::RandomGenerate()
{
	GenerationWay = 0;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			grid[i][j] = 2 * (rand() % 2) - 1;
			//Here I originally thought -0.5 firstly and *2 secondly would have the same effect, but it is different since they are "int".
		}
	}
	CalculateEnergyM();
	//PrintConfiguration();
}

void canvas::AllPositiveGenerate()
{
	GenerationWay = 1;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			grid[i][j] = 1;
		}
	}

	CalculateEnergyM();
	//PrintConfiguration();
}

void canvas::AllNegativeGenerate()
{
	GenerationWay = -1;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			grid[i][j] = -1;
		}
	}

	CalculateEnergyM();
	//PrintConfiguration();
}


bool canvas::PrintConfiguration()
{
	CalculateAverage();
	cout << "the total energy of the system is " << TotalEnergy << endl;
	cout << "the average energy of the system is " << AverageEnergy << endl;
	cout << "the TotalM of the system is " << TotalM << endl;
	cout << "the susceptibility of the system is" << AverageM << endl;
	cout << "The following is the configuration" << endl;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (grid[i][j]==1)
			{
				cout << "■";
			}
			else if(grid[i][j]==-1)
			{
				cout << "□";
			}
		}
		cout << endl;
	}
	return true;
}

bool canvas::SaveConfiguration(string TxtName)
{
	//将数据输出到txt中
	ofstream fout;

	//Test: J=1.11, setprecision=2
	//Result:J=1.1
	//So setprecision=2 is enough for our code
	//Besides, it shows that this function is more like <= rather than =
	std::ostringstream oss;
	oss << " GenerationWay=" << std::setprecision(2) << GenerationWay;
	oss <<" J=" << std::setprecision(2) << Jfactor;
	oss << " H=" << std::setprecision(2) << Hfactor;
	oss << " KbT=" << std::setprecision(2) << KbT;
	oss << " Size=" << std::setprecision(2) << size;
	oss << " SingleStep=" << std::setprecision(2) << SingleStep;
	std::string DocuName = oss.str();

	//Complex name could be used, but here we use the simplest one
	fout.open(TxtName);

	fout << "GenerationWay J H KbT size SingleStep"<< endl;
	fout << GenerationWay<<" "<< Jfactor << " " << Hfactor << " " << KbT << " " << size << " " << SingleStep << endl;

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			fout << grid[i][j] << " ";
		}
		fout << endl;
	}

	fout.close();

	return false;
}

//The return value does not represent whether the deltaE is positive, but whether the flip is successful
bool canvas::SingleSpinFlip(int row, int column)
{
	SingleStep += 1;
	double DeltaE = SingleChangedEnergy(row, column);
	if (DeltaE<0)
	{
		//cout << DeltaE << endl;
		grid[row][column] *= -1;
		TotalEnergy += DeltaE;
		TotalM += grid[row][column] * 2;
		//cout << "yes" << endl;
		return true;
	}
	else
	{

		if ((rand() % 10000)<exp(-DeltaE/KbT)*10000)
		{
			//cout << DeltaE << endl;
			grid[row][column] *= -1;
			TotalEnergy += DeltaE;
			TotalM += grid[row][column] * 2;
			//cout << "yes" << endl;
			return true;
		}
	}
	return false;
}

void canvas::CalculateEnergyM()
{
	TotalEnergy = 0;
	TotalM = 0;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			TotalEnergy += -Hfactor * grid[i][j];
			TotalEnergy += -Jfactor * grid[i][j] * TotalNeighborSpins(i, j);
			TotalM += grid[i][j];
		}
	}
	TotalEnergy /= 2;
}

void canvas::CalculateAverage()
{
	AverageEnergy = TotalEnergy / size / size;
	AverageM = abs(TotalM / size / size);
}

int canvas::WolffFlip(int row, int column)
{
	//We put the init of flag here because there is not a rule that one could only be flipped once
	bool ** flag = new bool* [size];
	for (int i = 0; i < size; i++)
	{
		flag[i] = new bool[size];
		for (int j = 0; j < size; j++)
		{
			flag[i][j] = false;//When it is false, it has not been flipped by Wolff
		}
	}
	flag[row][column] = true;
	int ClusterSize = 1;

	double P_adding = 1-exp(-2 * Jfactor / KbT);

	queue<vector<int>> Queue1;
	vector<int>* QueueElement;
	QueueElement = new vector<int>{ row, column };
	Queue1.push(*QueueElement);
	delete QueueElement;

	while (!Queue1.empty())
	{
		//I do not want to write another function, so just new two variables as "input"
		int i;
		int j;
		vector<int> Temp = Queue1.front();
		Queue1.pop();//When poping an element, it would be deleted automatically, so "delete" operation is not compulsory here
		i = Temp[0];
		j = Temp[1];

		int NeighborRow, NeighborColumn;
		//Here the spin function must be written before the flag list judgement, since the Neighbor coordinates are assigned values in the spin functions
		if (RightSpin(i,j,NeighborRow,NeighborColumn)==grid[i][j]&&flag[NeighborRow][NeighborColumn]==false)
		{
			if (rand()%10000<P_adding*10000)
			{
				QueueElement = new vector<int>{ NeighborRow,NeighborColumn };
				Queue1.push(*QueueElement);
				flag[NeighborRow][NeighborColumn] = true;
				ClusterSize += 1;
				delete QueueElement;
			}
		}
		if (LeftSpin(i, j, NeighborRow, NeighborColumn) == grid[i][j] && flag[NeighborRow][NeighborColumn] == false)
		{
			if (rand() % 10000 < P_adding * 10000)
			{
				QueueElement = new vector<int>{ NeighborRow,NeighborColumn };
				Queue1.push(*QueueElement);
				flag[NeighborRow][NeighborColumn] = true;
				ClusterSize += 1;
				delete QueueElement;
			}
		}
		if (UpSpin(i, j, NeighborRow, NeighborColumn) == grid[i][j] && flag[NeighborRow][NeighborColumn] == false)
		{
			if (rand() % 10000 < P_adding * 10000)
			{
				QueueElement = new vector<int>{ NeighborRow,NeighborColumn };
				Queue1.push(*QueueElement);
				flag[NeighborRow][NeighborColumn] = true;
				ClusterSize += 1;
				delete QueueElement;
			}
		}
		if (DownSpin(i, j, NeighborRow, NeighborColumn) == grid[i][j] && flag[NeighborRow][NeighborColumn] == false)
		{
			if (rand() % 10000 < P_adding * 10000)
			{
				QueueElement = new vector<int>{ NeighborRow,NeighborColumn };
				Queue1.push(*QueueElement);
				flag[NeighborRow][NeighborColumn] = true;
				ClusterSize += 1;
				delete QueueElement;
			}
		}

		//Do the flip for the element just poped
		//Why now? Because we need to use its grid value before
		//Flip every one in its loop is equivalent to flip all the cluster at once, and former method consumes less computation resource
		TotalEnergy += SingleChangedEnergy(i, j);
		grid[i][j] *= -1;
		TotalM += grid[i][j] * 2;
	}

	for (int i = 0; i < size; i++)
	{
		delete[]flag[i];
	}
	delete[]flag;

	return ClusterSize;
}

double canvas::SingleChangedEnergy(int row, int column)
{
	double PreviousEnergy =-Jfactor * grid[row][column] * TotalNeighborSpins(row, column) - Hfactor*grid[row][column];

	double NewEnergy= Jfactor * grid[row][column] * TotalNeighborSpins(row, column) + Hfactor * grid[row][column];

	return NewEnergy-PreviousEnergy;
}

int canvas::RightSpin(int row, int column)
{
	int RightColumnNum;
	if (column<size-1)
	{
		RightColumnNum = column + 1;
	}
	else if(column==size-1)
	{
		RightColumnNum = 0;
	}
	return grid[row][RightColumnNum];
}

int canvas::LeftSpin(int row, int column)
{
	int LeftColumnNum;
	if (column > 0)
	{
		LeftColumnNum = column - 1;
	}
	else if (column == 0)
	{
		LeftColumnNum = size-1;
	}
	return grid[row][LeftColumnNum];
}

int canvas::UpSpin(int row, int column)
{
	int UpRowNum;
	if (row>0)
	{
		UpRowNum = row - 1;
	}
	else if (row ==0)
	{
		UpRowNum = size - 1;
	}
	return grid[UpRowNum][column];
}

int canvas::DownSpin(int row, int column)
{
	int DownRowNum;
	if (row<size-1)
	{
		DownRowNum = row + 1;
	}
	else if(row==size-1)
	{
		DownRowNum = 0;
	}
	return grid[DownRowNum][column];
}

int canvas::TotalNeighborSpins(int row, int column)
{
	return RightSpin(row, column) + LeftSpin(row, column) + UpSpin(row, column) + DownSpin(row, column);
}

int canvas::RightSpin(int row, int column, int &ReturnRow, int &ReturnColumn)
{
	int RightColumnNum;
	if (column < size - 1)
	{
		RightColumnNum = column + 1;
	}
	else if (column == size - 1)
	{
		RightColumnNum = 0;
	}
	ReturnRow = row;
	ReturnColumn = RightColumnNum;
	return grid[row][RightColumnNum];
}

int canvas::LeftSpin(int row, int column, int &ReturnRow, int &ReturnColumn)
{
	int LeftColumnNum;
	if (column > 0)
	{
		LeftColumnNum = column - 1;
	}
	else if (column == 0)
	{
		LeftColumnNum = size - 1;
	}
	ReturnRow = row;
	ReturnColumn = LeftColumnNum;
	return grid[row][LeftColumnNum];
}

int canvas::UpSpin(int row, int column, int &ReturnRow, int &ReturnColumn)
{
	int UpRowNum;
	if (row > 0)
	{
		UpRowNum = row - 1;
	}
	else if (row == 0)
	{
		UpRowNum = size - 1;
	}
	ReturnRow = UpRowNum;
	ReturnColumn = column;
	return grid[UpRowNum][column];
}

int canvas::DownSpin(int row, int column, int &ReturnRow, int &ReturnColumn)
{
	int DownRowNum;
	if (row < size - 1)
	{
		DownRowNum = row + 1;
	}
	else if (row == size - 1)
	{
		DownRowNum = 0;
	}
	ReturnRow = DownRowNum;
	ReturnColumn = column;
	return grid[DownRowNum][column];
}