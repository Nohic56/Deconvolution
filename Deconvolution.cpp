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
	cout <<"File is then :"<<File<<endl;
	struct Feature_Struct TEMP;
	TEMP.Feature = 1153.5368;//300.0;
	TEMP.Mass = 0.0;
	TEMP.Charge = 0;
	Feature_List.push_back(TEMP);
	TEMP.Feature = 1071.214;//150.0;
	Feature_List.push_back(TEMP);
	TEMP.Feature = 999.867;//100.0;
	Feature_List.push_back(TEMP);
	TEMP.Feature = 937.438;//75.0;
	Feature_List.push_back(TEMP);
	
	cout<<"Feature_List.size() : " << Feature_List.size()<<endl;
	
	//classer les features si pas par décroissant
	double Feature_Mass_Sum = 0.0;
	
	for(int j =1;j<20;j++) //loop over charge
	{
		for(int i =0;i<10;i++) //loop over Isotope
		{
			for(int L=0;L<Feature_List.size();L++) //Loop over cluster
			{
				TEMP.Mass = (Feature_List[L].Feature + i/(j+L) )*(j+L);
				TEMP.Charge = j+L;
				Feature_Target.push_back(TEMP);
			}
			cout<<"Feature_Target with isotope ["<<i<<"]: ";
			for(int T=0;T<Feature_Target.size();T++){
				cout<<Feature_Target[T].Mass<<" "<<Feature_Target[T].Charge<<"\t";
				}
				cout<<endl;
			Feature_Target.clear();
			//~ TARGET = std
			
			//~ if (TARGET < New_TARGET)
			//~ {
				//~ TARGET = New_TARGET;
			//~ }
		}
	}
}


