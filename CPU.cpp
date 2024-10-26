#include "CPU.h"

enum ALUControl {
    ADD = 0b000,
    SUB = 0b001,
    AND = 0b010,
    OR  = 0b011,
    XOR = 0b100, // Add XOR control
    SRA = 0b101,
	SRL = 0b110,
};


CPU::CPU()
{
	PC = 0; //set PC to 0
	for (int i = 0; i < 4096; i++) //copy instrMEM
	{
		dmemory[i] = (0);
	}

	// initializing register file.
	RegisterFile myRegFile;

}


unsigned long CPU::readPC()
{
	return PC;
}
void CPU::incPC()
{
	PC ++; 
}

// Add other functions here ... 

void CPU::control(bitset<7> opcode){
				// The control Unit of CPU


				if (opcode == bitset<7> ("0110011")){ //R-type
					regWrite = 1;
					aluSrc = 0;
					branch = 0;
					memRead = 0;
					memWrite = 0;
					memToReg = 0;
					aluOp = bitset<2>("10");
					
				}
				else if (opcode == bitset<7> ("0010011")){ //I-type
					regWrite = 1;
					aluSrc = 1;
					branch = 0;
					memRead = 0;
					memWrite = 0;
					memToReg = 0;
					aluOp = bitset<2>("11");
				}

				else if (opcode == bitset<7> ("0000011")){ //lw
					regWrite = 1;
					aluSrc = 1;
					branch = 0;
					memRead = 1;
					memWrite = 0;
					memToReg = 1;
					aluOp = bitset<2>("00");
				}

				else if (opcode == bitset<7> ("0100011")){ //sw
					regWrite = 0;
					aluSrc = 1;
					branch = 0;
					memRead = 0;
					memWrite = 1;
					memToReg = 0;
					aluOp = bitset<2>("00");
				}

				else if (opcode == bitset<7> ("1100011")){ //beq
					regWrite = 0;
					aluSrc = 0;
					branch = 1;
					memRead = 0;
					memWrite = 0;
					memToReg = 0;
					aluOp = bitset<2>("01");
				}

			}


// Register Operations
void CPU::registerRead(bitset<5> rs1Bits, bitset<5> rs2Bits){
	//Reading register.
	//bitset<32> rs1Val;
	//bitset<32> rs2Val;
	//Read

	int rs1Index = rs1Bits.to_ulong();
	unsigned long rs1Int = myRegFile.getRegister(rs1Index);
	bitset<32> rs1Val(rs1Int);

	int rs2Index = rs2Bits.to_ulong();
	unsigned long rs2Int = myRegFile.getRegister(rs2Index);
	bitset<32> rs2Val(rs2Int);

}


void CPU::registerWrite(bitset<5> rd1Bits){

	//Writing register value.

	int rd1Index = rd1Bits.to_ulong();
	unsigned long rd1Int =  rd1Val.to_ulong();
	myRegFile.writeRegister(rd1Index, rd1Int);
	
}

// Immediate Generator
void CPU::immediateGenerator(bitset<32> instrBits, bitset<7> opcode){
	bitset<32>  immediate;
	if (opcode == bitset<7> ("0010011")) // I-type instruction
	{
		immediate = (instrBits >> 20) & bitset<32> (0xFFF);  // Extract bits 31:20
        if (instrBits[31]) immediate |= 0xFFFFF000; // sign extension
	}

	else if (opcode == bitset<7> ("0100011")){ //S-type
		immediate = ( instrBits >> 25 ) << 5 | (instrBits >> 7 & bitset<32>(0x1F));
		if (instrBits[31]) immediate |= 0xFFFFF000; // sign extension
	}
	else if (opcode == bitset<7>("0100011")) { // B-type instruction
    	// Extract and shift bits 31:25 to occupy bits 11:5 of the immediate
    	immediate = ((instrBits >> 25) & bitset<32>(0x7F)) << 5;
    	// Extract bits 11:8, shift left by 1 to have bits[0] = 0 and place them in the lower 5 bits of the immediate
    	immediate |= (instrBits >> 8 << 1) & bitset<32>(0x1F);
		immediate[12] = instrBits[31];
		immediate[11] = instrBits[7];
		
    
    // Apply sign extension if the 31st bit of instrBits is 1
    	if (instrBits[31]) immediate |= bitset<32>(0xFFFFF000);
	}
	else if (opcode == bitset<7>("0110111") || opcode == bitset<7>("0010111")) { // U-type (LUI and AUIPC)
    	immediate = (instrBits & bitset<32>(0xFFFFF000));  // Upper 20 bits, shifted left by 12
	}
	
	else if (opcode == bitset<7>("1101111")) { // J-type instruction
    	immediate = ((instrBits >> 12) & bitset<32>(0xFF)) << 12 |  // Bits 19:12
   						((instrBits >> 21) & bitset<32>(0x3FF)) << 1;   // Bits 30:21
		immediate[20] = instrBits[31];
		immediate[11] = instrBits[20];

    
    // Apply sign extension if the 31st bit is set
    	if (instrBits[31]) immediate |= bitset<32>(0xFFE00000);
	}


}


//Execution stage:

// Picking the second data.
void CPU::MuxForSecondSource(){
	if(aluSrc){
		aluSecondSrc = immediate;
	}
	else {
		aluSecondSrc = rs2Val;
	}
}


// ALU Control Unit
void CPU::aluControl(bitset<3> func3, bitset<1> func7){
	if (aluOp == bitset<2>(0b00)) { // For add
			aluControlInput = bitset<3> (ADD);
		} else if (aluOp == bitset<2>(0b01)) { // For subtract
			aluControlInput = bitset<3>(SUB);
		} else if (aluOp == bitset<2>(0b10)) { // R-type
			if (func3 == bitset<3>(0b000)){
				if (func7 == bitset<1>(0x1)) aluControlInput = bitset<3> (SUB); // SUB
				else aluControlInput = bitset<3> (ADD); // ADD
			}
			else if (func3 == bitset<3>(0b111)) aluControlInput = bitset<3> (AND); // AND
			else if (func3 == bitset<3>(0b110)) aluControlInput = bitset<3> (OR); // OR
			else if (func3 == bitset<3>(0b100)) aluControlInput = bitset<3> (XOR); // XOR
			else if (func3 == bitset<3>(0b101) && func7 == bitset<1>(0x1)) aluControlInput = bitset<3> (SRA); // SRA
			// Add more R-type operations as needed
		} else if (aluOp == bitset<2>(0b11)) { // I-type
			if (func3 == bitset<3>(0b000)) aluControlInput = bitset<3> (ADD); // I-type ADD
			else if (func3 == bitset<3>(0b100)) aluControlInput = bitset<3> (XOR); // I-type XOR
			else if (func3 == bitset<3>(0b110)) aluControlInput = bitset<3> (OR); // I-type OR
			else if (func3 == bitset<3>(0b101) && func7 == bitset<1>(0x1)) aluControlInput = bitset<3> (SRA); // I-type SRA
			// Add more I-type operations as needed
		}
		// Default case or error handling
		aluControlInput = bitset<3>(ADD); // or any default operation
}

//ALU functional Unit

void CPU::aluOperation(){

/*
ADD = 0b000,
    SUB = 0b001,
    AND = 0b010,
    OR  = 0b011,
    XOR = 0b100, // Add XOR control
    SRA = 0b101,
	SRL = 0b110,
*/
		unsigned long a = rs1Val.to_ulong();
		unsigned long b = aluSecondSrc.to_ulong();

        if (aluControlInput == std::bitset<3>(ADD)) { // Add
            aluResult = bitset<32>(a + b);
        } 
        else if (aluControlInput == std::bitset<3>(SUB)) { // Subtract
            aluResult = bitset<32>(a - b);
        } 
        else if (aluControlInput== std::bitset<3>(XOR)) { // XOR Operations
            aluResult = bitset<32>(a ^ b);
        } 
        else if (aluControlInput == std::bitset<3>(OR)){ // OR Operations
			aluResult = bitset<32>(a | b);
		}
		else if (aluControlInput == std::bitset<3>(SRA)){ // SRAI Operations
			aluResult = bitset<32>(a << b);
		}

		// Checking for Zero Flag.
		if (aluResult == bitset<32> (0x0)){
				zeroFlag = 1;
			}
		else zeroFlag = 0;

}





// Add block definition:

unsigned long addBlock(unsigned long num1, unsigned long num2){
	return num1 + num2;
}

// MUX block definition:
unsigned long MuxBlock(bool sel, unsigned long num1, unsigned long num2){
	/*
	sel = 0 --> num1
	sel = 1 --> num2
	*/

	if(sel == 0){
		return num1;
	}
	else{
		return num2;
	}
}