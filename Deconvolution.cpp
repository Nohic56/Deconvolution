// This code will read an list of features and give back the precusor potential mass

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cmath>
#include <vector>
#include <iomanip>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <typeinfo>

using namespace std;

#include "Deconvolution.h"
//~ cin << "hey there please type the mass list :"<<Feature_List;



Deconvolution::Deconvolution(int File)
{
	double HION = 1.00794-0.0005485799;//1.00782503214; // Mass of Hydrogen
	double NEU = 1.0086649; // Mass of Neutron
	cout << "---------------- Note : ----------------" << endl;
	cout << "Charge Max is 50" << endl;
	cout << "Isotope Max is 1 because you can't predict yet" << endl;
	cout << "Charge State should be consecutive" << endl;
	cout << "----------------------------------------" << endl;

	struct Feature_Struct TEMP;
	TEMP.Feature = 1091.7;//646.75655262;//646.7;1071.214;//150.0;//
	TEMP.Mass = 0.0;
	TEMP.Charge = 0;
	Feature_List.push_back(TEMP);
	TEMP.Feature = 1182.5;//616.00;//70;//999.867;//100.0;//
	Feature_List.push_back(TEMP);
	TEMP.Feature = 1290.0;//588.0;//937.438;//75.0;//;//
	Feature_List.push_back(TEMP);
	TEMP.Feature = 1418.8;//588.0;//937.438;//75.0;//;//
	Feature_List.push_back(TEMP);
	TEMP.Feature = 1576.3;//588.0;//937.438;//75.0;//;//
	Feature_List.push_back(TEMP);
	TEMP.Feature = 1773.3;//588.0;//937.438;//75.0;//;//
	Feature_List.push_back(TEMP);	
	//std::sort(Feature_List.Feature.rbegin(), Feature_List.Feature.rend());
	struct sort_Feature sF;
	sort(Feature_List.begin(), Feature_List.end(), sF);  // Sort according to feature decroissant order
	cout<<"Number of Feature given : " << Feature_List.size()<<endl;
	
	//classer les features si pas par ordre décroissant
	double Feature_Mass_Sum = 0.0;
	double TARGET = Feature_List[0].Feature;// set STD to big a number for the first round
	long unsigned int Nb_Row = Feature_List.size();
	double data[Nb_Row];
	int MEMO_j = 1;
	//int MEMO_i = 0;
	for(int j =1;j<50;j++) //loop over charge
	{
		//for(int i =0;i<1;i++) //loop over Isotope
		//{
			for (int Row = 0; Row < Nb_Row; Row++) //Loop over each cluster
			{
				//TEMP.Mass = (Feature_List[L].Feature - (i*(NEU / (j + L))) * (j + L)) - ((j + L) * HION);
				TEMP.Charge = j + Row;
				//TEMP.Mass = ((Feature_List[L].Feature - (i * (NEU / TEMP.Charge))) * TEMP.Charge) - (TEMP.Charge * HION);
				TEMP.Mass = (Feature_List[Row].Feature * TEMP.Charge) - (TEMP.Charge * HION); // ESIprot
				Feature_Target.push_back(TEMP);
				data[Row] = TEMP.Mass;
			}
			double New_TARGET = Standart_Dev(data,Nb_Row);

			///Debug
			///*
			//cout<<"Feature_Target with isotope ["<<i<<"]: ";
			for(int T=0;T<Feature_Target.size();T++)
			{
				//cout<<Feature_Target[T].Mass<<" ["<<Feature_Target[T].Charge<<"]\t";
				printf("Feature_Target[T].Mass is %8.9f Charge %i\n", Feature_Target[T].Mass, Feature_Target[T].Charge);
			}
			printf("Standard Deviation is %8.9f\n", New_TARGET);
			//*/
			Feature_Target.clear();
			if (TARGET > New_TARGET)
			{
				TARGET = New_TARGET;
				MEMO_j = j; //Charge
				//MEMO_i = i; //isotope
			}	
		//} // end Isotope
		cout << "############################################################################################"<< endl;
	}
	//cout << "Min STD : "<<TARGET<<" Charge : "<<MEMO_j <<endl;//<<" Isotopic : "<<MEMO_i<<endl;
	//cout<<"Here is the cluster parameter"<<endl;
	if (TARGET > 1)
	{
		cout << endl;
		cout << "############################################################################################" << endl;
		cout << "################################## Warning !!! #############################################"<<endl;
		cout << "############################################################################################" << endl;
		cout << "I still have a hight standar deviation value can be false" << endl;
		cout << "############################################################################################"<< endl;
	}
	else {}
	double Final_Mass = 0.0;
	for(int Row=0;Row<Nb_Row;Row++) //Loop over each cluster
	{
		Feature_List[Row].Charge = MEMO_j + Row;
		//Feature_List[L].Isotope = MEMO_i;
		Feature_List[Row].Mass = (Feature_List[Row].Feature * Feature_List[Row].Charge) - (Feature_List[Row].Charge * HION); //((Feature_List[L].Feature - (Feature_List[L].Isotope * (NEU / )Feature_List[L].Charge)) * Feature_List[L].Charge) - (Feature_List[L].Charge * HION);
		
		Final_Mass += Feature_List[Row].Mass;

		cout<<"Feature original mass given : "<<Feature_List[Row].Feature << endl;
		printf("Computed Protein Mass : %8.9f", Feature_List[Row].Mass);
		cout << "With Charge : " << Feature_List[Row].Charge << endl;
	}
	printf("Computed Final Mass Mass : %8.9f with Standart Dev : %8.9f \n", Final_Mass/ Nb_Row,TARGET);

	Fine_Deconvolution(Feature_List);
}

double Deconvolution::Standart_Dev(double data[],int Nb_Row)
{
	double sum = 0.0, mean=0.0, standardDeviation = 0.0, error = 0.0;
	
	for(int i = 0; i < Nb_Row; ++i)
	{
		sum += data[i];
	}
	mean = sum/Nb_Row;
    for(int i = 0; i < Nb_Row; ++i)
	{
		standardDeviation += pow(data[i] - mean, 2);
	}
	return sqrt(standardDeviation * pow(Nb_Row-1,-1));
}


void  Deconvolution::Fine_Deconvolution(vector<Feature_Struct> &Feature_List)
{
	/*
	double HION = 1.00794;
	double NEU = 1.0086649;
	cout << endl;
	cout << " Now I will compute a fine deconvolutiona and explanation of the cluster ..." << endl;
	for (int L = 0; L < Feature_List.size(); L++) //Loop over each cluster
	{
		cout << "Feature original mass given : " << Feature_List[L].Feature << endl;
		cout << "Computed Charge state: " << Feature_List[L].Charge << endl;
		cout << "Computed Protein Mass : " << Feature_List[L].Mass << endl;
		cout << "Computed commun Isotope" << Feature_List[L].Isotope << endl;
	}
	
	double TARGET = Feature_List[0].Feature; // set STD to big a number for the first round
	long unsigned int Nb_Row = Feature_List.size(); // size of the vector
	double data[Nb_Row];
	for (int L = 0; L < Feature_List.size(); L++) //Loop over each cluster
	{
		data[L] = ((Feature_List[L].Feature - ((Feature_List[L].Isotope * NEU) / (Feature_List[L].Charge + L))) * (Feature_List[L].Charge + L)) - ((Feature_List[L].Charge + L)* HION);
	}

	for (int L = 0; L < Feature_List.size(); L++) //Loop over each cluster
	{
		int BestIso = Feature_List[L].Isotope;
		for (int i = 0; i < 50; i++) //loop over Isotope
		{
			data[L] = ((Feature_List[L].Feature - ((Feature_List[L].Isotope * NEU) / (Feature_List[L].Charge + L))) * (Feature_List[L].Charge + L)) - ((Feature_List[L].Charge + L) * HION);
			double New_TARGET = Standart_Dev(data, Nb_Row);
			if (TARGET > New_TARGET)
			{
				TARGET = New_TARGET;
				BestIso = i;
				cout << "Best Iso ! " << i << endl;
			}
		}
		cout << " for feature " << Feature_List[L].Feature << " BestIso is " << BestIso << endl;
		Feature_List[L].Isotope = BestIso;
		data[L] = ((Feature_List[L].Feature - ((Feature_List[L].Isotope * NEU) / (Feature_List[L].Charge + L))) * (Feature_List[L].Charge + L)) - ((Feature_List[L].Charge + L) * HION);
	}
	*/
}