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
"""
author: uopensail
descripton: 利用Python将配置文件进行解析,给C++代码使用.

定义配置的文件的示例:

`feature_1 = feature_0 + 100`

具体的说明如下:
1. 语句的解析是按照Python的语法来解析的
2. 必须是赋值语句, 左边作为特征的名字, 右边是计算表达式
3. 表达式支持嵌套调用, 函数名称
4. 支出Python内置的一些操作符
    +,-,*,/,%,**

"""
import argparse
import ast
import json
import os
from typing import Any, List, Union

import toml
from operator_def import Operator
from type_def import Parameter, VariableType


def parse_argv_name(index: int, argv: ast.Name) -> Parameter:
    """处理参数——名称"""
    return Parameter(index=index, vtype=VariableType.VT_Selected_Feature,
                     value=argv.id)


def value_of_constant(v: Union[ast.Constant, ast.Str, ast.Num]):
    """获得constant的值.

    这里主要是为了不同版本兼容.
    """
    if isinstance(v, ast.Str):
        return v.s
    elif isinstance(v, ast.Num):
        return v.n
    return None


def parse_argv_constant(index: int, argv: Union[ast.Constant, ast.Str, ast.Num]) -> Parameter:
    """处理参数——常量"""
    value = value_of_constant(argv)
    if isinstance(value, str):
        return Parameter(index=index, vtype=VariableType.VT_String,
                         value=value)
    elif isinstance(value, float):
        return Parameter(index=index, vtype=VariableType.VT_Float,
                         value=value)
    elif isinstance(value, int):
        return Parameter(index=index, vtype=VariableType.VT_Int,
                         value=value)
    else:
        raise TypeError(f"not support type:{type(value)}")


def parse_argv_list(index: int, argv: ast.List) -> Parameter:
    """处理参数——常量列表"""
    assert (len(argv.elts) > 0)
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
        assert (type(fv) == type(v))

    return Parameter(index=index, vtype=dtype, value=argvs)


def parse_argv_call(index: int, argv: ast.Call) -> Parameter:
    """处理参数——函数调用"""
    assert (isinstance(argv.func, ast.Name))
    function, params = argv.func.id, []

    for i, argv in enumerate(argv.args):
        if isinstance(argv, (ast.Constant, ast.Num, ast.Str)):
            params.append(parse_argv_constant(index=i, argv=argv))
        elif isinstance(argv, ast.Name):
            # 这种情况是用了已有的特征
            params.append(parse_argv_name(index=i, argv=argv))
        elif isinstance(argv, ast.List):
            params.append(parse_argv_list(index=i, argv=argv))
        elif isinstance(argv, ast.Call):
            params.append(parse_argv_call(index=i, argv=argv))
        elif isinstance(argv, ast.BinOp):
            params.append(parse_argv_binop(index=i, argv=argv))
        else:
            raise TypeError(f"not support type:{type(argv)}")

    opr = Operator(name=f"anonymous_{id(params)}",
                   function=function, parameters=params)
    return Parameter(index=index, vtype=VariableType.VT_Anonymous_Feature, value=opr)


def parse_argv_binop(index: int, argv: ast.BinOp) -> Parameter:
    """处理参数——二元操作符

    其中必须至少有一个参数是输入的遍历, 不能都是常量.
    """
    function, params = None, []

    # 是不是字面值常量
    def is_literal_constant(v: Any):
        return isinstance(v, (ast.Constant, ast.Num, ast.Str, ast.List))
    if is_literal_constant(argv.left) and is_literal_constant(argv.right):
        raise RuntimeError("binop must have at least one variable")

    if isinstance(argv.op, ast.Div):
        function = "_div"
    elif isinstance(argv.op, ast.Add):
        function = "_add"
    elif isinstance(argv.op, ast.Sub):
        function = "_sub"
    elif isinstance(argv.op, ast.Mult):
        function = "_mul"
    elif isinstance(argv.op, ast.Mod):
        function = "_mod"
    elif isinstance(argv.op, ast.Pow):
        function = "_pow"
    else:
        raise TypeError(f"not support: {type(argv.op)}")

    params.append(parse_argv(0, argv.left))
    params.append(parse_argv(1, argv.right))

    opr = Operator(name=f"anonymous_{id(argv)}",
                   function=function, parameters=params)
    return Parameter(index=index, vtype=VariableType.VT_Anonymous_Feature, value=opr)


def parse_argv(index: int, argv: Any) -> Parameter:
    """处理参数的统一函数."""
    if isinstance(argv, (ast.Constant, ast.Num, ast.Str)):
        return parse_argv_constant(index=index, argv=argv)
    elif isinstance(argv, ast.Name):
        return parse_argv_name(index=index, argv=argv)
    elif isinstance(argv, ast.Call):
        return parse_argv_call(index=index, argv=argv)
    elif isinstance(argv, ast.List):
        return parse_argv_list(index=index, argv=argv)
    elif isinstance(argv, ast.BinOp):
        return parse_argv_binop(index=index, argv=argv)
    raise TypeError(f"not support type:{type(argv)}")


def parse_expression(expression: str, gid: int) -> Operator:
    """处理表达式."""

    module = ast.parse(expression)
    # 做一些必要的检查
    assert (isinstance(module, ast.Module))
    assert (isinstance(module.body, list) and len(module.body) == 1)
    assign_expr = module.body[0]
    assert (isinstance(assign_expr, ast.Assign))
    assign_children = list(ast.iter_child_nodes(assign_expr))
    assert (isinstance(assign_children, list) and len(assign_children) == 2)

    # 获得左边变量的名字
    assert (isinstance(assign_children[0], ast.Name))
    feature_name = assign_children[0].id

    # 处理右边函数表达式
    my_opr = None
    if isinstance(assign_children[1], ast.BinOp):
        tmp = parse_argv_binop(-1, assign_children[1])
        my_opr = tmp.value
    elif isinstance(assign_children[1], ast.Call):
        tmp = parse_argv_call(-1, assign_children[1])
        my_opr = tmp.value
    else:
        raise TypeError(f"not support type:{type(assign_children[1])}")
    assert (isinstance(tmp.value, Operator))
    my_opr.name = feature_name
    return my_opr, gid


def parse_expressions(expressions: List[Any]):
    """处理多个表达式, 并且讲相同的表达式进行合并."""
    # 遍历所有的opr
    def walk(my_opr: Operator, opr_list: list):
        for p in my_opr.parameters:
            if p.type == VariableType.VT_Anonymous_Feature:
                walk(p.value, opr_list)
        opr_list.append(my_opr)
    oprs = []

    groups = []
    for (expression, gid) in expressions:
        opr, gid = parse_expression(expression, gid)
        groups.append({"gid": gid, "opr": opr})
        walk(opr, oprs)
    anonymous_index, opr_list, status = 0,  [], True
    for opr in oprs:
        status = True
        for my_opr in opr_list:
            if opr == my_opr:
                opr.name = my_opr.name
                status = False
                break
        if not status:
            continue

        if opr.name.startswith("anonymous_"):
            opr.name = f"anonymous_{anonymous_index}"
            anonymous_index += 1
        opr_list.append(opr)
    return opr_list, groups


class Parser:
    @ staticmethod
    def do(input_file: str, output_file: str) -> dict:
        """处理表达式."""
        # 读取数据
        dic = json.load(open(input_file, "r"))
        expressions = []
        for item in dic["features"]:
            expressions.append((item['expr'], item['gid']))
        ops, groups = parse_expressions(expressions)
        configs = {}

        for op in ops:
            print(op)
            config = {'name': op.name, 'params': [],
                      'input_type': op.input_type.value}
            for p in op.parameters:
                param = {'type': p.type.value, 'index': p.index}
                if p.type == VariableType.VT_Anonymous_Feature:
                    if p.value.name not in configs:
                        raise ValueError(f"{p.value.name} unkown")
                    param['data'] = p.value.name
                else:
                    param['data'] = p.value
                    if p.type == VariableType.VT_Selected_Feature:
                        if p.value not in configs:
                            p.type = VariableType.VT_Origin_Feature
                            param['type'] = p.type.value
                config['params'].append(param)

            config['func_type'] = op.func_type.value
            config['func'] = op.function
            if len(config['params']) == 0:
                del config['params']
            if op.name.startswith("anonymous_"):
                config['type'] = VariableType.VT_Anonymous_Feature.value
            else:
                config['type'] = VariableType.VT_Selected_Feature.value
            configs[op.name] = config
            print(op)
        ret = []
        for _, conf in configs.items():
            ret.append(conf)
        ans = {"operators": ret, "groups": []}
        for item in groups:
            ans['groups'].append(
                {"name": item["opr"].name, "gid": item['gid']})
        # 写入配置文件
        toml.dump(ans, open(output_file, 'w'))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--input", "-i", type=str,
                        required=True, help="输入配置文件")
    parser.add_argument("--output", "-o", type=str,
                        required=True, help="输出配置文件")

    args = parser.parse_args()
    print(args)
    Parser.do(args.input, args.output)


if __name__ == "__main__":
    main()
