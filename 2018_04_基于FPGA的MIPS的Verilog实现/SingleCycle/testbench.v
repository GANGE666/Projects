`timescale 1ps/1ps
`include "top.v"
/*
module testbench();

  reg [31:0] a,b;
  reg [2:0] aluc;
  wire z;
  wire [31:0] r, r2;
  reg clk;
  
  always #5 clk=~clk;
  
  initial begin
    #0 a<=32'H00000011;
    #0 b<=32'H00000001;
    #0 aluc<=3'b110;   
    
  end
  
  alu myalu(a,b,aluc,r,z);
  
  assign r2 = a&b;
  
  

endmodule
*/

module _testbench();
  reg clk;
  reg reset;
  wire [31:0] writedata, dataadr;
  wire memwrite;
// instantiate device to be tested
  top dut (clk, reset, writedata, dataadr, memwrite);
// initialize test
  initial 
    begin
      reset <= 1; # 22; reset <= 0;
    end
// generate clock to sequence tests 
  always
    begin
      clk <= 1; # 5; clk <= 0; # 5;
    end
    
    // check results 
    always @ (negedge clk)
      begin
        if (memwrite) begin
          if (dataadr === 84 & writedata === 7) begin 
            $display ("Simulation succeeded");
            $stop;
          end else if (dataadr !== 80) begin 
            $display ("Simulation failed");
            $stop;
          end
        end
      end
      
endmodule
    
    
    