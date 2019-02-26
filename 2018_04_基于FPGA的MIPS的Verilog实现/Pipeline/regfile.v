module regfile(input clk,
               input we3,
               input [4:0] ra1,ra2,wa3,
               input [31:0] wd3,
               output [31:0] rd1, rd2);
  
  reg [31:0] rf[31:0];
  
  always@(clk)
    if(we3) 
		begin 
			rf[wa3] <= wd3;
			$display("R%d = %h", wa3, wd3);
		end
      
  assign rd1 = (ra1 != 0) ? rf[ra1] : 0;
  assign rd2 = (ra2 != 0) ? rf[ra2] : 0;
  
endmodule
