// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
//
// This program only needs to handle arguments that satisfy
// R0 >= 0, R1 >= 0, and R0*R1 < 32768.

// Put your code here.
//R0の足し算をR1回行うことで掛け算を実現する
@2
M=0 //初期化
@i //変数i(A=i)
M=1 //M[i]=1
(LOOP) //ラベルシンボルを設定
@i
D=M //D=i
@0
D=D-M //D=i-R0
@END
D;JGT
@1
D=M //D=R1
@2
M=D+M //R2=R1+R2
@i
M=M+1 //++i
@LOOP
0;JMP
(END)
@END
0;JMP