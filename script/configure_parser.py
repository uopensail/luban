#!/usr/bin/python
# -*- coding: UTF-8 -*-
"""
author: random
descripton: 利用Python将配置文件进行解析,给C++代码使用
"""
import toml
import traceback
from enum import IntEnum
from typing import List, Union


class VariableType(IntEnum):
    """
    变量的类型.

    VT_Constant_Int: 字面整型常量
    VT_Constant_Float: 字面浮点型常量
    VT_Constant_String: 字面字符串常量, 以引号作为字符串的开始和结束
    VT_Constant_IntList: 字面整型常量数组
    VT_Constant_FloatList: 字面浮点型常量数组
    VT_Constant_StringList: 字面字符串常量数组
    VT_RunTime_Int: 运行时的整型值
    VT_RunTime_Float: 运行时的浮点型值
    VT_RunTime_String: 运行时的字符串值
    VT_Original_Feature: 原始特征, tffeature类型的值, TensorFlow中定义
    VT_Named_Feature: 命名的变量, tffeature类型的值
    VT_Anonymous_Feature: 匿名变量, tffeature类型的值
    """

    VT_Not_Defined = 0
    VT_Constant_Int = 1
    VT_Constant_Float = 2
    VT_Constant_String = 3
    VT_Constant_IntList = 4
    VT_Constant_FloatList = 5
    VT_Constant_StringList = 6
    VT_RunTime_Int = 7
    VT_RunTime_Float = 8
    VT_RunTime_String = 9
    VT_Origin_Feature = 10
    VT_Named_Feature = 11
    VT_Anonymous_Feature = 12


class FunctionType(IntEnum):
    FT_Not_Defined = 0
    FT_Single_Mapper = 1
    FT_Single_Aggregate = 2
    FT_Hadamard_Cross = 3
    FT_Hadamard_Mapper = 4
    FT_Hadamard_Aggregate = 5


class Operator:
    # 前向声明
    pass


class Parameter:
    """
    参数定义.

    index: 函数调用时候的第几个参数, 下标从0开始计数
    type: 值的类型
    value: 对应的值, 如果是常量,value就是常量的值;
                    如果是原始变量, 就是对应变量的名字;
                    如果是生成型变量, 就是对应的算子的名字
    """

    def __init__(self, index: int, value: Union[str, Operator]) -> None:
        if isinstance(value, Operator):
            self.index = index
            self.type = VariableType.VT_Anonymous_Feature
            self.value = value
            return
        elif isinstance(value, list):
            self.type = get_list_type(value)
            self.index = index
            self.value = value
            return
        assert(isinstance(value, str))

        value = value.strip()
        if is_int(value):
            # 如果是整型就是VT_Constant_Int
            self.index = index
            self.type = VariableType.VT_Constant_Int
            self.value = int(value)
        elif is_float(value):
            # 如果是浮点型就是VT_Constant_Float
            self.index = index
            self.type = VariableType.VT_Constant_Float
            self.value = float(value)
        elif value.startswith('"'):
            # 如果引号不是成对的, 就是异常
            if len(value) < 2 or not value.endswith('"'):
                self.index = index
                self.type = VariableType.VT_Not_Defined
                self.value = value
            else:
                self.index = index
                self.type = VariableType.VT_Constant_String
                self.value = value[1:-1]
        else:
            self.index = index
            self.value = value
            if value.startswith('__RT_'):
                # 运行时的值以`__R_`开头
                if value.startswith('__RT_I_'):
                    self.type = VariableType.VT_RunTime_Int
                elif value.startswith('__RT_F_'):
                    self.type = VariableType.VT_RunTime_Float
                elif value.startswith('__RT_S_'):
                    self.type = VariableType.VT_RunTime_String
                else:
                    self.type = VariableType.VT_Not_Defined
            else:
                self.type = VariableType.VT_Named_Feature


class Operator:
    """
    定义算子操作.

    name: 算子的名字
    function: 函数的名字
    parameters: 输入的参数
    """

    def __init__(self, name: str, tokens: List[Union[Operator, str]]):
        assert(len(tokens) > 1 and isinstance(tokens[0], str))
        self.name, self.type, self.function, self.function_type = name, VariableType.VT_Not_Defined, tokens[
            0], FunctionType.FT_Not_Defined
        self.parameters = []
        for i, token in enumerate(tokens[1:]):
            param = Parameter(i, token)
            assert(param.type != VariableType.VT_Not_Defined)
            self.parameters.append(param)

    def update(self):
        """
        更新数据.

        """
        # 更新type
        if self.name.startswith('anonymous_'):
            self.type = VariableType.VT_Anonymous_Feature
        else:
            self.type = VariableType.VT_Named_Feature

        assert len(self.parameters) > 0
        type_0 = self.parameters[0].type
        if type_0 not in (VariableType.VT_Anonymous_Feature, VariableType.VT_Named_Feature, VariableType.VT_Origin_Feature):
            raise TypeError(f"{self} function type error")

        type_1 = VariableType.VT_Not_Defined if len(
            self.parameters) < 2 else self.parameters[1].type

        if type_1 not in (VariableType.VT_Anonymous_Feature, VariableType.VT_Named_Feature, VariableType.VT_Origin_Feature):
            # 单变量函数
            assert self.function.startswith(
                "s_map_") or self.function.startswith("s_agg_")
            if self.function.startswith("s_map_"):
                self.function_type = FunctionType.FT_Single_Mapper
            elif self.function.startswith("s_agg_"):
                self.function_type = FunctionType.FT_Single_Aggregate
            return

        # 两个变量的函数
        assert self.function.startswith(
            "b_map_") or self.function.startswith("b_agg_") or self.function.startswith("b_cross_")
        if self.function.startswith("b_map_"):
            self.function_type = FunctionType.FT_Hadamard_Mapper
        elif self.function.startswith("b_agg_"):
            self.function_type = FunctionType.FT_Hadamard_Aggregate
        elif self.function.startswith("b_cross_"):
            self.function_type = FunctionType.FT_Hadamard_Cross

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
        params = []
        for p in self.parameters:
            if p.type == VariableType.VT_Anonymous_Feature:
                params.append(f'`{p.value.name}`')
            elif p.type in (VariableType.VT_Named_Feature, VariableType.VT_Origin_Feature):
                params.append(f'`{p.value}`')
            elif p.type == VariableType.VT_Constant_String:
                params.append(f'"{p.value}"')
            else:
                params.append(str(p.value))
        return f'{self.name} = {self.function}({", ".join(params)})'


def get_list_type(__o: list):
    """
    获得list的类型.

    """
    assert(isinstance(__o, list) and len(__o) > 0)
    if isinstance(__o[0], int):
        for v in __o:
            assert(isinstance(v, int))
        return VariableType.VT_Constant_IntList
    elif isinstance(__o[0], float):
        for v in __o:
            assert(isinstance(v, float))
        return VariableType.VT_Constant_FloatList
    elif isinstance(__o[0], str):
        for v in __o:
            assert(isinstance(v, str))
        return VariableType.VT_Constant_StringList
    return VariableType.VT_Not_Defined


def is_float(value: str) -> bool:
    """
    判断是不是浮点型.

    """
    try:
        float(value)
        return True
    except:
        return False


def is_int(value: str) -> bool:
    """
    判断是不是浮点型.

    """
    try:
        int(value)
        return True
    except:
        return False


class Parser:
    @staticmethod
    def do(dsls: List[str]) -> dict:
        ops = Parser._parse_many(dsls)
        configs = []

        for op in ops:
            config = {'name': op.name, 'func': op.function, 'params': []}
            for p in op.parameters:
                param = {'type': p.type.value, 'index': p.index}
                if p.type == VariableType.VT_Anonymous_Feature:
                    if p.value.name not in configs:
                        raise ValueError(f"{p.value.name} unkown")
                    param['data'] = p.value.name
                else:
                    param['data'] = p.value
                    if p.type == VariableType.VT_Named_Feature:
                        if p.value not in configs:
                            p.type = VariableType.VT_Origin_Feature
                            param['type'] = p.type.value
                config['params'].append(param)
            op.update()
            config['func_type'] = op.function_type.value
            config['type'] = op.type.value
            print(op)
            configs.append(config)
        return configs

    @staticmethod
    def _parse_many(dsls: List[str]):
        """
        解析多条.

        返回的是: 符合生成的依赖顺序, 同时相同的计算进行合并
        """
        all_ops = []

        # 先生成所有的op
        for dsl in dsls:
            try:
                all_ops.extend(Parser._parse_one(dsl))
            except Exception as e:
                traceback.print_exc()
                print(e)

        # 遍历生成的op

        def update_features(new_op: Operator, index: int, my_features: List[Operator]) -> bool:
            for op in my_features:
                if op.name.startswith('anonymous_') and new_op == op:
                    new_op.name = op.name
                    return False
            new_op.name = f'anonymous_{index}'
            my_features.append(new_op)
            return True

        ordered_features, anonymous_index = [], 0
        for op in all_ops:
            # 未命名的op, 则是匿名op
            if op.name == "":
                if update_features(op, anonymous_index, ordered_features):
                    anonymous_index += 1
            else:
                ordered_features.append(op)
        return ordered_features

    @staticmethod
    def _parse_one(dsl: str) -> Operator:
        """
        解析dsl.

        """
        # 第一步先处理list的情形: [...]
        tmp, lists, list_index, list_str, status, list_dic = '', [], 0, '', 0, {}

        for s in dsl:
            if s == '[':
                list_str += s
                status = 1
            elif s == ']':
                list_str += s
                tmp_list = eval(list_str)
                lists.append(tmp_list)
                list_str, status = '', 0
                key = f'__LIST_{list_index}__'
                list_dic[key] = tmp_list
                tmp += key
                list_index += 1
            elif status == 1:
                list_str += s
            else:
                tmp += s

        tmp = tmp.replace(" ", "").replace("'", "\"")
        items = tmp.strip().split("=")
        assert(len(items) == 2)
        var_name = items[0].strip()
        tmp = items[1].strip()
        tmp = tmp.replace("(", " ( ").replace(")", " )").replace(",", " ")
        tokens = tmp.strip().split(" ")

        stack, all_ops = [], []
        for token in tokens:
            if token == ')':
                tmp = []
                while len(stack) > 0:
                    t = stack.pop(-1)
                    if t == '(':
                        if len(stack) == 0:
                            raise Exception(f"parse: {dsl} error")
                        tmp.append(stack.pop(-1))
                        break
                    else:
                        tmp.append(t)
                tokens = []
                for t in tmp[::-1]:
                    if isinstance(t, str) and t in list_dic:
                        tokens.append(list_dic[t])
                    else:
                        tokens.append(t)

                op = Operator("", tokens)
                all_ops.append(op)
                stack.append(op)
            else:
                stack.append(token)
        assert(len(stack) == 1)
        stack[0].name = var_name
        return all_ops


if __name__ == '__main__':
    dsls = [
        'u_d_cat1 = s_map_add(i_item, 100.5)',
        #'u_d_cat2 = b_cross_concat (u_d_cat1, i_item)',
    ]

    configs = Parser.do(dsls)
    toml.dump({"operators": configs}, open('config.toml', 'w'))
    # print(configs)
    print('\n\n\n')
    for item in configs:
        print(item)
