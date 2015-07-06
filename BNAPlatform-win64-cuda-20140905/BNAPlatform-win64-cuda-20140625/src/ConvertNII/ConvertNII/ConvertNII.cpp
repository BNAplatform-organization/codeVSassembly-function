# include <iostream>
# include <fstream>
# include <cmath>
# include <memory.h>
# include <cstring>
# include "Timer.h"
# include "dirent.h" 
//# include "mask_dt.h" 
# include <time.h>
using namespace std;

const int HdrLen = 352;
double ProbCut = 0.5; 


int main(int argc, char * argv[])
{

	if (argc < 3 || argc > 4) 
	{
		cerr<<"Input format: .\\ConvertNII.exe  file_to_be_converted  niiFile_for_mask  [ |threshold_for_mask]\n"
			<<"For example: .\\ConvertNII.exe  X.deg  mask.nii  [ |0.4]"<<endl;
		system("pause");
		exit(1);	
	}


	DIR *dp;
	struct dirent *dirp;
	if (NULL == (dp = opendir(argv[1])))
	{
		printf("can't open %s", argv[1]);
		exit (1);
	}
	int FileNumber = 0;
	string filenametmp;
	while((dirp = readdir(dp)) != NULL)
	{
		filenametmp = string(dirp->d_name);

		if (filenametmp.find_last_of('.') == -1)
			continue;
		if(filenametmp.length()>4 && (filenametmp.substr(filenametmp.find_last_of('.'),3).compare(".pc") == 0 || filenametmp.substr(filenametmp.find_last_of('.'),3).compare(".cp") == 0 ||filenametmp.substr(filenametmp.find_last_of('.'),4).compare(".deg") == 0 ||filenametmp.substr(filenametmp.find_last_of('.'),5).compare(".modu") == 0 ||filenametmp.substr(filenametmp.find_last_of('.'),4).compare(".eff") == 0 || filenametmp.substr(filenametmp.find_last_of('.'),3).compare(".bc") == 0) && filenametmp.size() - filenametmp.find_last_of('.') - 1 >= 2)
		{
			FileNumber++;
		}
	}
	cout<<FileNumber<<" files to be processed."<<endl;
	closedir(dp);
	string *filename = new string[FileNumber];
	dp = opendir(argv[1]);
	long long i = 0;
	while((dirp = readdir(dp)) != NULL)
	{
		filenametmp = string(dirp->d_name);
		if (filenametmp.find_last_of('.') == -1)
			continue;
		if(filenametmp.length()>4 && (filenametmp.substr(filenametmp.find_last_of('.'),3).compare(".pc") == 0 || filenametmp.substr(filenametmp.find_last_of('.'),3).compare(".cp") == 0 ||filenametmp.substr(filenametmp.find_last_of('.'),4).compare(".deg") == 0 ||filenametmp.substr(filenametmp.find_last_of('.'),5).compare(".modu") == 0 ||filenametmp.substr(filenametmp.find_last_of('.'),4).compare(".eff") == 0 || filenametmp.substr(filenametmp.find_last_of('.'),3).compare(".bc") == 0) && filenametmp.size() - filenametmp.find_last_of('.') - 1 >= 2)
		{
			filename[i++] = filenametmp;
		}
	}
	// Parse file name
	for (long long i = 0; i < FileNumber; i++)
	{
		string b = string(filename[i]);
		string c = string(argv[1]).append("\\").append(argv[2]);
		string a = string(argv[1]).append("\\").append(filename[i]);
		cout<<"\nconvert analysis for "<<a.c_str()<<" ..."<<endl;
		ifstream fin(a.c_str(), ios_base::binary);
		if (!fin.good())
		{	cout<<"Can't open\t"<<a.c_str()<<endl;	return 0;}

		const char * input = b.c_str();
		char * mask_file = argv[2];
		char * OutNii = new char [a.size() + 5]; 
		strcpy(OutNii, a.c_str());
		strcpy(OutNii + a.size(), ".nii");
		OutNii[a.size() + 4] = 0;
		//ifstream fin(input, ios::binary);
		//if (!fin.good())
		//{	cout<<"Can't open\t"<<input<<endl;	system("pause"); return 0;}
		int N = 0;
		fin.read((char*)&N, sizeof(int));
		float * fraw_data = new float [N];
		fin.read((char*)fraw_data, sizeof(int) * N);
		fin.close();
		int x_size, y_size, z_size, total_size, k = 0, l = 0;
		ifstream fin2(c.c_str(), ios_base::binary);
		//fin.open(mask_file, ios::binary);
		if (!fin2.good())
		{	cout<<"Can't open\t"<<mask_file<<endl;	return 0; }
		short hdr[HdrLen / 2];
		fin2.read((char*)hdr, HdrLen);
		x_size = hdr[21];
		y_size = hdr[22];
		z_size = hdr[23];
		total_size = x_size * y_size * z_size;
		
		float *mask = new float [total_size];
		if (hdr[35] == 2) 
		{
			unsigned char * mask_uc = new unsigned char [total_size];
			fin2.read((char*)mask_uc, sizeof(unsigned char) * total_size);
			for (int vm = 0; vm<total_size; vm++)
				mask[vm] = (float) mask_uc[vm];
			delete [] mask_uc;
		}
		else if(hdr[35] == 16)
		{
			fin2.read((char *)mask, sizeof(float) * total_size);
		}
		else
		{
			cout<<"mask data-type error, Only the type of unsigned char and float can be handled.\n";
			//system("pause");
			return -1;
		}
			
		
		fin2.close();

		int * iraw_data = (int*) fraw_data;
		int * convert_data = new int[total_size];
		float *convert_data_f = (float *) convert_data;		
		if (argc == 4)
			ProbCut = atof(argv[3]);
		if (input[strlen(input) - 3] == 'd' || input[strlen(input) - 4] == 'm')
		{
			hdr[35] = 8;
			hdr[36] = 32;
			for (k = 0; k < total_size; k++)
				if ((float)mask[k] >= ProbCut)
					convert_data[k] = iraw_data[l++];
				else 
					convert_data[k] = 0;
		}
		else if (input[strlen(input) - 2] == 'c' || input[strlen(input) - 3] == 'e' || input[strlen(input) - 2] == 'b' || input[strlen(input) - 2] == 'p')
		{
			hdr[35] = 16;
			hdr[36] = 32;
			for (k = 0; k < total_size; k++)
				if ((float) mask[k] >= ProbCut)
					convert_data_f[k] = fraw_data[l++];
				else
					convert_data_f[k] = 0;
		}
		else 
		{
			cout<<"Unknown file type:\t"<<input<<endl;
			return 0;
		}
		ofstream fout(OutNii, ios::binary | ios::out);
		fout.write((char*)hdr, HdrLen);
		fout.write((char*)convert_data, sizeof(int) * x_size * y_size * z_size);
		fout.close();
		delete []OutNii;
		delete []fraw_data;
		delete []mask;
		cout<<"Convert to .nii completed!"<<endl;
	}

	cout<<"==========================================================="<<endl;
	system("pause");
	return 0;
}
