`include"datapath.v"
`include"controller.v"

module mips(input         clk, reset,
            output [31:0] pc,
            input  [31:0] instr,
            output        memwrite,
            output [31:0] aluout, writedata,
            input  [31:0] readdata);

    wire toregaddition, memtoreg, branch, alusrcb, wrdst, regdst, regwrite, jadition, jump, pcsrc;
	wire [1:0] alusrca, signextsignal;
    wire [3:0] alucontrol;
  
    controller c(instr[31:26], instr[5:0], zero, toregaddition, memtoreg, memwrite, pcsrc, alusrca, alusrcb, wrdst, regdst, regwrite, jadition, jump, alucontrol, signextsignal);
                
    datapath dp(clk, reset, toregaddition, memtoreg, pcsrc, alusrca, alusrcb, wrdst, regdst, regwrite, jadition, jump, alucontrol, zero, pc, instr, aluout, writedata, readdata, signextsignal);
    
  endmodule
