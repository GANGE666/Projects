module dmem (input clk, we,
            input [31:0] a, wd, 
            output [31:0] rd);
  reg [31:0] RAM[63:0];
  assign rd = RAM[a[31:2]]; // word aligned
  always @(posedge clk)
   if(we)
      RAM[a[31:2]] <= wd; 
endmodule


module imem (input [5:0] a, output [31:0] rd);
  reg[31:0] RAM[63:0];
  initial
    begin
      //$readmemh ("memfile.dat", RAM);
      //$readmemh ("jaltest.dat", RAM);
	  //$readmemh("sltaddsubtest.dat", RAM);
	  //$readmemh("mipstest_ext.dat", RAM);
	  //$readmemh("Test_Single_Pipelined.dat", RAM);
	  $readmemh("mipstest_pipelined.dat", RAM);
	  //$readmemh("temp.dat",RAM);
    end
  assign rd = RAM[a]; // word aligned 
endmodule