//////////////////////////////////////////////////////////////////////////////////
// Company:
// Engineer: TQ
//
// Create Date: 2019/04/26 17:00:58
// Design Name:
// Module Name: mul_comp
// Project Name:
// Target Devices:
// Tool Versions:
// Description:
// * 有符号数16bit乘法
// * o = a*b

// Dependencies:
//
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
//
// 		31	30	29	28	27	26	25	24	23	22	21	20	19	18	17	16	15	14	13	12	11	10	9	8	7	6	5	4	3	2	1	0
// l1														S'	-	-	S	A	A	A	A	A	A	A	A	A	A	A	A	A	A	A	B
// l2														S'	A	A	A	A	A	A	A	A	A	A	A	A	A	A	A	A	B	C
// l3												1	S'	A	A	A	A	A	A	A	A	A	A	A	A	A	A	A	B	C
// l4										1	S'	A	A	A	A	A	A	A	A	A	A	A	A	A	A	A	B	C
// l5								1	S'	A	A	A	A	A	A	A	A	A	A	A	A	A	A	A	B	C
// l6						1	S'	A	A	A	A	A	A	A	A	A	A	A	A	A	A	A	B	C
// l7				1	S'	A	A	A	A	A	A	A	A	A	A	A	A	A	A	A	B	C
// l8		1	S'	A	A	A	A	A	A	A	A	A	A	A	A	A	A	A	D	C
// l9		X	X	X	X	X	X	X	X	X	X	X	X	X	X	X	X	X	E



//////////////////////////////////////////////////////////////////////////////////

module mul_comp_lcx(
         input clk,
         input rst_n,
         input [15:0]a,
         input [15:0]b,
         output [31:0]o_33

       );
////wire [19:0]l1, [19:1]l2, [21:3]l3, [23:5]l4, [25:7]l5, [27:9]l6, [29:11]l7, [31:13]l8, [31:14]l9;
wire [31:0]prod_l[1:9];//部分积
wire [9:1]s1[0:31];
wire [9:1]s1_temp[0:31];
wire [7:1]s2[0:31];//部分积
wire [6:1]s3[0:32];//部分积
wire [3:1]s4[0:32];//部分积
wire [31:0]d;
wire [31:0]s;
wire [31:0]o;
wire [31:0]carryo;
//wire [32:0]o_33;
assign o_33 = o;
reg  [6:1]s3_r[0:32];


genvar i,j;

// * 生成
assign {prod_l[3][21],prod_l[4][23],prod_l[5][25],prod_l[6][27],prod_l[7][29],prod_l[8][31]} = 6'b11_1111; // 最高位置1
assign prod_l[1][18:17] = {prod_l[1][16],prod_l[1][16]}; // 生成第一行的SS
//assign prod_l[9][31:15] = 0;

//* 调转
generate
  for (i = 1; i <= 9; i = i+1)
  begin
    for (j = 0; j <= 31; j = j+1)
    begin
      assign s1_temp[j][i] = prod_l[i][j];
    end
  end

  for (j = 0; j <= 19; j = j+1)
  begin
    assign s1[j][9:1] = s1_temp[j][9:1];
  end
  assign s1[20][7:1] = s1_temp[20][9:3];
  assign s1[21][7:1] = s1_temp[21][9:3];
  assign s1[22][6:1] = s1_temp[22][9:4];
  assign s1[23][6:1] = s1_temp[23][9:4];
  assign s1[24][5:1] = s1_temp[24][9:5];
  assign s1[25][5:1] = s1_temp[25][9:5];
  assign s1[26][4:1] = s1_temp[26][9:6];
  assign s1[27][4:1] = s1_temp[27][9:6];
  assign s1[28][3:1] = s1_temp[28][9:7];
  assign s1[29][3:1] = s1_temp[29][9:7];
  assign s1[30][2:1] = s1_temp[30][9:8];
  assign s1[31][2:1] = s1_temp[31][9:8];
endgenerate

// * 部分积
generate
  // * 生成Abox
  for (j = 1; j<9; j = j+1)
    begin
      for (i = 1; i < 16; i = i+1)
        begin
          A_box A_box_u_prod_l(
                  .b_i((j==1) ? {b[1:0],1'b0} : b[2*j-1:2*j-3]),
                  .a_i(a[i:i-1]),
                  .o(prod_l[j][i + (j-1)*2]) // 第8行从15开始
                );
        end
      if (j < 8)
        begin
          BC_box BC_box_u_prod_l_md(
                   .b_i((j==1) ? {b[1:0],1'b0} : b[2*j-1:2*j-3]),
                   .a_i(a[0]),
                   .b_box_o(prod_l[j][(j-1)*2]),
                   .c_box_o(prod_l[j+1][j*2-1])
                 );
        end
      else
        begin
          DE_box DE_box_u_prod_l_end(
                   .b_i(b[2*j-1:2*j-3]),
                   .a_i(a[0]),
                   .d_box_o(prod_l[8][14]),
                   .e_box_o(prod_l[9][14])
                 );
        end
      if (j<=2)
        begin
          SnS_box SnS_box_u_prod_l(
                    .b_i((j==1) ? {b[1:0],1'b0} : b[2*j-1:2*j-3]),
                    .a_i(a[15]),
                    .s_box_o(prod_l[j][(j==1) ? 16 : 18]),
                    .ns_box_o(prod_l[j][19])
                  );
        end
      else
        begin
          SnS_box SnS_box_u_prod_l(
                    .b_i(b[2*j-1:2*j-3]),
                    .a_i(a[15]),
                    .ns_box_o(prod_l[j][j*2+14])
                  );
        end
    end
endgenerate

// * 压缩
generate 
  assign s2[0][1] = s1[0][1];
  assign s2[1][1] = s1[1][1];
  assign s2[1][2] = s1[1][2];
  assign s2[2][1] = s1[2][1];
  assign s2[2][2] = s1[2][2];
  assign s2[3][1] = s1[3][1];
  assign s2[3][2] = s1[3][2];
  assign s2[3][3] = s1[3][3];
  assign s2[4][1] = s1[4][1];
  assign s2[4][2] = s1[4][2];
  assign s2[4][3] = s1[4][3];
  assign s2[5][1] = s1[5][1];
  assign s2[6][1] = s1[6][1];
  assign s2[7][1] = s1[7][1];
  assign s2[7][2] = s1[7][2];
  assign s2[8][1] = s1[8][1];
  assign s2[8][2] = s1[8][2];
  assign s2[11][1] = s1[11][1];
  assign s2[12][1] = s1[12][1];
  assign s2[13][1] = s1[13][1];
  assign s2[13][2] = s1[13][2];
  assign s2[16][1] = s1[16][1];
  assign s2[16][2] = s1[16][2];
  assign s2[17][1] = s1[17][1];
  assign s2[17][2] = s1[17][2];
  assign s2[18][1] = s1[18][1];
  assign s2[18][2] = s1[18][2];
  assign s2[22][1] = s1[22][1];
  assign s2[22][2] = s1[22][2];
  assign s2[23][1] = s1[23][1];
  assign s2[23][2] = s1[23][2];
  assign s2[23][3] = s1[23][3];
  assign s2[23][4] = s1[23][4];
  assign s2[23][5] = s1[23][5];
  assign s2[24][1] = s1[24][1];
  assign s2[25][1] = s1[25][1];
  assign s2[28][1] = s1[28][1];
  assign s2[28][2] = s1[28][2];
  assign s2[29][1] = s1[29][1];
  assign s2[29][2] = s1[29][2];
  assign s2[30][1] = s1[30][1];
  assign s2[31][1] = s1[31][1];
  comp32 comp32_u_s1_0(
    .a_i(s1[5][4:2]),
    .o({s2[5][2],s2[6][2]})
  );
  comp32 comp32_u_s1_1(
    .a_i(s1[6][4:2]),
    .o({s2[6][3],s2[7][3]})
  );
  comp32 comp32_u_s1_2(
    .a_i(s1[7][5:3]),
    .o({s2[7][4],s2[8][3]})
  );
  comp32 comp32_u_s1_3(
    .a_i(s1[8][5:3]),
    .o({s2[8][4],s2[9][1]})
  );
  comp32 comp32_u_s1_4(
    .a_i(s1[10][3:1]),
    .o({s2[10][1],s2[11][2]})
  );
  comp32 comp32_u_s1_5(
    .a_i(s1[10][6:4]),
    .o({s2[10][2],s2[11][3]})
  );
  comp32 comp32_u_s1_6(
    .a_i(s1[12][4:2]),
    .o({s2[12][2],s2[13][3]})
  );
  comp32 comp32_u_s1_7(
    .a_i(s1[12][7:5]),
    .o({s2[12][3],s2[13][4]})
  );
  comp32 comp32_u_s1_8(
    .a_i(s1[18][5:3]),
    .o({s2[18][3],s2[19][1]})
  );
  comp32 comp32_u_s1_9(
    .a_i(s1[21][3:1]),
    .o({s2[21][1],s2[22][3]})
  );
  comp32 comp32_u_s1_10(
    .a_i(s1[21][6:4]),
    .o({s2[21][2],s2[22][4]})
  );
  comp32 comp32_u_s1_11(
    .a_i(s1[22][5:3]),
    .o({s2[22][5],s2[23][6]})
  );
  comp32 comp32_u_s1_12(
    .a_i(s1[24][4:2]),
    .o({s2[24][2],s2[25][2]})
  );
  comp32 comp32_u_s1_13(
    .a_i(s1[25][4:2]),
    .o({s2[25][3],s2[26][1]})
  );
  comp32 comp32_u_s1_14(
    .a_i(s1[26][3:1]),
    .o({s2[26][2],s2[27][1]})
  );
  comp32 comp32_u_s1_15(
    .a_i(s1[27][3:1]),
    .o({s2[27][2],s2[28][3]})
  );
  comp63 comp63_u_s1_0(
    .a_i(s1[9][6:1]),
    .o({s2[9][2],s2[10][3],s2[11][4]})
  );
  comp63 comp63_u_s1_1(
    .a_i(s1[11][7:2]),
    .o({s2[11][5],s2[12][4],s2[13][5]})
  );
  comp63 comp63_u_s1_2(
    .a_i(s1[13][8:3]),
    .o({s2[13][6],s2[14][1],s2[15][1]})
  );
  comp63 comp63_u_s1_3(
    .a_i(s1[14][6:1]),
    .o({s2[14][2],s2[15][2],s2[16][3]})
  );
  comp63 comp63_u_s1_4(
    .a_i(s1[15][6:1]),
    .o({s2[15][3],s2[16][4],s2[17][3]})
  );
  comp63 comp63_u_s1_5(
    .a_i(s1[16][8:3]),
    .o({s2[16][5],s2[17][4],s2[18][4]})
  );
  comp63 comp63_u_s1_6(
    .a_i(s1[17][8:3]),
    .o({s2[17][5],s2[18][5],s2[19][2]})
  );
  comp63 comp63_u_s1_7(
    .a_i(s1[19][6:1]),
    .o({s2[19][3],s2[20][1],s2[21][3]})
  );
  comp63 comp63_u_s1_8(
    .a_i(s1[20][6:1]),
    .o({s2[20][2],s2[21][4],s2[22][6]})
  );
  comp233 comp233_u_s1_0(
    .a0_i(s1[14][9:7]),
    .a1_i({s1[15][8:7]}),
    .o({s2[14][3],s2[15][4],s2[16][6]})
  );
  comp233 comp233_u_s1_1(
    .a0_i(s1[18][8:6]),
    .a1_i({s1[19][8:7]}),
    .o({s2[18][6],s2[19][4],s2[20][3]})
  );
  assign s3[0][1] = s2[0][1];
  assign s3[1][1] = s2[1][1];
  assign s3[1][2] = s2[1][2];
  assign s3[2][1] = s2[2][1];
  assign s3[2][2] = s2[2][2];
  assign s3[3][1] = s2[3][1];
  assign s3[3][2] = s2[3][2];
  assign s3[3][3] = s2[3][3];
  assign s3[4][1] = s2[4][1];
  assign s3[4][2] = s2[4][2];
  assign s3[4][3] = s2[4][3];
  assign s3[5][1] = s2[5][1];
  assign s3[5][2] = s2[5][2];
  assign s3[6][1] = s2[6][1];
  assign s3[6][2] = s2[6][2];
  assign s3[6][3] = s2[6][3];
  assign s3[7][1] = s2[7][1];
  assign s3[8][1] = s2[8][1];
  assign s3[9][1] = s2[9][1];
  assign s3[9][2] = s2[9][2];
  assign s3[10][1] = s2[10][1];
  assign s3[10][2] = s2[10][2];
  assign s3[10][3] = s2[10][3];
  assign s3[11][1] = s2[11][1];
  assign s3[11][2] = s2[11][2];
  assign s3[12][1] = s2[12][1];
  assign s3[15][1] = s2[15][1];
  assign s3[19][1] = s2[19][1];
  assign s3[21][1] = s2[21][1];
  assign s3[24][1] = s2[24][1];
  assign s3[24][2] = s2[24][2];
  assign s3[26][1] = s2[26][1];
  assign s3[26][2] = s2[26][2];
  assign s3[27][1] = s2[27][1];
  assign s3[27][2] = s2[27][2];
  assign s3[28][1] = s2[28][1];
  assign s3[28][2] = s2[28][2];
  assign s3[28][3] = s2[28][3];
  assign s3[29][1] = s2[29][1];
  assign s3[29][2] = s2[29][2];
  assign s3[30][1] = s2[30][1];
  assign s3[31][1] = s2[31][1];
  comp32 comp32_u_s2_0(
    .a_i(s2[7][4:2]),
    .o({s3[7][2],s3[8][2]})
  );
  comp32 comp32_u_s2_1(
    .a_i(s2[8][4:2]),
    .o({s3[8][3],s3[9][3]})
  );
  comp32 comp32_u_s2_2(
    .a_i(s2[11][5:3]),
    .o({s3[11][3],s3[12][2]})
  );
  comp32 comp32_u_s2_3(
    .a_i(s2[12][4:2]),
    .o({s3[12][3],s3[13][1]})
  );
  comp32 comp32_u_s2_4(
    .a_i(s2[13][3:1]),
    .o({s3[13][2],s3[14][1]})
  );
  comp32 comp32_u_s2_5(
    .a_i(s2[13][6:4]),
    .o({s3[13][3],s3[14][2]})
  );
  comp32 comp32_u_s2_6(
    .a_i(s2[14][3:1]),
    .o({s3[14][3],s3[15][2]})
  );
  comp32 comp32_u_s2_7(
    .a_i(s2[15][4:2]),
    .o({s3[15][3],s3[16][1]})
  );
  comp32 comp32_u_s2_8(
    .a_i(s2[16][3:1]),
    .o({s3[16][2],s3[17][1]})
  );
  comp32 comp32_u_s2_9(
    .a_i(s2[17][3:1]),
    .o({s3[17][2],s3[18][1]})
  );
  comp32 comp32_u_s2_10(
    .a_i(s2[19][4:2]),
    .o({s3[19][2],s3[20][1]})
  );
  comp32 comp32_u_s2_11(
    .a_i(s2[20][3:1]),
    .o({s3[20][2],s3[21][2]})
  );
  comp32 comp32_u_s2_12(
    .a_i(s2[21][4:2]),
    .o({s3[21][3],s3[22][1]})
  );
  comp32 comp32_u_s2_13(
    .a_i(s2[22][3:1]),
    .o({s3[22][2],s3[23][1]})
  );
  comp32 comp32_u_s2_14(
    .a_i(s2[22][6:4]),
    .o({s3[22][3],s3[23][2]})
  );
  comp32 comp32_u_s2_15(
    .a_i(s2[25][3:1]),
    .o({s3[25][1],s3[26][3]})
  );
  comp63 comp63_u_s2_0(
    .a_i(s2[18][6:1]),
    .o({s3[18][2],s3[19][3],s3[20][3]})
  );
  comp63 comp63_u_s2_1(
    .a_i(s2[23][6:1]),
    .o({s3[23][3],s3[24][3],s3[25][2]})
  );
  comp233 comp233_u_s2_0(
    .a0_i(s2[16][6:4]),
    .a1_i({s2[17][5:4]}),
    .o({s3[16][3],s3[17][3],s3[18][3]})
  );

// ! FF state3 -> state4
  assign s4[0][1] = s3_r[0][1];
  assign s4[0][2] = 0;
  assign s4[0][3] = 0;
  assign s4[1][1] = s3_r[1][1];
  assign s4[1][2] = s3_r[1][2];
  assign s4[1][3] = 0;
  assign s4[2][1] = s3_r[2][1];
  assign s4[2][2] = s3_r[2][2];
  assign s4[2][3] = 0;
  assign s4[3][1] = s3_r[3][1];
  assign s4[3][2] = s3_r[3][2];
  assign s4[3][3] = s3_r[3][3];
  assign s4[4][1] = s3_r[4][1];
  assign s4[4][2] = s3_r[4][2];
  assign s4[4][3] = s3_r[4][3];
  assign s4[5][1] = s3_r[5][1];
  assign s4[5][2] = s3_r[5][2];
  assign s4[5][3] = 0;
  assign s4[6][1] = s3_r[6][1];
  assign s4[6][2] = s3_r[6][2];
  assign s4[6][3] = s3_r[6][3];
  assign s4[7][1] = s3_r[7][1];
  assign s4[7][2] = s3_r[7][2];
  assign s4[7][3] = 0;
  assign s4[8][1] = s3_r[8][1];
  assign s4[8][2] = s3_r[8][2];
  assign s4[8][3] = s3_r[8][3];
  assign s4[9][1] = s3_r[9][1];
  assign s4[9][2] = s3_r[9][2];
  assign s4[9][3] = s3_r[9][3];
  assign s4[10][1] = s3_r[10][1];
  assign s4[10][2] = s3_r[10][2];
  assign s4[10][3] = s3_r[10][3];
  assign s4[11][1] = s3_r[11][1];
  assign s4[11][2] = s3_r[11][2];
  assign s4[11][3] = s3_r[11][3];
  assign s4[12][1] = s3_r[12][1];
  assign s4[12][2] = s3_r[12][2];
  assign s4[12][3] = s3_r[12][3];
  assign s4[13][1] = s3_r[13][1];
  assign s4[13][2] = s3_r[13][2];
  assign s4[13][3] = s3_r[13][3];
  assign s4[14][1] = s3_r[14][1];
  assign s4[14][2] = s3_r[14][2];
  assign s4[14][3] = s3_r[14][3];
  assign s4[15][1] = s3_r[15][1];
  assign s4[15][2] = s3_r[15][2];
  assign s4[15][3] = s3_r[15][3];
  assign s4[16][1] = s3_r[16][1];
  assign s4[16][2] = s3_r[16][2];
  assign s4[16][3] = s3_r[16][3];
  assign s4[17][1] = s3_r[17][1];
  assign s4[17][2] = s3_r[17][2];
  assign s4[17][3] = s3_r[17][3];
  assign s4[18][1] = s3_r[18][1];
  assign s4[18][2] = s3_r[18][2];
  assign s4[18][3] = s3_r[18][3];
  assign s4[19][1] = s3_r[19][1];
  assign s4[19][2] = s3_r[19][2];
  assign s4[19][3] = s3_r[19][3];
  assign s4[20][1] = s3_r[20][1];
  assign s4[20][2] = s3_r[20][2];
  assign s4[20][3] = s3_r[20][3];
  assign s4[21][1] = s3_r[21][1];
  assign s4[21][2] = s3_r[21][2];
  assign s4[21][3] = s3_r[21][3];
  assign s4[22][1] = s3_r[22][1];
  assign s4[22][2] = s3_r[22][2];
  assign s4[22][3] = s3_r[22][3];
  assign s4[23][1] = s3_r[23][1];
  assign s4[23][2] = s3_r[23][2];
  assign s4[23][3] = s3_r[23][3];
  assign s4[24][1] = s3_r[24][1];
  assign s4[24][2] = s3_r[24][2];
  assign s4[24][3] = s3_r[24][3];
  assign s4[25][1] = s3_r[25][1];
  assign s4[25][2] = s3_r[25][2];
  assign s4[25][3] = 0;
  assign s4[26][1] = s3_r[26][1];
  assign s4[26][2] = s3_r[26][2];
  assign s4[26][3] = s3_r[26][3];
  assign s4[27][1] = s3_r[27][1];
  assign s4[27][2] = s3_r[27][2];
  assign s4[27][3] = 0;
  assign s4[28][1] = s3_r[28][1];
  assign s4[28][2] = s3_r[28][2];
  assign s4[28][3] = s3_r[28][3];
  assign s4[29][1] = s3_r[29][1];
  assign s4[29][2] = s3_r[29][2];
  assign s4[29][3] = 0;
  assign s4[30][1] = s3_r[30][1];
  assign s4[30][2] = 0;
  assign s4[30][3] = 0;
  assign s4[31][1] = s3_r[31][1];
  assign s4[31][2] = 0;
  assign s4[31][3] = 0;
endgenerate

// * FF
generate
for(i=0; i<33; i=i+1)
begin
  always @(posedge clk or negedge rst_n)
    begin
      if (!rst_n)
        begin
            s3_r[i] = 0;
          end
      else
        begin
              s3_r[i] = s3[i];
        end
    end
end
  
  
endgenerate

// * 合并

for(i=0;i<32;i=i+1)
begin
assign d[i] = ^{s4[i][1],s4[i][2],s4[i][3]};
assign s[i] = i==0 ? d[i] : d[i]^((s4[i-1][1]&s4[i-1][2])|(s4[i-1][3]&(s4[i-1][1]|s4[i-1][2])));
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

//assign o = o_33[31:15];
//assign o_33[0] = s4[0][1];
endmodule// mul_comp





module A_box(
          input [2:0]b_i,
          input [1:0]a_i,
          output wire o
        );
  LUT5 #(
        .INIT(32'h0E16_6870) // Specify LUT Contents
      ) LUT5_A_box (
        .O(o), // LUT general output
        .I0(b_i[0]), // LUT input
        .I1(b_i[1]), // LUT input
        .I2(b_i[2]), // LUT input
        .I3(a_i[0]), // LUT input
        .I4(a_i[1]) // LUT input
      );
endmodule // Abox

module BC_box(
         input [2:0]b_i,
         input a_i,
         output wire b_box_o,
         output wire c_box_o
       );
  LUT6_2 #(
            .INIT(64'h0000_1070_0000_6600)
         // .INIT(64'h0066_0000_0E08_0000) // Specify LUT Contents
        ) LUT6_2_BC_box (
          .O6(c_box_o), // 1-bit Cbox @ LUT6 output
          .O5(b_box_o), // 1-bit Bbox @ LUT5 output
          .I0(b_i[0]), // LUT input
          .I1(b_i[1]), // LUT input
          .I2(b_i[2]), // LUT input
          .I3(a_i), // LUT input
          .I4(0), // LUT input
          .I5(1) // LUT input control mux
        );
endmodule //BC_box

module DE_box(
         input [2:0]b_i,
         input a_i,
         output wire d_box_o,
         output wire e_box_o
       );
  LUT6_2 #(
             .INIT(64'h0000_7070_0000_1670) // Specify LUT Contents
          // .INIT(64'h0E68_0000_0E0E_0000) // Specify LUT Contents
        ) LUT6_2_DE_box (
          .O6(e_box_o), // 1-bit Ebox @ LUT6 output
          .O5(d_box_o), // 1-bit Dbox @ LUT5 output
          .I0(b_i[0]), // LUT input
          .I1(b_i[1]), // LUT input
          .I2(b_i[2]), // LUT input
          .I3(a_i), // LUT input
          .I4(0), // LUT input
          .I5(1) // LUT input control mux
        );
endmodule //DE_box

module SnS_box(
         input [2:0]b_i,
         input a_i,
         output wire s_box_o,
         output wire ns_box_o
       );
  LUT6_2 #(
             .INIT('h0000_F18F_0000_0E70) // Specify LUT Contents
          // .INIT('h0E70_0000_F18F_0000) // Specify LUT Contents
        ) LUT6_2_SnS_box (
          .O6(ns_box_o), // 1-bit nSbox @ LUT6 output
          .O5(s_box_o), // 1-bit Sbox @ LUT5 output
          .I0(b_i[0]), // LUT input
          .I1(b_i[1]), // LUT input
          .I2(b_i[2]), // LUT input
          .I3(a_i), // LUT input
          .I4(0), // LUT input
          .I5(1) // LUT input control mux
        );
endmodule //  SnS_box


module comp32(
         input [2:0]a_i,
         output wire [1:0]o
       );
  LUT6_2 #(
             .INIT('h0000_00E8_0000_0096) // Specify LUT Contents
          // .INIT('h6900_0000_1700_0000) // Specify LUT Contents
        ) LUT6_2_comp32_box (
          .O6(o[0]), // 1-bit o1 @ LUT6 output
          .O5(o[1]), // 1-bit o0 @ LUT5 output
          .I0(a_i[0]), // LUT input
          .I1(a_i[1]), // LUT input
          .I2(a_i[2]), // LUT input
          .I3(0), // LUT input
          .I4(0), // LUT input
          .I5(1) // LUT input control mux
        );
endmodule

module comp63(
         input [5:0]a_i,
         output wire [2:0]o
       );
  LUT6 #(//第i位
        .INIT('h6996_9669_9669_6996) // Specify LUT Contents
      ) LUT6_comp63_0_box (
        .O(o[2]), // 1-bit o0 @ LUT6 output
        .I0(a_i[0]), // LUT input
        .I1(a_i[1]), // LUT input
        .I2(a_i[2]), // LUT input
        .I3(a_i[3]), // LUT input
        .I4(a_i[4]), // LUT input
        .I5(a_i[5]) // LUT input 
      );
  LUT6 #(//第i+1位
        .INIT('h8117_177E_177E_7EE8) // Specify LUT Contents
        // .INIT('h177E_7EE8_7EE8_E881) // Specify LUT Contents
      ) LUT6_comp63_1_box (
        .O(o[1]), // 1-bit o1 @ LUT6 output
        .I0(a_i[0]), // LUT input
        .I1(a_i[1]), // LUT input
        .I2(a_i[2]), // LUT input
        .I3(a_i[3]), // LUT input
        .I4(a_i[4]), // LUT input
        .I5(a_i[5]) // LUT input 
      );
  LUT6 #(//第i+2位
        .INIT('hFEE8_E880_E880_8000) // Specify LUT Contents
        // .INIT('h0001_0117_0117_177F) // Specify LUT Contents
      ) LUT6_comp63_2_box (
        .O(o[0]), // 1-bit o2 @ LUT6 output
        .I0(a_i[0]), // LUT input
        .I1(a_i[1]), // LUT input
        .I2(a_i[2]), // LUT input
        .I3(a_i[3]), // LUT input
        .I4(a_i[4]), // LUT input
        .I5(a_i[5]) // LUT input 
      );
endmodule

module comp153(
         input a1_i,
         input [4:0]a0_i,
         output wire [2:0]o
       );
  LUT6 #(// 第i位
        .INIT('h9669_6996_9669_6996) // Specify LUT Contents
        // .INIT('h6996_9669_6996_9669) // Specify LUT Contents
      ) LUT6_comp153_0_box (
        .O(o[2]), // 1-bit o0 @ LUT6 output
        .I0(a0_i[0]), // LUT input
        .I1(a0_i[1]), // LUT input
        .I2(a0_i[2]), // LUT input
        .I3(a0_i[3]), // LUT input
        .I4(a0_i[4]), // LUT input
        .I5(a1_i) // LUT input 
      );
  LUT6 #(// 第i+1位
        .INIT('hE881_8117_177E_7EE8) // Specify LUT Contents
        // .INIT('h177E_7EE8_E881_8117) // Specify LUT Contents
      ) LUT6_comp153_1_box (
        .O(o[1]), // 1-bit o1 @ LUT6 output
        .I0(a0_i[0]), // LUT input
        .I1(a0_i[1]), // LUT input
        .I2(a0_i[2]), // LUT input
        .I3(a0_i[3]), // LUT input
        .I4(a0_i[4]), // LUT input
        .I5(a1_i) // LUT input 
      );
  LUT6 #(//第i+2位
        .INIT('hFFFE_FEE8_E880_8000) // Specify LUT Contents
        // .INIT('h0001_0117_177F_7FFF) // Specify LUT Contents
      ) LUT6_comp153_2_box (
        .O(o[0]), // 1-bit o2 @ LUT6 output
        .I0(a0_i[0]), // LUT input
        .I1(a0_i[1]), // LUT input
        .I2(a0_i[2]), // LUT input
        .I3(a0_i[3]), // LUT input
        .I4(a0_i[4]), // LUT input
        .I5(a1_i) // LUT input 
      );
endmodule

module comp233(
         input [1:0]a1_i,
         input [2:0]a0_i,
         output wire [2:0]o
       );
  LUT6_2 #(//第i，i+1位
        .INIT('hE817_17E8_9696_9696) // Specify LUT Contents
        // .INIT('h6969_6969_17E8_E817) // Specify LUT Contents
      ) LUT6_2_comp233_0_1_box (
        .O6(o[1]), // 1-bit o1 @ LUT6 output
        .O5(o[2]), // 1-bit o0 @ LUT5 output
        .I0(a0_i[0]), // LUT input
        .I1(a0_i[1]), // LUT input
        .I2(a0_i[2]), // LUT input
        .I3(a1_i[0]), // LUT input
        .I4(a1_i[1]), // LUT input 
        .I5(1) // 1bit control mux
      );
  LUT5 #(//第i+2位
        .INIT('hFFE8_E800) // Specify LUT Contents
        // .INIT('h0017_17FF) // Specify LUT Contents
      ) LUT5_comp233_2_box (
        .O(o[0]), // 1-bit o2 @ LUT5 output
        .I0(a0_i[0]), // LUT input
        .I1(a0_i[1]), // LUT input
        .I2(a0_i[2]), // LUT input
        .I3(a1_i[0]), // LUT input
        .I4(a1_i[1]) // LUT input
      );
endmodule

module xor2(// o1 = ^a1[1:0], o2 = ^a2[1:0]
       input [1:0]a1,
       input [1:0]a2,
       output wire o1,
       output wire o2
      );
    LUT6_2 #(
          .INIT('h0000_0FF0_0000_6666) // Specify LUT Contents
          // .INIT('h6666_0000_0FF0_0000) // Specify LUT Contents
        ) LUT6_2_xor2_box (
          .O6(o2), // 1-bit o1 @ LUT6 output
          .O5(o1), // 1-bit o0 @ LUT5 output
          .I0(a1[0]), // LUT input
          .I1(a1[1]), // LUT input
          .I2(a2[0]), // LUT input
          .I3(a2[1]), // LUT input
          .I4(0), // LUT input 
          .I5(1) // 1bit control mux
        );
endmodule
