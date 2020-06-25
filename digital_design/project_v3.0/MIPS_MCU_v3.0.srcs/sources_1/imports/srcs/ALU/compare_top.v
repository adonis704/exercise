`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2019/06/30 17:13:31
// Design Name: TQ
// Module Name: compare
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

module compare_top(
    input clk,
    input rst_n,
    input [31:0]a_in,
    input [31:0]b_in,
    input [31:0]a_re_in,
    input [31:0]b_re_in,
    input replace_a,
    input replace_b,
    output wire agtb_h_o,
    output wire agtb_l_o    
);
reg [31:0] a_reg;
reg [31:0] b_reg;
wire [31:0]a;
wire [31:0]b;

assign a = (replace_a==1)? a_re_in :a_reg;
assign b = (replace_b==1)? b_re_in :b_reg;


compare compare_u_h(
    .a_in(a[31:16]),
    .b_in(b[31:16]),
    .o(agtb_h_o)
);
compare compare_u_l(
    .a_in(a[15:0]),
    .b_in(b[15:0]),
    .o(agtb_l_o)
);
always @(posedge clk or negedge rst_n)
  begin
    if (!rst_n)
      begin
        a_reg <= 0;
        b_reg <= 0;
      end
    else
      begin
        a_reg <= a_in;
        b_reg <= b_in;
      end
  end
endmodule // compare_top