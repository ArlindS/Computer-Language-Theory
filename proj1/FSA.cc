/*
 * CISC 5200 (Computer Language Theory)
 * Spring, 2020
 *
 * Project 1: A Deterministic Finite-State Automaton
 *
 * Implementation file for FSA class
 *
 * Author: Arlind Stafaj
 * Date:   02/13/2020
 */

#include "FSA.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

// Constructor. Takes file stream as input and builds class object
FSA::FSA(ifstream &ifs)
{
    // read the file stream into class members  
    ifs >> sigma;
    ifs >> num_states;
    ifs >> start_state;

    // n = number of accept states. Used as counter for while loop
    int n = 0;   
    ifs >> n;
    while(n>0) {
	int input = 0;
	ifs >> input;
	accept_states.push_back(input);
	n --;
    }
    
    get_state_table(ifs);
}

// Takes information from ifs stream and enters it into a 2D vector 
void FSA::get_state_table(ifstream &ifs)
{
    // read all state changes into states and input into vector num_tables
    int states = 0;
    vector<int> num_tables;
    while (ifs >> states) {
	num_tables.push_back(states);
    }

    // count keeps track of index for elements in num_tables 
    int count = 0;
    unsigned int rows = num_states;
    unsigned int columns = sigma.size();

    // make state_table a empty 2d vector filled with 0's
    state_table = vector<vector<int>>(rows, vector<int>(columns,0));

    // read from vector to 2D vector 
    for (unsigned int i = 0; i < rows; i++)
    {
	for (unsigned int j = 0; j < columns; j++)
	{
	    state_table[i][j] = num_tables[count];
	    count++;
       }
    }
}

// Display contents of DFA
void FSA::describe()
{
    cout << "Alphabet = " << sigma << endl;
    cout << "# states = " << num_states << endl;
    cout << "Start state = " << start_state << endl;
    cout << "Accept states = ";
    for (unsigned int i = 0; i < accept_states.size(); i++)
    {
	if (i < accept_states.size()-1)
	    cout << accept_states[i] << ", ";
	else
	    cout << accept_states[i] << endl;
    }
    
    for (unsigned int i = 0; i < state_table.size(); i++)
    {
	cout << i + 1 << ":  ";
	for (unsigned int j = 0; j < state_table[i].size(); j++)
	{
	    cout << state_table[i][j] << ' ';
	}
	cout << endl;
    }
}

// Tracing function for input string (accepted or not accepted) 
void FSA::trace(const string& in_string)
{
    cout << "State trace: " << start_state << " ";

    // index for start state, used to traverse state_table
    int start = start_state;

    // checks each char of input string and determines where start
    // moves to
    for (char ch: in_string)
    {
	// the index of the alphabet is the column of state_table
	int col = sigma.find(ch);
        if (col >= 0)
	{
	    start = state_table[start-1][col];
	    cout << start << ' ';
	}
	else
	{
	    cerr << endl << "Bad input" << endl;
	    exit(1); // exit program if input is bad
	}
    }

    bool accepted = false;
    // runs for loop to check if start state is in accept_state
    for (unsigned int i = 0; i < accept_states.size(); i++)
    {
	if (start == accept_states[i])
	    accepted = true;
    }
    if (accepted)
	cout << "... accepted" << endl;
    else
	cout << "...not accepted" << endl;
}

