#!/usr/bin/python3
# -*- coding: UTF-8 -*-
"""
author: random
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
from enum import Enum
import os
import ast
import sys
import toml
import types
import inspect
import argparse
import function_def
from typing import Any, List, Union


class VariableType:
    """
    变量的类型.

    VT_Int: 字面整型常量
    VT_Float: 字面浮点型常量
    VT_String: 字面字符串常量, 以引号作为字符串的开始和结束
    VT_IntList: 字面整型常量数组
    VT_FloatList: 字面浮点型常量数组
    VT_StringList: 字面字符串常量数组
    VT_Original_Feature: 原始特征, tffeature类型的值, TensorFlow中定义
    VT_Selected_Feature: 命名的变量, tffeature类型的值
    VT_Anonymous_Feature: 匿名变量, tffeature类型的值
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

    def __init__(self, dtype: int) -> None:
        self.type = dtype

    def is_constant_scalar(self) -> bool:
        """是否是常量."""
        if self.type in (VariableType.VT_Int, VariableType.VT_Float,
                         VariableType.VT_String):
            return True
        return False

    def is_constant_array(self) -> bool:
        """是否是常量数组."""
        if self.type in (VariableType.VT_IntList, VariableType.VT_FloatList,
                         VariableType.VT_StringList):
            return True
        return False

    def is_constant(self) -> bool:
        return self.is_constant_scalar() or self.is_constant_array()

    def is_variable(self) -> bool:
        """是否是变量."""
        if self.type in (VariableType.VT_Anonymous_Feature,
                         VariableType.VT_Origin_Feature,
                         VariableType.VT_Selected_Feature):
            return True
        return False


class FunctionType(Enum):
    # 函数的类型
    FT_Not_Defined = 0
    FT_RealTime_Func = 1
    FT_Unary_Mapper_Func = 2
    FT_Unary_Aggregate_Func = 3
    FT_Hadamard_Mapper_Func = 4
    FT_Hadamard_Aggregate_Func = 5


# 内置函数二元操作符函数的定义
GLOBAL_FUNCTION_DEFINITIONS = {}


class Parameter:
    """
    参数定义.

    index: 函数调用时候的第几个参数, 下标从0开始计数
    type: 值的类型
    value: 对应的值, 如果是常量,value就是常量的值;
                    如果是原始变量, 就是对应变量的名字;
                    如果是生成型变量, 就是对应的算子的名字
    """

    def __init__(self, index: int, type: VariableType, value: Any):
        self.index, self.type, self.value = index, type, value


class Operator:
    """
    定义算子操作.

    name: 算子的名字
    function: 函数的名字
    parameters: 输入的参数
    """

    def __init__(self, name: str, function: str, parameters: List[Parameter]):
        # 内置函数的类型匹配
        global GLOBAL_FUNCTION_DEFINITIONS
        self.name, self.function, self.parameters, self.func_type = name, function, parameters, FunctionType.FT_Not_Defined

        assert (self.function in GLOBAL_FUNCTION_DEFINITIONS)
        assert (len(GLOBAL_FUNCTION_DEFINITIONS[self.function]['argvs']) == len(
            self.parameters))
        for i, p in enumerate(self.parameters):
            if p.type.is_constant():
                p.type = GLOBAL_FUNCTION_DEFINITIONS[self.function]['argvs'][i]

    def __get_varibale_count__(self) -> int:
        if not self.__check__():
            return 0
        if len(self.parameters) == 0:
            return 0
        if len(self.parameters) == 1:
            return 1
        var_count = 0
        for p in self.parameters:
            if p.type.is_variable():
                var_count += 1
        return var_count

    def __check__(self) -> bool:
        """check"""

        if len(self.parameters) == 0:
            return False
        if len(self.parameters) == 1:
            return self.parameters[0].type.is_variable()
        var_count = 0
        for p in self.parameters:
            if p.type.is_variable():
                if p.index >= 2:
                    # variable's index must <= 1
                    return False
                var_count += 1
        if var_count > 2 or var_count == 0:
            # variable count must <= 2
            return False
        return True

    def __swap_if_need__(self):
        if not self.__check__():
            return
        if len(self.parameters) >= 2 and self.parameters[0].type.is_constant() \
                and self.parameters[0].type.is_variable() \
            and self.function in ["_add", "_sub", "_mul", "_div", "_mod", "_pow", "concat"]:
            self.parameters[0], self.parameters[1] = self.parameters[1], self.parameters[0]
            self.function += "_0_1"

    def __builtin_function_rename__(self):
        if self.function in ["floor", "ceil", "log", "exp", "log"]:
            self.function = f"_{self.function}"

    def __update_function_type__(self):
        global GLOBAL_FUNCTION_DEFINITIONS
        if len(self.parameters) == 0:
            self.func_type = FunctionType.FT_RealTime_Func
            return
        func_info = GLOBAL_FUNCTION_DEFINITIONS[self.function]
        var_count = self.__get_varibale_count__()
        if func_info['return'].is_constant_array():
            if var_count == 2:
                self.func_type = FunctionType.FT_Hadamard_Aggregate_Func
            else:
                self.func_type = FunctionType.FT_Unary_Aggregate_Func
            return
        if func_info['return'].is_constant_scalar():
            if var_count == 2:
                self.func_type = FunctionType.FT_Hadamard_Mapper_Func
            else:
                self.func_type = FunctionType.FT_Unary_Mapper_Func
            return

    def update(self):
        """update other information."""
        self.__update_function_type__()
        self.__swap_if_need__()
        self.__builtin_function_rename__()

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
            if p.type.type == VariableType.VT_Anonymous_Feature:
                params.append(f'`{p.value.name}`')
            elif p.type.type in (VariableType.VT_Selected_Feature, VariableType.VT_Origin_Feature):
                params.append(f'`{p.value}`')
            elif p.type.type == VariableType.VT_String:
                params.append(f'"{p.value}"')
            else:
                params.append(str(p.value))
        return f'{self.name} = {self.function}({", ".join(params)})'


def parse_argv_name(index: int, argv: ast.Name) -> Parameter:
    """处理参数——名称"""
    return Parameter(index=index, type=VariableType(VariableType.VT_Selected_Feature),
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
        return Parameter(index=index, type=VariableType(VariableType.VT_String),
                         value=value)
    elif isinstance(value, float):
        return Parameter(index=index, type=VariableType(VariableType.VT_Float),
                         value=value)
    elif isinstance(value, int):
        return Parameter(index=index, type=VariableType(VariableType.VT_Int),
                         value=value)
    else:
        raise TypeError(f"not support type:{type(value)}")


def parse_argv_list(index: int, argv: ast.List) -> Parameter:
    """处理参数——常量列表"""
    assert (len(argv.elts) > 0)
    elt, dtype = argv.elts[0], VariableType(VariableType.VT_Not_Defined)
    fv = value_of_constant(elt)
    if isinstance(fv, str):
        dtype = VariableType(VariableType.VT_StringList)
    elif isinstance(fv, float):
        dtype = VariableType(VariableType.VT_FloatList)
    elif isinstance(fv, int):
        dtype = VariableType(VariableType.VT_IntList)
    else:
        raise TypeError(f"not support type:{type(fv)}")

    argvs = list(map(value_of_constant, argv.elts))
    for v in argvs:
        assert (type(fv) == type(v))

    return Parameter(index=index, type=dtype,
                     value=argvs)


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
    return Parameter(index=index, type=VariableType(VariableType.VT_Anonymous_Feature),
                     value=opr)


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
        function = f"_div"
    elif isinstance(argv.op, ast.Add):
        function = f"_add"
    elif isinstance(argv.op, ast.Sub):
        function = f"_sub"
    elif isinstance(argv.op, ast.Mult):
        function = f"_mul"
    elif isinstance(argv.op, ast.Mod):
        function = f"_mod"
    elif isinstance(argv.op, ast.Pow):
        function = f"_pow"
    else:
        raise TypeError(f"not support: {type(argv.op)}")

    params.append(parse_argv(0, argv.left))
    params.append(parse_argv(1, argv.right))

    opr = Operator(name=f"anonymous_{id(argv)}",
                   function=function, parameters=params)
    return Parameter(index=index, type=VariableType(VariableType.VT_Anonymous_Feature),
                     value=opr)


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


def parse_expression(expression: str) -> Operator:
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
    return my_opr


def parse_expressions(expressions: List[str]):
    """处理多个表达式, 并且讲相同的表达式进行合并."""
    # 遍历所有的opr
    def walk(my_opr: Operator, opr_list: list):
        for p in my_opr.parameters:
            if p.type.type == VariableType.VT_Anonymous_Feature:
                walk(p.value, opr_list)
        opr_list.append(my_opr)
    oprs = []

    for expression in expressions:
        opr = parse_expression(expression)
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
    print(opr_list)
    return opr_list


class Parser:
    @ staticmethod
    def load_function_definitions(name: str):
        """加载函数定义."""
        global GLOBAL_FUNCTION_DEFINITIONS
        # 获得类型

        def python_type_to_variable_type(t: type):
            if t == float:
                return VariableType(VariableType.VT_Float)
            elif t == int:
                return VariableType(VariableType.VT_Int)
            elif t == str:
                return VariableType(VariableType.VT_String)
            elif t == List[float]:
                return VariableType(VariableType.VT_FloatList)
            elif t == List[int]:
                return VariableType(VariableType.VT_IntList)
            elif t == List[str]:
                return VariableType(VariableType.VT_StringList)
            raise TypeError(f"not support type:{t}")

        function_def_module = sys.__dict__['modules'][name]
        for k, v in function_def_module.__dict__.items():
            if isinstance(v, types.FunctionType):
                assert (v.__name__ == k and "return" in v.__annotations__)

                tmp = {"return": python_type_to_variable_type(
                    v.__annotations__["return"]), "argvs": []}

                sig = inspect.signature(v)
                if len(sig.parameters) != len(v.__annotations__)-1:
                    raise RuntimeError(
                        f'params must be annotated: {v.__annotations__}')

                for name in sig.parameters:
                    tmp['argvs'].append(
                        python_type_to_variable_type(v.__annotations__[name]))
                GLOBAL_FUNCTION_DEFINITIONS[k] = tmp

    @ staticmethod
    def do(dsls: List[str]) -> dict:
        """处理表达式."""
        ops = parse_expressions(dsls)
        configs = {}

        for op in ops:
            print(op)
            config = {'name': op.name, 'params': []}
            for p in op.parameters:
                param = {'type': p.type.type, 'index': p.index}
                if p.type.type == VariableType.VT_Anonymous_Feature:
                    if p.value.name not in configs:
                        raise ValueError(f"{p.value.name} unkown")
                    param['data'] = p.value.name
                else:
                    param['data'] = p.value
                    if p.type.type == VariableType.VT_Selected_Feature:
                        if p.value not in configs:
                            p.type.type = VariableType.VT_Origin_Feature
                            param['type'] = p.type.type
                config['params'].append(param)
            op.update()
            config['func_type'] = op.func_type.value
            config['func'] = op.function
            if len(config['params']) == 0:
                del config['params']
            if op.name.startswith("anonymous_"):
                config['type'] = VariableType.VT_Anonymous_Feature
            else:
                config['type'] = VariableType.VT_Selected_Feature
            configs[op.name] = config
            print(op)
        ret = []
        for _, conf in configs.items():
            ret.append(conf)
        return ret


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--input", "-i", type=str,
                        required=True, help="输入配置文件")
    parser.add_argument("--output", "-o", type=str,
                        required=True, help="输出配置文件")
    parser.add_argument("--function", "-f", type=str,
                        required=False, help="定义的函数")
    args = parser.parse_args()
    print(args)

    # 加载function_def
    Parser.load_function_definitions("function_def")
    if args.function is not None:
        # 动态引入文件
        dir_path, base_name = os.path.dirname(
            args.function), os.path.basename(args.function)
        assert (base_name.endswith('.py'))
        name = base_name[:-3]
        sys.path.append(dir_path)
        exec(f"import {name}")
        Parser.load_function_definitions(name)

    # 读取数据
    data = open(args.input, "r").read()
    expressions = filter(lambda _: len(_) > 0, map(
        lambda _: _.strip(), data.split('\n')))

    # 处理
    configs = Parser.do(expressions)

    # 写入配置文件
    toml.dump({"operators": configs}, open(args.output, 'w'))
