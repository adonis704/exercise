`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company:
// Engineer:
//
// Create Date: 2019/07/01 16:29:28
// Design Name: TQ
// Module Name: ALU
// Project Name:
// Target Devices:
// Tool Versions:
// Description:
// * a[31:16]*b[31:16]+a[31:16]*b[31:16]+c;
// Dependencies:
//
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
//
//////////////////////////////////////////////////////////////////////////////////


module ALU (
         input clk,
         input rst_n,
         input [31:0] a_in,
         input [31:0] b_in,
         input [31:0] c_in,
         input [31:0] a_r_2_in,
         input [31:0] b_r_2_in,
         input [31:0] c_r_in,
         input  replace_a_2,
         input  replace_b_2,
         input  replace_c,
         output [1:0] a_gt_b_o,
         output [31:0] d_o
       );
parameter OP_MUL = 00;
parameter OP_CMP = 11;
parameter OP_SHF = 01;

// multiply
mul_top mul_top_u(
          .clk(clk),
          .rst_n(rst_n),
          .a_in(a_in),
          .b_in(b_in),
          .c_in(c_in),
          .c_re_in(c_r_in),
          .replace({replace_a_2,replace_b_2,replace_c}),
          .o(d_o)
        );

// compare
compare_top compare_top_u(
    .clk                     ( clk               ),
    .rst_n                   ( rst_n             ),
    .a_in                    ( a_in       [31:0] ),
    .b_in                    ( b_in       [31:0] ),
    .a_re_in                 ( a_r_2_in     [31:0] ),
    .b_re_in                 ( b_r_2_in     [31:0] ),
    .replace_a               ( replace_a_2         ),
    .replace_b               ( replace_b_2         ),
    .agtb_h_o                ( a_gt_b_o[1]          ),
    .agtb_l_o                ( a_gt_b_o[0]          )
        );

// shift

endmodule
