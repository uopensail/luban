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
"""
author: timepi
descripton: Use Python to parse the configuration file 
            for C++ code to use.

"""
import argparse
import ast
import json
from typing import Any, List, Union

import toml
from .operatordef import Operator
from .typedef import Parameter, VariableType

GLOBAL_OPERATORS = []
GLOBAL_OPERATOR_INDEX = -1


def parse_argv_name(index: int, argv: ast.Name) -> Parameter:
    """parse the argument name

    Args:
        index (int): argument index
        argv (ast.Name): argument name

    Returns:
        Parameter: wrapper for the argument
    """
    return Parameter(index=index, vtype=VariableType.VT_Variable, value=argv.id)


def value_of_constant(v: Union[ast.Constant, ast.Str, ast.Num]) -> Any:
    """get the value of a constant.

    Args:
        v (Union[ast.Constant, ast.Str, ast.Num]): value of constant, support different versions

    Returns:
        Any: return the value of the constant
    """
    if isinstance(v, ast.Str):
        return v.s
    if isinstance(v, ast.Num):
        return v.n
    return None


def parse_argv_constant(
    index: int, argv: Union[ast.Constant, ast.Str, ast.Num]
) -> Parameter:
    """parse constant argument

    Args:
        index (int): _description_
        argv (Union[ast.Constant, ast.Str, ast.Num]): constant argument

    Raises:
        TypeError: not supported

    Returns:
        Parameter: wrapper for the argument
    """
    value = value_of_constant(argv)
    if isinstance(value, str):
        return Parameter(index=index, vtype=VariableType.VT_String, value=value)
    if isinstance(value, float):
        return Parameter(index=index, vtype=VariableType.VT_Float, value=value)
    if isinstance(value, int):
        return Parameter(index=index, vtype=VariableType.VT_Int, value=value)
    raise TypeError(f"not support type:{type(value)}")


def parse_argv_list(index: int, argv: ast.List) -> Parameter:
    """processing parameters - constant list.

    Args:
        index (int): argument index
        argv (ast.List): argument

    Raises:
        TypeError: type not support

    Returns:
        Parameter: wrapper for the argument
    """
    assert len(argv.elts) > 0
    elt, dtype = argv.elts[0], VariableType.VT_Not_Defined
    fv = value_of_constant(elt)
    if isinstance(fv, str):
        dtype = VariableType.VT_StringList
    elif isinstance(fv, float):
        dtype = VariableType.VT_FloatList
    elif isinstance(fv, int):
        dtype = VariableType.VT_IntList
    else:
        raise TypeError(f"not support type:{type(fv)}")

    argvs = list(map(value_of_constant, argv.elts))
    for v in argvs:
        assert type(fv) == type(v)

    return Parameter(index=index, vtype=dtype, value=argvs)


def parse_argv_call(index: int, argv: ast.Call) -> Parameter:
    """processing parameters - function call.

    Args:
        index (int): argument index
        argv (ast.Call): argument

    Raises:
        TypeError: type not support

    Returns:
        Parameter: wrapper for the argument
    """
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
    return Parameter(index=index, vtype=VariableType.VT_Variable, value=opr)


def parse_argv_binop(index: int, argv: ast.BinOp) -> Parameter:
    """processing parameter -- binary operator. At least one
    parameter must be input traversal, not all constants.

    Args:
        index (int): argument index
        argv (ast.BinOp): argument

    Raises:
        RuntimeError: const express not support
        TypeError: op not support

    Returns:
        Parameter: wrapper for the argument
    """
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
    return Parameter(index=index, vtype=VariableType.VT_Variable, value=opr)


def parse_argv(index: int, argv: Any) -> Parameter:
    """unified function for processing parameters.

    Args:
        index (int): argument index
        argv (Any): argument

    Raises:
        TypeError: type not suuport

    Returns:
        Parameter: wrapper for the argument
    """
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
    """Process expressions.

    Args:
        expression (str): expression to process
        gid (int): group id

    Raises:
        TypeError: _description_

    Returns:
        Operator: top operator
    """
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
    """Process multiple expressions and merge the same expressions

    Args:
        expressions (List[str]): expressions to process

    Returns:
        List[Operator]: operators to returns
    """
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


def check_outputs(slots: List[dict]):
    """check wheather outputs's slots

    Args:
        slots (List[dict]): slots
    """
    slots.sort(key=lambda x: x["slot"])
    for i in range(len(slots)):
        assert slots[i]["slot"] == i


def parse(input_file: str, output_file: str):
    """do parse

    Args:
        input_file (str): json config
        output_file (str): toml config

    Raises:
        ValueError: variable unknown
    """
    dic = json.load(open(input_file, "r"))
    check_outputs(dic["outputs"])
    oprs = parse_expressions(dic["transforms"])
    variables = {}
    ret = []
    for op in oprs:
        config = {
            "name": op.name,
            "args": [],
            "input_type": op.input_type.value,
            "func": op.function,
            "func_type": op.func_type.value,
        }
        for p in op.parameters:
            param = {
                "type": p.type.value,
                "index": p.index,
                "data": p.value,
            }
            if p.type == VariableType.VT_Variable:
                if isinstance(p.value, Operator):
                    if p.value.name not in variables:
                        raise ValueError(f"{p.value.name} unknown")
                    param["data"] = p.value.name
            config["args"].append(param)

        if len(config["args"]) == 0:
            del config["args"]
        ret.append(config)
        variables[op.name] = True
    ans = {"transforms": ret, "outputs": dic["outputs"]}
    toml.dump(ans, open(output_file, "w"))


def main():
    """main function"""
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--input", "-i", type=str, required=True, help="json config file"
    )
    parser.add_argument(
        "--output", "-o", type=str, required=True, help="toml config file"
    )
    args = parser.parse_args()
    parse(args.input, args.output)


if __name__ == "__main__":
    main()
