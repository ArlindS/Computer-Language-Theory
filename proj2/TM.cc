/**********************************************************************
 *
 * CISC 5200 (Computer Language Theory)
 * Spring, 2020
 *
 * Project 2: A Turing Machine Emulator
 *
 * Implementation file for TM class
 *
 * Author: Arlind Stafaj
 * Date:   26 March 2020
 *
 **********************************************************************/

#include "TM.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

using namespace std;

// overload input operator to read into a Table_entry
istream& operator>>(istream &is, Table_entry& entry)
{
    char p1, p2, c1, c2; // p = parenthesis, c = comma
    
    return is >> p1 >> entry.new_state >> c1 >> entry.new_char
       >> c2 >> entry.direction >> p2; 
}

// overload output operator to output the contents of a Table_entry
ostream& operator<<(ostream &os, const Table_entry& entry)
{
    // placeholder -2 is used to display table heading 
    if (entry.new_state == -2)
        return os << setw(7) << entry.new_char << setw(5);

    return os << '(' << setw(3) << entry.new_state << ", "
	        << entry.new_char << ", " <<  entry.direction << ')';
}


// Constructor for TM class
TM::TM(ifstream &ifs)
{
    ifs >> num_states;
    ifs >> sigma;
    ifs >> gamma; 
    blank = gamma.back();

    // Initialize a deafault Table_entry to create a multi-dimensional vector of specified size
    state_table = vector<vector<Table_entry> >(num_states+1, vector<Table_entry>(gamma.size()));

    for (int i = 0; i <= num_states; i++)
    {
        for (long unsigned int j = 0; j < gamma.size(); j++)
        {
            // fill first row with table heading values
            if (i == 0)
                state_table[i][j].new_state = -2, state_table[i][j].new_char = gamma[j],
                    state_table[i][j].direction = ' ';
            else 
                ifs >> state_table[i][j];
        }
    }
}

// provides a description of TM  
void TM::describe()
{
    cout << "sigma = " << sigma << endl;
    cout << "gamma = " << gamma << endl;
    cout << "blank = " << blank << endl;
    cout << "number of states = " << num_states << endl;

    cout << "state transition table: \n";
    
    for (long unsigned int i = 0; i < state_table.size(); i++)
    {
        // print first row without the number indicating current state
        if (i == 0)
        {
            cout << "   ";
            for (long unsigned int j = 0; j < state_table[i].size(); j++)
                cout << state_table[i][j] << ' ';
            cout << endl;
        }
        else 
        {
            cout << i << ": ";
            for (long unsigned int j = 0; j < state_table[i].size(); j++)
                cout << state_table[i][j] << ' ';
            cout << endl;
        }
    }
}

// Trace the flow of TM 
bool TM::trace(string& in_string)
{
    // iterate over string and change to format to false if not correct format
    for (char ch: in_string)
    {
        auto loc = sigma.find(ch);
        if (loc ==string::npos)
        {
            cout << "Bad Input insert only " << sigma << "'s, Try again.\n" ;
            return false;
        }
    }

    // create a Table_entry to initiate values
    Table_entry e;
    e.new_state = 1;

    // add blank at end of string
    in_string = in_string + blank;
    int pos = 0; 

    // check if string is between length 1 and 8
    bool req_length = false; 
    if ((in_string.length() < 10 && in_string.length() > 0)) req_length = true;


    while ((e.new_state != 0 || e.new_state != -1))
    {
        int n = gamma.find(in_string[pos]); // used to find column 
        print_config(e.new_state, pos, in_string);

        e.new_char = state_table[e.new_state][n].new_char;
        e.direction = state_table[e.new_state][n].direction;
        e.new_state = state_table[e.new_state][n].new_state;

        if(e.new_char != ' ') in_string[pos] = e.new_char;

        if (e.direction == 'L') pos--;
        else if (e.direction == 'R') pos++;
        else cout << "Illegal direction " << e.direction << endl;

        if (e.new_state == 0 && req_length) return true;
        else if (e.new_state == -1) return false;
        else if (e.new_state == 0 && !req_length) return false;
    }
    return false;
}

// prints current configuration of TM 
void TM::print_config(int state, unsigned int pos, const string& in_string)
{
        for (long unsigned int i=0; i < in_string.length(); i++)
        {
            if (i == pos)
                cout << 'q' << state << ' ' << in_string[i] << ' ';
            else 
                cout << in_string[i] << ' ';
        }
        cout << endl;
}
