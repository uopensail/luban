grammar luban;
// antlr4 -Dlanguage=Python3 luban.g4

start                   : arithmetic_expression EOF;

arithmetic_expression   : arithmetic_expression T_MOD arithmetic_expression                                 # ModArithmeticExpression
                        | arithmetic_expression T_MUL arithmetic_expression                                 # MulArithmeticExpression
                        | arithmetic_expression T_DIV arithmetic_expression                                 # DivArithmeticExpression
                        | arithmetic_expression T_ADD arithmetic_expression                                 # AddArithmeticExpression
                        | arithmetic_expression T_SUB arithmetic_expression                                 # SubArithmeticExpression
                        | IDENTIFIER '(' ')'                                                                # RuntTimeFuncArithmeticExpression
                        | IDENTIFIER '(' arithmetic_expression (',' arithmetic_expression)* ')'             # FuncArithmeticExpression
                        | IDENTIFIER type_marker?                                                           # ColumnArithmeticExpression
                        | STRING                                                                            # StringArithmeticExpression
                        | INTEGER                                                                           # IntegerArithmeticExpression
                        | DECIMAL                                                                           # DecimalArithmeticExpression
                        | STRING_LIST                                                                       # StringLISTArithmeticExpression
                        | INTEGER_LIST                                                                      # IntegerLISTArithmeticExpression
                        | DECIMAL_LIST                                                                      # DecimalLISTArithmeticExpression
                        | '(' arithmetic_expression ')'                                                     # PlainArithmeticExpression
                        ;

type_marker             : '[' (T_INT|T_FLOAT|T_STRING|T_INTS|T_FLOATS|T_STRINGS) ']' ;

T_ADD                   : '+' ;
T_SUB                   : '-' ;
T_MUL                   : '*' ;
T_DIV                   : '/' ;
T_MOD                   : '%' ;

// reserved keywords
T_INT                   : I N T '6' '4' ;
T_FLOAT                 : F L O A T '3' '2' ;
T_STRING                : S T R I N G ;
T_INTS                  : I N T '6' '4' S;
T_FLOATS                : F L O A T '3' '2' S;
T_STRINGS               : S T R I N G S;

// Support case-insensitive keywords and allowing case-sensitive identifiers
fragment A              : ('a'|'A') ;
fragment B              : ('b'|'B') ;
fragment C              : ('c'|'C') ;
fragment D              : ('d'|'D') ;
fragment E              : ('e'|'E') ;
fragment F              : ('f'|'F') ;
fragment G              : ('g'|'G') ;
fragment H              : ('h'|'H') ;
fragment I              : ('i'|'I') ;
fragment J              : ('j'|'J') ;
fragment K              : ('k'|'K') ;
fragment L              : ('l'|'L') ;
fragment M              : ('m'|'M') ;
fragment N              : ('n'|'N') ;
fragment O              : ('o'|'O') ;
fragment P              : ('p'|'P') ;
fragment Q              : ('q'|'Q') ;
fragment R              : ('r'|'R') ;
fragment S              : ('s'|'S') ;
fragment T              : ('t'|'T') ;
fragment U              : ('u'|'U') ;
fragment V              : ('v'|'V') ;
fragment W              : ('w'|'W') ;
fragment X              : ('x'|'X') ;
fragment Y              : ('y'|'Y') ;
fragment Z              : ('z'|'Z') ;

IDENTIFIER              : [_a-zA-Z][_a-zA-Z0-9]* ;   

INTEGER_LIST            : '(' INTEGER (',' INTEGER)* ')' ;
INTEGER                 : '-'? '0' | [1-9] [0-9]* ;

DECIMAL_LIST            : '(' DECIMAL (',' DECIMAL)* ')' ;
DECIMAL                 : '-'? ('0' | [1-9] [0-9]*) '.' [0-9]* ;

STRING_LIST             : '(' STRING (',' STRING)* ')' ;
STRING                  : '"' (ESC | SAFECODEPOINT)* '"' ;

fragment ESC            : '\\' (["\\/bfnrt] | UNICODE) ;
fragment UNICODE        : 'u' HEX HEX HEX HEX ;
fragment HEX            : [0-9a-fA-F] ;
fragment SAFECODEPOINT  : ~ ["\\\u0000-\u001F] ;

// Ignore whitespace
WS                      : [ \t\n\r] + -> skip ;