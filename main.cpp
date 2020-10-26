#include<iostream>
#include<fstream>
#include"errors.h"
#include"enigma.h"

using namespace std;

int main(int argc, char** argv)
{
	int err=NO_ERROR;
        Enigma enigma(argc-4);
	enigma.machine_conf(argc, argv, err);
	if (err!=NO_ERROR)
	{
		return err;
	}


	enigma.message(err);
	return err;
}
