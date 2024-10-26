#include <iostream>
#include <bitset>
#include <stdio.h>
#include<stdlib.h>
#include <string>
using namespace std;


// class instruction { // optional
// public:
// 	bitset<32> instr;//instruction
// 	instruction(bitset<32> fetch); // constructor

// };



class CPU {
private:
	bitset<8> dmemory[512]; //data memory byte addressable in little endian fashion;
	unsigned long PC; //pc 

	//important control signals
	bool branch;
	bool memRead;
	bool memToReg;
	bitset<2> aluOp;
	bool memWrite;
	bool aluSrc;
	bool regWrite;
	bitset<3> aluControlInput;
	bool zeroFlag;

	//important movement of data.

	//Decode stage:
	bitset<32> rs1Val;
	bitset<32> rs2Val;
	bitset<32> immediate;
	bitset<32> readData;
	bitset<32> rd1Val;


	//Execution stage:
	bitset<32> aluResult;
	bitset<32> aluSecondSrc;


	//Memory stage:
	bitset<32> dataRead;
	

	


	// register file

	RegisterFile myRegFile;
	


public:
	CPU();
	unsigned long readPC();
	void incPC();
	void control(bitset<7> opcode);

	//Decode operations
	void registerRead(bitset<5> rs1Bits, bitset<5> rs2Bits);
	void registerWrite(bitset<5> rd1Bits);
	void immediateGenerator(bitset<32> instrBits, bitset<7> opcode);

	//Execution operations
	void aluControl(bitset<3> func3, bitset<1> func7);
	void MuxForSecondSource();
	void aluOperation();
	
	//Memory operations
	void memoryOperation();
	
	
};

// add other functions and objects here


// Register file definition
class RegisterFile{

private:
	unsigned long* registers; // pointer to the registers

public:

	//Constructor
	RegisterFile(){
		registers = new unsigned long[32]; // allocating for 32 registers

		//Initialize all registers to 0
		for (int i = 0; i < 32; i++){
			registers[i] = 0;
		}
	};

	~RegisterFile(){
		delete[] registers; // freeing allocated memories
	};

	// Declaration of useful methods:

	// Reading registers:
	unsigned long getRegister(int index){
		if (index >= 0 && index < 32){
			return registers[index];
		}
		return -1; // an invalid index
	}

	//writing registers:
	void writeRegister(int index, unsigned long value){
		if (index >= 1 && index < 32){
			registers[index] = value; // excludes 0 since x0 should always store 0.
		}
	};





};
