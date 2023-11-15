#!/usr/bin/python3
# -*- coding: UTF-8 -*-
#
# `LuBan` - 'c++ tool for transformating and hashing features'
# Copyright (C) 2019 - present timepi <timepi123@gmail.com>
# LuBan is provided under: GNU Affero General Public License (AGPL3.0)
# https://www.gnu.org/licenses/agpl-3.0.html unless stated otherwise.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Affero General Public License for more details.
#
# author: timepi
# descripton: Use Python to parse the configuration file
#            for C++ code to use.

from .luban_types import (
    Function,
    Variable,
    DataType,
    Int64,
    Int64s,
    Float32,
    Float32s,
    String,
    Strings,
)
from .lubanLexer import lubanLexer
from .lubanParser import lubanParser
from .lubanListener import lubanListener


class LubanListener(lubanListener):
    def __init__(self):
        self.parameters = []

    # Enter a parse tree produced by lubanParser#start.
    def enterStart(self, ctx: lubanParser.StartContext):
        pass

    # Exit a parse tree produced by lubanParser#start.
    def exitStart(self, ctx: lubanParser.StartContext):
        pass

    # Exit a parse tree produced by lubanParser#AddArithmeticExpression.
    def exitAddArithmeticExpression(
        self, ctx: lubanParser.AddArithmeticExpressionContext
    ):
        right = self.parameters.pop()
        left = self.parameters.pop()
        self.parameters.append(Function("+", [left, right]))

    # Exit a parse tree produced by lubanParser#StringLISTArithmeticExpression.
    def exitStringLISTArithmeticExpression(
        self, ctx: lubanParser.StringLISTArithmeticExpressionContext
    ):
        list_text = ctx.STRING_LIST().getText()[1:-1]
        self.parameters.append(Strings(list[eval(eval(list_text))]))

    # Exit a parse tree produced by lubanParser#StringArithmeticExpression.
    def exitStringArithmeticExpression(
        self, ctx: lubanParser.StringArithmeticExpressionContext
    ):
        self.parameters.append(String(ctx.STRING().getText()))

    # Exit a parse tree produced by lubanParser#IntegerLISTArithmeticExpression.
    def exitIntegerLISTArithmeticExpression(
        self, ctx: lubanParser.IntegerLISTArithmeticExpressionContext
    ):
        list_text = ctx.INTEGER_LIST().getText()[1:-1]
        self.parameters.append(Int64s(list(eval(list_text))))

    # Exit a parse tree produced by lubanParser#IntegerArithmeticExpression.
    def exitIntegerArithmeticExpression(
        self, ctx: lubanParser.IntegerArithmeticExpressionContext
    ):
        self.parameters.append(Int64(int(ctx.INTEGER().getText())))

    # Exit a parse tree produced by lubanParser#DecimalLISTArithmeticExpression.
    def exitDecimalLISTArithmeticExpression(
        self, ctx: lubanParser.DecimalLISTArithmeticExpressionContext
    ):
        list_text = ctx.DECIMAL_LIST().getText()[1:-1]
        self.parameters.append(Float32s(list[eval(eval(list_text))]))

    # Exit a parse tree produced by lubanParser#DecimalArithmeticExpression.
    def exitDecimalArithmeticExpression(
        self, ctx: lubanParser.DecimalArithmeticExpressionContext
    ):
        self.parameters.append(Float32(float(ctx.DECIMAL().getText())))

    # Exit a parse tree produced by lubanParser#FuncArithmeticExpression.
    def exitFuncArithmeticExpression(
        self, ctx: lubanParser.FuncArithmeticExpressionContext
    ):
        size = len(ctx.arithmetic_expression())
        args = []
        for _ in range(size):
            args.append(self.parameters.pop())
        args = args[::-1]
        func = ctx.IDENTIFIER().getText().lower()
        self.parameters.append(Function(func, args))

    # Exit a parse tree produced by lubanParser#ColumnArithmeticExpression.
    def exitColumnArithmeticExpression(
        self, ctx: lubanParser.ColumnArithmeticExpressionContext
    ):
        marker = "[float32]"
        if ctx.type_marker() is not None:
            marker = ctx.type_marker().getText().lower()
        dtype = DataType.kFloat32
        if marker == "[int64]":
            dtype = DataType.kInt64
        elif marker == "[string]":
            dtype = DataType.kString
        elif marker == "[float32]":
            dtype = DataType.kFloat32
        elif marker == "[int64s]":
            dtype = DataType.kInt64s
        elif marker == "[strings]":
            dtype = DataType.kStrings
        elif marker == "[float32s]":
            dtype = DataType.kFloat32s
        self.parameters.append(Variable(ctx.IDENTIFIER().getText(), dtype))

    # Exit a parse tree produced by lubanParser#DivArithmeticExpression.
    def exitDivArithmeticExpression(
        self, ctx: lubanParser.DivArithmeticExpressionContext
    ):
        right = self.parameters.pop()
        left = self.parameters.pop()
        self.parameters.append(Function("/", [left, right]))

    # Exit a parse tree produced by lubanParser#SubArithmeticExpression.
    def exitSubArithmeticExpression(
        self, ctx: lubanParser.SubArithmeticExpressionContext
    ):
        right = self.parameters.pop()
        left = self.parameters.pop()
        self.parameters.append(Function("-", [left, right]))

    # Exit a parse tree produced by lubanParser#ModArithmeticExpression.
    def exitModArithmeticExpression(
        self, ctx: lubanParser.ModArithmeticExpressionContext
    ):
        right = self.parameters.pop()
        left = self.parameters.pop()
        self.parameters.append(Function("%", [left, right]))

    # Exit a parse tree produced by lubanParser#RuntTimeFuncArithmeticExpression.
    def exitRuntTimeFuncArithmeticExpression(
        self, ctx: lubanParser.RuntTimeFuncArithmeticExpressionContext
    ):
        self.parameters.append(Function(ctx.IDENTIFIER().getText().lower(), []))

    # Exit a parse tree produced by lubanParser#MulArithmeticExpression.
    def exitMulArithmeticExpression(
        self, ctx: lubanParser.MulArithmeticExpressionContext
    ):
        right = self.parameters.pop()
        left = self.parameters.pop()
        self.parameters.append(Function("*", [left, right]))
