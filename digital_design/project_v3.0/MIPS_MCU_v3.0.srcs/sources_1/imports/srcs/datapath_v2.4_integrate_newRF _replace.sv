parameter SUPERSCALAR_AMT = 4;

parameter WIDTH = 32;

parameter WIDTH_RF_ADDR = 6;//regfile地址位宽
parameter NUM_RF = 64;//寄存器数量，NUM_RF = 2 ^ WIDTH_RF_ADDR

parameter WIDTH_IM_ADDR = 10;//instr_mem地址位宽
parameter WIDTH_INSTR = 32;//instr位宽
parameter NUM_IM = 1024;//指令寄存器数量

parameter WIDTH_ALUCrtl = 2;
parameter NULL = 2'b00;

parameter CPIA = 2'b00;
parameter CPID = 2'b01;
parameter CPXA = 2'b10;
parameter CPXD = 2'b11;

//D flip flop
module flopr #(parameter WIDTH = 32, Depth = SUPERSCALAR_AMT) 
            (input clk, rst_n,
            input [WIDTH-1:0] d [Depth-1:0],
            output reg [WIDTH-1:0] q [Depth-1:0]);
    genvar i;
    
    generate
        for(i=0;i<SUPERSCALAR_AMT;i=i+1)
        begin
            always @ (posedge clk, negedge rst_n)
                if(~rst_n)  q[i]<=0;
        
                else        q[i]<=d[i];
        end
    endgenerate
endmodule

//1-bit Depth D flip flop
module flopr1 #(parameter WIDTH = 32)
            (input clk, rst_n,
            input [WIDTH-1:0] d,
            output reg [WIDTH-1:0] q);
    
    always @ (posedge clk, negedge rst_n)
                if(~rst_n)  q<=0;
        
                else        q<=d;
endmodule

//NOTE:PCreg专用，带使能，不设置深度
module flopren #(parameter WIDTH = 32)
                (input clk, rst_n, en,
                input [WIDTH-1:0] d,
                output reg [WIDTH-1:0] q);

    always @ (posedge clk, negedge rst_n)
        if(~rst_n)  q <= 0;
        else if(en) q <= d;
        else    q <= q;
endmodule
            
module mux3(
            input [WIDTH-1:0] d0, d1, d2,
            input [1:0] s,
            output [WIDTH-1:0] y);

    assign y = s[1] ? d2 : (s[0] ? d1 : d0);
endmodule

//3个读端，两个写端
//NOTE:没有序号为4的端口
//NOTE:第0号寄存器的内容始终为0
/*
module reg_file(
                input clk,
                input we,
                input [WIDTH_RF_ADDR-1:0] ra1 [SUPERSCALAR_AMT-1:0],ra2 [SUPERSCALAR_AMT-1:0], ra3 [SUPERSCALAR_AMT-1:0], 
                wa5 [SUPERSCALAR_AMT-1:0], wa6 [SUPERSCALAR_AMT-1:0],
                input [31:0] wd5 [SUPERSCALAR_AMT-1:0], wd6 [SUPERSCALAR_AMT-1:0],
                output [31:0] rd1 [SUPERSCALAR_AMT-1:0], rd2 [SUPERSCALAR_AMT-1:0], rd3 [SUPERSCALAR_AMT-1:0]);

    reg [31:0] rf [NUM_RF-1:0];

    genvar i;
    
    generate
        for(i=0;i<SUPERSCALAR_AMT;i=i+1)
        begin
            always @ (posedge clk)
                if(we) 
                begin
                    rf[wa5[i]] <= wd5[i];
                    rf[wa6[i]] <= wd6[i];
                end
        end
    endgenerate

    generate
        for(i=0;i<SUPERSCALAR_AMT;i=i+1)
        begin
            assign rd1[i] = (ra1[i] != 0)? rf[ra1[i]] : 0;
            assign rd2[i] = (ra2[i] != 0)? rf[ra2[i]] : 0;
            assign rd3[i] = (ra3[i] != 0)? rf[ra3[i]] : 0;
        end
    endgenerate
endmodule
*/

module instr_mem_fit_in_datapath(
                input clk, rst_n,
                input [WIDTH_IM_ADDR-1:0] a,
                output [WIDTH_INSTR-1:0] rd [SUPERSCALAR_AMT-1:0]);

    wire ena;

    assign ena = 1'b1;
    
    instr_mem_01 instr_mem_01(
        .clka(clk),
        .ena(ena),
        .addra(a),
        .douta(rd[0]));
      
    instr_mem_02 instr_mem_02(
        .clka(clk),
        .ena(ena),
        .addra(a),
        .douta(rd[1]));
        
    instr_mem_03 instr_mem_03(
        .clka(clk),
        .ena(ena),
        .addra(a),
        .douta(rd[2])); 
        
    instr_mem_04 instr_mem_04(
        .clka(clk),
        .ena(ena),
        .addra(a),
        .douta(rd[3]));           
      
endmodule

module switch(
                input [1:0] switch_op,//第一位0代表i，第一位1代表x
                input [31:0] rsM, rtM,
                input [1:0] agtbM,
                output reg [31:0] rsNM, rtNM);//switch之后的rs和rt
    
    reg [1:0] d;
    
    always @ (*)
    begin
        case (switch_op)
            CPIA:
                begin
                if(agtbM[1])
                    d[1] = 1;
                else
                    d[1] = 0;
                if(agtbM[0])
                    d[0] = 1;
                else 
                    d[0] = 0;
                end
            CPID:
                begin
                if(agtbM[1])
                    d[1] = 0;
                else 
                    d[1] = 1;
                if(agtbM[0])
                    d[0] = 0;
                else 
                    d[0] = 1;
                end
            CPXA:
                begin
                if(agtbM[1])
                    d[1] = 1;
                else
                    d[1] = 0;
                if(agtbM[0])
                    d[0] = 1;
                else 
                    d[0] = 0;
                end
            CPXD:
                begin
                if(agtbM[1])
                    d[1] = 0;
                else 
                    d[1] = 1;
                if(agtbM[0])
                    d[0] = 0;
                else 
                    d[0] = 1;
                end
            default:
                d = 2'b0;
        endcase
    end
    
    always @ (*)
    begin
        if(~switch_op[1])//寄存器内部比较的情况
            case(d)
            2'b00: //
                begin
                rsNM = rsM;
                rtNM = rtM;
                end
            2'b01:
                begin 
                rsNM = rsM;
                rtNM = {rtM[15:0], rtM[31:16]};
                end
            2'b10:
                begin
                rsNM = {rsM[15:0], rsM[31:16]};
                rtNM = rtM;
                end
            2'b11:
                begin
                rsNM = {rsM[15:0], rsM[31:16]};
                rtNM = {rtM[15:0], rtM[31:16]};
                end
            endcase
        else //2个寄存器之间比较
            case(d)
            2'b00:
                begin
                rsNM = rsM;
                rtNM = rtM;
                end
            2'b01:
                begin
                rsNM = {rsM[31:16], rtM[15:0]};
                rtNM = {rtM[31:16], rsM[15:0]};
                end
            2'b10:
                begin
                rsNM = {rtM[31:16], rsM[15:0]};
                rtNM = {rsM[31:16], rtM[15:0]};
                end
            2'b11:
                begin
                rsNM = rtM;
                rtNM = rsM;
                end
            endcase
        end
endmodule

//NOTE:虚假的RAM
module ram(
            input wire [31:0] ra [SUPERSCALAR_AMT-1:0],
            output wire [31:0] rd [SUPERSCALAR_AMT-1:0]);
    
    genvar i;
    generate
        for(i=0;i<SUPERSCALAR_AMT;i=i+1)
        begin
            assign rd[i] = ra[i];
        end
    endgenerate
endmodule

module datapath_fetch(
                    input clk, rst_n, PCen,
                    output [WIDTH_INSTR-1:0] instrF [SUPERSCALAR_AMT-1:0]);
    
    wire [WIDTH_IM_ADDR-1:0] PC, PCNext;

    flopren       #(WIDTH_IM_ADDR)    PCflopr(clk, rst_n, PCen, PCNext, PC);    
    instr_mem_fit_in_datapath        IM(clk, rst_n, PC,instrF);
    assign PCNext = PC + 1;//NOTE:是+1还是+4
endmodule

module datapath_decode(
                        input clk, rst_n, pc_begin,
                        input we [SUPERSCALAR_AMT-1:0],
                        input addr_select [SUPERSCALAR_AMT-1:0],//0代表选rs和rt的地址，1代表选rd的地址

                        input [WIDTH_INSTR-1:0] instrD [SUPERSCALAR_AMT-1:0],
                        input [31:0] rsW [SUPERSCALAR_AMT-1:0], rtW [SUPERSCALAR_AMT-1:0],
                        input [WIDTH_RF_ADDR-1:0] ArsW [SUPERSCALAR_AMT-1:0], ArtW [SUPERSCALAR_AMT-1:0],
                        
                        input [31:0] test_vector_in,
                        input [5:0] r_addr, w_addr,
                        input [3:0] w_en [1:0],

                        output [31:0] rsD [SUPERSCALAR_AMT-1:0], rtD [SUPERSCALAR_AMT-1:0], rdD [SUPERSCALAR_AMT-1:0],
                        output [WIDTH_RF_ADDR-1:0] ArsdD [SUPERSCALAR_AMT-1:0], ArtD [SUPERSCALAR_AMT-1:0],
                        output halD,

                        output [31:0] data_out [7:0]);
    
    wire [5:0]instrD_rs [SUPERSCALAR_AMT-1:0], instrD_rt [SUPERSCALAR_AMT-1:0], instrD_rd [SUPERSCALAR_AMT-1:0];
    wire [SUPERSCALAR_AMT-1:0] we5_i, we6_i;

    wire [5:0] ra1 [SUPERSCALAR_AMT-1:0], ra2 [SUPERSCALAR_AMT-1:0], ra3 [SUPERSCALAR_AMT-1:0], wa5 [SUPERSCALAR_AMT-1:0], wa6 [SUPERSCALAR_AMT-1:0];
    wire [31:0] wd5 [SUPERSCALAR_AMT-1:0], wd6 [SUPERSCALAR_AMT-1:0];
    wire [SUPERSCALAR_AMT-1:0] we5, we6;
    
        
    genvar i;
    
    generate
        for(i=0;i<SUPERSCALAR_AMT;i=i+1)
        begin
            assign wa5[i] = ~pc_begin ? w_addr : ArsW[i];
            assign wa6[i] = ~pc_begin ? w_addr : ArtW[i];
            assign wd5[i] = ~pc_begin ? test_vector_in : rsW[i];
            assign wd6[i] = ~pc_begin ? test_vector_in : rtW[i];
            assign ra1[i] = ~pc_begin ? r_addr :instrD_rs[i];
            assign ra2[i] = ~pc_begin ? r_addr :instrD_rt[i];
            assign ra3[i] = ~pc_begin ? r_addr :instrD_rd[i];
        end
    endgenerate
    
    assign we5 = ~pc_begin ? w_en[1] : we5_i;
    assign we6 = ~pc_begin ? w_en[0] : we6_i;

    assign data_out[0] = rsD[0];
    assign data_out[1] = rsD[3];
    assign data_out[2] = rtD[0];
    assign data_out[3] = rtD[3];
    assign data_out[4] = rdD[0]; 
    assign data_out[5] = rdD[1]; 
    assign data_out[6] = rdD[2]; 
    assign data_out[7] = rdD[3]; 

    assign halD = instrD[0][31];
    
    generate
        for(i=0;i<SUPERSCALAR_AMT;i=i+1)
        begin
            assign instrD_rs[i] = instrD[i][26:21];
            assign instrD_rt[i] = instrD[i][20:15];
            assign instrD_rd[i] = instrD[i][14:9];
            assign we5_i[i] = we[i];
            assign we6_i[i] = we[i];
        end
    endgenerate
    
    reg_file rf (
        .clk    (clk),
        .rst_n(rst_n),
        .ra1_i    (ra1),//$rs
        .ra2_i    (ra2),//$rt
        .ra3_i    (ra3),//$rd
        .wa5_i    (wa5),//也可能是rd的地址
        .wa6_i    (wa6),
        .wd5_i    (wd5),//也可能是rd
        .wd6_i    (wd6),
        .we5_i    (we5),
        .we6_i    (we6),
        .rd1_o    (rsD),
        .rd2_o    (rtD),
        .rd3_o    (rdD));

    generate
        for(i=0;i<SUPERSCALAR_AMT;i=i+1)
        begin
            assign ArsdD[i] = addr_select[i] ? instrD_rd[i] : instrD_rs[i];//$rs或$rd的地址
            assign ArtD[i] = addr_select[i] ? 5'b0 : instrD_rt[i];//$rt或$0的地址
        end
    endgenerate
endmodule

//TODO:重定向
//NOTE:如果要修改超标量的值，这里也要改
module datapath_execute1(
                        input clk, rst_n,
                        input [31:0] rsE1 [SUPERSCALAR_AMT-1:0], rtE1 [SUPERSCALAR_AMT-1:0], rdE1 [SUPERSCALAR_AMT-1:0], rd_reM [SUPERSCALAR_AMT-1:0],
                        input replace [SUPERSCALAR_AMT-1:0],//c的重定向控制信号。0不重定向，1重定向
                        input replace_a_1 [SUPERSCALAR_AMT-1:0], replace_b_1 [SUPERSCALAR_AMT-1:0],
                        input replace_a_2 [SUPERSCALAR_AMT-1:0], replace_b_2 [SUPERSCALAR_AMT-1:0],
                        input [1:0] mux_a_1_replace_op [SUPERSCALAR_AMT-1:0], mux_b_1_replace_op [SUPERSCALAR_AMT-1:0],
                        input [1:0] mux_a_2_replace_op [SUPERSCALAR_AMT-1:0], mux_b_2_replace_op [SUPERSCALAR_AMT-1:0],
                        input [31:0] arsW [SUPERSCALAR_AMT-1:0], rtNW [SUPERSCALAR_AMT-1:0],

                        output [1:0] agtbE1 [SUPERSCALAR_AMT-1:0],
                        output [31:0] alu_outE1 [SUPERSCALAR_AMT-1:0]);
    

    wire [31:0] a_r_1 [SUPERSCALAR_AMT-1:0], b_r_1 [SUPERSCALAR_AMT-1:0];
    wire [31:0] a_r_2 [SUPERSCALAR_AMT-1:0], b_r_2 [SUPERSCALAR_AMT-1:0];
    wire [31:0] ALUSrcA [SUPERSCALAR_AMT-1:0], ALUSrcB [SUPERSCALAR_AMT-1:0];
    genvar i;   

    generate
        for(i=0;i<SUPERSCALAR_AMT;i=i+1)
        begin
            assign a_r_1[i] = ~mux_a_1_replace_op [i][1] ? (~mux_a_1_replace_op[i][0] ? arsW[0] : arsW[1]) : (~mux_a_1_replace_op [i][0] ? arsW[2] : arsW[3]);
            assign b_r_1[i] = ~mux_b_1_replace_op [i][1] ? (~mux_b_1_replace_op[i][0] ? rtNW[0] : rtNW[1]) : (~mux_b_1_replace_op [i][0] ? rtNW[2] : rtNW[3]);
            assign ALUSrcA[i] = ~replace_a_1[i] ? rsE1[i] : a_r_1[i];
            assign ALUSrcB[i] = ~replace_b_1[i] ? rtE1[i] : b_r_1[i];
            assign a_r_2[i] = ~mux_a_2_replace_op [i][1] ? (~mux_a_2_replace_op[i][0] ? arsW[0] : arsW[1]) : (~mux_a_2_replace_op [i][0] ? arsW[2] : arsW[3]);
            assign b_r_2[i] = ~mux_b_2_replace_op [i][1] ? (~mux_b_2_replace_op[i][0] ? rtNW[0] : rtNW[1]) : (~mux_b_2_replace_op [i][0] ? rtNW[2] : rtNW[3]);

            ALU     alu(
                .clk        (clk),
                .rst_n     (rst_n),
                .a_in      (ALUSrcA[i]),
                .b_in      (ALUSrcB[i]),
                .c_in      (rdE1[i]),
                .a_r_2_in   (a_r_2[i]),
                .b_r_2_in   (b_r_2[i]),
                .c_r_in     (rd_reM[i]),
                .replace_a_2    (replace_a_2[i]),
                .replace_b_2    (replace_b_2[i]),
                .replace_c  (replace[i]),
                .d_o    (alu_outE1[i]),
                .a_gt_b_o       (agtbE1[i]));
        end
    endgenerate


endmodule

module datapath_memory(
                        input [1:0] switch_opM [SUPERSCALAR_AMT-1:0],
                        input [1:0] ars_opM [SUPERSCALAR_AMT-1:0],//MUX选择信号

                        input [31:0] alu_outM [SUPERSCALAR_AMT-1:0],
                        input [1:0] agtbM [SUPERSCALAR_AMT-1:0],
                        input [31:0] rsM [SUPERSCALAR_AMT-1:0], rtM [SUPERSCALAR_AMT-1:0],
                        output [31:0] rsNM [SUPERSCALAR_AMT-1:0], rtNM [SUPERSCALAR_AMT-1:0],
                        output [31:0] arsM [SUPERSCALAR_AMT-1:0]);
    
    wire [31:0] ReadData [SUPERSCALAR_AMT-1:0];
    
    genvar i;
    
    generate
        for(i=0;i<SUPERSCALAR_AMT;i=i+1)
        begin
            switch  switch(
                .switch_op  (switch_opM[i]),
                .rsM        (rsM[i]),
                .rtM        (rtM[i]),
                .agtbM      (agtbM[i]),
                .rsNM       (rsNM[i]),
                .rtNM       (rtNM[i]));
        end
    endgenerate

    ram     ram(alu_outM, ReadData);

    generate
        for(i=0;i<SUPERSCALAR_AMT;i=i+1)
        begin
            mux3    mux_ars(alu_outM[i], ReadData[i], rsNM[i], ars_opM[i], arsM[i]);
        end
    endgenerate
endmodule

module decoder(
                input [4:0] instr_opcodeD [SUPERSCALAR_AMT-1:0],
                output reg RegWriteD [SUPERSCALAR_AMT-1:0], AddrSelectD [SUPERSCALAR_AMT-1:0],
                output reg [WIDTH_ALUCrtl-1:0] AluCtrlD [SUPERSCALAR_AMT-1:0],
                output reg [1:0] switch_opD [SUPERSCALAR_AMT-1:0], ars_opD [SUPERSCALAR_AMT-1:0]);

    genvar i;
    generate 
        for(i=0;i<SUPERSCALAR_AMT;i=i+1)
        begin
            always @(*)
            begin
                AddrSelectD[i] = instr_opcodeD[i][2];
                
                if(instr_opcodeD[i][4])
                begin
                    RegWriteD[i] = 0;
                    AluCtrlD[i] = NULL;
                    switch_opD[i] = 2'b00;
                    ars_opD[i] = 2'b00;
                end
                else if (instr_opcodeD[i][2])
                begin
                    RegWriteD[i] = 1;
                    AluCtrlD[i] = NULL; //FIXME:
                    switch_opD[i] = 2'b00; //FIXME:用不用改成2b'xx?
                    ars_opD[i] = 2'b00;
                end
                else
                begin
                    RegWriteD[i] = 1;
                    AluCtrlD[i] = NULL; //FIXME:
                    switch_opD[i] = instr_opcodeD[i][1:0];
                    ars_opD[i] = 2'b10;
                end
            end
        end
    endgenerate
endmodule
        
module clash_control (
    input AddrSelectE1 [SUPERSCALAR_AMT-1:0],
    input [WIDTH_RF_ADDR-1:0] ArsdE1 [SUPERSCALAR_AMT-1:0], ArsdE2 [SUPERSCALAR_AMT-1:0], ArsdM [SUPERSCALAR_AMT-1:0], ArsdW [SUPERSCALAR_AMT-1:0],
    input [WIDTH_RF_ADDR-1:0] ArtE1 [SUPERSCALAR_AMT-1:0], ArtE2 [SUPERSCALAR_AMT-1:0], ArtW [SUPERSCALAR_AMT-1:0],
    output reg replace [SUPERSCALAR_AMT-1:0],
    output reg replace_a_1 [SUPERSCALAR_AMT-1:0], replace_b_1 [SUPERSCALAR_AMT-1:0],
    output reg [1:0] mux_a_1_replace_op [SUPERSCALAR_AMT-1:0], mux_b_1_replace_op [SUPERSCALAR_AMT-1:0],
    output reg replace_a_2 [SUPERSCALAR_AMT-1:0], replace_b_2 [SUPERSCALAR_AMT-1:0],
    output reg [1:0] mux_a_2_replace_op [SUPERSCALAR_AMT-1:0], mux_b_2_replace_op [SUPERSCALAR_AMT-1:0]);

    genvar i;

    //乘法的重定向
    generate   
        for(i=0;i<SUPERSCALAR_AMT;i=i+1)
        begin
            always @ (*)
                if(AddrSelectE1[i] && ArsdE1[i] == ArsdM[i])
                    replace[i] = 1;
                else
                    replace[i] = 0;
        end
    endgenerate

    //排序的重定向
    generate
        for(i=0;i<SUPERSCALAR_AMT;i=i+1)
        begin
            always @ (*)
                casex({ArsdE1[i] == ArsdW[0], ArsdE1[i] == ArsdW[1], ArsdE1[i] == ArsdW[2], ArsdE1[i] == ArsdW[3]})
                    {1'b1,1'bx,1'bx,1'bx}: 
                        begin
                        replace_a_1[i] = 1;
                        mux_a_1_replace_op[i] = 2'b00;
                        end
                    {1'bx,1'b1,1'bx,1'bx}:
                        begin
                        replace_a_1[i] = 1;
                        mux_a_1_replace_op[i] = 2'b01;
                        end
                    {1'bx,1'bx,1'b1,1'bx}:
                        begin
                        replace_a_1[i] = 1;
                        mux_a_1_replace_op[i] = 2'b10;
                        end
                    {1'bx,1'bx,1'bx,1'b1}:
                        begin
                        replace_a_1[i] = 1;
                        mux_a_1_replace_op[i] = 2'b11;
                        end
                    {1'b0,1'b0,1'b0,1'b0}:
                        begin
                        replace_a_1[i] = 0;
                        mux_a_1_replace_op[i] = 2'bxx;
                        end
                    default:
                        begin
                        replace_a_1[i] = 0;
                        mux_a_1_replace_op[i] = 2'bxx;
                        end
                endcase

            always @ (*)
                casex({ArtE1[i] == ArtW[0], ArtE1[i] == ArtW[1], ArtE1[i] == ArtW[2], ArtE1[i] == ArtW[3]})
                    {1'b1,1'bx,1'bx,1'bx}: 
                        begin
                        replace_b_1[i] = 1;
                        mux_b_1_replace_op[i] = 2'b00;
                        end
                    {1'bx,1'b1,1'bx,1'bx}:
                        begin
                        replace_b_1[i] = 1;
                        mux_b_1_replace_op[i] = 2'b01;
                        end
                    {1'bx,1'bx,1'b1,1'bx}:
                        begin
                        replace_b_1[i] = 1;
                        mux_b_1_replace_op[i] = 2'b10;
                        end
                    {1'bx,1'bx,1'bx,1'b1}:
                        begin
                        replace_b_1[i] = 1;
                        mux_b_1_replace_op[i] = 2'b11;
                        end
                    {1'b0,1'b0,1'b0,1'b0}:
                        begin
                        replace_b_1[i] = 0;
                        mux_b_1_replace_op[i] = 2'bxx;
                        end
                    default:
                        begin
                        replace_b_1[i] = 0;
                        mux_b_1_replace_op[i] = 2'bxx;
                        end
                endcase                   

            always @ (*)
                casex({ArsdE2[i] == ArsdW[0], ArsdE2[i] == ArsdW[1], ArsdE2[i] == ArsdW[2], ArsdE2[i] == ArsdW[3]})
                    {1'b1,1'bx,1'bx,1'bx}: 
                        begin
                        replace_a_2[i] = 1;
                        mux_a_2_replace_op[i] = 2'b00;
                        end
                    {1'bx,1'b1,1'bx,1'bx}:
                        begin
                        replace_a_2[i] = 1;
                        mux_a_2_replace_op[i] = 2'b01;
                        end
                    {1'bx,1'bx,1'b1,1'bx}:
                        begin
                        replace_a_2[i] = 1;
                        mux_a_2_replace_op[i] = 2'b10;
                        end
                    {1'bx,1'bx,1'bx,1'b1}:
                        begin
                        replace_a_2[i] = 1;
                        mux_a_2_replace_op[i] = 2'b11;
                        end
                    {1'b0,1'b0,1'b0,1'b0}:
                        begin
                        replace_a_2[i] = 0;
                        mux_a_2_replace_op[i] = 2'bxx;
                        end
                    default:
                        begin
                        replace_a_2[i] = 0;
                        mux_a_2_replace_op[i] = 2'bxx;
                        end
                endcase

            always @ (*)
                casex({ArtE2[i] == ArtW[0], ArtE2[i] == ArtW[1], ArtE2[i] == ArtW[2], ArtE2[i] == ArtW[3]})
                    {1'b1,1'bx,1'bx,1'bx}: 
                        begin
                        replace_b_2[i] = 1;
                        mux_b_2_replace_op[i] = 2'b00;
                        end
                    {1'bx,1'b1,1'bx,1'bx}:
                        begin
                        replace_b_2[i] = 1;
                        mux_b_2_replace_op[i] = 2'b01;
                        end
                    {1'bx,1'bx,1'b1,1'bx}:
                        begin
                        replace_b_2[i] = 1;
                        mux_b_2_replace_op[i] = 2'b10;
                        end
                    {1'bx,1'bx,1'bx,1'b1}:
                        begin
                        replace_b_2[i] = 1;
                        mux_b_2_replace_op[i] = 2'b11;
                        end
                    {1'b0,1'b0,1'b0,1'b0}:
                        begin
                        replace_b_2[i] = 0;
                        mux_b_2_replace_op[i] = 2'bxx;
                        end
                    default:
                        begin
                        replace_b_2[i] = 0;
                        mux_b_2_replace_op[i] = 2'bxx;
                        end
                endcase  
        end
    endgenerate                 

endmodule

module datapath(input clk, rst_n, pc_begin,

                input [31:0] test_vector_in,
                input [5:0] r_addr, w_addr,
                input [3:0] w_en [1:0],

                output [31:0] data_out [7:0],
                output halW);
    
    //ND for not delay, 即没有经过寄存器时延的
    wire [WIDTH_INSTR-1:0] instrF [SUPERSCALAR_AMT-1:0], instrD [SUPERSCALAR_AMT-1:0], instrD_ND [SUPERSCALAR_AMT-1:0];
    wire RegWriteD [SUPERSCALAR_AMT-1:0] , RegWriteE1 [SUPERSCALAR_AMT-1:0], RegWriteE2 [SUPERSCALAR_AMT-1:0], 
        RegWriteM [SUPERSCALAR_AMT-1:0], RegWriteW [SUPERSCALAR_AMT-1:0];
    wire AddrSelectD [SUPERSCALAR_AMT-1:0], AddrSelectE1 [SUPERSCALAR_AMT-1:0];
    wire [WIDTH_ALUCrtl-1:0] AluCtrlD [SUPERSCALAR_AMT-1:0], AluCtrlE1 [SUPERSCALAR_AMT-1:0];
    wire [1:0] switch_opD [SUPERSCALAR_AMT-1:0], switch_opE1 [SUPERSCALAR_AMT-1:0], switch_opE2 [SUPERSCALAR_AMT-1:0],
        switch_opM [SUPERSCALAR_AMT-1:0], ars_opD [SUPERSCALAR_AMT-1:0], ars_opE1 [SUPERSCALAR_AMT-1:0], ars_opE2 [SUPERSCALAR_AMT-1:0], 
        ars_opM [SUPERSCALAR_AMT-1:0];

    wire [31:0] rsD [SUPERSCALAR_AMT-1:0], rtD [SUPERSCALAR_AMT-1:0], rdD [SUPERSCALAR_AMT-1:0], rsE1 [SUPERSCALAR_AMT-1:0], 
        rtE1 [SUPERSCALAR_AMT-1:0], rdE1 [SUPERSCALAR_AMT-1:0], rsE2 [SUPERSCALAR_AMT-1:0], rtE2 [SUPERSCALAR_AMT-1:0], rsM [SUPERSCALAR_AMT-1:0], 
        rtM [SUPERSCALAR_AMT-1:0], rsNM [SUPERSCALAR_AMT-1:0], arsM [SUPERSCALAR_AMT-1:0], rtNM [SUPERSCALAR_AMT-1:0], 
        arsW_ND [SUPERSCALAR_AMT-1:0], rtNW_ND [SUPERSCALAR_AMT-1:0], arsW [SUPERSCALAR_AMT-1:0], rtNW [SUPERSCALAR_AMT-1:0];
    wire [31:0] rd_reM [SUPERSCALAR_AMT-1:0]; //重定向自M阶段的rd
    wire [WIDTH_RF_ADDR-1:0] ArsdD [SUPERSCALAR_AMT-1:0], ArtD [SUPERSCALAR_AMT-1:0], ArsdE1 [SUPERSCALAR_AMT-1:0], ArtE1 [SUPERSCALAR_AMT-1:0], 
        ArsdE2 [SUPERSCALAR_AMT-1:0], ArtE2 [SUPERSCALAR_AMT-1:0], ArsdM [SUPERSCALAR_AMT-1:0], ArtM [SUPERSCALAR_AMT-1:0], 
        ArsdW_ND [SUPERSCALAR_AMT-1:0], ArtW_ND [SUPERSCALAR_AMT-1:0], ArsdW [SUPERSCALAR_AMT-1:0],
        ArtW [SUPERSCALAR_AMT-1:0];

    wire [1:0] agtbE1 [SUPERSCALAR_AMT-1:0], agtbE2 [SUPERSCALAR_AMT-1:0], agtbM [SUPERSCALAR_AMT-1:0];
    wire [31:0] alu_outE1 [SUPERSCALAR_AMT-1:0], alu_outE2 [SUPERSCALAR_AMT-1:0], alu_outM [SUPERSCALAR_AMT-1:0];

    wire [4:0] instrD_opcode [SUPERSCALAR_AMT-1:0];

    wire replace [SUPERSCALAR_AMT-1:0];
    wire replace_a_1 [SUPERSCALAR_AMT-1:0], replace_b_1 [SUPERSCALAR_AMT-1:0];
    wire replace_a_2 [SUPERSCALAR_AMT-1:0], replace_b_2 [SUPERSCALAR_AMT-1:0];
    wire [1:0] mux_a_1_replace_op [SUPERSCALAR_AMT-1:0], mux_b_1_replace_op [SUPERSCALAR_AMT-1:0];
    wire [1:0] mux_a_2_replace_op [SUPERSCALAR_AMT-1:0], mux_b_2_replace_op [SUPERSCALAR_AMT-1:0];

    //NOTE:由于指令数是4的倍数，这里仅提供一个hal信号
    wire halD, halE1, halE2, halM;
    
    genvar i;
    
    generate
        for(i=0;i<SUPERSCALAR_AMT;i=i+1)
        begin
            assign instrD_opcode[i] = instrD[i][31:27];
        end
    endgenerate

    //datapath开始
    datapath_fetch  fetch(clk, rst_n, pc_begin, instrF);

    generate
        for(i=0;i<SUPERSCALAR_AMT;i=i+1)
        begin
            assign instrD_ND[i] = instrF[i];
        end
    endgenerate
    flopr #(WIDTH_INSTR) instrreg(clk, rst_n, instrF, instrD);

    decoder decoder(instrD_opcode, RegWriteD, AddrSelectD, AluCtrlD, switch_opD, ars_opD);    
    //TODO:控制信号RegWrite, Addr_SelectD
    datapath_decode decode(
        .clk(clk), 
        .rst_n(rst_n), 
        .pc_begin(pc_begin),
        .we(RegWriteW),
        .addr_select(AddrSelectD), 
        .instrD(instrD_ND), 
        .rsW(arsW_ND), 
        .rtW(rtNW_ND), 
        .ArsW(ArsdW_ND), 
        .ArtW(ArtW_ND), 
        .test_vector_in(test_vector_in),
        .r_addr(r_addr),
        .w_addr(w_addr),
        .w_en(w_en),
        .rsD(rsD), 
        .rtD(rtD), 
        .rdD(rdD), 
        .ArsdD(ArsdD), 
        .ArtD(ArtD),
        .halD(halD),
        .data_out(data_out));

    flopr #(32) rfreg1(clk, rst_n, rsD, rsE1);
    flopr #(32) rfreg2(clk, rst_n, rtD, rtE1);
    flopr #(32) rfreg3(clk, rst_n, rdD, rdE1);
    flopr #(WIDTH_RF_ADDR) rfreg4(clk, rst_n, ArsdD, ArsdE1);
    flopr #(WIDTH_RF_ADDR) rfreg5(clk, rst_n, ArtD, ArtE1);
    
    flopr #(1) crtlD2E1_1(clk, rst_n, RegWriteD, RegWriteE1);
    flopr #(WIDTH_ALUCrtl) crtlD2E1_2(clk, rst_n, AluCtrlD, AluCtrlE1);
    flopr #(2) crtlD2E1_3(clk, rst_n, switch_opD, switch_opE1);
    flopr #(2) crtlD2E1_4(clk, rst_n, ars_opD, ars_opE1);
    flopr #(1) crtlD2E1_5(clk, rst_n, AddrSelectD, AddrSelectE1);
    flopr1 #(1) crtlD2E1_6(clk, rst_n, halD, halE1);

    //TODO:控制信号alu_ctrlE1
    datapath_execute1 exe1(
        .clk(clk),
        .rst_n(rst_n), 
        .rsE1(rsE1), 
        .rtE1(rtE1), 
        .rdE1(rdE1), 
        .rd_reM(alu_outM), 
        .replace(replace),
        .replace_a_1(replace_a_1),
        .replace_b_1(replace_b_1),
        .replace_a_2(replace_a_2),
        .replace_b_2(replace_b_2),
        .mux_a_1_replace_op(mux_a_1_replace_op),
        .mux_b_1_replace_op(mux_b_1_replace_op),
        .mux_a_2_replace_op(mux_a_2_replace_op),
        .mux_b_2_replace_op(mux_b_2_replace_op),
        .arsW(arsW),
        .rtNW(rtNW),
        .agtbE1(agtbE1), 
        .alu_outE1(alu_outE1));

    //NOTE:空流水线阶段E2
    flopr #(32) alureg1(clk, rst_n, alu_outE1, alu_outM);
    flopr #(2)  alureg2(clk, rst_n, agtbE1, agtbM);

    flopr #(32) alureg3(clk, rst_n, rsE1, rsE2);
    flopr #(32) alureg4(clk, rst_n, rtE1, rtE2);
    flopr #(WIDTH_RF_ADDR) alureg5(clk, rst_n, ArsdE1, ArsdE2);
    flopr #(WIDTH_RF_ADDR) alureg6(clk, rst_n, ArtE1, ArtE2);
    
    flopr #(1) crtlE12E2_1(clk, rst_n, RegWriteE1, RegWriteE2);
    flopr #(2) crtlE12E2_2(clk, rst_n, switch_opE1, switch_opE2);
    flopr #(2) crtlE12E2_3(clk, rst_n, ars_opE1, ars_opE2);

    flopr1 #(1) crtlE12E2_4(clk, rst_n, halE1, halE2);

    
    //flopr #(32) alureg7(clk, rst_n, alu_outE2, alu_outM);
    //flopr #(2)  alureg8(clk, rst_n, agtbE2, agtbM);
    flopr #(32) alureg9(clk, rst_n, rsE2, rsM);
    flopr #(32) alureg10(clk, rst_n, rtE2, rtM);
    flopr #(WIDTH_RF_ADDR) alureg11(clk, rst_n, ArsdE2, ArsdM);
    flopr #(WIDTH_RF_ADDR) alureg12(clk, rst_n, ArtE2, ArtM);   

    flopr #(1) crtlE22M_1(clk, rst_n, RegWriteE2, RegWriteM);
    flopr #(2) crtlE22M_2(clk, rst_n, switch_opE2, switch_opM);
    flopr #(2) crtlE22M_3(clk, rst_n, ars_opE2, ars_opM); 
    
    flopr1 #(1) crtlE22M_4(clk, rst_n, halE2, halM);

    //重定向部分
    clash_control clash_control(
        .AddrSelectE1(AddrSelectE1),
        .ArsdE1(ArsdE1),
        .ArsdE2(ArsdE2),
        .ArsdM(ArsdM),
        .ArsdW(ArsdW),
        .ArtE1(ArtE1),
        .ArtE2(ArtE2),
        .ArtW(ArtW),
        .replace(replace),
        .replace_a_1(replace_a_1),
        .replace_b_1(replace_b_1),
        .mux_a_1_replace_op(mux_a_1_replace_op),
        .mux_b_1_replace_op(mux_b_1_replace_op),
        .replace_a_2(replace_a_2),
        .replace_b_2(replace_b_2),
        .mux_a_2_replace_op(mux_a_2_replace_op),
        .mux_b_2_replace_op(mux_b_2_replace_op));

    //TODO:控制信号switch_op, ars_op
    datapath_memory mem(switch_opM, ars_opM, alu_outM, agtbM, rsM, rtM, rsNM, rtNM, arsM);

    flopr #(1) crtlM2W(clk, rst_n, RegWriteM, RegWriteW);
    flopr #(WIDTH_RF_ADDR) dataM2W_1(clk, rst_n, ArsdM, ArsdW);
    flopr #(WIDTH_RF_ADDR) dataM2W_2(clk, rst_n, ArtM, ArtW);
    flopr #(32) dataM2W_3(clk, rst_n, arsM, arsW);
    flopr #(32) dataM2W_4(clk, rst_n, rtNM, rtNW);

    flopr1 #(1) crtlM2W_1(clk, rst_n, halM, halW);
   
    generate
        for(i=0;i<SUPERSCALAR_AMT;i=i+1)
        begin
            assign arsW_ND[i] = arsM[i];
            assign rtNW_ND[i] = rtNM[i];
            assign ArsdW_ND[i] = ArsdM[i];
            assign ArtW_ND[i] = ArtM[i];
        end
    endgenerate

endmodule


module reg_file(
         input clk,
         input rst_n,
         input [WIDTH_RF_ADDR-1:0] ra1_i [SUPERSCALAR_AMT-1:0],ra2_i [SUPERSCALAR_AMT-1:0], ra3_i [SUPERSCALAR_AMT-1:0],
         wa5_i [SUPERSCALAR_AMT-1:0] , wa6_i [SUPERSCALAR_AMT-1:0] ,
         input  [31:0] wd5_i [SUPERSCALAR_AMT-1:0], wd6_i [SUPERSCALAR_AMT-1:0],
         input  [SUPERSCALAR_AMT-1:0]we5_i,
         input  [SUPERSCALAR_AMT-1:0]we6_i,
         output [31:0] rd1_o [SUPERSCALAR_AMT-1:0], rd2_o [SUPERSCALAR_AMT-1:0], rd3_o [SUPERSCALAR_AMT-1:0])
         ;

    genvar i;

    reg  [31:0] rf_w_d [1:8];
    wire [31:0] rf_r_d [1:8];
    reg  [2:0] rf_w_addr [1:8];
    reg  [2:0] rf_r_addr [1:8];
    reg  rf_reg_c_e[1:4];
    reg  rf_we [1:8];

    // addr Decoder
    reg_addr_decoder reg_addr_decoder_u_r(
                    .addr_1_i(ra1_i),
                    .addr_2_i(ra2_i),
                    .we({we6_i,we5_i}),
                    .addr_o(rf_r_addr)
                    );
    reg_addr_decoder reg_addr_decoder_u_w(
                    .addr_1_i(wa5_i),
                    .addr_2_i(wa6_i),
                    .we({we6_i,we5_i}),
                    .addr_o(rf_w_addr),
                    .we_o(rf_we [1:8])
                    );
    // output_mux
    output_mux output_mux_u_rd1(
    .clk(clk),
    .rst_n(rst_n),
        .addr__i(ra1_i),
        .d1_i(rf_r_d[1]),
        .d2_i(rf_r_d[3]),
        .d3_i(rf_r_d[5]),
        .d4_i(rf_r_d[7]),
        .d_o(rd1_o)
    );
    output_mux output_mux_u_rd2(
        .clk(clk),
    .rst_n(rst_n),
        .addr__i(ra2_i),
        .d1_i(rf_r_d[2]),
        .d2_i(rf_r_d[4]),
        .d3_i(rf_r_d[6]),
        .d4_i(rf_r_d[8]),
        .d_o(rd2_o)
    );
    // wd_demux
    reg_data_demux reg_data_demux_u(
        .addr_1_i(wa5_i),
        .addr_2_i(wa6_i),
        .da_1_i(wd5_i),
        .da_2_i(wd6_i),
        .da_o(rf_w_d)
    );
    // w_regc_en
    generate
        for (i=2;i<=8;i=i+2)
            begin
               assign rf_reg_c_e[i/2] = ((wa5_i[i/2-1]==54+i) || (wa5_i[i/2-1]==55+i)) ? we5_i[i/2-1] : 0;
            end
    endgenerate

    generate
        for(i=1;i<=8;i=i+1)
        begin
//            if (i>=3&&i<=6)
//                begin
//                    blk_mem_shared blk_mem_u_3456 (
//                        //write
//                        .clka(clk),    // input wire clka
//                        .ena(1),      // input wire ena
//                        .wea(rf_we[i]),      // input wire [0 : 0] wea
//                        .addra(rf_w_addr[i]),  // input wire [1 : 0] addra
//                        .dina(rf_w_d[i]),    // input wire [31 : 0] dina
//                        // read
//                        .clkb(clk),    // input wire clkb
//                        .enb(1),      // input wire enb
//                        .addrb(rf_r_addr[i]),  // input wire [1 : 0] addrb
//                        .doutb(rf_r_d[i])  // output wire [31 : 0] doutb
//                        );
//                end
//            else 
//                begin
//                    blk_mem_only blk_mem_u_1278 (
//                        //write
//                        .clka(clk),    // input wire clka
//                        .ena(1),      // input wire ena
//                        .wea(rf_we[i]),      // input wire [0 : 0] wea
//                        .addra(rf_w_addr[i]),  // input wire [2 : 0] addra
//                        .dina(rf_w_d[i]),    // input wire [31 : 0] dina
//                        // read
//                        .clkb(clk),    // input wire clkb
//                        .enb(1),      // input wire enb
//                        .addrb(rf_r_addr[i]),  // input wire [2 : 0] addrb
//                        .doutb(rf_r_d[i])  // output wire [31 : 0] doutb
//                    );
//                end
            if (i<=4)
                begin
                    blk_mem_c blk_mem_u_c (
                        //write
                        .clka(clk),    // input wire clka
                        .ena(1),      // input wire ena
                        .wea(rf_reg_c_e[i]),      // input wire [0 : 0] wea
                        .addra(wd5_i[i-1]),  // input wire [2 : 0] addra
                        .dina(wd5_i[i-1]),    // input wire [31 : 0] dina
                        // read
                        .clkb(clk),    // input wire clkb
                        .enb(1),      // input wire enb
                        .addrb(ra3_i[i-1]),  // input wire [2 : 0] addrb
                        .doutb(rd3_o[i-1])  // output wire [31 : 0] doutb
                    );
                end
        end
    endgenerate

blk_mem_shared_3 blk_mem_u_3 (
                        //write
                        .clka(clk),    // input wire clka
                        .ena(1),      // input wire ena
                        .wea(rf_we[3]),      // input wire [0 : 0] wea
                        .addra(rf_w_addr[3]),  // input wire [1 : 0] addra
                        .dina(rf_w_d[3]),    // input wire [31 : 0] dina
                        // read
                        .clkb(clk),    // input wire clkb
                        .enb(1),      // input wire enb
                        .addrb(rf_r_addr[3]),  // input wire [1 : 0] addrb
                        .doutb(rf_r_d[3])  // output wire [31 : 0] doutb
                        );
blk_mem_shared_4 blk_mem_u_4 (
                        //write
                        .clka(clk),    // input wire clka
                        .ena(1),      // input wire ena
                        .wea(rf_we[4]),      // input wire [0 : 0] wea
                        .addra(rf_w_addr[4]),  // input wire [1 : 0] addra
                        .dina(rf_w_d[4]),    // input wire [31 : 0] dina
                        // read
                        .clkb(clk),    // input wire clkb
                        .enb(1),      // input wire enb
                        .addrb(rf_r_addr[4]),  // input wire [1 : 0] addrb
                        .doutb(rf_r_d[4])  // output wire [31 : 0] doutb
                        );
blk_mem_shared_5 blk_mem_u_5 (
                        //write
                        .clka(clk),    // input wire clka
                        .ena(1),      // input wire ena
                        .wea(rf_we[5]),      // input wire [0 : 0] wea
                        .addra(rf_w_addr[5]),  // input wire [1 : 0] addra
                        .dina(rf_w_d[5]),    // input wire [31 : 0] dina
                        // read
                        .clkb(clk),    // input wire clkb
                        .enb(1),      // input wire enb
                        .addrb(rf_r_addr[5]),  // input wire [1 : 0] addrb
                        .doutb(rf_r_d[5])  // output wire [31 : 0] doutb
                        );
blk_mem_shared_6 blk_mem_u_6 (
                        //write
                        .clka(clk),    // input wire clka
                        .ena(1),      // input wire ena
                        .wea(rf_we[6]),      // input wire [0 : 0] wea
                        .addra(rf_w_addr[6]),  // input wire [1 : 0] addra
                        .dina(rf_w_d[6]),    // input wire [31 : 0] dina
                        // read
                        .clkb(clk),    // input wire clkb
                        .enb(1),      // input wire enb
                        .addrb(rf_r_addr[6]),  // input wire [1 : 0] addrb
                        .doutb(rf_r_d[6])  // output wire [31 : 0] doutb
                        );

blk_mem_only_1 blk_mem_u_1 (
                        //write
                        .clka(clk),    // input wire clka
                        .ena(1),      // input wire ena
                        .wea(rf_we[1]),      // input wire [0 : 0] wea
                        .addra(rf_w_addr[1]),  // input wire [1 : 0] addra
                        .dina(rf_w_d[1]),    // input wire [31 : 0] dina
                        // read
                        .clkb(clk),    // input wire clkb
                        .enb(1),      // input wire enb
                        .addrb(rf_r_addr[1]),  // input wire [1 : 0] addrb
                        .doutb(rf_r_d[1])  // output wire [31 : 0] doutb
                        );
blk_mem_only_2 blk_mem_u_2 (
                        //write
                        .clka(clk),    // input wire clka
                        .ena(1),      // input wire ena
                        .wea(rf_we[2]),      // input wire [0 : 0] wea
                        .addra(rf_w_addr[2]),  // input wire [1 : 0] addra
                        .dina(rf_w_d[2]),    // input wire [31 : 0] dina
                        // read
                        .clkb(clk),    // input wire clkb
                        .enb(1),      // input wire enb
                        .addrb(rf_r_addr[2]),  // input wire [1 : 0] addrb
                        .doutb(rf_r_d[2])  // output wire [31 : 0] doutb
                        ); 
blk_mem_only_7 blk_mem_u_7 (
                        //write
                        .clka(clk),    // input wire clka
                        .ena(1),      // input wire ena
                        .wea(rf_we[7]),      // input wire [0 : 0] wea
                        .addra(rf_w_addr[7]),  // input wire [1 : 0] addra
                        .dina(rf_w_d[7]),    // input wire [31 : 0] dina
                        // read
                        .clkb(clk),    // input wire clkb
                        .enb(1),      // input wire enb
                        .addrb(rf_r_addr[7]),  // input wire [1 : 0] addrb
                        .doutb(rf_r_d[7])  // output wire [31 : 0] doutb
                        );          
blk_mem_only_8 blk_mem_u_8 (
                        //write
                        .clka(clk),    // input wire clka
                        .ena(1),      // input wire ena
                        .wea(rf_we[8]),      // input wire [0 : 0] wea
                        .addra(rf_w_addr[8]),  // input wire [1 : 0] addra
                        .dina(rf_w_d[8]),    // input wire [31 : 0] dina
                        // read
                        .clkb(clk),    // input wire clkb
                        .enb(1),      // input wire enb
                        .addrb(rf_r_addr[8]),  // input wire [1 : 0] addrb
                        .doutb(rf_r_d[8])  // output wire [31 : 0] doutb
                        );                                    
                                            
                        
endmodule

module reg_addr_decoder(
    input [5:0]addr_1_i[3:0],
    input [5:0]addr_2_i[3:0],
    input [7:0]we,
    output reg [2:0]addr_o[1:8],
    output reg we_o[1:8]
  );
  always @(*)
  begin
      casex ({addr_1_i[0],addr_1_i[1]})
          {6'bx,6'd1},{6'd1,6'bx}: {we_o[1], addr_o [1]} = {we[0]|we[1], 3'd0};
          {6'bx,6'd4},{6'd4,6'bx}: {we_o[1], addr_o [1]} = {we[0]|we[1], 3'd1};
          {6'bx,6'd6},{6'd6,6'bx}: {we_o[1], addr_o [1]} = {we[0]|we[1], 3'd2};
          {6'bx,6'd7},{6'd7,6'bx}: {we_o[1], addr_o [1]} = {we[0]|we[1], 3'd3};
          {6'bx,6'd33},{6'd33,6'bx}: {we_o[1], addr_o [1]} = {we[0]|we[1], 3'd4};
          {6'bx,6'd36},{6'd36,6'bx}: {we_o[1], addr_o [1]} = {we[0]|we[1], 3'd5};
          {6'bx,6'd38},{6'd38,6'bx}: {we_o[1], addr_o [1]} = {we[0]|we[1], 3'd6};
          {6'bx,6'd39},{6'd39,6'bx}: {we_o[1], addr_o [1]} = {we[0]|we[1], 3'd7};
          default: {we_o[1], addr_o [1]} = {1'b0, 3'bx};
      endcase
      casex ({addr_2_i[0],addr_2_i[1]})
          {6'bx,6'd2},{6'd2,6'bx}: {we_o[2], addr_o [2]} = {we[4]|we[5], 3'd0};
          {6'bx,6'd3},{6'd3,6'bx}: {we_o[2], addr_o [2]} = {we[4]|we[5], 3'd1};
          {6'bx,6'd5},{6'd5,6'bx}: {we_o[2], addr_o [2]} = {we[4]|we[5], 3'd2};
          {6'bx,6'd8},{6'd8,6'bx}: {we_o[2], addr_o [2]} = {we[4]|we[5], 3'd3};
          {6'bx,6'd34},{6'd34,6'bx}: {we_o[2], addr_o [2]} = {we[4]|we[5], 3'd4};
          {6'bx,6'd35},{6'd35,6'bx}: {we_o[2], addr_o [2]} = {we[4]|we[5], 3'd5};
          {6'bx,6'd37},{6'd37,6'bx}: {we_o[2], addr_o [2]} = {we[4]|we[5], 3'd6};
          {6'bx,6'd40},{6'd40,6'bx}: {we_o[2], addr_o [2]} = {we[4]|we[5], 3'd7};
          default: {we_o[2], addr_o [2]} = {1'b0, 3'bx};
      endcase
      casex ({addr_1_i[2],addr_1_i[3]})
          {6'bx,6'd25},{6'd25,6'bx}: {we_o[7], addr_o [7]} = {we[2]|we[3], 3'd0};
          {6'bx,6'd28},{6'd28,6'bx}: {we_o[7], addr_o [7]} = {we[2]|we[3], 3'd1};
          {6'bx,6'd30},{6'd30,6'bx}: {we_o[7], addr_o [7]} = {we[2]|we[3], 3'd2};
          {6'bx,6'd31},{6'd31,6'bx}: {we_o[7], addr_o [7]} = {we[2]|we[3], 3'd3};
          {6'bx,6'd41},{6'd41,6'bx}: {we_o[7], addr_o [7]} = {we[2]|we[3], 3'd4};
          {6'bx,6'd44},{6'd44,6'bx}: {we_o[7], addr_o [7]} = {we[2]|we[3], 3'd5};
          {6'bx,6'd46},{6'd46,6'bx}: {we_o[7], addr_o [7]} = {we[2]|we[3], 3'd6};
          {6'bx,6'd47},{6'd47,6'bx}: {we_o[7], addr_o [7]} = {we[2]|we[3], 3'd7};
          default: {we_o[7], addr_o [7]} = {1'b0, 3'bx};
      endcase
      casex ({addr_2_i[2],addr_2_i[3]})
          {6'bx,6'd26},{6'd26,6'bx}: {we_o[8], addr_o [8]} = {we[6]|we[7], 3'd0};
          {6'bx,6'd27},{6'd27,6'bx}: {we_o[8], addr_o [8]} = {we[6]|we[7], 3'd1};
          {6'bx,6'd29},{6'd29,6'bx}: {we_o[8], addr_o [8]} = {we[6]|we[7], 3'd2};
          {6'bx,6'd32},{6'd32,6'bx}: {we_o[8], addr_o [8]} = {we[6]|we[7], 3'd3};
          {6'bx,6'd42},{6'd42,6'bx}: {we_o[8], addr_o [8]} = {we[6]|we[7], 3'd4};
          {6'bx,6'd43},{6'd43,6'bx}: {we_o[8], addr_o [8]} = {we[6]|we[7], 3'd5};
          {6'bx,6'd45},{6'd45,6'bx}: {we_o[8], addr_o [8]} = {we[6]|we[7], 3'd6};
          {6'bx,6'd48},{6'd48,6'bx}: {we_o[8], addr_o [8]} = {we[6]|we[7], 3'd7};
          default: {we_o[8], addr_o [8]} = {1'b0, 3'bx};
      endcase
      casex ({addr_1_i[0],addr_1_i[1],addr_1_i[2],addr_1_i[3]})
        {6'd10,6'bx,6'bx,6'bx},{6'bx,6'd10,6'bx,6'bx},{6'bx,6'bx,6'd10,6'bx},{6'bx,6'bx,6'bx,6'd10}: {we_o[3],addr_o[3] }={ |we[3:0], 3'd0};
        {6'd11,6'bx,6'bx,6'bx},{6'bx,6'd11,6'bx,6'bx},{6'bx,6'bx,6'd11,6'bx},{6'bx,6'bx,6'bx,6'd11}: {we_o[3],addr_o[3] }={ |we[3:0], 3'd1};
        {6'd13,6'bx,6'bx,6'bx},{6'bx,6'd13,6'bx,6'bx},{6'bx,6'bx,6'd13,6'bx},{6'bx,6'bx,6'bx,6'd13}: {we_o[3],addr_o[3] }={ |we[3:0], 3'd2};
        {6'd16,6'bx,6'bx,6'bx},{6'bx,6'd16,6'bx,6'bx},{6'bx,6'bx,6'd16,6'bx},{6'bx,6'bx,6'bx,6'd16}: {we_o[3],addr_o[3] }={ |we[3:0], 3'd3};
        default: {we_o[3],addr_o[3] }={1'b0,3'bx}; 
      endcase
      casex ({addr_2_i[0],addr_2_i[1],addr_2_i[2],addr_2_i[3]})
        {6'd9,6'bx,6'bx,6'bx},{6'bx,6'd9,6'bx,6'bx},{6'bx,6'bx,6'd9,6'bx},{6'bx,6'bx,6'bx,6'd9}:{we_o[4],addr_o[4] }={ |we[7:4], 3'd0};
        {6'd12,6'bx,6'bx,6'bx},{6'bx,6'd12,6'bx,6'bx},{6'bx,6'bx,6'd12,6'bx},{6'bx,6'bx,6'bx,6'd12}:{we_o[4],addr_o[4] }={ |we[7:4],3'd1};
        {6'd14,6'bx,6'bx,6'bx},{6'bx,6'd14,6'bx,6'bx},{6'bx,6'bx,6'd14,6'bx},{6'bx,6'bx,6'bx,6'd14}: {we_o[4],addr_o[4] }={ |we[7:4],3'd2};
        {6'd15,6'bx,6'bx,6'bx},{6'bx,6'd15,6'bx,6'bx},{6'bx,6'bx,6'd15,6'bx},{6'bx,6'bx,6'bx,6'd15}:{we_o[4],addr_o[4] }={ |we[7:4], 3'd3};
        default: {we_o[4],addr_o[4] }={1'b0, 3'bx}; 
      endcase
      casex ({addr_1_i[0],addr_1_i[1],addr_1_i[2],addr_1_i[3]})
        {6'd18,6'bx,6'bx,6'bx},{6'bx,6'd18,6'bx,6'bx},{6'bx,6'bx,6'd18,6'bx},{6'bx,6'bx,6'bx,6'd18}: {we_o[5],addr_o[5] }={ |we[3:0], 3'd0};
        {6'd19,6'bx,6'bx,6'bx},{6'bx,6'd19,6'bx,6'bx},{6'bx,6'bx,6'd19,6'bx},{6'bx,6'bx,6'bx,6'd19}: {we_o[5],addr_o[5] }={ |we[3:0], 3'd1};
        {6'd21,6'bx,6'bx,6'bx},{6'bx,6'd21,6'bx,6'bx},{6'bx,6'bx,6'd21,6'bx},{6'bx,6'bx,6'bx,6'd21}: {we_o[5],addr_o[5] }={ |we[3:0], 3'd2};
        {6'd24,6'bx,6'bx,6'bx},{6'bx,6'd24,6'bx,6'bx},{6'bx,6'bx,6'd24,6'bx},{6'bx,6'bx,6'bx,6'd24}: {we_o[5],addr_o[5] }={ |we[3:0], 3'd3};
        default:  {we_o[5],addr_o[5] }={ 1'b0, 3'bx}; 
      endcase
      casex ({addr_2_i[0],addr_2_i[1],addr_2_i[2],addr_2_i[3]})
        {6'd17,6'bx,6'bx,6'bx},{6'bx,6'd17,6'bx,6'bx},{6'bx,6'bx,6'd17,6'bx},{6'bx,6'bx,6'bx,6'd17}: {we_o[6],addr_o[6] }={ |we[7:4], 3'd0};
        {6'd20,6'bx,6'bx,6'bx},{6'bx,6'd20,6'bx,6'bx},{6'bx,6'bx,6'd20,6'bx},{6'bx,6'bx,6'bx,6'd20}: {we_o[6],addr_o[6] }={ |we[7:4], 3'd1};
        {6'd22,6'bx,6'bx,6'bx},{6'bx,6'd22,6'bx,6'bx},{6'bx,6'bx,6'd22,6'bx},{6'bx,6'bx,6'bx,6'd22}: {we_o[6],addr_o[6] }={ |we[7:4], 3'd2};
        {6'd23,6'bx,6'bx,6'bx},{6'bx,6'd23,6'bx,6'bx},{6'bx,6'bx,6'd23,6'bx},{6'bx,6'bx,6'bx,6'd23}: {we_o[6],addr_o[6] }={ |we[7:4], 3'd3};
        default:{we_o[6],addr_o[6] }={1'b0, 3'bx}; 
      endcase    
  end
endmodule // reg_decoder

module reg_data_demux(
    input [5:0]addr_1_i[3:0],
    input [5:0]addr_2_i[3:0],
    input [31:0]da_1_i[3:0],
    input [31:0]da_2_i[3:0],
    output reg [31:0]da_o[1:8]
    );
    always @(*)
    begin
        casex ({addr_1_i[0],addr_1_i[1]})
            {6'd4,6'bx},{6'd1,6'bx},{6'd6,6'bx},{6'd7,6'bx},{6'd33,6'bx},{6'd36,6'bx},{6'd38,6'bx},{6'd39,6'bx}: da_o[1] = da_1_i[0];
            {6'bx,6'd1},{6'bx,6'd4},{6'bx,6'd6},{6'bx,6'd7},{6'bx,6'd33},{6'bx,6'd36},{6'bx,6'd38},{6'bx,6'd39}: da_o[1] = da_1_i[1];
            default: da_o[1] =32'bx;
        endcase
        
        casex ({addr_1_i[0],addr_1_i[1]})
            {6'd4,6'bx},{6'd1,6'bx},{6'd6,6'bx},{6'd7,6'bx},{6'd33,6'bx},{6'd36,6'bx},{6'd38,6'bx},{6'd39,6'bx}: da_o[1] = da_1_i[0];
            {6'bx,6'd1},{6'bx,6'd4},{6'bx,6'd6},{6'bx,6'd7},{6'bx,6'd33},{6'bx,6'd36},{6'bx,6'd38},{6'bx,6'd39}: da_o[1] = da_1_i[1];
            default: da_o[1] =32'bx;
        endcase

        casex ({addr_2_i[0],addr_2_i[1]})
            {6'd2,6'bx},{6'd3,6'bx},{6'd5,6'bx},{6'd8,6'bx},{6'd34,6'bx},{6'd35,6'bx},{6'd37,6'bx},{6'd40,6'bx}: da_o[2] = da_2_i[0];
            {6'bx,6'd2},{6'bx,6'd3},{6'bx,6'd5},{6'bx,6'd8},{6'bx,6'd34},{6'bx,6'd35},{6'bx,6'd37},{6'bx,6'd40}: da_o[2] = da_2_i[1];
            default: da_o[2] =32'bx;
        endcase

        casex ({addr_1_i[2],addr_1_i[3]})
            {6'd25,6'bx},{6'd28,6'bx},{6'd30,6'bx},{6'd31,6'bx},{6'd41,6'bx},{6'd44,6'bx},{6'd46,6'bx},{6'd47,6'bx}: da_o[7] = da_1_i[2];
            {6'bx,6'd25},{6'bx,6'd28},{6'bx,6'd30},{6'bx,6'd31},{6'bx,6'd41},{6'bx,6'd44},{6'bx,6'd46},{6'bx,6'd47}: da_o[7] = da_1_i[3];
            default: da_o[7] =32'bx;
        endcase

        casex ({addr_2_i[2],addr_2_i[3]})
            {6'd26,6'bx},{6'd27,6'bx},{6'd29,6'bx},{6'd32,6'bx},{6'd42,6'bx},{6'd43,6'bx},{6'd45,6'bx},{6'd48,6'bx}: da_o[8] = da_2_i[2];
            {6'bx,6'd26},{6'bx,6'd27},{6'bx,6'd29},{6'bx,6'd32},{6'bx,6'd42},{6'bx,6'd43},{6'bx,6'd45},{6'bx,6'd48}: da_o[8] = da_2_i[3];
            default: da_o[8] =32'bx;
        endcase

        casex ({addr_1_i[0],addr_1_i[1],addr_1_i[2],addr_1_i[3]})
        {6'd10,6'bx,6'bx,6'bx},{6'd11,6'bx,6'bx,6'bx},{6'd13,6'bx,6'bx,6'bx},{6'd16,6'bx,6'bx,6'bx}: da_o[3] = da_1_i[0];
        {6'bx,6'd10,6'bx,6'bx},{6'bx,6'd11,6'bx,6'bx},{6'bx,6'd13,6'bx,6'bx},{6'bx,6'd16,6'bx,6'bx}: da_o[3] = da_1_i[1];
        {6'bx,6'bx,6'd10,6'bx},{6'bx,6'bx,6'd11,6'bx},{6'bx,6'bx,6'd13,6'bx},{6'bx,6'bx,6'd16,6'bx}: da_o[3] = da_1_i[2];
        {6'bx,6'bx,6'bx,6'd10},{6'bx,6'bx,6'bx,6'd11},{6'bx,6'bx,6'bx,6'd13},{6'bx,6'bx,6'bx,6'd16}: da_o[3] = da_1_i[3];
        default:da_o[3] = 32'bx;
    endcase

    casex ({addr_2_i[0],addr_2_i[1],addr_2_i[2],addr_2_i[3]})
        {6'd9,6'bx,6'bx,6'bx},{6'd12,6'bx,6'bx,6'bx},{6'd14,6'bx,6'bx,6'bx},{6'd15,6'bx,6'bx,6'bx}:da_o[4] = da_2_i[0];
        {6'bx,6'd9,6'bx,6'bx},{6'bx,6'd12,6'bx,6'bx},{6'bx,6'd14,6'bx,6'bx},{6'bx,6'd15,6'bx,6'bx}:da_o[4] = da_2_i[1];
        {6'bx,6'bx,6'd9,6'bx},{6'bx,6'bx,6'd12,6'bx},{6'bx,6'bx,6'd14,6'bx},{6'bx,6'bx,6'd15,6'bx}:da_o[4] = da_2_i[2];
        {6'bx,6'bx,6'bx,6'd9},{6'bx,6'bx,6'bx,6'd12},{6'bx,6'bx,6'bx,6'd14},{6'bx,6'bx,6'bx,6'd15}:da_o[4] = da_2_i[3];
        default:da_o[4] = 32'bx;
    endcase

    casex ({addr_1_i[0],addr_1_i[1],addr_1_i[2],addr_1_i[3]})
        {6'd18,6'bx,6'bx,6'bx},{6'd19,6'bx,6'bx,6'bx},{6'd21,6'bx,6'bx,6'bx},{6'd24,6'bx,6'bx,6'bx}:da_o[5] = da_1_i[0];
        {6'bx,6'd18,6'bx,6'bx},{6'bx,6'd19,6'bx,6'bx},{6'bx,6'd21,6'bx,6'bx},{6'bx,6'd24,6'bx,6'bx}:da_o[5] = da_1_i[1];
        {6'bx,6'bx,6'd18,6'bx},{6'bx,6'bx,6'd19,6'bx},{6'bx,6'bx,6'd21,6'bx},{6'bx,6'bx,6'd24,6'bx}:da_o[5] = da_1_i[2];
        {6'bx,6'bx,6'bx,6'd18},{6'bx,6'bx,6'bx,6'd19},{6'bx,6'bx,6'bx,6'd21},{6'bx,6'bx,6'bx,6'd24}:da_o[5] = da_1_i[3];
        default:da_o[5] = 32'bx;
    endcase

    casex ({addr_2_i[0],addr_2_i[1],addr_2_i[2],addr_2_i[3]})
        {6'd17,6'bx,6'bx,6'bx},{6'd20,6'bx,6'bx,6'bx},{6'd22,6'bx,6'bx,6'bx},{6'd23,6'bx,6'bx,6'bx}:da_o[6] = da_2_i[0];
        {6'bx,6'd17,6'bx,6'bx},{6'bx,6'd20,6'bx,6'bx},{6'bx,6'd22,6'bx,6'bx},{6'bx,6'd23,6'bx,6'bx}:da_o[6] = da_2_i[1];
        {6'bx,6'bx,6'd17,6'bx},{6'bx,6'bx,6'd20,6'bx},{6'bx,6'bx,6'd22,6'bx},{6'bx,6'bx,6'd23,6'bx}:da_o[6] = da_2_i[2];
        {6'bx,6'bx,6'bx,6'd17},{6'bx,6'bx,6'bx,6'd20},{6'bx,6'bx,6'bx,6'd22},{6'bx,6'bx,6'bx,6'd23}:da_o[6] = da_2_i[3];
        default: da_o[6] = 32'bx;
    endcase
    end
endmodule

module output_mux(
    input clk,
    input rst_n,
    input [5:0]addr__i[3:0],
    input [31:0]d1_i,
    input [31:0]d2_i,
    input [31:0]d3_i,
    input [31:0]d4_i,
    output reg [31:0]d_o[3:0]
  );
  genvar i;
    reg [5:0]addr__i_r[3:0];
  for (i=0;i<=3;i=i+1)
  begin
always @(posedge clk or negedge rst_n)
    if(!rst_n)addr__i_r[i]<=0;
    else addr__i_r[i]<=addr__i[i];
end
  generate
    for(i=0;i<=3;i=i+1)
      begin
      always@(*)
      begin
        if(i<=1)
          begin
            casex (addr__i_r[i])
              0:d_o[i] = 0;
              1,2,3,4,5,6,7,8: d_o[i] = d1_i;
              9,10,11,12,13,14,15,16: d_o[i] = d2_i;
              17,18,19,20,21,22,23,24: d_o[i] = d3_i;              
              default: d_o[i] = 32'bx;
            endcase
          end
        else
          begin
            casex (addr__i_r[i])
              0:d_o[i] = 0;
              9,10,11,12,13,14,15,16: d_o[i] = d2_i;
              17,18,19,20,21,22,23,24: d_o[i] = d3_i;
              25,26,27,28,29,30,31,32: d_o[i] = d4_i;              
              default: d_o[i] = 32'bx;
            endcase
          end
      end
      end
  endgenerate

endmodule