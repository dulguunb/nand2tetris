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
//256(row)x512 (n * m)
// Put your code here.


(OUTERLOOP)
  @KBD
  D=M

  @PRESSED
  D;JGT

  @NOTPRESSED
  0;JMP


(PRESSED)
@R0
M=-1
@DRAW
0;JMP

(NOTPRESSED)
@R0
M=0
@DRAW
0;JMP

  (DRAW)
    @8191
    D=A
    @R1
    M=D
    @i
    M=0

    (INNERLOOP)
      @R1
      D=M
      @pos
      M=D
      @SCREEN
      D=A
      @pos
      M=M+D

      
      @R0
      D=M // selected color
      @pos
      A=M
      M=D

      @R1
      D=M-1
      M=D

      @INNERLOOP
      D;JGE

      
      

@OUTERLOOP
0;JMP
