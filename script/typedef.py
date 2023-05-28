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

import os
import sys
import enum
import types
import inspect
from typing import Any, List, Union


class VariableType(enum.IntEnum):
    """Type of variable

    VT_Int: literal integer constant
    VT_Float: literal floating point constant
    VT_String: literal string constant, with quotation marks as the beginning and end of the string
    VT_IntList: literal integer constant array
    VT_FloatList: literal floating point constant array
    VT_StringList: literal string constant array
    VT_Variable: variable
    """

    VT_Not_Defined = 0
    VT_Int = 1
    VT_Float = 2
    VT_String = 3
    VT_IntList = 4
    VT_FloatList = 5
    VT_StringList = 6
    VT_Variable = 7

    def is_constant_scalar(self) -> bool:
        """if the variable is constant scalar

        Returns:
            bool: _description_
        """
        if self.value in (
            VariableType.VT_Int,
            VariableType.VT_Float,
            VariableType.VT_String,
        ):
            return True
        return False

    def is_constant_array(self) -> bool:
        """if the variable is constant array

        Returns:
            bool: _description_
        """
        if self.value in (
            VariableType.VT_IntList,
            VariableType.VT_FloatList,
            VariableType.VT_StringList,
        ):
            return True
        return False

    def is_constant(self) -> bool:
        """if the variable is constant

        Returns:
            bool: _description_
        """
        return self.is_constant_scalar() or self.is_constant_array()

    def is_variable(self) -> bool:
        return self.value == VariableType.VT_Variable


class FunctionType(enum.IntEnum):
    """Type of function.

    FT_Not_Defined: not defined
    FT_Simple_Func: simple function
    FT_Unary_Func: unary function
    FT_Binary_Func: binary function
    """

    FT_Not_Defined = 0
    FT_Simple_Func = 1
    FT_Unary_Func = 2
    FT_Binary_Func = 3


class Parameter:
    def __init__(self, index: int, vtype: VariableType, value: Any):
        """Parameter definition.

        Args:
            index(int): the number of parameters when the function is called. The index starts from 0
            vtype(VariableType): type of value
            value(Any): the corresponding value.
                if it is a constant, value is the value of the constant;
                if it is the original variable, it is the name of the corresponding variable;
                if it is a generating variable, it is the name of the corresponding operator
        """
        self.index, self.type, self.value = index, vtype, value


class FunctionInputType(enum.IntEnum):
    """function input type.
    format is listed:
    F(unction)I(nput)_Unary_S(ize)_{args_count}_L(ocation)_{variable_index}_Type
    F(unction)I(nput)_Binary_S(ize)_{args_count}_L(ocation)_{variable_index}_{variable_index}_Type

    """

    FI_Not_Defined = 0
    FI_Unary_S_1_L_1_Type = 1
    FI_Unary_S_2_L_1_Type = 2
    FI_Unary_S_2_L_2_Type = 3
    FI_Unary_S_3_L_1_Type = 4
    FI_Unary_S_3_L_2_Type = 5
    FI_Unary_S_3_L_3_Type = 6
    FI_Unary_S_4_L_1_Type = 7
    FI_Unary_S_4_L_2_Type = 8
    FI_Unary_S_4_L_3_Type = 9
    FI_Unary_S_4_L_4_Type = 10
    FI_Unary_S_5_L_1_Type = 11
    FI_Unary_S_5_L_2_Type = 12
    FI_Unary_S_5_L_3_Type = 13
    FI_Unary_S_5_L_4_Type = 14
    FI_Unary_S_5_L_5_Type = 15
    FI_Binary_S_2_L_1_2_Type = 16
    FI_Binary_S_3_L_1_2_Type = 17
    FI_Binary_S_3_L_1_3_Type = 18
    FI_Binary_S_3_L_2_3_Type = 19
    FI_Binary_S_4_L_1_2_Type = 20
    FI_Binary_S_4_L_1_3_Type = 21
    FI_Binary_S_4_L_1_4_Type = 22
    FI_Binary_S_4_L_2_3_Type = 23
    FI_Binary_S_4_L_2_4_Type = 24
    FI_Binary_S_4_L_3_4_Type = 25
    FI_Binary_S_5_L_1_2_Type = 26
    FI_Binary_S_5_L_1_3_Type = 27
    FI_Binary_S_5_L_1_4_Type = 28
    FI_Binary_S_5_L_1_5_Type = 29
    FI_Binary_S_5_L_2_3_Type = 30
    FI_Binary_S_5_L_2_4_Type = 31
    FI_Binary_S_5_L_2_5_Type = 32
    FI_Binary_S_5_L_3_4_Type = 33
    FI_Binary_S_5_L_3_5_Type = 34
    FI_Binary_S_5_L_4_5_Type = 35
