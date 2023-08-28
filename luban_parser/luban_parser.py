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

import argparse
import ast
import enum
import json
from typing import Any, List, Union

GLOBAL_OPERATORS = []
GLOBAL_OPERATOR_INDEX = -1

STRING_TYPE_MARKER = "___STRING__"
INT64_TYPE_MARKER = "__INT64__"
FLOAT_TYPE_MARKER = "__FLOAT__"

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


def replace_type_mark(value: str) -> str:
    value = value.replace("<std::string>", STRING_TYPE_MARKER)
    value = value.replace("<float>", FLOAT_TYPE_MARKER)
    value = value.replace("<int64_t>", INT64_TYPE_MARKER)
    return value


def recover_type_mark(value: str) -> str:
    value = value.replace(STRING_TYPE_MARKER, "<std::string>")
    value = value.replace(FLOAT_TYPE_MARKER, "<float>")
    value = value.replace(INT64_TYPE_MARKER, "<int64_t>")
    return value


class DataType(enum.IntEnum):
    kError = -1
    kInt64 = 0
    kFloat32 = 1
    kString = 2
    kInt64Array = 3
    kFloat32Array = 4
    kStringArray = 5
    kVariable = 6

    def is_constant_scalar(self) -> bool:
        return self.value in (
            DataType.kInt64,
            DataType.kFloat32,
            DataType.kString,
        )

    def is_constant_array(self) -> bool:
        return self.value in (
            DataType.kInt64Array,
            DataType.kFloat32Array,
            DataType.kStringArray,
        )

    def is_constant(self) -> bool:
        return self.is_constant_scalar() or self.is_constant_array()

    def is_variable(self) -> bool:
        return self.value == DataType.kVariable


class Parameter:
    def __init__(self, index: int, vtype: DataType, value: Any):
        self.index = index
        self.type = vtype
        self.value = value


class Operator:
    def __init__(self, name: str, function: str, parameters: List[Parameter]):
        self.name = name
        self.function = recover_type_mark(function)
        self.parameters = parameters
        self.flag = 0
        self._math_func_type_check()
        self._update_flag()

    def __eq__(self, __o: object) -> bool:
        if __o is None or not isinstance(__o, Operator):
            return False
        if self.function != __o.function or len(self.parameters) != len(__o.parameters):
            return False
        __o.parameters.sort(key=lambda _: _.index)
        for p1, p2 in zip(self.parameters, __o.parameters):
            if p1.index != p2.index or p1.type != p2.type or p1.value != p2.value:
                return False
        return True

    def _update_flag(self):
        flag_str = "".join(
            map(lambda arg: "0" if arg.type.is_constant() else "1", self.parameters)
        )
        self.flag = int(flag_str[::-1], 2)

    def _math_func_type_check(self):
        tmp, idx, suffix = self.function, self.function.find("<"), ""
        if idx > 0:
            tmp = tmp[:idx]
            suffix = self.function[idx:]

        if tmp not in MATH_FUNCTIONS:
            return

        self.function = f"_{self.function}"

        # default float type
        for arg in self.parameters:
            if arg.type.is_constant():
                arg.value, arg.type = float(arg.value), DataType.kFloat32

        if self.function == "mod":
            for arg in self.parameters:
                if arg.type.is_constant():
                    arg.value, arg.type = int(arg.value), DataType.kInt64
            return

        if suffix == "<int64_t>":
            for arg in self.parameters:
                if arg.type.is_constant():
                    arg.value, arg.type = int(arg.value), DataType.kInt64

    def __repr__(self) -> str:
        params = []
        if len(self.parameters) == 0:
            return f"{self.name} = {self.function}()"
        for p in self.parameters:
            if p.type == DataType.kVariable:
                params.append(f"`{p.value}`")
            elif p.type == DataType.kString:
                params.append(f'"{p.value}"')
            else:
                params.append(str(p.value))
        return f'{self.name} = {self.function}({", ".join(params)})'


def parse_argv_name(index: int, argv: ast.Name) -> Parameter:
    return Parameter(index=index, vtype=DataType.kVariable, value=argv.id)


def value_of_constant(v: Union[ast.Constant, ast.Str, ast.Num]) -> Any:
    if isinstance(v, ast.Str):
        return v.s
    if isinstance(v, ast.Num):
        return v.n
    return None


def parse_argv_constant(
    index: int, argv: Union[ast.Constant, ast.Str, ast.Num]
) -> Parameter:
    value = value_of_constant(argv)
    if isinstance(value, str):
        return Parameter(index=index, vtype=DataType.kString, value=value)
    if isinstance(value, float):
        return Parameter(index=index, vtype=DataType.kFloat32, value=value)
    if isinstance(value, int):
        return Parameter(index=index, vtype=DataType.kInt64, value=value)
    raise TypeError(f"not support type:{type(value)}")


def parse_argv_list(index: int, argv: ast.List) -> Parameter:
    assert len(argv.elts) > 0
    elt, dtype = argv.elts[0], DataType.kError
    fv = value_of_constant(elt)
    if isinstance(fv, str):
        dtype = DataType.kStringArray
    elif isinstance(fv, float):
        dtype = DataType.kFloat32Array
    elif isinstance(fv, int):
        dtype = DataType.kInt64Array
    else:
        raise TypeError(f"not support type:{type(fv)}")

    argvs = list(map(value_of_constant, argv.elts))
    for v in argvs:
        assert type(fv) == type(v)

    return Parameter(index=index, vtype=dtype, value=argvs)


def parse_argv_call(index: int, argv: ast.Call) -> Parameter:
    global GLOBAL_OPERATOR_INDEX, GLOBAL_OPERATORS
    assert isinstance(argv.func, ast.Name)
    function, params = argv.func.id, []

    for i, argv in enumerate(argv.args):
        if isinstance(argv, (ast.Constant, ast.Num, ast.Str)):
            params.append(parse_argv_constant(index=i, argv=argv))
        elif isinstance(argv, ast.Name):
            # in this case, use the existing features
            params.append(parse_argv_name(index=i, argv=argv))
        elif isinstance(argv, ast.List):
            params.append(parse_argv_list(index=i, argv=argv))
        elif isinstance(argv, ast.Call):
            params.append(parse_argv_call(index=i, argv=argv))
        elif isinstance(argv, ast.BinOp):
            params.append(parse_argv_binop(index=i, argv=argv))
        else:
            raise TypeError(f"not support type:{type(argv)}")
    GLOBAL_OPERATOR_INDEX += 1

    opr = Operator(
        name=f"anonymous_{GLOBAL_OPERATOR_INDEX}", function=function, parameters=params
    )
    GLOBAL_OPERATORS.append(opr)
    return Parameter(index=index, vtype=DataType.kVariable, value=opr)


def parse_argv_binop(index: int, argv: ast.BinOp) -> Parameter:
    global GLOBAL_OPERATOR_INDEX, GLOBAL_OPERATORS
    function, params = None, []

    def is_literal_constant(v: Any):
        return isinstance(v, (ast.Constant, ast.Num, ast.Str, ast.List))

    if is_literal_constant(argv.left) and is_literal_constant(argv.right):
        raise RuntimeError("binop must have at least one variable")

    if isinstance(argv.op, ast.Div):
        function = "div"
    elif isinstance(argv.op, ast.Add):
        function = "add"
    elif isinstance(argv.op, ast.Sub):
        function = "sub"
    elif isinstance(argv.op, ast.Mult):
        function = "mul"
    elif isinstance(argv.op, ast.Mod):
        function = "mod"
    elif isinstance(argv.op, ast.Pow):
        function = "pow"
    else:
        raise TypeError(f"not support: {type(argv.op)}")

    params.append(parse_argv(0, argv.left))
    params.append(parse_argv(1, argv.right))
    GLOBAL_OPERATOR_INDEX += 1
    opr = Operator(
        name=f"anonymous_{GLOBAL_OPERATOR_INDEX}", function=function, parameters=params
    )
    GLOBAL_OPERATORS.append(opr)
    return Parameter(index=index, vtype=DataType.kVariable, value=opr)


def parse_argv(index: int, argv: Any) -> Parameter:
    if isinstance(argv, (ast.Constant, ast.Num, ast.Str)):
        return parse_argv_constant(index=index, argv=argv)
    if isinstance(argv, ast.Name):
        return parse_argv_name(index=index, argv=argv)
    if isinstance(argv, ast.Call):
        return parse_argv_call(index=index, argv=argv)
    if isinstance(argv, ast.List):
        return parse_argv_list(index=index, argv=argv)
    if isinstance(argv, ast.BinOp):
        return parse_argv_binop(index=index, argv=argv)
    TypeError(f"not support type:{type(argv)}")


def parse_expression(expression: str) -> Operator:
    expression = replace_type_mark(expression)
    module = ast.parse(expression)
    # do some checking
    assert isinstance(module, ast.Module)
    assert isinstance(module.body, list) and len(module.body) == 1
    assign_expr = module.body[0]
    assert isinstance(assign_expr, ast.Assign)
    assign_children = list(ast.iter_child_nodes(assign_expr))
    assert isinstance(assign_children, list) and len(assign_children) == 2

    # Get the name of the variable on the left
    assert isinstance(assign_children[0], ast.Name)
    feature_name = assign_children[0].id

    # process the right function expression
    my_opr = None
    if isinstance(assign_children[1], ast.BinOp):
        tmp = parse_argv_binop(-1, assign_children[1])
        my_opr = tmp.value
    elif isinstance(assign_children[1], ast.Call):
        tmp = parse_argv_call(-1, assign_children[1])
        my_opr = tmp.value
    else:
        raise TypeError(f"not support type:{type(assign_children[1])}")
    assert isinstance(tmp.value, Operator)
    my_opr.name = feature_name
    return my_opr


def parse_expressions(expressions: List[str]) -> List[Operator]:
    global GLOBAL_OPERATOR_INDEX, GLOBAL_OPERATORS
    for expression in expressions:
        parse_expression(expression)
    unique_oprs = []
    for opr in GLOBAL_OPERATORS:
        exists = False
        for unique_opr in unique_oprs:
            if opr == unique_opr:
                opr.name = unique_opr.name
                exists = True
                break
        if not exists:
            unique_oprs.append(opr)
    return unique_oprs


def parse_features(config: dict) -> dict:
    features = {}
    for feature in config["features"]:
        dtype = DataType(feature["type"])

        hash = feature.get("hash", False)
        padding = None
        if dtype in (DataType.kInt64, DataType.kInt64Array):
            padding = int(feature.get("padding", 0))
        elif dtype in (DataType.kFloat32, DataType.kFloat32Array):
            padding = float(feature.get("padding", 0))
        elif dtype == DataType.kString:
            hash = True
            dtype = DataType.kInt64
            padding = int(feature.get("padding", 0))
        elif dtype == DataType.kStringArray:
            hash = True
            dtype = DataType.kInt64Array
            padding = int(feature.get("padding", 0))

        features[feature["name"]] = {
            "name": feature["name"],
            "expr": feature.get("expr", ""),
            "type": dtype,
            "padding": padding,
            "group": -1,
            "offset": 0,
            "hash": hash,
            "dim": feature.get("dim", 1),
        }

    return features


def parse_group(config: dict, features: dict) -> tuple:
    groups, feas = [], []

    for i, group in enumerate(config["groups"]):
        offset = 0
        isfloat = False
        dtype = DataType.kInt64
        for fea in group:
            tmp = {
                "name": fea,
                "type": features[fea]["type"],
                "padding": features[fea]["padding"],
                "group": i,
                "offset": offset,
                "hash": features[fea]["hash"],
                "dim": features[fea]["dim"],
            }

            offset += tmp["dim"]
            if tmp["type"] in (DataType.kFloat32, DataType.kFloat32Array):
                isfloat = True
            feas.append(tmp)
        if isfloat:
            dtype = DataType.kFloat32
        groups.append({"id": i, "width": offset, "type": dtype})
    return groups, feas


def parse(input_file: str, output_file: str):
    """do parse

    Args:
        input_file (str): json config
        output_file (str): json config

    Raises:
        ValueError: variable unknown
    """
    configure = json.load(open(input_file, "r"))

    features = parse_features(configure)

    expressions = []
    for k, v in features.items():
        if len(v["expr"]) > 0:
            expressions.append("%s = %s" % (k, v["expr"]))

    oprs = parse_expressions(expressions)
    variables = {}
    ret = []
    for op in oprs:
        config = {
            "func": op.function,
            "name": op.name,
            "flag": op.flag,
            "args": [],
            "vars": [],
        }
        for p in op.parameters:
            if p.type == DataType.kVariable:
                if isinstance(p.value, Operator):
                    if p.value.name not in variables:
                        raise ValueError(f"{p.value.name} unknown")
                    config["vars"].append(p.value.name)
                else:
                    config["vars"].append(p.value)
            else:
                config["args"].append({"type": p.type.value, "value": p.value})
        ret.append(config)
        variables[op.name] = True
    groups, features = parse_group(configure, features)
    ans = {"transforms": ret, "groups": groups, "features": features}
    json.dump(ans, open(output_file, "w"))


def main():
    """main function"""
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--input", "-i", type=str, required=True, help="json config file"
    )
    parser.add_argument(
        "--output", "-o", type=str, required=True, help="json config file"
    )
    args = parser.parse_args()
    parse(args.input, args.output)


if __name__ == "__main__":
    main()
