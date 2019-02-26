module alu(input [31:0] a, input [31:0] b, input [3:0] alucontrol, output [31:0] aluout, output zero);
	
	reg [31:0] result;

	
	//alucontrol:010 ADD; 110 Subtract; 000 AND; 001 OR; 101 XOR; 100 XAND; 111 SLT
	
	always@(*)
	  begin
		case(alucontrol)
			4'b010: begin
						result=a+b;						//add
						if(result[31] != a[31] && result[31] != b[31]) 
							$stop; //overflow 
					end
			4'b110: begin
						result=a-b;						//sub
						if((a[31] != b[31] && result[31] == b[31])||(a != b && result == 0)) 
							$stop; //overflow 
					end
			4'b000: result=a&b;						//and
			4'b001: result=a|b;						//or
			4'b101: result=a|~b;					//xor
			4'b111: case({a[31], b[31]})			//slt
						2'b00: result=(a<b)?1:0;		//++
						2'b01: result=0;				//+-
						2'b10: result=1;				//-+
						2'b11: result=(a>b)?1:0;		//--
					endcase	
			
			//other code
			4'b1000:  result = b << a;				//sll
			4'b1010:  result = b >> a;				//srl
			4'b1001:  result = {b[15:0], 16'b0};	//lui
			4'b1011:  result=a+b;					//addu
			4'b1100:  result=a-b;					//subu
			4'b1101:  result=(a<b)?1:0;				//sltu
		endcase
		
	end
	
	assign zero = (result == 0)? 1 : 0;
	
	assign aluout = result;
	
endmodule