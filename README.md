# FraCtuS
Lexer, parser and interpreter (evaluator) of the _FraCtuS_ language. This project is strongly based on excellent
[tutorial by Ruslan Spivak](https://ruslanspivak.com/). Some features were also taken from 
[this site](http://www.craftinginterpreters.com/). I would like to recommend both of these tutorials to
anyone who want to build their own compiler/interpreter from scratch.  
This program was written for university course and still contains many bugs and lacks many important feautures.

## The language
The _FraCtuS_ language is a simple Pascal/C based language
with built-in fraction type. Variables must be declared at the beginning of a block (like in Pascal). Procedures can
take parameters and return values (like in C).

Types supported are: boolean, integer, fraction and string. _Fraction_ type actually represents a mixed number with
integer as a whole number, another integer as a fraction's numerator and a natural number as a denominator.
Sadly, arrays are not supported :(

### Grammar of FraCtuS language
```
Program      = "program" , Identifier , ';' , Block , '.' ;
Block        = VarPart , FuncPart , CompStment ;
VarPart      = empty | "var" , VarDecl , ';' , { VarDecl , ';' } ;
VarDecl      = Identifier , { ',' , Identifier }, ':' , Type ;
FuncPart     = { FuncDecl , ';' } ;
FuncDecl     = Type , WhiteSpace , Identifier , '(' , [ Type , Identifier {, ',' ,
               Type, Identifier } ] , ')' , ';' , Block ;
CompStment   = "begin" , Stment , { ';' , Stment } , "end" ;
Stment       = Assignment | FuncCall | Return | IfStment | WhileStment | CompStment ;
Assignment   = Var , '=' , Expr ;
FuncCall     = Identifier , '(' [, Expr, { "," , Expr } ], ')' ;
Return       = "return" , Expr ;
IfStment     = SimpleIf [, "else" , Stment ] ;
SimpleIf     = "if" , Expr , "then" , Stment ;
WhileStment  = "while" , Expr , "do" , Stment ;
Expr         = SimpExpr [, RelOp , SimpExpr ] ;
SimpExpr     = [Sign ,] Term , { AddOp , Term } | Bool ;
Term         = Factor , { MultOp , Factor } ;
Factor       = FuncCall | Var | Constant | '(' , Expr , ')' | '!' , Factor ;
RelOp        = '==' | '<' | '>' | '<=' | '!=' | '>=' | "or" | "and" ;
EqRelOp      = '==' | '!='
Sign         = '+' | '-' ;
AddOp        = '+' | '-' ; # med priority
MultOp       = '*' | "/" ; # high priority
Var          = Identifier ;
Type         = "fraction" , "integer" , "boolean" , "string" ;

##

Identifier          = Alpha , { Alpha | Digit } ;
Fraction            = SimpleFraction | Int , [ '.' , SimpleFraction ] ;
SimpleFraction      = Int , "_" , NaturalNum;
Int                 = "0" | [ "-" , ] NaturalNum ;
NaturalNum          = DigitWithoutZero , { Digit } ;
Digit               = "0" | DigitWithoutZero ;
DigitWithoutZero    = "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
String              = '"' , { AllChars - '"' } , '"' ;
Alpha               =  "A" | "B" | "C" | "D" | "E" | "F" | "G"
                     | "H" | "I" | "J" | "K" | "L" | "M" | "N"
                     | "O" | "P" | "Q" | "R" | "S" | "T" | "U"
                     | "V" | "W" | "X" | "Y" | "Z"
                     | "a" | "b" | "c" | "d" | "e" | "f" | "g"
                     | "h" | "i" | "j" | "k" | "l" | "m" | "n"
                     | "o" | "p" | "q" | "r" | "s" | "t" | "u"
                     | "v" | "w" | "x" | "y" | "z" ;
Bool                = "false" | "true" ;
Constant            = Fraction | String | ConstIdent ;
ConstIdent          = Identifier ;
Comment             = "#" ,  AllChars ;
WhiteSpace          = ? all whitespace characters ?
AllChars            = ? all visible characters ? ;
```

## Compilation and running
### How to compile
```
mkdir build && cd build
cmake ..
make
```

### Run with test input file
```
./fractus ../in2.txt
```
