`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2019/07/05 08:57:19
// Design Name: 
// Module Name: testbench_o1
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


module testbench_o1;
    reg clk, rst_n, pc_begin;
    reg [31:0] test_vector_in;
    reg [5:0] r_addr, w_addr;
    reg [3:0] w_en [1:0];
    
    wire [31:0] data_out [7:0];
    wire halW;
    
    initial
    begin
        clk = 0;
        rst_n = 0;
        pc_begin = 0;
        #50 rst_n = 1;
        #50 pc_begin = 1;
        test_vector_in = 31'b0;
        r_addr = 6'b0;
        w_addr = 6'b0;
        w_en[1] = 0;
        w_en[0] = 0;
        forever # 50 clk = ~ clk;
    end
    
    datapath MCU(clk, rst_n, pc_begin, test_vector_in, r_addr, w_addr, w_en, data_out, halW);
endmodule
