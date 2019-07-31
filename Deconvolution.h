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
	int Isotope;
	double Feature;
	double Mass;
	double STD;
};
struct sort_Feature { inline bool operator() (const struct  Feature_Struct& A, const struct Feature_Struct& B) { return A.Feature > B.Feature; } };
class Deconvolution
{
public:
	vector<Feature_Struct> Feature_List;
	vector<Feature_Struct> Feature_Target;
	Deconvolution(int);
	double Standart_Dev(double data[],int);
	void Fine_Deconvolution(vector<Feature_Struct>&);
private:

};
#endif
