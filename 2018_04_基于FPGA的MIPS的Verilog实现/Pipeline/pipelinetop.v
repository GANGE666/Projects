`include"pipeline.v"

module pipelinetop(	input	clk, reset);
	
	//IF
	wire	[31:0]	instr, pcplus4;
	
	wire	[31:0]	instrD, pcplus4D;
	
	//ID
	wire			wreg, m2reg, wmem, jal;
    wire  [3:0]	alucontrol;
	wire			aluimm, shift;
	wire			wpcir;
	wire	[1:0]	pcsource;
	wire	[31:0]	bpc, jpc;
	wire	[31:0]	dpc4, da, db, dimm;
	wire	[4:0]	drn;
	
	wire        	ewreg, em2reg, ewmem, ejal;
	wire	[3:0]	alucontrolE;
	wire			ealuimm, eshift;
	wire	[31:0]	ea, eb, SignimmE, epc4, instrE;
	wire	[4:0]	WriteRegE;
	
	//EXE
	wire	[31:0]	ealu;
	wire			mwreg, mm2reg, mwmem;
	wire	[31:0]	malu, mb;
	wire	[4:0]	mrn, ern;

	//MEM
	wire	[31:0]	mdo;
	wire	wwreg, wm2reg;
	wire	[31:0]	walu, wdo;
	wire	[4:0]	wrn;

	//WB
	wire	[31:0]	wdi;
	

	IF	IF(clk, reset, bpc, da, jpc, pcsource, wpcir, instr, pcplus4);
	
	IFIDreg IFIDreg(clk, reset, wpcir, instr, pcplus4, instrD, pcplus4D);
	
	ID	ID(	clk,
			instrD, pcplus4D,
			mrn, mm2reg, mwreg, ern, em2reg, ewreg,
			wreg, m2reg, wmem, jal, alucontrol, aluimm, shift, wpcir, pcsource,
			bpc, jpc,
			ealu, malu, mdo,
			wwreg, wrn, wdi,
			da, db, dimm, drn);
			
	IDEXEreg IDEXEreg(	clk, reset,
						wreg, m2reg, wmem, jal,	alucontrol, aluimm, shift,
						ewreg, em2reg, ewmem, ejal, alucontrolE, ealuimm,eshift,
						da, db, dimm, pcplus4D, instrD, drn,
						ea, eb, SignimmE, epc4, instrE, WriteRegE);	
	
	EXE EXE(	ea, eb, SignimmE, epc4, WriteRegE,
				ejal, alucontrolE, ealuimm, eshift, ealu, ern);
	
	EXEMEMreg EXEMEMreg(	clk, reset,
							ewreg, em2reg, ewmem,
							mwreg, mm2reg, mwmem,
							ealu, eb, ern,
							malu, mb, mrn);
						
	MEM MEM(clk, mwmem, malu, mb, mdo);
	
	MEMWBreg MEMWBreg(clk, reset,
						mwreg, mm2reg,
						wwreg, wm2reg,
						malu, mdo, mrn,
						walu, wdo, wrn);
						
	WB WB(walu, wdo, wm2reg, wdi);
	
	
	
	
	
	
endmodule