`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company:
// Engineer:
//
// Create Date: 2019/07/01 16:32:54
// Design Name: TQ
// Module Name: mul_top
// Project Name:
// Target Devices:
// Tool Versions:
// Description:
// * a[31:16]*b[31:16]+a[31:16]*b[31:16]+c;
//
// Dependencies:
//
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
//
//////////////////////////////////////////////////////////////////////////////////


module mul_top(
         input clk,
         input rst_n,
         input [31:0] a_in,
         input [31:0] b_in,
         input [31:0] c_in,
         input [31:0] c_re_in,
         input replace,
         output [31:0] o
       );

wire [31:0]prod[2:1];
wire [31:0]d;
wire [31:0]s;
wire [31:0]carryo;
wire [31:0]c;
reg  [31:0]c_reg;

assign c = (replace==1)? c_re_in :c_reg;

always @(posedge clk or negedge rst_n)
  begin
    if (!rst_n)
      begin
        c_reg <= 0;
      end
    else
      begin
        c_reg <= c_in;
      end
  end


mul_comp_lcx mul_comp_u_1(
           .clk(clk),
           .rst_n(rst_n),
           .a(a_in[31:16]),
           .b(b_in[31:16]),
           .o_33(prod[1])
         );
mul_comp_lcx mul_comp_u_2(
           .clk(clk),
           .rst_n(rst_n),
           .a(a_in[15:0]),
           .b(b_in[15:0]),
           .o_33(prod[2])
         );

genvar i;
for(i=0;i<32;i=i+1)
begin
assign d[i] = ^{prod[1][i],prod[2][i],c[i]};
assign s[i] = i==0 ? d[i] : d[i]^((prod[1][i-1]&prod[2][i-1])|(c[i-1]&(prod[1][i-1]|prod[2][i-1])));
end
for(i=1; i<9; i = i+1)
  begin
    CARRY4 CARRY4_inst (
             .CO(carryo[i*4-1:i*4-4]), // 4-bit carry out
             .O(o[i*4-1:i*4-4]), // 4-bit carry chain XOR data out
             .CI((i==1) ? 0 : carryo[(i-1)*4-1]), // 1-bit carry cascade input
             .CYINIT(0), // 1-bit carry initialization
             .DI(d[i*4-1:i*4-4]), // 4-bit carry-MUX data in
             .S(s[i*4-1:i*4-4]) // 4-bit carry-MUX select input
           );
  end


endmodule
