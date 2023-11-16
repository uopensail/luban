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

import enum


class DataType(enum.IntEnum):
    kError = -1
    kInt64 = 0
    kFloat32 = 1
    kString = 2
    kInt64s = 3
    kFloat32s = 4
    kStrings = 5
    kVariable = 6


class Parameter:
    @property
    def dtype(self):
        return DataType.kError

    def to_list(self):
        return []

    def __eq__(self, __o: object) -> bool:
        return False


class Int64(Parameter):
    def __init__(self, value):
        assert isinstance(value, int)
        self.value = value

    @property
    def dtype(self):
        return DataType.kInt64

    def __eq__(self, __o: object) -> bool:
        if __o is None or not isinstance(__o, Int64):
            return False
        return self.value == __o.value


class Int64s(Parameter):
    def __init__(self, value):
        assert isinstance(value, list)
        for v in value:
            assert isinstance(v, int)
        self.value = value

    @property
    def dtype(self):
        return DataType.kInt64s

    def __eq__(self, __o: object) -> bool:
        if __o is None or not isinstance(__o, Int64s):
            return False
        return self.value == __o.value


class Float32(Parameter):
    def __init__(self, value):
        assert isinstance(value, float)
        self.value = value

    @property
    def dtype(self):
        return DataType.kFloat32

    def __eq__(self, __o: object) -> bool:
        if __o is None or not isinstance(__o, Float32):
            return False
        return self.value == __o.value


class Float32s(Parameter):
    def __init__(self, value):
        assert isinstance(value, list)
        for v in value:
            assert isinstance(v, float)
        self.value = value

    @property
    def dtype(self):
        return DataType.kFloat32s

    def __eq__(self, __o: object) -> bool:
        if __o is None or not isinstance(__o, Float32s):
            return False
        return self.value == __o.value


class String(Parameter):
    def __init__(self, value):
        assert isinstance(value, str)
        self.value = value[1:-1]

    @property
    def dtype(self):
        return DataType.kString

    def __eq__(self, __o: object) -> bool:
        if __o is None or not isinstance(__o, String):
            return False
        return self.value == __o.value


class Strings(Parameter):
    def __init__(self, value):
        assert isinstance(value, list)
        for v in value:
            assert isinstance(v, str)
        self.value = value

    @property
    def dtype(self):
        return DataType.kStrings

    def __eq__(self, __o: object) -> bool:
        if __o is None or not isinstance(__o, Strings):
            return False
        return self.value == __o.value


class Variable(Parameter):
    def __init__(self, value, data_type):
        assert isinstance(value, str)
        self.value = value
        self.data_type = data_type

    @property
    def dtype(self):
        return self.data_type

    def __eq__(self, __o: object) -> bool:
        if __o is None or not isinstance(__o, Variable):
            return False
        return self.value == __o.value and self.data_type == __o.data_type


function_io_types = {
    "+": {"in": [DataType.kFloat32, DataType.kFloat32], "out": DataType.kFloat32},
    "-": {"in": [DataType.kFloat32, DataType.kFloat32], "out": DataType.kFloat32},
    "*": {"in": [DataType.kFloat32, DataType.kFloat32], "out": DataType.kFloat32},
    "/": {"in": [DataType.kFloat32, DataType.kFloat32], "out": DataType.kFloat32},
    "%": {"in": [DataType.kInt64, DataType.kInt64], "out": DataType.kInt64},
    "mini": {"in": [DataType.kInt64, DataType.kInt64], "out": DataType.kInt64},
    "minf": {"in": [DataType.kFloat32, DataType.kFloat32], "out": DataType.kFloat32},
    "min": {"in": [DataType.kFloat32, DataType.kFloat32], "out": DataType.kFloat32},
    "maxi": {"in": [DataType.kInt64, DataType.kInt64], "out": DataType.kInt64},
    "maxf": {"in": [DataType.kFloat32, DataType.kFloat32], "out": DataType.kFloat32},
    "max": {"in": [DataType.kFloat32, DataType.kFloat32], "out": DataType.kFloat32},
    "average": {"in": [DataType.kFloat32s], "out": DataType.kFloat32},
    "averagei": {"in": [DataType.kInt64s], "out": DataType.kFloat32},
    "averagef": {"in": [DataType.kFloat32s], "out": DataType.kFloat32},
    "stddev": {"in": [DataType.kFloat32s], "out": DataType.kFloat32},
    "stddevi": {"in": [DataType.kInt64s], "out": DataType.kFloat32},
    "stddevf": {"in": [DataType.kFloat32s], "out": DataType.kFloat32},
    "variance": {"in": [DataType.kFloat32s], "out": DataType.kFloat32},
    "variancei": {"in": [DataType.kInt64s], "out": DataType.kFloat32},
    "variancef": {"in": [DataType.kFloat32s], "out": DataType.kFloat32},
    "min_max": {
        "in": [DataType.kFloat32, DataType.kFloat32, DataType.kFloat32],
        "out": DataType.kFloat32,
    },
    "min_maxi": {
        "in": [DataType.kInt64, DataType.kInt64, DataType.kInt64],
        "out": DataType.kFloat32,
    },
    "min_maxf": {"in": [DataType.kFloat32], "out": DataType.kFloat32},
    "norm": {"in": [DataType.kFloat32s], "out": DataType.kFloat32},
    "normi": {"in": [DataType.kInt64s], "out": DataType.kFloat32},
    "normf": {"in": [DataType.kFloat32], "out": DataType.kFloat32},
    "binarize": {"in": [DataType.kFloat32, DataType.kFloat32], "out": DataType.kInt64},
    "binarizei": {"in": [DataType.kInt64, DataType.kInt64], "out": DataType.kInt64},
    "binarizef": {"in": [DataType.kFloat32, DataType.kFloat32], "out": DataType.kInt64},
    "bucketize": {
        "in": [DataType.kFloat32, DataType.kFloat32s],
        "out": DataType.kInt64,
    },
    "bucketizei": {"in": [DataType.kInt64, DataType.kInt64s], "out": DataType.kInt64},
    "bucketizef": {
        "in": [DataType.kFloat32, DataType.kFloat32s],
        "out": DataType.kInt64,
    },
    "normalizei": {
        "in": [DataType.kInt64s, DataType.kFloat32],
        "out": DataType.kFloat32s,
    },
    "normalizef": {
        "in": [DataType.kFloat32s, DataType.kFloat32],
        "out": DataType.kFloat32s,
    },
    "normalize": {
        "in": [DataType.kFloat32s, DataType.kFloat32],
        "out": DataType.kFloat32s,
    },
    "topki": {"in": [DataType.kInt64s, DataType.kInt64], "out": DataType.kInt64s},
    "topkf": {"in": [DataType.kFloat32s, DataType.kInt64], "out": DataType.kFloat32s},
    "topks": {"in": [DataType.kStrings, DataType.kInt64], "out": DataType.kStrings},
    "topk": {"in": [DataType.kStrings, DataType.kInt64], "out": DataType.kStrings},
    "addi": {"in": [DataType.kInt64, DataType.kInt64], "out": DataType.kInt64},
    "add": {"in": [DataType.kFloat32, DataType.kFloat32], "out": DataType.kFloat32},
    "addf": {"in": [DataType.kFloat32, DataType.kFloat32], "out": DataType.kFloat32},
    "subi": {"in": [DataType.kInt64, DataType.kInt64], "out": DataType.kInt64},
    "subf": {"in": [DataType.kFloat32, DataType.kFloat32], "out": DataType.kFloat32},
    "sub": {"in": [DataType.kFloat32, DataType.kFloat32], "out": DataType.kFloat32},
    "muli": {"in": [DataType.kInt64, DataType.kInt64], "out": DataType.kInt64},
    "mulf": {"in": [DataType.kFloat32, DataType.kFloat32], "out": DataType.kFloat32},
    "mul": {"in": [DataType.kFloat32, DataType.kFloat32], "out": DataType.kFloat32},
    "divi": {"in": [DataType.kInt64, DataType.kInt64], "out": DataType.kInt64},
    "divf": {"in": [DataType.kFloat32, DataType.kFloat32], "out": DataType.kFloat32},
    "div": {"in": [DataType.kFloat32, DataType.kFloat32], "out": DataType.kFloat32},
    "mod": {"in": [DataType.kInt64, DataType.kInt64], "out": DataType.kInt64},
    "pow": {"in": [DataType.kFloat32, DataType.kFloat32], "out": DataType.kFloat32},
    "round": {"in": [DataType.kFloat32], "out": DataType.kInt64},
    "floor": {"in": [DataType.kFloat32], "out": DataType.kInt64},
    "ceil": {"in": [DataType.kFloat32], "out": DataType.kInt64},
    "log": {"in": [DataType.kFloat32], "out": DataType.kFloat32},
    "exp": {"in": [DataType.kFloat32], "out": DataType.kFloat32},
    "log10": {"in": [DataType.kFloat32], "out": DataType.kFloat32},
    "log2": {"in": [DataType.kFloat32], "out": DataType.kFloat32},
    "sqrt": {"in": [DataType.kFloat32], "out": DataType.kFloat32},
    "abs": {"in": [DataType.kFloat32], "out": DataType.kFloat32},
    "absf": {"in": [DataType.kFloat32], "out": DataType.kFloat32},
    "absi": {"in": [DataType.kInt64], "out": DataType.kInt64},
    "sin": {"in": [DataType.kFloat32], "out": DataType.kFloat32},
    "asin": {"in": [DataType.kFloat32], "out": DataType.kFloat32},
    "sinh": {"in": [DataType.kFloat32], "out": DataType.kFloat32},
    "asinh": {"in": [DataType.kFloat32], "out": DataType.kFloat32},
    "cos": {"in": [DataType.kFloat32], "out": DataType.kFloat32},
    "acos": {"in": [DataType.kFloat32], "out": DataType.kFloat32},
    "cosh": {"in": [DataType.kFloat32], "out": DataType.kFloat32},
    "acosh": {"in": [DataType.kFloat32], "out": DataType.kFloat32},
    "tanh": {"in": [DataType.kFloat32], "out": DataType.kFloat32},
    "atan": {"in": [DataType.kFloat32], "out": DataType.kFloat32},
    "tanh": {"in": [DataType.kFloat32], "out": DataType.kFloat32},
    "atanh": {"in": [DataType.kFloat32], "out": DataType.kFloat32},
    "sigmoid": {"in": [DataType.kFloat32], "out": DataType.kFloat32},
    "year": {"in": [], "out": DataType.kString},
    "month": {"in": [], "out": DataType.kString},
    "day": {"in": [], "out": DataType.kString},
    "date": {"in": [], "out": DataType.kString},
    "hour": {"in": [], "out": DataType.kString},
    "minute": {"in": [], "out": DataType.kString},
    "second": {"in": [], "out": DataType.kString},
    "now": {"in": [], "out": DataType.kInt64},
    "from_unixtime": {
        "in": [DataType.kInt64, DataType.kString],
        "out": DataType.kString,
    },
    "unix_timestamp": {
        "in": [DataType.kString, DataType.kString],
        "out": DataType.kInt64,
    },
    "date_add": {"in": [DataType.kString, DataType.kInt64], "out": DataType.kString},
    "date_sub": {"in": [DataType.kString, DataType.kInt64], "out": DataType.kString},
    "date_diff": {"in": [DataType.kString, DataType.kString], "out": DataType.kInt64},
    "reverse": {"in": [DataType.kString], "out": DataType.kString},
    "upper": {"in": [DataType.kString], "out": DataType.kString},
    "lower": {"in": [DataType.kString], "out": DataType.kString},
    "substr": {
        "in": [DataType.kString, DataType.kInt64, DataType.kInt64],
        "out": DataType.kString,
    },
    "concat": {"in": [DataType.kString, DataType.kString], "out": DataType.kString},
    "z_score": {
        "in": [DataType.kFloat32, DataType.kFloat32, DataType.kFloat32],
        "out": DataType.kString,
    },
    "box_cox": {"in": [DataType.kFloat32, DataType.kFloat32], "out": DataType.kFloat32},
    "cross": {"in": [DataType.kStrings, DataType.kStrings], "out": DataType.kStrings},
    "concat_ws": {"in": [DataType.kString, DataType.kStrings], "out": DataType.kStrings},

}


class Function(Parameter):
    def __init__(self, func, args):
        self.name = ""
        assert isinstance(func, str)
        self.func = func
        assert isinstance(args, list)
        for arg in args:
            assert isinstance(arg, Parameter)
        self.args = args
        if func in ["+", "-", "*", "/"]:
            dic = {"+": "add", "-": "sub", "*": "mul", "/": "div"}
            assert len(args) == 2
            assert args[0].dtype == args[1].dtype
            self.data_type = args[0].dtype
            if self.data_type == DataType.kFloat32:
                self.func = f"{dic[self.func]}f"
            elif self.data_type == DataType.kInt64:
                self.func = f"{dic[self.func]}i"
            else:
                raise TypeError(f"type: {self.data_type} not supported")
            return
        if func == "%":
            self.args = "mod"
        iotypes = function_io_types[func]
        self.data_type = iotypes["out"]

    @property
    def flags(self):
        flags = []
        for arg in self.args:
            if isinstance(arg, Function) or isinstance(arg, Variable):
                flags.append("1")
            else:
                flags.append("0")
        if len(flags) == 0:
            return 0
        return int("".join(flags[::-1]), 2)

    @property
    def argvs(self):
        args = []
        for arg in self.args:
            if isinstance(arg, Function) or isinstance(arg, Variable):
                continue
            args.append({"type": arg.dtype.value, "value": arg.value})
        return args

    @property
    def vars(self):
        vars = []
        for arg in self.args:
            if isinstance(arg, Function):
                vars.append(arg.name)
            elif isinstance(arg, Variable):
                vars.append(arg.value)
        return vars

    @property
    def dtype(self):
        return self.data_type

    def to_list(self):
        array = []
        for arg in self.args:
            array.extend(arg.to_list())
        array.append(self)
        return array

    def to_json(self):
        return {
            "func": self.func,
            "name": self.name,
            "flag": self.flags,
            "vars": self.vars,
            "args": self.argvs,
        }

    def __eq__(self, __o: object) -> bool:
        if __o is None or not isinstance(__o, Function):
            return False
        if self.func != __o.func or len(self.args) != len(__o.args):
            return False

        for p1, p2 in zip(self.args, __o.args):
            if p1 == p2:
                continue
            return False
        return True
