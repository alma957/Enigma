#include<iostream>
#include<fstream>
#include"errors.h"
#include"enigma.h"
#include"rotor.h"
#include"error_1.h"
using namespace std;

Enigma::Enigma(int no_of_rotor)
{
        for (int i=0; i<26; i++)
	{
		plug[i]=-1;
		refl[i]=-1;
	}

        no_of_rotors=no_of_rotor;
	
	if (no_of_rotor>0)
	{
		rotor= new Rotor[no_of_rotor];
	} else {
		rotor=nullptr; ///can be null if not specified

}
}

Enigma::~Enigma()
{
	if(no_of_rotors>0)
		delete [] rotor;///delete rotor so no memory leaks

}


void Enigma::machine_conf(int argc, char** argv, int& err)
{
	if (insuf_num_par(argc, err)==INSUFFICIENT_NUMBER_OF_PARAMETERS)
	{
		cerr<<"Insufficient number of parameters provided on the command line"<<endl;
		return;
	}
	configuration_plugboard(argv[1], err);
	if (err!=NO_ERROR)
	{
		return;
	}
	configuration_reflector(argv[2], err);
	if (err!=NO_ERROR)
	{
		return;
	}
	
	if (argc>3)  ///if number of arguments passed is more than 3 then we must have a rotor (argc includes ./enigma as well)
	{
		 configuration_rotors(argc, argv, err);
	}
	if (err!=NO_ERROR)
	{
		return;
	}
}

int Enigma::insuf_num_par(int argc, int& err)
{
	if(argc<3)
	{
		err=INSUFFICIENT_NUMBER_OF_PARAMETERS;
		return INSUFFICIENT_NUMBER_OF_PARAMETERS;
	}
	return NO_ERROR;
}
int Enigma::file_error(ifstream& input, int& err)
{
	if(input.fail())
	{
		err=ERROR_OPENING_CONFIGURATION_FILE;
		return ERROR_OPENING_CONFIGURATION_FILE;
	}
	return NO_ERROR;
}
void Enigma::configuration_plugboard(const char* filename, int& err)
{
	int i;
         
	int pair[2];

	ifstream input;
	if (file_error(input, err)==ERROR_OPENING_CONFIGURATION_FILE)
	{
		cerr<<"Error opening file "<<filename<<endl;
		return;
	}
	input.open(filename);
	input>>ws;
        
	for (i=0;!input.eof();i++)
	{
		input>>pair[i%2];
        if (i>25)  ///means we have specified 27 parameter (i starts from zero)
	{
		err=INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
		cerr<<"Incorrect number of plugboard parameters in file "<<filename<<endl;
		return;
	}
	
		if (non_num_char(input, err)==NON_NUMERIC_CHARACTER)
		{
			cerr<<"Non-numeric character in plugboard file "<<filename<<endl;
			return;
		}


                if (inv_index(pair, i, err)==INVALID_INDEX)
		{
			cerr<<"Invalid index in plugboard file "<<filename<<endl;
			return ;
		}

		if (imp_plug_conf(pair,i, err)==IMPOSSIBLE_PLUGBOARD_CONFIGURATION)
		{
			cerr<<"Impossible plugboard configuration in file "<<filename<<endl;
			return;
		}

		if (i%2==1)  ///once every to cycles we set the pairs
		{
			plug[pair[0]]=pair[1];
			plug[pair[1]]=pair[0];
		}
		input>>ws;
	}
	if (inc_num_parameters(i, err)==INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS)  ///here we look for odd number of parameters
	{
		cerr<<"Incorrect number of plugboard parameters in file "<<filename<<endl;
		return;

	}


	setplug(); ///set remaing plugs
}

void Enigma::configuration_reflector(const char* filename, int& err)
{
	int i;
	int pair[2];
	ifstream input;
	input.open(filename);
	if (file_error(input, err)==ERROR_OPENING_CONFIGURATION_FILE)
	{
		cerr<<"Error opening file "<<filename<<endl;
		return;
	}

	input>>ws;
	for (i=0; !input.eof(); i++)
	{
		input>>pair[i%2];
                if (i>25)  ///same as for plugboards
		{
                err=INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;			
		cerr<<"Incorrect number of reflector parameters in file "<<filename<<endl;
		return;
	        }
		

		if(non_num_char(input, err)==NON_NUMERIC_CHARACTER)
		{
			cerr<<"Non-numeric character in file "<<filename<<endl;
			return;
		}
		if (inv_index(pair, i, err))
		{
			cerr<<"Invalid index in file "<<filename<<endl;
			return ;
		}

		if (imp_ref_conf(pair, i, err)==INVALID_REFLECTOR_MAPPING)
		{
			cerr<<"Invalid reflector mapping in file "<<filename<<endl;
			return ;
		}

		if (i%2==1)
		{
			refl[pair[0]]=pair[1];
			refl[pair[1]]=pair[0];
		}
		input>>ws;

	}
	        if(inc_num_ref(i, err)==INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS)  ///same as for plugboards
        	{   
		cerr<<"Incorrect number of reflector parameters in file "<<filename<<endl;
		return;
	        }


}




int Enigma::imp_plug_conf(int pair[], int i, int& err)
{
	for (int j=0;j<26;j++)
	{
		if((plug[j]==pair[i%2]))   ///if already specified error
		{
                        err=IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
			return IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
		}
	}
	if (i%2==1 && pair[0]==pair[1])  /// cannot map to itself
		{
			err=IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
			return IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
		}

	return NO_ERROR;
}



int Enigma::inc_num_parameters(int i, int& err)
{
	if(i%2==1)
	{
		err=INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
		return INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
	}
	return NO_ERROR;
}


void Enigma::setplug()
{
	for (int i=0; i<26; i++)
	if (plug[i]==-1)  ///if plug not specified then it remained equal to -1 and we equal it to itself
		plug[i]=i;
}




int Enigma::imp_ref_conf(int pair[], int i, int& err)
{
	for (int j=0;j<26;j++)
	{
	       if((refl[j]==pair[i%2]))  ///already a mapping present then get in function
	       {
		       err=INVALID_REFLECTOR_MAPPING;
		       return INVALID_REFLECTOR_MAPPING;
	       }
	}

	if (i%2==1 && pair[0]==pair[1]) ///if it maps to itself get in function
	{
		err=INVALID_REFLECTOR_MAPPING;
	        return INVALID_REFLECTOR_MAPPING;
	}

	return NO_ERROR;
}

int Enigma::inc_num_ref(int i, int& err)
{
	for (int j=0; j<26; j++)
	{
	if (refl[j]==-1)  ///if a reflector is still equal to -1 then we have insufficient number of reflector parameter
	{
		err=INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
		return INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
	}
	}

	return NO_ERROR;
}

void Enigma::configuration_rotors(int argc, char** argv, int& err)
{
	ifstream input_pos(argv[argc-1]);
	ifstream input_rotor;
        if (file_error(input_pos, err)==ERROR_OPENING_CONFIGURATION_FILE)
	{
		cerr<<"Error opening file "<<argv[argc-1]<<endl;
		return;
	}


	for (int rot_n=0; rot_n<no_of_rotors;rot_n++)
	{
		rotor[rot_n].configuration_pos(argc, argv,input_pos,rot_n,err);
		if(err!=NO_ERROR)
			return;
	
	input_rotor.open(argv[rot_n+3]);
	if (file_error(input_rotor, err)==ERROR_OPENING_CONFIGURATION_FILE)
	{
		cerr<<"Error opening file "<<argv[rot_n+3]<<endl;
		return;
	}

	rotor[rot_n].maps(argv, rot_n,input_rotor,err);
        
	if(err!=NO_ERROR)
		return;

	rotor[rot_n].conf_notch(argv, rot_n, input_rotor, err);

	if (err!=NO_ERROR)
		return;

	input_rotor.close();


}
}


void Enigma::message(int& err)
{
	char input_stream, output_stream;

         
	cin>>ws>>input_stream;

	while(input_stream!='.' && !cin.eof())
	{
		if (inv_input(input_stream, err)==INVALID_INPUT_CHARACTER)
		{
			cerr<<"Invalid input. You may type only capital letters"<<endl;
			return;
		}

                output_stream=light_bulb(input_stream-'A')+'A'; ///say A maps to C and we type A; output_stream=light_bulb(0)+'A'=2+'A'='C';
		cout<<output_stream;

		cin>>ws>>input_stream;
	}
}
		
int Enigma::inv_input(char ch, int& err)
{
	if (ch <'A'||ch>'Z')
	{
		err=INVALID_INPUT_CHARACTER;
		return INVALID_INPUT_CHARACTER;
	}
	return NO_ERROR;
}


int Enigma::light_bulb(int input) 
{
	turn_rotor();///turn rotor

	input=plug[input];///plug change
	for (int i=no_of_rotors-1; i>=0; i--)
		input=rotor[i].input_forward(input);///right to left so rotor to the right is first in which letter gets in
	
	input=refl[input]; ///reflector change

	for (int i=0;i<=no_of_rotors-1; i++)
		input=rotor[i].input_backwards(input);///left to right

	input=plug[input]; /// plug change

	return input;
}

void Enigma::turn_rotor()
{
	int i=no_of_rotors -1;

	while (i>=0 && rotor[i].turn()) /// it turns the right rotor always as turn() is called and if turn() returns true then we turn the rotor to the left as well and so on...
		i--; 
}
















