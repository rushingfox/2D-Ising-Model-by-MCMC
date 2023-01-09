#include "canvas.h"
#include <time.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <iomanip>
using namespace std;

int main()
{
	int random_seed = 1;
	srand(random_seed);

	int size = 100;
	int generation_num = 1000000000;
	//Generation Method, 1 allpos, -1 allnega, 0 allrandom
	int GenerationWay = 1;
	//J H
	double J = 1;
	double H = 0;
	//T
	double MaxTemperature = 2.905;
	double MinTemperature = 2.9;
	double TemperatureGap = 0.01;
	
	//data type: AverageEnergy 1 or AverageM 2
	//string OutputData = "AverageEnergy";
	//string OutputData = "AverageM";
	string OutputData = "All";

	//Generate the random configuration
	/*
	canvas canvas_PrintConfiguration(size, J, H, 0, 0);
	canvas_PrintConfiguration.SaveConfiguration("Configuration.txt");
	*/

	//below is Single-Spin-Flip--------------------------------------------------------------------------------------
	//Delete the next notation symbol to run the single spin flip
	
	/*
	std::ostringstream oss_s;
	//oss_s << " Z axis is " << OutputData;
	oss_s << " Size=" << std::setprecision(2) << size;
	oss_s << " GenerationNum=" << std::setprecision(2) << generation_num;
	oss_s << " GenerationWay=" << std::setprecision(2) << GenerationWay;
	oss_s << " J=" << std::setprecision(2) << J;
	oss_s << " H=" << std::setprecision(2) << H;
	oss_s << " KbT starts from" << std::setprecision(2) << MinTemperature;
	oss_s << " KbT ends at" << std::setprecision(2) << MaxTemperature;
	oss_s << " KbT's step is" << std::setprecision(2) << TemperatureGap;
	std::string comment_s = oss_s.str();


	ofstream fout_energy, fout_AverageM;
	fout_energy.open("Energy.txt");
	fout_AverageM.open("AverageM.txt");
	fout_energy << comment_s << endl;
	fout_AverageM << comment_s << endl;

	for (double t = MinTemperature; t <= MaxTemperature; t+=TemperatureGap)
	{
		cout << t << endl;

		//clock start
		clock_t start, end;
		start = clock();
		
		canvas canvas_s(size, J, H, t,GenerationWay);
		for (int g = 0; g <= generation_num; g++)
		{
			canvas_s.SingleSpinFlip(rand() % size, rand() % size);
			if (g%1000000==0)
			{
				canvas_s.CalculateAverage();
				if (OutputData=="AverageEnergy")
				{
					fout_energy << canvas_s.AverageEnergy << " ";
				}
				else if(OutputData=="AverageM")
				{
					fout_AverageM << canvas_s.AverageM << " ";
				}
				else if (OutputData == "All")
				{
					fout_energy << canvas_s.AverageEnergy << " ";
					fout_AverageM << canvas_s.AverageM << " ";
				}
				else
				{
					cout << "The output data you select is not correct" << endl;
					abort();
				}
			}
		}
		fout_energy << endl;
		fout_AverageM << endl;
		canvas_s.SaveConfiguration(to_string(t) + ".txt");
		
		//clock end
		end = clock();
		cout << "time = " << double(end - start) / CLOCKS_PER_SEC << "s" << endl;
	}
	fout_energy.close();
	fout_AverageM.close();
	*/

	//below is Wolff--------------------------------------------------------------------------------------
	//Delete the next notation symbol to run the wolff
	
	//the parameter change compared to the single flip
	generation_num = 2000;
	//Re-generate the comment for Wolff
	std::ostringstream oss_w;
	oss_w << " Z axis is " << OutputData;
	oss_w << " Size=" << std::setprecision(2) << size;
	oss_w << " GenerationNum=" << std::setprecision(2) << generation_num;
	oss_w << " GenerationWay=" << std::setprecision(2) << GenerationWay;
	oss_w << " J=" << std::setprecision(2) << J;
	oss_w << " H=" << std::setprecision(2) << H;
	oss_w << " KbT starts from" << std::setprecision(2) << MinTemperature;
	oss_w << " KbT ends at" << std::setprecision(2) << MaxTemperature;
	oss_w << " KbT's step is" << std::setprecision(2) << TemperatureGap;
	std::string comment_w = oss_w.str();

	ofstream fout_energy, fout_AverageM;
	fout_energy.open("Energy.txt");
	fout_AverageM.open("AverageM.txt");
	fout_energy << comment_w << endl;
	fout_AverageM << comment_w << endl;

	for (double t = MinTemperature; t < MaxTemperature; t += TemperatureGap)
	{
		cout << t << endl;
		
		//clock start
		clock_t start, end;
		start = clock();

		canvas canvas_w(size, J, H, t, GenerationWay);
		for (int g = 0; g < generation_num; g++)
		{
			canvas_w.WolffFlip(rand() % size, rand() % size);
			if (true)
			{
				canvas_w.CalculateAverage();
				if (OutputData == "AverageEnergy")
				{
					fout_energy << canvas_w.AverageEnergy << " ";
				}
				else if (OutputData == "AverageM")
				{
					fout_AverageM << canvas_w.AverageM << " ";
				}
				else if (OutputData == "All")
				{
					fout_energy << canvas_w.AverageEnergy << " ";
					fout_AverageM << canvas_w.AverageM << " ";
				}
				else
				{
					cout << "The output data you select is not correct" << endl;
					abort();
				}
			}
			
			if (abs(t-1.5)<0.001 || abs(t - 2.9) < 0.001)
			{
				canvas_w.SaveConfiguration(to_string(t) +" "+to_string(g)+ ".txt");
			}
			
		}
		fout_energy << endl;
		fout_AverageM << endl;
		//canvas_w.SaveConfiguration(to_string(t) + ".txt");

		//clock end
		end = clock();
		cout << "time = " << double(end - start) / CLOCKS_PER_SEC << "s" << endl;
	}
	fout_energy.close();
	fout_AverageM.close();
	
	return 0;
}