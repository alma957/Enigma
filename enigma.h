#include<iostream>
#include<fstream>
#include"errors.h"
#include"rotor.h"
struct Enigma
{
	Enigma(int no_of_rotor);  /* constructor */
        ~Enigma(); /* destructor*/
	int plug[26]; /* array of plug*/
	int refl[26]; /*array of reflector*/
	int no_of_rotors; /*number of rotors specified on the command line*/
	Rotor* rotor; /*array of rotor*/

        /*function to configure the machine, argc is the number of parameters passed, argv is a pointer to a pointer pointing the file passed on the command line. err is the error set in the main set to 0. it is passed by reference so that any error will result in a modification of the error value in main to achieve the correct exit code; this applies to all function below when int& err is specified*/
        void machine_conf(int argc, char** argv, int& err); 

	/*function return INSUFFICIENT_NUMBER_OF_PARAMETERS if paramater passed on the command line is less than 3 else return NO_ERROR*/
	int insuf_num_par(int argc, int&err);

	/*function return ERROR_OPENING_CONFIGURATION_FILE if file not opened correctly else return 0*/
	int file_error(std::ifstream& input, int& err);

	/*function to configure the plugboard; filename is the plugboard filename*/
	void configuration_plugboard(const char* filename, int& err);

	/*function to configure the plugboard; filename is the reflector filename*/
	void configuration_reflector(const char* filename, int& err);
        
	/*function to configure the rotors argc argv and err as above function*/
	void configuration_rotors(int argc, char** argv, int& err);

	/*function return  IMPOSSIBLE_PLUGBOARD_CONFIGURATION if plugboard configuration is invalid else 0*/
	int imp_plug_conf(int pair[], int i, int& err);
	/*function returns INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS  if a wrong number of plugboard parameters has been specified else return NO_ERROR*/
	int inc_num_parameters(int i, int&err);

	/*function return  INVALID_REFLECTOR_MAPPING if the reflector mapping is invalid else NO_ERROR*/
	int imp_ref_conf(int pair[], int i, int& err);

	/*function returns INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS if the number of reflector parameter is wrong else NO_ERROR*/
	int inc_num_ref(int i, int& err);

	/*function set plugs not specified in plugboard file equal to themselves*/
	void setplug();

	/*function to message text. Input is what is typed in and output is what comes out.*/
	void message(int& err);

	/*function return INVALID_INPUT_CHARACTER  if input character is different from capital letter else 0*/
	int inv_input(char ch, int& err);

	/*function that follow the path from the keyboard to the output*/
	int light_bulb(int input);

	/*function to turn the rotor*/
	void turn_rotor();
};

