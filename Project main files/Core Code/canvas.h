#pragma once
#include <string>
using namespace std;
const double Kb = 1.380649E-23; //m^2 kg s^-2 K^-1

class canvas
{
public:
	//Initialization
	canvas(int size, double J, double H, double KbT, int PorNorR);
	//Deleting grid, since others could be deleted automatically
	~canvas();

	//The next 4 are initialized in the Generate function
	//which has included the calculate functions(for all and average)
	//It means in the initial state we have updated them
	double TotalEnergy;//total energy, here the formula = -J\sum \sigma_i*\sigma_j-H\sum \sigma_k
	double TotalM;//=\sum \sigma
	double AverageEnergy;
	double AverageM;

	int SingleStep=0;//Record how many steps for Single-Flip now

	int GenerationWay;//0 for random, 1 for all positive, -1 for all negative
	void RandomGenerate();
	void AllPositiveGenerate();
	void AllNegativeGenerate();

	//It is only a simple version print configuration function in C++,
	//for more beuautiful picture the python files could be used 
	bool PrintConfiguration();

	//Save the configuration to a txt file
	bool SaveConfiguration(string TxtName);

	//Do Single-Spin-Flip for one time
	//The decision of which site is flipped, is not included in this function
	bool SingleSpinFlip(int row, int column);

	//Currently only for the case H=0
	//Return the cluster size in this flip activity
	int WolffFlip(int row, int column);

	//In SSF and Wolff, we only calculate the total physical quantities
	//because calculate the average for every time would cause more time, and it's also unnecessary
	void CalculateAverage();
private:
	//The next variables are initialized in the constructor function
	int** grid;
	int size;
	double Jfactor;//representing the strength of mutual interaction
	double Hfactor;//representing the out field strength
	double KbT;//beta could calculated by = pow(KbT, -1)
	
	double RealTemperature;
	//Just for further use, we mainly use KbT for input and calculation.
	//Because nearly all the literature would use T*(KbT) rather than Real T

	//Since calculate average is only two lines of code, why we choose to build another funtion?
	//Why not just write it in the CalculateEnergyM's function body?
	//The reason is: For the initial state it is ok, but later in the program
	//we may gain the total values through other approaches, for example,
	//If we have calculated the changed energy, the best way is to just add the changed quantity to
	// the current value, instead of calculate the whole system.
	// in the print function we need to use CalculateAverage singly.
	void CalculateEnergyM();

	//Calculate the changed energy if a site is flipped
	double SingleChangedEnergy(int row, int column);
	
	//Return the spin of neighboring cells
	//Please note, our direction is defined by:
	//from up to down, the row number is increasing
	//from left to right, the column number is increasing
	int RightSpin(int row, int column);
	int LeftSpin(int row, int column);
	int UpSpin(int row, int column);
	int DownSpin(int row, int column);
	int TotalNeighborSpins(int row, int column);

	//Overload all the four direction functions-Since Wolff algrithom needs the coordinates of points
	//When using these four functions, the last two variables are designed for passing reference
	int RightSpin(int row, int column,int &ReturnRow, int &ReturnColumn);
	int LeftSpin(int row, int column, int &ReturnRow, int &ReturnColumn);
	int UpSpin(int row, int column, int &ReturnRow, int &ReturnColumn);
	int DownSpin(int row, int column, int &ReturnRow, int &ReturnColumn);
};