// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.
//keyがキーボードのアドレスを表すように指定,@key→M

(LOOP)
@KBD
D=M //D=M[KBD]
@LOOP
D;JEQ //if D=0 goto LOOP
@SCREEN
D=A-1 //左隅のスクリーンbit-1
@i
M=D
(INC)
@i
M=M+1 //インクリメント
D=M //D=i
@24576
D=D-A //(i-24576)
@LOOP1
D;JEQ //(i-24576)=0ならループを抜ける
@32767
D=A //D=31
@i
A=M //Aが塗りつぶしたいRAMアドレス
M=D
M=M+D
M=M+1
@INC
0;JMP
(LOOP1)
@KBD
D=M //D=M[KBD]
@LOOP1
D;JNE //if D=1 goto LOOP1
@SCREEN
D=A-1 //左隅のスクリーンbit-1
@i
M=D
(RES)
@i
M=M+1 //インクリメント
D=M
@24576
D=D-A //D-24576
@LOOP
D;JEQ
@i
A=M //Aが塗りつぶしたいRAMアドレス
M=0
@RES
0;JMP