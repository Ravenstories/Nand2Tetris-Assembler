#include <iostream>
#include <fstream>
#include <map>
#include <bitset>
#include <string>
#include "parseC.h"

using namespace std;

// Symbol table for Symbols and addresses;
map<string, int> symbolTable;
int variablePosition = 16;

// Symbol table
map<string, int> predefinedSymbols = {
    {"R0", 0}, {"R1", 1}, {"R2", 2}, {"R3", 3},
    {"R4", 4}, {"R5", 5}, {"R6", 6}, {"R7", 7},
    {"R8", 8}, {"R9", 9}, {"R10", 10}, {"R11", 11},
    {"R12", 12}, {"R13", 13}, {"R14", 14}, {"R15", 15},
	{"SCREEN", 16384}, {"KBD", 24576}, {"SP", 0},
	{"LCL", 1}, {"ARG", 2}, {"THIS", 3}, {"THAT", 4},
};

// Function to initialize the symbol table with predefined symbols
void initializeSymbolTable() {
    symbolTable.insert(predefinedSymbols.begin(), predefinedSymbols.end());
}

// Function to add a new symbol to the symbol table
void addSymbol(string label, int address) {
    symbolTable[label] = address;
	//symbolTable.insert({ label, bitset<16>(address).to_string() });
}

// Function to check if a symbol is already in the symbol table
bool contains(string label) {
	return symbolTable.find(label) != symbolTable.end();
}

// Function to parse and assemble A-instructions
string parseAInstruction(string line) {
    string symbol = line.substr(1); // Remove '@' symbol
    int address;

    // Check if it's a numeric address
    if (isdigit(symbol[0])) {
        address = stoi(symbol);
    }
    else {
        // Check if the symbol exists in the symbol table
		if (!contains(symbol)) {
            // If not, add it to the symbol table with the next available address
            address = variablePosition;
            addSymbol(symbol, address);
			variablePosition++;
        }
        else {
            // If it exists, retrieve its address from the symbol table
			address = symbolTable[symbol];
            std::cout << "Existing address:  " << symbol << ": " << address << endl;
        }
    }

    // Convert the address to a 16-bit binary string
    return bitset<16>(address).to_string();
}

// looping through the file to save the labels
void loopThroughLabels(ifstream& inputFile, string line) {
	int lineCount = 0;
    while (getline(inputFile, line))
    {
        char firstChar = line[0];
        if (firstChar == '(') {
            // Label declaration, e.g., (LOOP)
            string label = line.substr(1, line.length() - 2); // Remove parentheses
            addSymbol(label, lineCount); // Store label in the symbol table
            std::cout << "Label added:  " << label << endl;
        }
        else
        {
            lineCount++;
        }
    }
    std::cout << "Looping through labels complete" << endl;
}

// looping through the file to translate the A and C instructions
void loopThroughInstructions(ifstream& inputFile, ofstream& outputFile, string line) 
{
	while (getline(inputFile, line)) 
    {
        // Remove whitespace
        line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
        // Remove comments
        line = line.substr(0, line.find("//"));
        // Skip empty lines
        if (line.empty()) continue;
        
		// Parse A-instructions
        if (!line.empty()) {
            char firstChar = line[0];
            if (firstChar == '@') {
                // A-instruction
                string machineCode = parseAInstruction(line);
                outputFile << machineCode << endl;
                std::cout << "A instruction added:  " << machineCode << endl;
			}
            else if (firstChar == '(') { std::cout << "Looping over label" << endl; }
            else {
				// C-instruction;
                string machineCode = parseCInstruction(line);
				outputFile << machineCode << endl;
                std::cout << "C instruction added:  " << machineCode << endl;
            }
        }
	}
}

int main() {
    initializeSymbolTable();

    // Open the input file
    string inputFileName = "input.asm";
    // Create the output file
    string outputFileName = "output.txt";

    ifstream inputFile(inputFileName);
    ofstream outputFile(outputFileName);

    string line;
    
	loopThroughLabels(inputFile, line);
    inputFile.clear();
    inputFile.seekg(0);
	loopThroughInstructions(inputFile, outputFile, line);
    
    inputFile.close();
    outputFile.close();

    std::cout << "Assembly successfully assembled to " << outputFileName << endl;

    return 0;
}

