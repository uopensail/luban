# Generated from luban.g4 by ANTLR 4.13.1
# encoding: utf-8
from antlr4 import *
from io import StringIO
import sys
if sys.version_info[1] > 5:
	from typing import TextIO
else:
	from typing.io import TextIO

def serializedATN():
    return [
        4,1,24,66,2,0,7,0,2,1,7,1,2,2,7,2,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,5,1,19,8,1,10,1,12,1,22,9,1,1,1,1,1,1,1,1,1,3,
        1,28,8,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,40,8,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,1,57,8,
        1,10,1,12,1,60,9,1,1,2,1,2,1,2,1,2,1,2,0,1,2,3,0,2,4,0,1,1,0,11,
        16,78,0,6,1,0,0,0,2,39,1,0,0,0,4,61,1,0,0,0,6,7,3,2,1,0,7,8,5,0,
        0,1,8,1,1,0,0,0,9,10,6,1,-1,0,10,11,5,17,0,0,11,12,5,1,0,0,12,40,
        5,2,0,0,13,14,5,17,0,0,14,15,5,1,0,0,15,20,3,2,1,0,16,17,5,3,0,0,
        17,19,3,2,1,0,18,16,1,0,0,0,19,22,1,0,0,0,20,18,1,0,0,0,20,21,1,
        0,0,0,21,23,1,0,0,0,22,20,1,0,0,0,23,24,5,2,0,0,24,40,1,0,0,0,25,
        27,5,17,0,0,26,28,3,4,2,0,27,26,1,0,0,0,27,28,1,0,0,0,28,40,1,0,
        0,0,29,40,5,23,0,0,30,40,5,19,0,0,31,40,5,21,0,0,32,40,5,22,0,0,
        33,40,5,18,0,0,34,40,5,20,0,0,35,36,5,1,0,0,36,37,3,2,1,0,37,38,
        5,2,0,0,38,40,1,0,0,0,39,9,1,0,0,0,39,13,1,0,0,0,39,25,1,0,0,0,39,
        29,1,0,0,0,39,30,1,0,0,0,39,31,1,0,0,0,39,32,1,0,0,0,39,33,1,0,0,
        0,39,34,1,0,0,0,39,35,1,0,0,0,40,58,1,0,0,0,41,42,10,15,0,0,42,43,
        5,10,0,0,43,57,3,2,1,16,44,45,10,14,0,0,45,46,5,8,0,0,46,57,3,2,
        1,15,47,48,10,13,0,0,48,49,5,9,0,0,49,57,3,2,1,14,50,51,10,12,0,
        0,51,52,5,6,0,0,52,57,3,2,1,13,53,54,10,11,0,0,54,55,5,7,0,0,55,
        57,3,2,1,12,56,41,1,0,0,0,56,44,1,0,0,0,56,47,1,0,0,0,56,50,1,0,
        0,0,56,53,1,0,0,0,57,60,1,0,0,0,58,56,1,0,0,0,58,59,1,0,0,0,59,3,
        1,0,0,0,60,58,1,0,0,0,61,62,5,4,0,0,62,63,7,0,0,0,63,64,5,5,0,0,
        64,5,1,0,0,0,5,20,27,39,56,58
    ]

class lubanParser ( Parser ):

    grammarFileName = "luban.g4"

    atn = ATNDeserializer().deserialize(serializedATN())

    decisionsToDFA = [ DFA(ds, i) for i, ds in enumerate(atn.decisionToState) ]

    sharedContextCache = PredictionContextCache()

    literalNames = [ "<INVALID>", "'('", "')'", "','", "'['", "']'", "'+'", 
                     "'-'", "'*'", "'/'", "'%'" ]

    symbolicNames = [ "<INVALID>", "<INVALID>", "<INVALID>", "<INVALID>", 
                      "<INVALID>", "<INVALID>", "T_ADD", "T_SUB", "T_MUL", 
                      "T_DIV", "T_MOD", "T_INT", "T_FLOAT", "T_STRING", 
                      "T_INTS", "T_FLOATS", "T_STRINGS", "IDENTIFIER", "INTEGER_LIST", 
                      "INTEGER", "DECIMAL_LIST", "DECIMAL", "STRING_LIST", 
                      "STRING", "WS" ]

    RULE_start = 0
    RULE_arithmetic_expression = 1
    RULE_type_marker = 2

    ruleNames =  [ "start", "arithmetic_expression", "type_marker" ]

    EOF = Token.EOF
    T__0=1
    T__1=2
    T__2=3
    T__3=4
    T__4=5
    T_ADD=6
    T_SUB=7
    T_MUL=8
    T_DIV=9
    T_MOD=10
    T_INT=11
    T_FLOAT=12
    T_STRING=13
    T_INTS=14
    T_FLOATS=15
    T_STRINGS=16
    IDENTIFIER=17
    INTEGER_LIST=18
    INTEGER=19
    DECIMAL_LIST=20
    DECIMAL=21
    STRING_LIST=22
    STRING=23
    WS=24

    def __init__(self, input:TokenStream, output:TextIO = sys.stdout):
        super().__init__(input, output)
        self.checkVersion("4.13.1")
        self._interp = ParserATNSimulator(self, self.atn, self.decisionsToDFA, self.sharedContextCache)
        self._predicates = None




    class StartContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser

        def arithmetic_expression(self):
            return self.getTypedRuleContext(lubanParser.Arithmetic_expressionContext,0)


        def EOF(self):
            return self.getToken(lubanParser.EOF, 0)

        def getRuleIndex(self):
            return lubanParser.RULE_start

        def enterRule(self, listener:ParseTreeListener):
            if hasattr( listener, "enterStart" ):
                listener.enterStart(self)

        def exitRule(self, listener:ParseTreeListener):
            if hasattr( listener, "exitStart" ):
                listener.exitStart(self)




    def start(self):

        localctx = lubanParser.StartContext(self, self._ctx, self.state)
        self.enterRule(localctx, 0, self.RULE_start)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 6
            self.arithmetic_expression(0)
            self.state = 7
            self.match(lubanParser.EOF)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class Arithmetic_expressionContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser


        def getRuleIndex(self):
            return lubanParser.RULE_arithmetic_expression

     
        def copyFrom(self, ctx:ParserRuleContext):
            super().copyFrom(ctx)


    class PlainArithmeticExpressionContext(Arithmetic_expressionContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a lubanParser.Arithmetic_expressionContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def arithmetic_expression(self):
            return self.getTypedRuleContext(lubanParser.Arithmetic_expressionContext,0)


        def enterRule(self, listener:ParseTreeListener):
            if hasattr( listener, "enterPlainArithmeticExpression" ):
                listener.enterPlainArithmeticExpression(self)

        def exitRule(self, listener:ParseTreeListener):
            if hasattr( listener, "exitPlainArithmeticExpression" ):
                listener.exitPlainArithmeticExpression(self)


    class AddArithmeticExpressionContext(Arithmetic_expressionContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a lubanParser.Arithmetic_expressionContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def arithmetic_expression(self, i:int=None):
            if i is None:
                return self.getTypedRuleContexts(lubanParser.Arithmetic_expressionContext)
            else:
                return self.getTypedRuleContext(lubanParser.Arithmetic_expressionContext,i)

        def T_ADD(self):
            return self.getToken(lubanParser.T_ADD, 0)

        def enterRule(self, listener:ParseTreeListener):
            if hasattr( listener, "enterAddArithmeticExpression" ):
                listener.enterAddArithmeticExpression(self)

        def exitRule(self, listener:ParseTreeListener):
            if hasattr( listener, "exitAddArithmeticExpression" ):
                listener.exitAddArithmeticExpression(self)


    class StringArithmeticExpressionContext(Arithmetic_expressionContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a lubanParser.Arithmetic_expressionContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def STRING(self):
            return self.getToken(lubanParser.STRING, 0)

        def enterRule(self, listener:ParseTreeListener):
            if hasattr( listener, "enterStringArithmeticExpression" ):
                listener.enterStringArithmeticExpression(self)

        def exitRule(self, listener:ParseTreeListener):
            if hasattr( listener, "exitStringArithmeticExpression" ):
                listener.exitStringArithmeticExpression(self)


    class IntegerArithmeticExpressionContext(Arithmetic_expressionContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a lubanParser.Arithmetic_expressionContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def INTEGER(self):
            return self.getToken(lubanParser.INTEGER, 0)

        def enterRule(self, listener:ParseTreeListener):
            if hasattr( listener, "enterIntegerArithmeticExpression" ):
                listener.enterIntegerArithmeticExpression(self)

        def exitRule(self, listener:ParseTreeListener):
            if hasattr( listener, "exitIntegerArithmeticExpression" ):
                listener.exitIntegerArithmeticExpression(self)


    class DecimalArithmeticExpressionContext(Arithmetic_expressionContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a lubanParser.Arithmetic_expressionContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def DECIMAL(self):
            return self.getToken(lubanParser.DECIMAL, 0)

        def enterRule(self, listener:ParseTreeListener):
            if hasattr( listener, "enterDecimalArithmeticExpression" ):
                listener.enterDecimalArithmeticExpression(self)

        def exitRule(self, listener:ParseTreeListener):
            if hasattr( listener, "exitDecimalArithmeticExpression" ):
                listener.exitDecimalArithmeticExpression(self)


    class FuncArithmeticExpressionContext(Arithmetic_expressionContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a lubanParser.Arithmetic_expressionContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def IDENTIFIER(self):
            return self.getToken(lubanParser.IDENTIFIER, 0)
        def arithmetic_expression(self, i:int=None):
            if i is None:
                return self.getTypedRuleContexts(lubanParser.Arithmetic_expressionContext)
            else:
                return self.getTypedRuleContext(lubanParser.Arithmetic_expressionContext,i)


        def enterRule(self, listener:ParseTreeListener):
            if hasattr( listener, "enterFuncArithmeticExpression" ):
                listener.enterFuncArithmeticExpression(self)

        def exitRule(self, listener:ParseTreeListener):
            if hasattr( listener, "exitFuncArithmeticExpression" ):
                listener.exitFuncArithmeticExpression(self)


    class ColumnArithmeticExpressionContext(Arithmetic_expressionContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a lubanParser.Arithmetic_expressionContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def IDENTIFIER(self):
            return self.getToken(lubanParser.IDENTIFIER, 0)
        def type_marker(self):
            return self.getTypedRuleContext(lubanParser.Type_markerContext,0)


        def enterRule(self, listener:ParseTreeListener):
            if hasattr( listener, "enterColumnArithmeticExpression" ):
                listener.enterColumnArithmeticExpression(self)

        def exitRule(self, listener:ParseTreeListener):
            if hasattr( listener, "exitColumnArithmeticExpression" ):
                listener.exitColumnArithmeticExpression(self)


    class DivArithmeticExpressionContext(Arithmetic_expressionContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a lubanParser.Arithmetic_expressionContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def arithmetic_expression(self, i:int=None):
            if i is None:
                return self.getTypedRuleContexts(lubanParser.Arithmetic_expressionContext)
            else:
                return self.getTypedRuleContext(lubanParser.Arithmetic_expressionContext,i)

        def T_DIV(self):
            return self.getToken(lubanParser.T_DIV, 0)

        def enterRule(self, listener:ParseTreeListener):
            if hasattr( listener, "enterDivArithmeticExpression" ):
                listener.enterDivArithmeticExpression(self)

        def exitRule(self, listener:ParseTreeListener):
            if hasattr( listener, "exitDivArithmeticExpression" ):
                listener.exitDivArithmeticExpression(self)


    class SubArithmeticExpressionContext(Arithmetic_expressionContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a lubanParser.Arithmetic_expressionContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def arithmetic_expression(self, i:int=None):
            if i is None:
                return self.getTypedRuleContexts(lubanParser.Arithmetic_expressionContext)
            else:
                return self.getTypedRuleContext(lubanParser.Arithmetic_expressionContext,i)

        def T_SUB(self):
            return self.getToken(lubanParser.T_SUB, 0)

        def enterRule(self, listener:ParseTreeListener):
            if hasattr( listener, "enterSubArithmeticExpression" ):
                listener.enterSubArithmeticExpression(self)

        def exitRule(self, listener:ParseTreeListener):
            if hasattr( listener, "exitSubArithmeticExpression" ):
                listener.exitSubArithmeticExpression(self)


    class IntegerLISTArithmeticExpressionContext(Arithmetic_expressionContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a lubanParser.Arithmetic_expressionContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def INTEGER_LIST(self):
            return self.getToken(lubanParser.INTEGER_LIST, 0)

        def enterRule(self, listener:ParseTreeListener):
            if hasattr( listener, "enterIntegerLISTArithmeticExpression" ):
                listener.enterIntegerLISTArithmeticExpression(self)

        def exitRule(self, listener:ParseTreeListener):
            if hasattr( listener, "exitIntegerLISTArithmeticExpression" ):
                listener.exitIntegerLISTArithmeticExpression(self)


    class ModArithmeticExpressionContext(Arithmetic_expressionContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a lubanParser.Arithmetic_expressionContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def arithmetic_expression(self, i:int=None):
            if i is None:
                return self.getTypedRuleContexts(lubanParser.Arithmetic_expressionContext)
            else:
                return self.getTypedRuleContext(lubanParser.Arithmetic_expressionContext,i)

        def T_MOD(self):
            return self.getToken(lubanParser.T_MOD, 0)

        def enterRule(self, listener:ParseTreeListener):
            if hasattr( listener, "enterModArithmeticExpression" ):
                listener.enterModArithmeticExpression(self)

        def exitRule(self, listener:ParseTreeListener):
            if hasattr( listener, "exitModArithmeticExpression" ):
                listener.exitModArithmeticExpression(self)


    class RuntTimeFuncArithmeticExpressionContext(Arithmetic_expressionContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a lubanParser.Arithmetic_expressionContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def IDENTIFIER(self):
            return self.getToken(lubanParser.IDENTIFIER, 0)

        def enterRule(self, listener:ParseTreeListener):
            if hasattr( listener, "enterRuntTimeFuncArithmeticExpression" ):
                listener.enterRuntTimeFuncArithmeticExpression(self)

        def exitRule(self, listener:ParseTreeListener):
            if hasattr( listener, "exitRuntTimeFuncArithmeticExpression" ):
                listener.exitRuntTimeFuncArithmeticExpression(self)


    class DecimalLISTArithmeticExpressionContext(Arithmetic_expressionContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a lubanParser.Arithmetic_expressionContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def DECIMAL_LIST(self):
            return self.getToken(lubanParser.DECIMAL_LIST, 0)

        def enterRule(self, listener:ParseTreeListener):
            if hasattr( listener, "enterDecimalLISTArithmeticExpression" ):
                listener.enterDecimalLISTArithmeticExpression(self)

        def exitRule(self, listener:ParseTreeListener):
            if hasattr( listener, "exitDecimalLISTArithmeticExpression" ):
                listener.exitDecimalLISTArithmeticExpression(self)


    class MulArithmeticExpressionContext(Arithmetic_expressionContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a lubanParser.Arithmetic_expressionContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def arithmetic_expression(self, i:int=None):
            if i is None:
                return self.getTypedRuleContexts(lubanParser.Arithmetic_expressionContext)
            else:
                return self.getTypedRuleContext(lubanParser.Arithmetic_expressionContext,i)

        def T_MUL(self):
            return self.getToken(lubanParser.T_MUL, 0)

        def enterRule(self, listener:ParseTreeListener):
            if hasattr( listener, "enterMulArithmeticExpression" ):
                listener.enterMulArithmeticExpression(self)

        def exitRule(self, listener:ParseTreeListener):
            if hasattr( listener, "exitMulArithmeticExpression" ):
                listener.exitMulArithmeticExpression(self)


    class StringLISTArithmeticExpressionContext(Arithmetic_expressionContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a lubanParser.Arithmetic_expressionContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def STRING_LIST(self):
            return self.getToken(lubanParser.STRING_LIST, 0)

        def enterRule(self, listener:ParseTreeListener):
            if hasattr( listener, "enterStringLISTArithmeticExpression" ):
                listener.enterStringLISTArithmeticExpression(self)

        def exitRule(self, listener:ParseTreeListener):
            if hasattr( listener, "exitStringLISTArithmeticExpression" ):
                listener.exitStringLISTArithmeticExpression(self)



    def arithmetic_expression(self, _p:int=0):
        _parentctx = self._ctx
        _parentState = self.state
        localctx = lubanParser.Arithmetic_expressionContext(self, self._ctx, _parentState)
        _prevctx = localctx
        _startState = 2
        self.enterRecursionRule(localctx, 2, self.RULE_arithmetic_expression, _p)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 39
            self._errHandler.sync(self)
            la_ = self._interp.adaptivePredict(self._input,2,self._ctx)
            if la_ == 1:
                localctx = lubanParser.RuntTimeFuncArithmeticExpressionContext(self, localctx)
                self._ctx = localctx
                _prevctx = localctx

                self.state = 10
                self.match(lubanParser.IDENTIFIER)
                self.state = 11
                self.match(lubanParser.T__0)
                self.state = 12
                self.match(lubanParser.T__1)
                pass

            elif la_ == 2:
                localctx = lubanParser.FuncArithmeticExpressionContext(self, localctx)
                self._ctx = localctx
                _prevctx = localctx
                self.state = 13
                self.match(lubanParser.IDENTIFIER)
                self.state = 14
                self.match(lubanParser.T__0)
                self.state = 15
                self.arithmetic_expression(0)
                self.state = 20
                self._errHandler.sync(self)
                _la = self._input.LA(1)
                while _la==3:
                    self.state = 16
                    self.match(lubanParser.T__2)
                    self.state = 17
                    self.arithmetic_expression(0)
                    self.state = 22
                    self._errHandler.sync(self)
                    _la = self._input.LA(1)

                self.state = 23
                self.match(lubanParser.T__1)
                pass

            elif la_ == 3:
                localctx = lubanParser.ColumnArithmeticExpressionContext(self, localctx)
                self._ctx = localctx
                _prevctx = localctx
                self.state = 25
                self.match(lubanParser.IDENTIFIER)
                self.state = 27
                self._errHandler.sync(self)
                la_ = self._interp.adaptivePredict(self._input,1,self._ctx)
                if la_ == 1:
                    self.state = 26
                    self.type_marker()


                pass

            elif la_ == 4:
                localctx = lubanParser.StringArithmeticExpressionContext(self, localctx)
                self._ctx = localctx
                _prevctx = localctx
                self.state = 29
                self.match(lubanParser.STRING)
                pass

            elif la_ == 5:
                localctx = lubanParser.IntegerArithmeticExpressionContext(self, localctx)
                self._ctx = localctx
                _prevctx = localctx
                self.state = 30
                self.match(lubanParser.INTEGER)
                pass

            elif la_ == 6:
                localctx = lubanParser.DecimalArithmeticExpressionContext(self, localctx)
                self._ctx = localctx
                _prevctx = localctx
                self.state = 31
                self.match(lubanParser.DECIMAL)
                pass

            elif la_ == 7:
                localctx = lubanParser.StringLISTArithmeticExpressionContext(self, localctx)
                self._ctx = localctx
                _prevctx = localctx
                self.state = 32
                self.match(lubanParser.STRING_LIST)
                pass

            elif la_ == 8:
                localctx = lubanParser.IntegerLISTArithmeticExpressionContext(self, localctx)
                self._ctx = localctx
                _prevctx = localctx
                self.state = 33
                self.match(lubanParser.INTEGER_LIST)
                pass

            elif la_ == 9:
                localctx = lubanParser.DecimalLISTArithmeticExpressionContext(self, localctx)
                self._ctx = localctx
                _prevctx = localctx
                self.state = 34
                self.match(lubanParser.DECIMAL_LIST)
                pass

            elif la_ == 10:
                localctx = lubanParser.PlainArithmeticExpressionContext(self, localctx)
                self._ctx = localctx
                _prevctx = localctx
                self.state = 35
                self.match(lubanParser.T__0)
                self.state = 36
                self.arithmetic_expression(0)
                self.state = 37
                self.match(lubanParser.T__1)
                pass


            self._ctx.stop = self._input.LT(-1)
            self.state = 58
            self._errHandler.sync(self)
            _alt = self._interp.adaptivePredict(self._input,4,self._ctx)
            while _alt!=2 and _alt!=ATN.INVALID_ALT_NUMBER:
                if _alt==1:
                    if self._parseListeners is not None:
                        self.triggerExitRuleEvent()
                    _prevctx = localctx
                    self.state = 56
                    self._errHandler.sync(self)
                    la_ = self._interp.adaptivePredict(self._input,3,self._ctx)
                    if la_ == 1:
                        localctx = lubanParser.ModArithmeticExpressionContext(self, lubanParser.Arithmetic_expressionContext(self, _parentctx, _parentState))
                        self.pushNewRecursionContext(localctx, _startState, self.RULE_arithmetic_expression)
                        self.state = 41
                        if not self.precpred(self._ctx, 15):
                            from antlr4.error.Errors import FailedPredicateException
                            raise FailedPredicateException(self, "self.precpred(self._ctx, 15)")
                        self.state = 42
                        self.match(lubanParser.T_MOD)
                        self.state = 43
                        self.arithmetic_expression(16)
                        pass

                    elif la_ == 2:
                        localctx = lubanParser.MulArithmeticExpressionContext(self, lubanParser.Arithmetic_expressionContext(self, _parentctx, _parentState))
                        self.pushNewRecursionContext(localctx, _startState, self.RULE_arithmetic_expression)
                        self.state = 44
                        if not self.precpred(self._ctx, 14):
                            from antlr4.error.Errors import FailedPredicateException
                            raise FailedPredicateException(self, "self.precpred(self._ctx, 14)")
                        self.state = 45
                        self.match(lubanParser.T_MUL)
                        self.state = 46
                        self.arithmetic_expression(15)
                        pass

                    elif la_ == 3:
                        localctx = lubanParser.DivArithmeticExpressionContext(self, lubanParser.Arithmetic_expressionContext(self, _parentctx, _parentState))
                        self.pushNewRecursionContext(localctx, _startState, self.RULE_arithmetic_expression)
                        self.state = 47
                        if not self.precpred(self._ctx, 13):
                            from antlr4.error.Errors import FailedPredicateException
                            raise FailedPredicateException(self, "self.precpred(self._ctx, 13)")
                        self.state = 48
                        self.match(lubanParser.T_DIV)
                        self.state = 49
                        self.arithmetic_expression(14)
                        pass

                    elif la_ == 4:
                        localctx = lubanParser.AddArithmeticExpressionContext(self, lubanParser.Arithmetic_expressionContext(self, _parentctx, _parentState))
                        self.pushNewRecursionContext(localctx, _startState, self.RULE_arithmetic_expression)
                        self.state = 50
                        if not self.precpred(self._ctx, 12):
                            from antlr4.error.Errors import FailedPredicateException
                            raise FailedPredicateException(self, "self.precpred(self._ctx, 12)")
                        self.state = 51
                        self.match(lubanParser.T_ADD)
                        self.state = 52
                        self.arithmetic_expression(13)
                        pass

                    elif la_ == 5:
                        localctx = lubanParser.SubArithmeticExpressionContext(self, lubanParser.Arithmetic_expressionContext(self, _parentctx, _parentState))
                        self.pushNewRecursionContext(localctx, _startState, self.RULE_arithmetic_expression)
                        self.state = 53
                        if not self.precpred(self._ctx, 11):
                            from antlr4.error.Errors import FailedPredicateException
                            raise FailedPredicateException(self, "self.precpred(self._ctx, 11)")
                        self.state = 54
                        self.match(lubanParser.T_SUB)
                        self.state = 55
                        self.arithmetic_expression(12)
                        pass

             
                self.state = 60
                self._errHandler.sync(self)
                _alt = self._interp.adaptivePredict(self._input,4,self._ctx)

        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.unrollRecursionContexts(_parentctx)
        return localctx


    class Type_markerContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser

        def T_INT(self):
            return self.getToken(lubanParser.T_INT, 0)

        def T_FLOAT(self):
            return self.getToken(lubanParser.T_FLOAT, 0)

        def T_STRING(self):
            return self.getToken(lubanParser.T_STRING, 0)

        def T_INTS(self):
            return self.getToken(lubanParser.T_INTS, 0)

        def T_FLOATS(self):
            return self.getToken(lubanParser.T_FLOATS, 0)

        def T_STRINGS(self):
            return self.getToken(lubanParser.T_STRINGS, 0)

        def getRuleIndex(self):
            return lubanParser.RULE_type_marker

        def enterRule(self, listener:ParseTreeListener):
            if hasattr( listener, "enterType_marker" ):
                listener.enterType_marker(self)

        def exitRule(self, listener:ParseTreeListener):
            if hasattr( listener, "exitType_marker" ):
                listener.exitType_marker(self)




    def type_marker(self):

        localctx = lubanParser.Type_markerContext(self, self._ctx, self.state)
        self.enterRule(localctx, 4, self.RULE_type_marker)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 61
            self.match(lubanParser.T__3)
            self.state = 62
            _la = self._input.LA(1)
            if not((((_la) & ~0x3f) == 0 and ((1 << _la) & 129024) != 0)):
                self._errHandler.recoverInline(self)
            else:
                self._errHandler.reportMatch(self)
                self.consume()
            self.state = 63
            self.match(lubanParser.T__4)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx



    def sempred(self, localctx:RuleContext, ruleIndex:int, predIndex:int):
        if self._predicates == None:
            self._predicates = dict()
        self._predicates[1] = self.arithmetic_expression_sempred
        pred = self._predicates.get(ruleIndex, None)
        if pred is None:
            raise Exception("No predicate with index:" + str(ruleIndex))
        else:
            return pred(localctx, predIndex)

    def arithmetic_expression_sempred(self, localctx:Arithmetic_expressionContext, predIndex:int):
            if predIndex == 0:
                return self.precpred(self._ctx, 15)
         

            if predIndex == 1:
                return self.precpred(self._ctx, 14)
         

            if predIndex == 2:
                return self.precpred(self._ctx, 13)
         

            if predIndex == 3:
                return self.precpred(self._ctx, 12)
         

            if predIndex == 4:
                return self.precpred(self._ctx, 11)
         




