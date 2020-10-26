#ifndef ROTOR_H
#define ROTOR_H

#include <fstream>

struct Rotor {
		Rotor();
		int map[26][2]; /// [0] is right to left mapping and [1] the opposite
		bool notch[26]; ///true if there is a notch at position
		int pos_n; ///increase by 1 when rotor rotates
                /* argc number of parameter passed on command line, argv is a pointer to a pointer that points to the file passed on the command ;ine, ifstream pos open filename and err is passed by reference as we can do modification to err in the main file (which is initialised to 0) to achieve right exit code*/
		void configuration_pos(int argc , char** argv, std::ifstream& pos, int rot_n, int& err);  

		/*function returns NO_ROTOR_STARTING_POSITION if for a rotor a starting position is not specified else 0*/
		int no_rot_start(std::ifstream& input,int rot_n, int& err);

	       /*function responsible to set the mappings of rotors*/	
		void maps(char** argv, int rot_n, std::ifstream& input, int& err); 

		/*function to configurate notch*/
		void conf_notch(char** argv, int rot_n, std::ifstream& input, int& err); 

		/* function returns INVALID_ROTOR_MAPPING if there is no mapping for  rotor else return 0*/
		int no_rot_map(int& err, std::ifstream& input);

                
                /* function return INVALID_ROTOR_MAPPING if mappings are invalid else return 0*/
		int inv_rotor(int& err, int mapping, int i);


		/*function return true if rotor moves in a position where a notch is present else false*/
                bool turn();

		/* function that maps from right rotor to left rotor*/
		int  input_forward(int input);

		/*function that maps from left rotor to right rotor (backwards)*/
		int  input_backwards(int input);
};
#endif
	        
