grammar luban;
options { caseInsensitive=true; }
// antlr4 -Dlanguage=Python3 luban.g4

start                   : arithmetic_expression EOF;

arithmetic_expression   : arithmetic_expression T_MOD arithmetic_expression                                 # ModArithmeticExpression
                        | arithmetic_expression T_MUL arithmetic_expression                                 # MulArithmeticExpression
                        | arithmetic_expression T_DIV arithmetic_expression                                 # DivArithmeticExpression
                        | arithmetic_expression T_ADD arithmetic_expression                                 # AddArithmeticExpression
                        | arithmetic_expression T_SUB arithmetic_expression                                 # SubArithmeticExpression
                        | IDENTIFIER BRACKET_OPEN BRACKET_CLOSE                                                                # RuntTimeFuncArithmeticExpression
                        | IDENTIFIER BRACKET_OPEN arithmetic_expression (COMMA arithmetic_expression)* BRACKET_CLOSE             # FuncArithmeticExpression
                        | IDENTIFIER type_marker                                                         # ColumnArithmeticExpression
                        | STRING                                                                            # StringArithmeticExpression
                        | INTEGER                                                                           # IntegerArithmeticExpression
                        | DECIMAL                                                                           # DecimalArithmeticExpression
                        | STRING_LIST                                                                       # StringLISTArithmeticExpression
                        | INTEGER_LIST                                                                      # IntegerLISTArithmeticExpression
                        | DECIMAL_LIST                                                                      # DecimalLISTArithmeticExpression
                        | BRACKET_OPEN arithmetic_expression BRACKET_CLOSE                                                     # PlainArithmeticExpression
                        ;

type_marker             : SQUARE_OPEN (T_INT|T_FLOAT|T_STRING|T_INTS|T_FLOATS|T_STRINGS) SQUARE_CLOSE ;


BRACKET_OPEN        : '(';
BRACKET_CLOSE       : ')';
SQUARE_OPEN         : '[';
SQUARE_CLOSE        : ']';
DOT                 : '.';
COMMA: ',';
QUOTA: '"';


T_ADD                   : '+' ;
T_SUB                   : '-' ;
T_MUL                   : '*' ;
T_DIV                   : '/' ;
T_MOD                   : '%' ;

// reserved keywords
T_INT                   : 'int64';
T_INTS                  : 'int64s' ;
T_FLOAT                 : 'float32';
T_FLOATS                : 'float32s';
T_STRING                : 'string';
T_STRINGS               : 'strings';


IDENTIFIER              options { caseInsensitive=false; } : [_a-zA-Z][_a-zA-Z0-9]*; // 变量

INTEGER_LIST            : BRACKET_OPEN (INTEGER COMMA)* INTEGER BRACKET_CLOSE ;
INTEGER                 : '-'? '0' | [1-9] [0-9]* ;

DECIMAL_LIST            : BRACKET_OPEN (DECIMAL COMMA)* DECIMAL BRACKET_CLOSE ;
DECIMAL                 : '-'? ('0' | [1-9] [0-9]*) '.' [0-9]* ;

STRING_LIST             : BRACKET_OPEN (STRING COMMA)* STRING BRACKET_CLOSE ;

STRING options { caseInsensitive=false; } : QUOTA (ESC | SAFECODEPOINT)* QUOTA;

fragment ESC: '\\' (["\\/bfnrt] | UNICODE);
fragment UNICODE: 'u' HEX HEX HEX HEX;
fragment HEX options { caseInsensitive=false; }  : [0-9a-fA-F];
fragment SAFECODEPOINT: ~ ["\\\u0000-\u001F];

// Ignore whitespace
WS                      : [ \t\n\r] + -> skip ;