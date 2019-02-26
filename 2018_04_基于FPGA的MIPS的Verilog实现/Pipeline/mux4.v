module mux4 #(parameter WIDTH = 8)
            (input [WIDTH-1:0] d0, d1, d2, d3,
            input  [1:0]       s,
            output [WIDTH-1:0] y);
  
  reg [WIDTH-1:0] r;
  
  always@(*)
	   case(s)
		    2'b00: r<=d0;
		    2'b01: r<=d1;
		    2'b10: r<=d2;
		    2'b11: r<=d3;
	   endcase
  
  assign y = r;
  //assign y = s ? d1 :d0;
endmodule
