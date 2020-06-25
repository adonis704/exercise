`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2019/07/05 11:02:06
// Design Name: 
// Module Name: MCU_01
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


module test_circuit(input clk, rst_n);
    parameter  N_CYCLE_WRITE = 32;   
    parameter  N_CYCLE_MCU2RAM = 32;
    
    (*MARK_DEBUG = "TRUE"*) wire [31:0] test_data;
    //wire [5:0] test_data_addr;
    (*MARK_DEBUG = "TRUE"*) reg [31:0] verify_data;
    //wire [5:0] verify_data_addr;
    reg en_cnt,wea_RAM;
    wire [1:0] w_en[3:0];
    reg [5:0]wr_reg_addr_cnt;
    reg [5:0]read_reg_addr_cnt;
    wire [2:0]read_mux;
    reg [2:0]read_mux_r;
    reg pc_begin;
    wire [31:0]data_out[7:0];
    wire HAL;
    wire [5:0]r_addr,w_addr;
    wire [3:0]w_e[1:0];
    assign w_e[1] = {w_en[3][1],w_en[2][1],w_en[1][1],w_en[0][1]};
    assign w_e[0] = {w_en[3][0],w_en[2][0],w_en[1][0],w_en[0][0]};


    wire [31:0] RAM_data;
    
    (*MARK_DEBUG = "TRUE"*) wire [19:0] cnt_test;
    
always @ (posedge clk or negedge rst_n)
begin
    if(!rst_n)
        read_mux_r<=0;
    else 
        read_mux_r <= read_mux;
end


    always@(posedge clk or negedge rst_n)
        begin
            if (!rst_n)
            begin 
                wr_reg_addr_cnt <= 0;
                pc_begin <= 0;
            end
            else
            begin
             if(wr_reg_addr_cnt<=N_CYCLE_WRITE)
                    begin
                        wr_reg_addr_cnt<=wr_reg_addr_cnt + 1;
                        pc_begin <= 0;
                    end
                else
                    begin
                        wr_reg_addr_cnt <= wr_reg_addr_cnt;
                        pc_begin <= 1;
                    end     
                    
               end          
        end

    always@(posedge clk or negedge rst_n)
        begin
            if(!rst_n)
            begin
                read_reg_addr_cnt <= 0;
                wea_RAM <= 0;
            end
            else if (HAL)
                    begin 
                        read_reg_addr_cnt <= (read_reg_addr_cnt <=N_CYCLE_MCU2RAM)?read_reg_addr_cnt+1:read_reg_addr_cnt;
                        wea_RAM <= (read_reg_addr_cnt <=N_CYCLE_MCU2RAM)?1:0;
                    end
                else 
                begin
                    read_reg_addr_cnt <= read_reg_addr_cnt;
                    wea_RAM <= 0;
                end
        end 

    always @(*)
     begin
        case(read_mux_r)
            0: verify_data = data_out[0];
            1: verify_data = data_out[1];
            2: verify_data = data_out[2];
            3: verify_data = data_out[3];
            4: verify_data = data_out[4];
            5: verify_data = data_out[5];
            6: verify_data = data_out[6];
            7: verify_data = data_out[7];
            default:verify_data = 32'bx;
        endcase
     end
    datapath mcu_u(
            .clk(clk),
            .rst_n(rst_n),
            .test_vector_in(test_data),
            .w_en(w_e),
            .pc_begin(pc_begin),
            .r_addr(r_addr),
            .w_addr(w_addr),
            .data_out(data_out),
            .halW(HAL)
            );
        
    cnt cnt_u(
            .clk(clk),
            .rst_n(rst_n),
            .en(1),
            .cnt_test(cnt_test));
            
    test_ROM test_ROM_u (
            .clka(clk),    // input wire clka
            .ena(1),      // input wire ena
            .addra(wr_reg_addr_cnt),  // input wire [5 : 0] addra
            .douta(test_data));  // output wire [31 : 0] douta
            
    verify_RAM verify_RAM_u (
        .clka(clk),    //  wire clka
        .ena(1),      // input wire ena
        .wea(wea_RAM),      // input wire [0 : 0] wea
        .addra(read_reg_addr_cnt),  // input wire [4 : 0] addra
        .dina(verify_data));    // input wire [31 : 0] dina
    wr_reg_addr wr_reg_addr_u(
            .clka(clk),    // input wire clka
            .ena(1),      // input wire ena
            .addra(wr_reg_addr_cnt),  // input wire [5 : 0] addra
            .douta({w_addr,w_en[3][0],w_en[0][0],w_en[3][1],w_en[0][1]})  // output wire [13 : 0] douta//{?????we5[4],we6[4]...}
            );
        assign w_en[2]=0;
        assign w_en[1]=0;
    read_reg_addr read_reg_addr_u (
            .clka(clk),    // input wire clka
            .ena(1),      // input wire ena
            .addra(read_reg_addr_cnt),  // input wire [5 : 0] addra
            .douta({r_addr,read_mux})  // output wire [11 : 0] douta
        );
    ila_0 ILA_u (
        .clk(clk), // input wire clk
        .probe0(test_data), // input wire [31:0]  probe0  
        .probe1(verify_data), // input wire [31:0]  probe1 
        .probe2(cnt_test)
	    ); // input wire [19:0]  probe2
	   
endmodule


//module MCU_01(
//    input clk, rst_n,
//    input [31:0] test_vector_in,
//    input [5:0] r_addr,
//    input [5:0] w_addr,    
//    input [3:0] w_en[1:0],
//    input pc_begin,
//    output HAL,
//    output [31:0] data_out[7:0]
//    );
//    wire [5:0]r_addr_temp[3:0];
//    wire [5:0]w_addr_temp[3:0];
//    wire [31:0]wd_temp[3:0];
//    wire [31:0]da_a_temp[3:0];
//    wire [31:0]da_b_temp[3:0];
//    wire [31:0]da_c_temp[3:0];
//    assign r_addr_temp[0] = r_addr;
//    assign r_addr_temp[1] = r_addr;
//    assign r_addr_temp[2] = r_addr;
//    assign r_addr_temp[3] = r_addr;
//    assign w_addr_temp[0] = w_addr;
//    assign w_addr_temp[1] = w_addr;
//    assign w_addr_temp[2] = w_addr;
//    assign w_addr_temp[3] = w_addr;
//    assign wd_temp[0] = test_vector_in;
//    assign wd_temp[1] = test_vector_in;
//    assign wd_temp[2] = test_vector_in;
//    assign wd_temp[3] = test_vector_in;
//    assign data_out[0] = da_a_temp[0];
//    assign data_out[1] = da_a_temp[3];
//    assign data_out[2] = da_b_temp[0];
//    assign data_out[3] = da_b_temp[3];
//    assign data_out[4] = da_c_temp[0];
//    assign data_out[5] = da_c_temp[1];
//    assign data_out[6] = da_c_temp[2];
//    assign data_out[7] = da_c_temp[3];

//    reg_file regfile_u(
//        .clk(clk),
//        .rst_n(rst_n),
//        .ra1_i(r_addr_temp),
//        .ra2_i(r_addr_temp),
//        .ra3_i(r_addr_temp),
//        .wa5_i(w_addr_temp),
//        .wa6_i(w_addr_temp),
//        .wd5_i(wd_temp),
//        .wd6_i(wd_temp),
//        .we5_i(w_en[1]),
//        .we6_i(w_en[0]),
//        .rd1_o(da_a_temp),
//        .rd2_o(da_b_temp),
//        .rd3_o(da_c_temp)
//        );
        
//endmodule

module cnt(
    input en,clk,rst_n,
    output reg [19:0] cnt_test);
        
    always @ (posedge clk, negedge rst_n)
        if(~rst_n)
            cnt_test <= 0;
        else if(en)
            cnt_test <= cnt_test + 1;
        else 
            cnt_test <= cnt_test;
endmodule           
