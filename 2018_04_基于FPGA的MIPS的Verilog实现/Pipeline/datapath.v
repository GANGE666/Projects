`include "flopr.v"
`include "adder.v"
`include "sl2.v"
`include "mux2.v"
`include "mux4.v"
`include "regfile.v"
`include "signext.v"
`include "alu.v"


module datapath(input         clk, reset,
                input         toregaddition, memtoreg, pcsrc,
				input  [1:0]  alusrca, 
                input         alusrcb, 
				input		  wrdst, regdst,
                input         regwrite, 
				input		  jaddition, jump,
                input  [3:0]  alucontrol,
                output        zero,
                output [31:0] pc,
                input  [31:0] instr,
                output [31:0] aluout,writedata,
                input  [31:0] readdata,
				input  [1:0]  signextsignal);
                
  wire [4:0]  writereg;
  wire [31:0] pcnext, pcnextbr, pcplus4, pcbranch;
  wire [31:0] signimm, signimmsh;
  wire [31:0] rd1;
  wire [31:0] srca, srcb;
  wire [31:0] result;
  
  //reset pc signal
  flopr #(32) pcreg(clk, reset, pcnext, pc);
  
  //pcplus4 = pc + 4
  adder       pcadd1(pc, 32'b100, pcplus4);
  
  //sign extend
  signext     se(instr[15:0], signextsignal, signimm);
  //shift left
  sl2         immsh(signimm, signimmsh);
  //shift left result + pcplus4
  adder       pcadd2(pcplus4, signimmsh, pcbranch);
  
  //whether should beq
  mux2 #(32)  pcbrmux(pcplus4, pcbranch, pcsrc, pcnextbr);
  //jump ?
  mux4 #(32)  pcmux(pcnextbr, {pcplus4[31:28], instr[25:0], 2'b00}, srca, 32'b0, {jaddition, jump}, pcnext);
  
  
  
  regfile     rf(clk,regwrite, instr[25:21], instr[20:16], writereg, result, rd1, writedata);
  
  //write reg number
  mux4 #(5)   wrmux(instr[20:16], instr[15:11], 5'b11111, 5'b00000, {wrdst, regdst}, writereg);
  
  //aluout / readdata /PC+4 -> write reg
  mux4 #(32)  resmux(aluout, readdata, pcplus4, 32'b0, {toregaddition, memtoreg}, result);
  
  //reg RD2 / (const) signex -> ALU srcB
  mux2 #(32)  srcbmux(writedata, signimm, alusrcb, srcb);
  
  //reg RD1 / (const) signex -> ALU srcA
  //mux2 #(32)  srcamux(rd1, {27'b0,instr[10:6]}, alusrca, srca);
  mux4 #(32)  srcamux(rd1, {27'b0,instr[10:6]}, writedata, writedata, alusrca, srca);
  
  alu         alu(srca, srcb, alucontrol, aluout, zero);
  
endmodule
