#include "CPU.h"

#include <iostream>
#include <bitset>
#include <stdio.h>
#include<stdlib.h>
#include <string>
#include<fstream>
#include <sstream>
using namespace std;

/*
Add all the required standard and developed libraries here
*/

#include <cstdint> // to get unsigned long


/*
Put/Define any helper function/definitions you need here
*/



// main function


int main(int argc, char* argv[])
{
	/* This is the front end of your project.
	You need to first read the instructions that are stored in a file and load them into an instruction memory.
	*/

	/* Each cell should store 1 byte. You can define the memory either dynamically, or define it as a fixed size with size 4KB (i.e., 4096 lines).
	 Each instruction is 32 bits (i.e., 4 lines, saved in little-endian mode).
	Each line in the input file is stored as an hex and is 1 byte (each four lines are one instruction). You need to read the file line by line and 
	store it into the memory. You may need a mechanism to convert these values to bits so that you can read opcodes, operands, etc.
	*/

	char instMem[4096];  //one instMem = one byte = 2 hex


	if (argc < 2) {
		//cout << "No file name entered. Exiting...";
		return -1;
	}

	ifstream infile(argv[1]); //open the file
	if (!(infile.is_open() && infile.good())) {
		cout<<"error opening file\n";
		return 0; 
	}
	string line; 
	int i = 0;
	while (infile) {
			infile>>line;
			stringstream line2(line);
			char x; 
			line2>>x;
			instMem[i] = x; // be careful about hex

			//cout<<instMem[i];

			i++;
			line2>>x;
			instMem[i] = x; // be careful about hex
			//cout<<instMem[i]<<endl;
			i++;
		}
	int maxPC= i/4; // makes sense, every 4 bytes is for instructions.

	/* Instantiate your CPU object here.  CPU class is the main class in this project that defines different components of the processor.
	CPU class also has different functions for each stage (e.g., fetching an instruction, decoding, etc.).
	*/

	CPU myCPU;  // call the approriate constructor here to initialize the processor...  
	// make sure to create a variable for PC and resets it to zero (e.g., unsigned int PC = 0); 



	/* OPTIONAL: Instantiate your Instruction object here. */
	//Instruction myInst; 
	
	bool done = true;
	while (done == true) // processor's main loop. Each iteration is equal to one clock cycle.  
	{
		//fetch
		/*
		read PC value, use it to get instructions. 
		*/
		unsigned long pcVal = myCPU.readPC();

		/*because instMem is stored char by char, each char representing a hex. 
        Meaning 32 bit instruction = 8 hex long. */
		int index = pcVal * 8;
		unsigned long i = index;

		string instr = ""; // To store the instruction in string format.

		// Getting instruction from instMem (ordered to counter little endianess).
		instr = instr + instMem[i+6] + instMem[i+7] + instMem[i+4] + instMem[i+5] + instMem[i+2] + instMem[i+3] + instMem[i+0] + instMem[i+1];
		//cout << instr << endl;
		unsigned long long instrNum = stoull(instr, nullptr, 16);
		bitset<32> instrBits(instrNum);
		cout << instrBits << endl;

		// Slicing the bits into useful counterparts:

		bitset<7> opcode((instrBits.to_ulong()) & 0x7F); // getting the opcode
		bitset<5> rs1((instrBits.to_ulong() >> 15) & 0x1F); // shifting to position 15, extracting the 5 bits for source register 1.
		bitset<5> rs2((instrBits.to_ulong() >> 20) & 0x1F); // source register 2
		bitset<5> rd((instrBits.to_ulong() >> 7) & 0x1F); // destination register.
		bitset<32> immediateBits(instrBits.to_ulong()); // bits for immediate generator.
		bitset<1> func7((instrBits.to_ulong() >> 30) & 0x1); // bit for func 7.
		bitset<3> func3((instrBits.to_ulong() >> 12) & 0x7); // bits for func3. 

		// decode


		
		// ... 
		myCPU.incPC();
		if (myCPU.readPC() > maxPC)
			break;

		// We still need to add a checker where the code will end when it actually end.
		// OPCODE = 000...000 --> ends
	}
 	int a0 =0;
	int a1 =0; 
	// print the results (you should replace a0 and a1 with your own variables that point to a0 and a1)
	  cout << "(" << a0 << "," << a1 << ")" << endl;
	
	return 0;

}