#include"error_1.h"
#include"errors.h"

#include<iostream>
#include<fstream>

using namespace std;

 int inv_index(int pair[],int i, int& err) {
	  if(pair[i%2]>25||pair[i%2]<0)
        {
                err=INVALID_INDEX;
                return INVALID_INDEX;
        }
        return NO_ERROR;
}

int inv_index_2(int position,int& err)
{
	if(position>25||position<0)
	{
		err=INVALID_INDEX;
		return INVALID_INDEX;
	}
	return NO_ERROR;
}
int non_num_char(std::ifstream& input, int& err)
{
	int stream = input.tellg();
	char ch;

        input.get(ch);
        while(!input.eof() && !isspace(ch))
        {
                if(!isdigit(ch))
                {
                        err=NON_NUMERIC_CHARACTER;
                        return NON_NUMERIC_CHARACTER;
		}
        input.get(ch);
        }

	input.seekg(stream);

        return NO_ERROR;
}


