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

from typing import List
from typedef import FunctionInputType, FunctionType, Parameter, VariableType


MATH_FUNCTIONS = [
    "add",
    "sub",
    "mul",
    "div",
    "mod",
    "pow",
    "round",
    "floor",
    "ceil",
    "log",
    "exp",
    "log10",
    "log2",
    "sqrt",
    "abs",
    "sin",
    "sinh",
    "asin",
    "asinh",
    "cos",
    "cosh",
    "acos",
    "acosh",
    "tan",
    "tanh",
    "atan",
    "atanh",
    "sigmoid",
]


class Operator:
    """
    define an operator

    name: operator name: defult is ""
    function: function name
    parameters: arguments
    """

    def __init__(self, name: str, function: str, parameters: List[Parameter]):
        """operator init function

        Args:
            name (str): name of operator
            function (str): function of operator
            parameters (List[Parameter]): arguments of operator
        """
        self.name, self.function = name, function
        self.parameters, self.func_type = parameters, FunctionType.FT_Not_Defined
        self.input_type = FunctionInputType.FI_Not_Defined
        self._check_args_len()
        self._add_prefix()
        self._update_type()

    def __eq__(self, __o: object) -> bool:
        """Judge whether it is the same operator.

        Args:
            __o (object): operator to compare

        Returns:
            bool: is same operator
        """
        if __o is None:
            return False
        if not isinstance(__o, Operator):
            return False
        if self.function != __o.function:
            return False
        if len(self.parameters) != len(__o.parameters):
            return False
        __o.parameters.sort(key=lambda _: _.index)
        for p1, p2 in zip(self.parameters, __o.parameters):
            if p1.index != p2.index or p1.type != p2.type or p1.value != p2.value:
                return False
        return True

    def _add_prefix(self):
        """math function has prefix `_`"""
        global MATH_FUNCTIONS
        if self.function in MATH_FUNCTIONS:
            if self.function == 'mod':
                for arg in self.parameters:
                    if arg.type.is_constant():
                        arg.value = int(arg.value)
                        arg.type = VariableType.VT_Int
            else:
                for arg in self.parameters:
                    if arg.type.is_constant():
                        arg.value = float(arg.value)
                        arg.type = VariableType.VT_Float

            self.function = f"_{self.function}"

    def _check_args_len(self):
        """check arguments length

        Raises:
            ValueError: arguments lenght must <= 5
        """
        if len(self.parameters) > 5:
            raise ValueError(
                f"not support: len(argvs) = {len(self.parameters) } > 5")

    def _update_type(self):
        """update types of operator

        Raises:
            ValueError: variable number <= 2
            ValueError: const expression not support
        """
        vars = []
        for i, arg in enumerate(self.parameters):
            if arg.type.is_variable():
                vars.append(str(i + 1))

        if len(vars) > 2:
            raise ValueError(
                f"not support: variable number = {len(vars)}  > 2")
        args_len = len(self.parameters)
        if len(vars) == 2:
            self.input_type = eval(
                f'FunctionInputType.FI_Binary_S_{args_len}_L_{"_".join(vars)}_Type'
            )
            self.func_type = FunctionType.FT_Unary_Func
            return
        if len(vars) == 1:
            self.input_type = eval(
                f"FunctionInputType.FI_Unary_S_{args_len}_L_{vars[0]}_Type"
            )
            self.func_type = FunctionType.FT_Unary_Func
            return
        if len(vars) == 0:
            if args_len > 0:
                raise ValueError("not support constant expression")
            self.input_type = FunctionInputType.FI_Not_Defined
            self.func_type = FunctionType.FT_Simple_Func
            return

    def __repr__(self) -> str:
        """to string.

        Returns:
            str: return string representation
        """
        params = []
        if len(self.parameters) == 0:
            return f"{self.name} = {self.function}()"
        for p in self.parameters:
            if p.type == VariableType.VT_Variable:
                params.append(f"`{p.value}`")
            elif p.type == VariableType.VT_String:
                params.append(f'"{p.value}"')
            else:
                params.append(str(p.value))
        return f'{self.name} = {self.function}({", ".join(params)})'
