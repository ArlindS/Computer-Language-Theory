/*
 * CISC 5200 (Computer Language Theory)
 * Spring, 2020
 *
 * Project 1: A Deterministic Finite-State Automaton
 *
 * Given a file describing a DFA, this program reads strings from
 * input, saying whether the DFA accepts the string.  If the string is
 * accepted, then its trail through the FSA is printed as well.
 *
 * The program does no error-checking on the input file. 
 *
 * Author: A. G. Werschulz
 * Date:   28 Jan 2020
 */

#include "FSA.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    // command-line argument munging
    if (argc != 2) {
        cerr << "usage: " << argv[0] << " fsa_file\n"
             << "where fsa_file is the data describing the FSA\n";
        return 1;
    }

    // open the FSA data file
    ifstream fsa_data(argv[1]);
    if (!fsa_data) {
        cerr << argv[0] << ": couldn't open " << argv[1] << endl;
        return 2;
    }

    // initialize the FSA and print its description
    FSA fsa(fsa_data);
    cout << "FSA description:\n";
    fsa.describe();
    cout << endl;

    // prompt-read-eval loop
    cout << "Enter input strings, one line at a time:\n";
    while (true) {
        cout << "? ";
        string in_string;
        getline(cin, in_string);
        if (!cin) break;
        fsa.trace(in_string);
    }
}

