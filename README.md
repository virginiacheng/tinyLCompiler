# tinyLCompiler
 Description: Create a compiler and optimizer for tinyL Language

For this project, I was required to fill in the code. For Compiler.c file, I worked on variable(), expr(), assign(), read(), print(), stmt(), morestmts(), stmtlist(), program(). For Optimizer.c, I was required to write the basic algorithm to eliminate the dead code in the given set of instructions.
 
-------------------------------------------------
            CFG for tinyL LANGUAGE
     PROGRAM ::= STMTLIST .
     STMTLIST ::= STMT MORESTMTS
     MORESTMTS ::= ; STMTLIST | epsilon
     STMT ::= ASSIGN | READ | PRINT
     ASSIGN ::= VARIABLE = EXPR
     READ ::= ! VARIABLE
     PRINT ::= # VARIABLE
     EXPR ::= + EXPR EXPR |
              - EXPR EXPR |
              * EXPR EXPR |
              & EXPR EXPR |
              ^ EXPR EXPR |
              VARIABLE |
              DIGIT
     VARIABLE ::= a | b | c | d | e
     DIGIT ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
     NOTE: tokens are exactly a single character long
     Example expressions:
           +12.
           &1b.
           +*34-78.
           -*+1^2a58.
     Example programs;
         !a;!b;c=+3*ab;d=+c1;#d.
         b=-*+1&2a58;#b.
 ---------------------------------------------------
