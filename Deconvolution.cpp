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
	double HION = 1.00782503214; // Mass of Hydrogen
	double NEU = 1.0086649; // Mass of Neutron
	cout << "---------------- Note : ----------------" << endl;
	cout << "Charge Max is 50" << endl;
	cout << "Isotope Max is 1 because you can't predict yet" << endl;
	cout << "Charge State should be consecutive" << endl;
	//cout << "Please check that i am orderered in decroisant order !" << endl;
	cout << "----------------------------------------" << endl;

	//cout <<"File is then :"<<File<<endl;
	struct Feature_Struct TEMP;
	TEMP.Feature = 646.69796264;//646.75655262;//646.7;1071.214;//150.0;//
	TEMP.Mass = 0.0;
	TEMP.Charge = 0;
	Feature_List.push_back(TEMP);
	TEMP.Feature = 615.9502009769;//616.00;//70;//999.867;//100.0;//
	Feature_List.push_back(TEMP);
	TEMP.Feature = 680.683062128;//588.0;//937.438;//75.0;//;//
	Feature_List.push_back(TEMP);
	//TEMP.Feature = 562.56;//1153.5368;//300.0;//
	//Feature_List.push_back(TEMP);
	//std::sort(Feature_List.Feature.rbegin(), Feature_List.Feature.rend());
	struct sort_Feature sF;
	sort(Feature_List.begin(), Feature_List.end(), sF);  // Sort according to feature decroissant order
	cout<<"Feature_List.size() : " << Feature_List.size()<<endl;
	
	//classer les features si pas par ordre décroissant
	double Feature_Mass_Sum = 0.0;
	double TARGET = Feature_List[0].Feature;// set STD to big a number for the first round
	int MEMO_j = 1;
	int MEMO_i = 0;
				

	for(int j =1;j<30;j++) //loop over charge
	{
		for(int i =0;i<1;i++) //loop over Isotope
		{
			long unsigned int Nb_Row = Feature_List.size();
			double data[Nb_Row];
			for (int L = 0; L < Feature_List.size(); L++) //Loop over each cluster
			{
				//TEMP.Mass = (Feature_List[L].Feature - (i*(NEU / (j + L))) * (j + L)) - ((j + L) * HION);
				TEMP.Charge = j + L;
				TEMP.Mass = ((Feature_List[L].Feature - (i * (NEU / TEMP.Charge))) * TEMP.Charge) - (TEMP.Charge * HION);
				Feature_Target.push_back(TEMP);
				data[L] = TEMP.Mass;
			}
			double New_TARGET = Standart_Dev(data,Nb_Row);

			///Debug
			///*
			cout<<"Feature_Target with isotope ["<<i<<"]: ";
			for(int T=0;T<Feature_Target.size();T++)
			{
				//cout<<Feature_Target[T].Mass<<" ["<<Feature_Target[T].Charge<<"]\t";
				printf("Mono %8.9f Feature_Target[T].Mass is %8.9f %i", Feature_List[T].Feature - (i * (NEU / Feature_Target[T].Charge)), Feature_Target[T].Mass, Feature_Target[T].Charge);
			}
			cout<<endl;
			cout << "Standard Deviation is " << New_TARGET<<endl;
			printf("Standard Deviation is %8.9f", New_TARGET);
			cout<<endl;
			//*/
			Feature_Target.clear();
			if (TARGET > New_TARGET)
			{
				TARGET = New_TARGET;
				MEMO_j = j; //Charge
				MEMO_i = i; //isotope
			}	
		}
	}
	cout << "Min STD : "<<TARGET<<" Charge : "<<MEMO_j<<" Isotopic : "<<MEMO_i<<endl;
	cout<<"Here is the cluster parameter"<<endl;
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
	for(int L=0;L<Feature_List.size();L++) //Loop over each cluster
	{
		Feature_List[L].Charge = MEMO_j + L;
		Feature_List[L].Isotope = MEMO_i;
		Feature_List[L].Mass = ((Feature_List[L].Feature - (Feature_List[L].Isotope * (NEU / Feature_List[L].Charge))) * Feature_List[L].Charge) - (Feature_List[L].Charge * HION);
		Final_Mass += Feature_List[L].Mass;

		cout<<"Feature original mass given : "<<Feature_List[L].Feature << endl;
		printf("Computed Protein Mass : %8.9f", Feature_List[L].Mass);
		cout << "With Charge : " << Feature_List[L].Charge << endl;
	}
	printf("Computed Final Mass Mass : %8.9f \n", Final_Mass/ Feature_List.size());

	Fine_Deconvolution(Feature_List);
}

double Deconvolution::Standart_Dev(double data[],int Nb_Row)
{
	double sum = 0.0, mean=0.0, standardDeviation = 0.0;
	
	for(int i = 0; i < Nb_Row; ++i)
	{
		sum += data[i];
	}

	mean = sum/Nb_Row;
	
    for(int i = 0; i < Nb_Row; ++i)
	{
		standardDeviation += pow(data[i] - mean, 2);
	}
	return sqrt(standardDeviation / Nb_Row);
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