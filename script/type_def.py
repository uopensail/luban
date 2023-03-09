#!/usr/bin/python3
# -*- coding: UTF-8 -*-
#
# `LuBan` - 'c++ tool for transformating and hashing features'.
# Copyright (C) 2019 - present uopensail <timepi123@gmail.com>
#
# This file is part of `LuBan`.
#
# `LuBan` is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# `LuBan` is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with `LuBan`.  If not, see <http://www.gnu.org/licenses/>.
#
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
    VT_Original_Feature: original feature, value of tffeature type, defined in TensorFlow
    VT_Selected_Feature: named variable, value of tffeature type
    VT_Anonymous_Feature: anonymous variable, value of tffeature type
    """

    VT_Not_Defined = 0
    VT_Int = 1
    VT_Float = 2
    VT_String = 3
    VT_IntList = 4
    VT_FloatList = 5
    VT_StringList = 6
    VT_Origin_Feature = 7
    VT_Selected_Feature = 8
    VT_Anonymous_Feature = 9

    def is_constant_scalar(self) -> bool:
        """if the variable is constant scalar

        Returns:
            bool: _description_
        """
        if self.value in (VariableType.VT_Int, VariableType.VT_Float,
                          VariableType.VT_String):
            return True
        return False

    def is_constant_array(self) -> bool:
        """if the variable is constant array

        Returns:
            bool: _description_
        """
        if self.value in (VariableType.VT_IntList, VariableType.VT_FloatList,
                          VariableType.VT_StringList):
            return True
        return False

    def is_constant(self) -> bool:
        """if the variable is constant

        Returns:
            bool: _description_
        """
        return self.is_constant_scalar() or self.is_constant_array()

    def is_variable(self) -> bool:
        if self.value in (VariableType.VT_Anonymous_Feature,
                          VariableType.VT_Origin_Feature,
                          VariableType.VT_Selected_Feature):
            return True
        return False


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


def python_type_to_variable_type(t: type) -> VariableType:
    """python type to VariableType."""

    if t == float:
        return VariableType.VT_Float
    elif t == int:
        return VariableType.VT_Int
    elif t == str:
        return VariableType.VT_String
    elif t == List[float]:
        return VariableType.VT_FloatList
    elif t == List[int]:
        return VariableType.VT_IntList
    elif t == List[str]:
        return VariableType.VT_StringList
    raise TypeError(f"not support type:{t}")


class FunctionInputType(enum.IntEnum):
    """function input type."""
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
