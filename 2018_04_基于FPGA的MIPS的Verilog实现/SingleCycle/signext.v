module signext(input [15:0] a, input [1:0] signextsignal, output [31:0] y);
  
	//addi : - 1
	//		 + 0
	//ori  : 0
	//andi : 1
	reg [31:0] r;
	always@(*)
		case(signextsignal)
			2'b00: r = {{16{a[15]}},a};
			2'b10: r = {16'b0, a};
			2'b11: r = {16'b1111111111111111, a};
			default : r = {{16{a[15]}},a};
		endcase
  
	assign y = r;
	 
endmodule
