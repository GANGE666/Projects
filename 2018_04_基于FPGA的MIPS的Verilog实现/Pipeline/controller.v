module aludec(input      [5:0] funct, op,
              input      [1:0] aluop,
              output reg [3:0] alucontrol);

  always @(*)
    case (aluop)
      2'b00: alucontrol <= 4'b010; //add
      2'b01: alucontrol <= 4'b110; //sub
	  
	  default: case(op)
		6'b001111: alucontrol <= 4'b1001; //LUI
		6'b001010: alucontrol <= 4'b0111; //SLTI
		6'b001101: alucontrol <= 4'b0001; //ORI
		6'b001100: alucontrol <= 4'b0000; //ANDI
		default: case(funct)
			6'b100000: alucontrol <= 4'b010; //ADD
			6'b100010: alucontrol <= 4'b110; //SUB
			6'b100100: alucontrol <= 4'b000; //AND
			6'b100101: alucontrol <= 4'b001; //OR
			6'b101010: alucontrol <= 4'b111; //SLT
            6'b100001: alucontrol <= 4'b1011; //ADDU
			6'b100011: alucontrol <= 4'b1100; //SUBU
			6'b101011: alucontrol <= 4'b1101; //SLTU
			
		  //other code
            6'b000000: alucontrol <= 4'b1000; //SLL
			6'b000010: alucontrol <= 4'b1010; //SRL
		  
            default: alucontrol <= 4'bxxx; //???
			endcase
        endcase
      endcase
endmodule

module maindec(input      [5:0] op, funct,
               output           toregaddition, memtoreg, memwrite,
               output           branch,
			   output  	  [1:0] alusrca,
			   output           alusrcb,
               output           wrdst, regdst, regwrite,
               output           jadition, jump,
               output     [1:0] aluop,
			   output			bne,
			   output	  [1:0] signextsignal);
  
  reg [10:0] controls;
  
  assign {regwrite, regdst, alusrca, alusrcb, branch, memwrite, memtoreg, jump, aluop} = controls;
  
  always @(*)
    case(op)
      6'b000000: 
        case(funct)
          6'b000000: controls <= 11'b11010000011; //SLL
		  6'b000010: controls <= 11'b11010000011; //SRL
		  6'b001000: controls <= 11'b00000000000; //Jr
		  6'b001001: controls <= 11'b11000000000; //Jalr
          default:   controls <= 11'b11000000011; //Rtyp
        endcase
      6'b100011: controls <= 11'b10001001000; //LW
      6'b101011: controls <= 11'b00001010000; //SW
      6'b000100: controls <= 11'b00000100001; //BEQ
	  6'b000101: controls <= 11'b00000100001; //BNE
      6'b001000: controls <= 11'b10001000000; //ADDI
	  6'b001101: controls <= 11'b10001000011; //ORI
	  6'b001100: controls <= 11'b10001000011; //ANDI
	  6'b001111: controls <= 11'b10101000011; //LUI
	  6'b001010: controls <= 11'b10001000011; //SLTI
      6'b000010: controls <= 11'b00000000100; //J
	  6'b000011: controls <= 11'b10000000100; //Jar
      default:   controls <= 11'bxxxxxxxxxxx; //???
    endcase
	
	assign bne = (op == 6'b000101) ? 1 : 0; //BNE
	assign jadition = ((op == 6'b000000 && funct == 6'b001000)||(op == 6'b000000 && funct == 6'b001001)) ? 1 : 0; //Jr Jalr
	assign wrdst = (op == 6'b000011) ? 1 : 0; //Jal
	assign toregaddition = ((op == 6'b000011)||(op == 6'b000000 && funct == 6'b001001)) ? 1 : 0; //Jal Jalr
	assign signextsignal = (op == 6'b001100)?2'b11:((op == 6'b001101)?2'b10:2'b00); //andi ori other
	
	
  endmodule
  
module controller(  input   [5:0] op, funct,
                    input         zero,
                    output        toregaddition, memtoreg, memwrite,
                    output        pcsrc,
					output  [1:0] alusrca,
					output        alusrcb,
                    output        wrdst, regdst, regwrite,
                    output        jadition, jump,
                    output  [3:0] alucontrol,
					output	[1:0] signextsignal);
                    
    wire [1:0] aluop;
    wire       branch, bne;
	
    
    maindec md(op, funct, toregaddition, memtoreg, memwrite, branch, alusrca, alusrcb, wrdst, regdst, regwrite, jadition, jump, aluop, bne, signextsignal);
    
    aludec ad(funct, op, aluop, alucontrol);
    
    assign pcsrc = (~bne & branch & zero) | (bne & branch & ~zero);
    
  endmodule
         
