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
               output			wreg, m2reg, wmem, jal, jrjalr, aluimm, shift, 
			   output	[1:0]	signextsignal,
			   output 			regrt, jump, beq, bne, sllsrl,
			   output			i_rs, i_rt,
			   output	[1:0]	aluop);
  
  reg [10:0] controls;
  assign {wreg, regrt, shift, aluimm, wmem, m2reg, jump, aluop, i_rs, i_rt} = controls;
  //assign {regwrite, regdst, alusrca, alusrcb, branch, memwrite, memtoreg, jump, aluop} = controls;
  assign jal = ((op == 6'b000011)||(op == 6'b000000 && funct == 6'b001001)) ? 1 : 0; //Jr Jalr
  assign jrjalr = ((op == 6'b000000 && funct == 6'b001000)||(op == 6'b000000 && funct == 6'b001001)) ? 1 : 0; //Jr Jalr
  assign signextsignal = (op == 6'b001100)?2'b11:((op == 6'b001101)?2'b10:2'b00); //andi ori other
  
  always @(*)
    case(op)
      6'b000000: 
        case(funct)
          6'b000000: controls = 11'b11100001101; //SLL
		  6'b000010: controls = 11'b11100001101; //SRL
		  6'b001000: controls = 11'b00000010010; //Jr
		  6'b001001: controls = 11'b11000010010; //Jalr
          default:   controls = 11'b11000001111; //Rtyp
        endcase
      6'b100011: controls = 11'b10010100001; //LW
      6'b101011: controls = 11'b00011000001; //SW
      6'b000100: controls = 11'b00000000111; //BEQ
	  6'b000101: controls = 11'b00000000111; //BNE
      6'b001000: controls = 11'b10010000011; //ADDI
	  6'b001101: controls = 11'b10010001111; //ORI
	  6'b001100: controls = 11'b10010001111; //ANDI
	  6'b001111: controls = 11'b10010001101; //LUI
	  6'b001010: controls = 11'b10010001111; //SLTI
      6'b000010: controls = 11'b00000010000; //J
	  6'b000011: controls = 11'b10000010010; //Jar
      default:   controls = 11'bxxxxxxxxxxx; //???
    endcase
	
	assign beq = (op == 6'b000100) ? 1 : 0; //BEQ	
	assign bne = (op == 6'b000101) ? 1 : 0; //BNE
	assign sllsrl = (op == 6'b000000) && ((funct == 6'b000000) || (funct == 6'b000010)) ? 1 : 0; //SLL SRL
	/*
	assign jadition = ((op == 6'b000000 && funct == 6'b001000)||(op == 6'b000000 && funct == 6'b001001)) ? 1 : 0; //Jr Jalr
	assign wrdst = (op == 6'b000011) ? 1 : 0; //Jal
	assign toregaddition = ((op == 6'b000011)||(op == 6'b000000 && funct == 6'b001001)) ? 1 : 0; //Jal Jalr
	assign signextsignal = (op == 6'b001100)?2'b11:((op == 6'b001101)?2'b10:2'b00); //andi ori other
	*/
	
  endmodule
  
module ControlUnit(  input   [5:0] op, funct,
					input	[4:0]	ern, mrn, rs, rt,
					input	[4:0]	ewreg, mwreg, em2reg, mm2reg,
					output			wreg, m2reg, wmem, jal, 
					output  [3:0]	alucontrol,
					output			aluimm, shift,
					output			wpcir,
					output	[1:0]	pcsource, signextsignal,
					output			regrt,
					input			rsrtequ,
					output			sllsrl,
					output 	[1:0]	fwda, fwdb);
                    
    wire [1:0] 	aluop;
	wire		jump, beq, bne, b;
	wire		i_rs, i_rt;
	wire		wregorg, wmemorg, stall;
    
    maindec md(op, funct,
				wregorg, m2reg, wmemorg, jal, jrjalr,
				aluimm, shift,
				signextsignal, regrt,
				jump, beq, bne,
				sllsrl,
				i_rs, i_rt,
				aluop);
	
    aludec ad(funct, op, aluop, alucontrol);
    
	assign b = (beq & rsrtequ) | (bne & ~rsrtequ);
	
	assign pcsource = ~(jump | jrjalr) ? ( ~(b) ? 2'b00 : 2'b01) : (jrjalr ? 2'b10 : 2'b11); 
	
	reg [1:0] dam, dbm;
	always @ (ewreg, mwreg, ern, mrn, em2reg, mm2reg, rs, rt) 	begin
		dam = 2'b00;
		
		if(ewreg &(ern != 0) & (ern == rs) & ~em2reg) begin
			dam = 2'b01;
		end else begin
			if(mwreg & (mrn != 0) & (mrn == rs) && ~mm2reg) begin
				dam = 2'b10;
			end else begin
				if(mwreg & (mrn != 0) & (mrn == rs) & mm2reg) begin
					dam = 2'b11;
				end
			end
		end
		
		dbm = 2'b00;
		if(ewreg &(ern != 0) & (ern == rt) & ~em2reg) begin
			dbm = 2'b01;
		end else begin
			if(mwreg & (mrn != 0) & (mrn == rt) && ~mm2reg) begin
				dbm = 2'b10;
			end else begin
				if(mwreg & (mrn != 0) & (mrn == rt) & mm2reg) begin
					dbm = 2'b11;
				end
			end
		end
		
	end
	assign fwda = dam;
	assign fwdb = dbm;
	
	assign wpcir = ewreg & em2reg & (ern != 0) &(i_rs & (ern == rs) | i_rt & (ern == rt));
    //assign wpcir = ~stall;
	assign wreg = wregorg & ~wpcir;
	assign wmem = wmemorg & ~wpcir;
	
  endmodule
         
