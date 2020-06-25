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


module compare(
    input [15:0] a_in,
    input [15:0]  b_in,
    output wire o
    );
    genvar i;
    wire [7:0] gt;
    wire [7:0] eq;
    wire [3:0]carry_1;
    wire [3:0]c;
    for (i=0; i<16;i=i+2) begin
        GT GT_u(a_in[i+1:i],b_in[i+1:i],gt[i/2],eq[i/2]); end
    CARRY4 CARRY4_u1 (
      .CO(carry_1), // 4-bit carry out
      .O(), // 4-bit carry chain XOR data out
      .CI(0), // 1-bit carry cascade input
      .CYINIT(1), // 1-bit carry initialization
      .DI(gt[3:0]), // 4-bit carry-MUX data in
      .S(eq[3:0]) // 4-bit carry-MUX select input
    );
    CARRY4 CARRY4_u2 (
      .CO(c), // 4-bit carry out
      .O(), // 4-bit carry chain XOR data out
      .CI(carry_1[3]), // 1-bit carry cascade input
      .CYINIT(1), // 1-bit carry initialization
      .DI(gt[7:4]), // 4-bit carry-MUX data in
      .S(eq[7:4]) // 4-bit carry-MUX select input
    );
    assign o = c[3];
endmodule

module GT(
    input[1:0] a,
    input[1:0] b,
    output wire o_gt,
    output wire o_eq
    );
    assign o_gt =( a[0] & !b[0] & !a[1]) | (!a[1] & b[1]) | (a[0] & !b[0] & b[1]);
    assign o_eq = (!a[0] & !b[0] & !a[1] & !b[1]) | (a[0] & b[0] & !a[1] & !b[1])|( !a[0] & !b[0] & a[1] & b[1]) |( a[0] & b[0] & a[1] & b[1]);
endmodule