`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2019/07/05 12:08:47
// Design Name: 
// Module Name: tb_mcu
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


module tb_mcu();
reg clk=0;
reg rst_n=0;

initial 
begin
    forever #10 clk = ~clk;
end

test_circuit mcu_u(.clk(clk),.rst_n(rst_n));

initial 
begin
 #20 rst_n = 1;
end

initial 
begin
#1000;
end

endmodule
