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

from typing import List
from type_def import Parameter, FunctionCenter
from type_def import FunctionType, VariableType


class Operator:
    """
    定义算子操作.

    name: 算子的名字
    function: 函数的名字
    parameters: 输入的参数
    """

    def __init__(self, name: str, function: str, parameters: List[Parameter]):
        # 内置函数的类型匹配
        self.name, self.function, self.var_count = name, function, 0
        self.parameters, self.func_type = parameters, FunctionType.FT_Not_Defined

        assert (self.function in FunctionCenter.FunctionDefintionDict)
        assert (len(FunctionCenter.FunctionDefintionDict[self.function]['argvs']) == len(
            self.parameters))
        func_info = FunctionCenter.FunctionDefintionDict[self.function]

        # add builtin function prefix
        if self.function in ["floor", "ceil", "log", "exp", "log", "identity"]:
            self.function = f"_{self.function}"

        var_index = []
        for i, param in enumerate(self.parameters):
            if param.type.is_constant():
                param.type = FunctionCenter.FunctionDefintionDict[self.function]['argvs'][i]
            elif param.type.is_variable():
                var_index.append(i)
        self.var_count = len(var_index)
        if self.var_count > 2:
            raise ValueError(f"variable number > 2")
        elif self.var_count == 2:
            if var_index[1] >= 2:
                raise IndexError(f"variable index must in (0, 1)")
            if func_info['return'].is_constant_array():
                self.func_type = FunctionType.FT_Binary_Aggregate_Func
            else:
                self.func_type = FunctionType.FT_Binary_Mapper_Func
        elif self.var_count == 1:
            if var_index[0] >= 2:
                raise IndexError(f"variable index must in (0, 1)")
            elif var_index[0] == 1:
                # put var in the 0 index
                self.parameters[0], self.parameters[1] = self.parameters[1], self.parameters[0]
                self.parameters[0].index, self.parameters[1].index = 0, 1
                self.function += "_0_1"
            if func_info['return'].is_constant_array():
                self.func_type = FunctionType.FT_Unary_Aggregate_Func
            else:
                self.func_type = FunctionType.FT_Unary_Mapper_Func
            if self.function == "_identity":
                self.func_type = FunctionType.FT_Simple_Func
        else:
            self.func_type = FunctionType.FT_Simple_Func

    def __eq__(self, __o: object) -> bool:
        """
        判断是不是同一个算子.

        处理函数一样, 参数一样则被认为是同一个算子
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

    def __repr__(self) -> str:
        """转成字符串."""
        params = []
        if len(self.parameters) == 0:
            return f'{self.name} = {self.function}()'
        for p in self.parameters:
            if p.type == VariableType.VT_Anonymous_Feature:
                params.append(f'`{p.value.name}`')
            elif p.type in (VariableType.VT_Selected_Feature, VariableType.VT_Origin_Feature):
                params.append(f'`{p.value}`')
            elif p.type == VariableType.VT_String:
                params.append(f'"{p.value}"')
            else:
                params.append(str(p.value))
        return f'{self.name} = {self.function}({", ".join(params)})'
