`include"mem.v"
`include"ControlUnit.v"


module IF(	input			clk, reset,
			input	[31:0]	bpc, da, jpc,
			input	[1:0]	pcsource,
			input			wpcir,
			output	[31:0]	instr, pcplus4);

	wire [31:0]	pc, pcnext;
	wire [31:0] pcold;
	wire		s;
			
  //reset pc signal
  flopr #(32) pcreg(clk, reset, wpcir, pcnext, pcold);
  
  //pcplus4 = pc + 4
  adder       pcadd1(pc, 32'b100, pcplus4);
  //Fetch Instruction
  imem imem (pc[7:2], instr);
  
  /*
  //4->1 
  mux4 #(32)  pcmux(pcplus4, bpc, da, jpc, pcsource, pcnext);
  
  //mux2 for update pcvalue
  assign	s = pcsource == 0 ? 0 : 1;
  mux2 #(32) newpcmux(pcold, pcnext, s, pc);
  */
  mux4 #(32) pcchose(pcold, bpc, da, jpc, pcsource, pc);
  adder		pcadd2(pc, 32'b100, pcnext);
  
endmodule

module IFIDreg(	input			clk, reset, wpcir,
				input	[31:0]	instrF, pcplus4F,
				output	[31:0]	instrD, pcplus4D);
				
	reg [31:0] instr, pcplus4;
	
	//write
	always@(posedge clk or posedge reset)
		begin 
			if(reset == 1)
				begin
					instr <= 0;
					pcplus4 <= 0;
				end
			else if(~wpcir)
				begin 
					instr <= instrF;
					pcplus4 <= pcplus4F;
				end	
		end
	
	//read
	assign instrD = instr;
	assign pcplus4D = pcplus4;
	
endmodule

module ID(	input			clk,
			//from IFIDreg
			input	[31:0]	instr, dpc4,			
			
			//input signal
			input	[4:0]	mrn,
			input			mm2reg, mwreg,
			input	[4:0]	ern,
			input			em2reg, ewreg,			
			
			//output control signal
			output			wreg, m2reg, wmem, jal, 
            output  [3:0]	alucontrol,
			output			aluimm, shift,
			
			output			wpcir,
			output	[1:0]	pcsource,
			//control signal end
			
			//to IF
			output	[31:0]	bpc, jpc,
			
			//data conflict
			input	[31:0]	ealu, malu, mmo,
			
			//write regfile
			input			regwriteW, // from WB
			input	[4:0]	writeregW, // from WB
			input	[31:0]	result,

			//to IDEXEreg
			output	[31:0]	da, db, dimm,
			output	[4:0]	drn);
	
	wire	[1:0]	signextsignal;
	wire	regrt;
	wire	rsrtequ;
	wire 	[1:0]	fwda, fwdb;
	wire	[31:0]	rd1, rd2;
	wire	sllsrl;
	wire	[31:0]	dimmold;
	
	//
	ControlUnit Controller (instr[31:26], instr[5:0], 
							ern, mrn, instr[25:21], instr[20:16], ewreg, mwreg, em2reg, mm2reg,
							wreg, m2reg, wmem, jal, alucontrol, aluimm, shift, wpcir, pcsource,
							signextsignal,regrt, rsrtequ, sllsrl,
							fwda, fwdb);
	
	//jpc
	assign jpc = {dpc4[31:28], instr[25:0], 2'b00};	
	
	//shift left
	assign bpc = dpc4 + {{4'b0}, instr[15:0], 2'b00};							
							
	regfile     rf(clk,regwriteW, instr[25:21], instr[20:16], writeregW, result, rd1, rd2);
	
	//da db mux
	mux4 #(32)	damux(rd1, ealu, malu, mmo, fwda, da);
	mux4 #(32)	dbmux(rd2, ealu, malu, mmo, fwdb, db);
	
	assign rsrtequ = ~|(da^db);

	//write reg number
    mux2 #(5)   wrmux(instr[20:16], instr[15:11], regrt, drn);
	
	//sign extend
    signext     se(instr[15:0], signextsignal, dimmold);
	
	mux2 #(32) sllmux(dimmold, {27'b0, instr[10:6]}, sllsrl, dimm);

endmodule

module IDEXEreg(	input			clk, reset, 
					//input control signal
					input        	wreg, m2reg, wmem, jal,
					input	[3:0]	alucontrolD,
					input			aluimm, shift,
					//input control signal end
					//output control signal
					output        	ewreg, em2reg, ewmem, ejal,
					output	[3:0]	alucontrolE,
					output			ealuimm, eshift,
					//output control signal end
					
					input	[31:0]	da, db, SignimmD, dpc, instrD,
					input	[4:0]	WriteRegD,
					output	[31:0]	ea, eb, SignimmE, epc4, instrE,
					output	[4:0]	WriteRegE
					);
	
	reg	[9:0] 	signals;
	reg [31:0]	RD1, RD2, Signimm, pcplus4, instr;
	reg [4:0]	WriteReg;
	
	//write
	always@(posedge clk or posedge reset)
		begin
			if(reset == 1)
				begin
					signals <= 0;
					RD1 <= 0;
					RD2 <= 0;
					WriteReg <= 0;
					Signimm <= 0;
					pcplus4 <= 0;
				end
			else
				begin 
					signals <= {wreg, m2reg, wmem, jal, alucontrolD, aluimm, shift};
					RD1 <= da;
					RD2 <= db;
					WriteReg <= WriteRegD;
					Signimm <= SignimmD;
					pcplus4 <= dpc;
					instr <= instrD;
				end		
		end
	
	//read
	assign {ewreg, em2reg, ewmem, ejal, alucontrolE, ealuimm, eshift} = signals;
	assign	ea = RD1;
	assign	eb = RD2;
	assign	WriteRegE = WriteReg;
	assign	SignimmE = Signimm;
	assign	epc4 = pcplus4;
	assign	instrE = instr;
	
	
	
endmodule

module EXE(	input	[31:0]	ea, eb, eimm,
			input	[31:0]	epc4,
			input	[4:0]	ern0,
			//signal
			input			ejal,
			input	[3:0]	alucontrol,
			input			ealuimm, eshift,
			//signal end
			output	[31:0]	ealu,
			output	[4:0]	ern);

	wire [31:0]	srcb, srca, aluout;
	wire		zero;
	
	//ea eb
	mux2 #(32)	eamux(ea, eimm, eshift, srca);
	mux2 #(32)	ebmux(eb, eimm, ealuimm, srcb);
	
	alu         alu(srca, srcb, alucontrol, aluout, zero);
	
	//alu mux
	//mux2 #(32)	elaumux(aluout, epc4 + 4, ejal, ealu);
	mux2 #(32)	elaumux(aluout, epc4, ejal, ealu);
	
	assign ern = ejal ? 5'b11111 : ern0;

endmodule

module EXEMEMreg(	input			clk, reset,
					//input control signal
					input			ewreg, em2reg, ewmem,
					//input control signal end
					//output control signal
					output			mwreg, mm2reg, mwmem,
					//output control signal end
					input	[31:0]	ealu, eb,
					input	[4:0]	ern,
					output	[31:0]	malu, mb,
					output	[4:0]	mrn
					);
					
	reg	[2:0] 	signals;
	reg [31:0]	alu, b;
	reg	[4:0]	rn;
	
	//write
	always@(posedge clk or posedge reset)
		begin 
			if(reset == 1)
				begin
					signals <= 0;
					alu <= 0;
					b <= 0;
					rn <= 0;
				end
			else
				begin
					signals <= {ewreg, em2reg, ewmem};
					alu <= ealu;
					b <= eb;
					rn <= ern;
				end
		end			
	
	//read
	assign	{mwreg, mm2reg, mwmem} = signals;
	assign	malu = alu;
	assign	mb = b;
	assign	mrn = rn;
		
endmodule

module MEM(	input			clk,
			//signal
			input			mwmem,
			//signal end
			input	[31:0]	malu, mb,
			output	[31:0]	readdata);
			
	dmem	dmem(clk, mwmem, malu, mb, readdata);
			
endmodule

module MEMWBreg(	input			clk, reset,
					//signal
					input	mwreg, mm2reg,
					output	wwreg, wm2reg,
					//signal end
					input	[31:0]	malu, mdo,
					input	[4:0]	mrn,
					output	[31:0]	walu, wdo,
					output	[4:0]	wrn);
					
	reg	[1:0] 	signals;
	reg [31:0]	alu, ddo;
	reg	[4:0]	rn;
	
	//write
	always@(posedge clk or posedge reset)
		begin 
			if(reset == 1)
				begin
					signals <= 0;
					alu <= 0;
					ddo <= 0;
					rn <= 0;
				end
			else
				begin
					signals <= {mwreg, mm2reg};
					alu <= malu;
					ddo <= mdo;
					rn <= mrn;
				end
		end			
	
	//read
	assign	{wwreg, wm2reg} = signals;
	assign	walu = alu;
	assign	wdo = ddo;
	assign	wrn = rn;
	
endmodule

module WB(	input	[31:0]	walu, mdo,
			input			wm2reg,
			output	[31:0]	wdi);
			
	//aluout / readdata /PC+4 -> write reg
  mux2 #(32)  resmux(walu, mdo, wm2reg, wdi);
	
	
endmodule