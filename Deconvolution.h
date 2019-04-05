#ifndef Deconvolution_H
#define Deconvolution_H
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <vector>
#include <limits.h>
#include <float.h>
#include <algorithm>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
using namespace std;

struct Feature_Struct // Structure to record data
{
	int Charge;
	double Feature;
	double Mass;
	double STD;
};

class Deconvolution
{
public:
	vector<Feature_Struct> Feature_List;
	vector<Feature_Struct> Feature_Target;
	Deconvolution(int);
	double Standart_Dev(double data[],int);
private:

};
#endif
