#include <map>
#include <string>

using namespace std;

string parseCInstruction(string line) {
    // Define the C-instruction components: dest, comp, and jump
    string dest = "000";
    string comp;
    string jump = "000";

    // Split the line into dest=comp;jump
    size_t equalsPos = line.find('=');
    size_t semicolonPos = line.find(';');

    if (equalsPos != string::npos) {
        dest = line.substr(0, equalsPos);

        if (semicolonPos != string::npos) {
            comp = line.substr(equalsPos + 1, semicolonPos - equalsPos - 1);
            jump = line.substr(semicolonPos + 1);
        }
        else {
            comp = line.substr(equalsPos + 1);
        }
    }
    else {
        if (semicolonPos != string::npos) {
            comp = line.substr(0, semicolonPos);
            jump = line.substr(semicolonPos + 1);
        }
        else {
            comp = line; // The entire line is the computation part
        }
    }
    std::cout << "Comp:  " << comp << endl;
    std::cout << "Jump:  " << jump << endl;
    std::cout << "Dest:  " << dest << endl;


    // Define the binary representations of common computation operations
    map<string, string> compCodes = {
        // Computations where a is 0
        {"0",   "0101010"},
        {"1",   "0111111"},
        {"-1",  "0111010"},
        {"D",   "0001100"},
        {"A",   "0110000"},
        {"!D",  "0001101"},
        {"!A",  "0110001"},
        {"-D",  "0001111"},
        {"-A",  "0110011"},
        {"D+1", "0011111"},
        {"A+1", "0110111"},
        {"D-1", "0001110"},
        {"A-1", "0110010"},
        {"D+A", "0000010"},
        {"D-A", "0010011"},
        {"A-D", "0000111"},
        {"D&A", "0000000"},
        {"D|A", "0010101"},
        // Computations where a is 1
        {"M",   "1110000"},
        {"!M",  "1110001"},
        {"-M",  "1110011"},
        {"M+1", "1110111"},
        {"M-1", "1110010"},
        {"D+M", "1000010"},
        {"D-M", "1010011"},
        {"M-D", "1000111"},
        {"D&M", "1000000"},
        {"D|M", "1010101"}
    };

    // Define the binary representations of common destination codes
    map<string, string> destCodes = {
        {"000", "000"},
        {"M", "001"},
        {"D", "010"},
        {"MD", "011"},
        {"A", "100"},
        {"AM", "101"},
        {"AD", "110"},
        {"AMD", "111"}
    };

    // Define the binary representations of common jump codes
    map<string, string> jumpCodes = {
        {"000", "000"},
        {"JGT", "001"},
        {"JEQ", "010"},
        {"JGE", "011"},
        {"JLT", "100"},
        {"JNE", "101"},
        {"JLE", "110"},
        {"JMP", "111"}
    };

    // Assemble the C-instruction
    string cInstruction = "111" + compCodes[comp] + destCodes[dest] + jumpCodes[jump];
    
    return cInstruction;
}
