#include"rotor.h"
#include"errors.h"
#include<fstream>
#include<iostream>
#include"error_1.h"
using namespace std;

Rotor::Rotor()
{
	for (int i=0; i<26; i++)
		notch[i]=false;
}



void Rotor::configuration_pos(int argc, char** argv, ifstream& pos, int rot_n, int& err)
{
	int position;
        pos>>ws;
	if (no_rot_start(pos, rot_n,err)==NO_ROTOR_STARTING_POSITION)
	{
		cerr<<"No rotor starting position for rotor  "<<rot_n<<" in file "<<argv[argc -1]<<endl;
		return;
	}

	if (non_num_char(pos, err)==NON_NUMERIC_CHARACTER)
	{
		cerr<<"Non-Numeric character in rotor position in file "<<argv[argc -1]<<endl;
		return;
	}

	pos>>position;
        if (inv_index_2(position, err)==INVALID_INDEX)
	{
		cerr<<"Invalid index in rotor position in file "<<argv[argc-1]<<endl;
		return;
	}
	pos_n=position;
}


int Rotor::no_rot_start(ifstream& input, int rot_n, int& err)

{
	if (input.eof())
	{
		err=NO_ROTOR_STARTING_POSITION;
		return NO_ROTOR_STARTING_POSITION;
	}
	return NO_ERROR;
}

void Rotor::maps(char** argv,int rot_n, ifstream& input, int& err)
{
	int maps_;

	for (int i=0; i<26; i++)
	{
		input>>ws;
	if(no_rot_map(err, input)==INVALID_ROTOR_MAPPING)
	{
		cerr<<"Invalid rotor mapping in file "<<argv[rot_n+3]<<endl;
		return;
	}

	if(non_num_char(input, err)==NON_NUMERIC_CHARACTER)
	{
		cerr<<"Non-numeric character in rotor mapping file "<<argv[rot_n+3]<<endl;
		return;
	}

	input>>maps_;

	if (inv_index_2(maps_, err)==INVALID_INDEX)
	{
		cerr<<"Invalid index in rotor mapping file "<<argv[rot_n+3]<<endl;
		return ;
	}

	if(inv_rotor(err, maps_, i)==INVALID_ROTOR_MAPPING)
	{

		cerr<<"Invalid rotor mapping in file "<<argv[rot_n+3]<<endl;
		return;
	}

        map[i][0]=maps_;
	map[maps_][1]=i;


}
}


void Rotor::conf_notch(char** argv, int rot_n, ifstream& input, int& err)
{
	int notch_ind;

	input>>ws;

	while(!input.eof())
	{
		if (non_num_char(input, err)==NON_NUMERIC_CHARACTER)
		{
			cerr<<"Non-numeric carachter at notch position in file "<<argv[rot_n+3]<<endl;
			return;
		}
		input>>notch_ind;
		if(inv_index_2(notch_ind,err)==INVALID_INDEX)
		{
			cerr<<"Invalid index at notch position in file "<<argv[rot_n+3]<<endl;
			return;
		}

		notch[notch_ind]=true;
		input>>ws;
	}
}



int Rotor::inv_rotor(int& err, int mapping, int i)
{
	for  (int j=0; j<i; j++)
	{
		if (map[j][0]==mapping)
		{
			err=INVALID_ROTOR_MAPPING;
			return INVALID_ROTOR_MAPPING;
		}
	}
	return NO_ERROR;
}


int Rotor::no_rot_map(int &err, ifstream& input)
{
	if (input.eof())
	{
		err=INVALID_ROTOR_MAPPING;
		return INVALID_ROTOR_MAPPING;
	}
	return NO_ERROR;
}


bool Rotor::turn()
{
	pos_n=(pos_n + 1)%26; /// turn rotor

	if (notch[pos_n])    /// and return true if notch at pos so we rotate also next one if function returns true (as the function is recalled in enigma.cpp) and so on...
		return true;

	return false;
}


int Rotor::input_forward(int input)  ///map forward
{
	input=(input+pos_n)%26;  
	input=map[input][0];   
	input=(input-pos_n+26)%26;

	return input;
}


int Rotor::input_backwards(int input)   ///map backward
{
	input=(input+pos_n)%26;
	input=map[input][1];
	input=(input-pos_n+26)%26;

	return input;
}





	
