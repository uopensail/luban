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
import antlr4
import json
from .luban import LubanListener
from .lubanLexer import lubanLexer
from .lubanParser import lubanParser
from .luban_types import DataType


def parse_expression(expression: str):
    """parse each expression

    Args:
        expression (str): expression to parse

    Returns:
        _type_: list of nodes
    """
    items = expression.split("=")
    assert len(items) == 2
    lexer = lubanLexer(antlr4.InputStream(items[1].strip()))
    stream = antlr4.CommonTokenStream(lexer)
    parser = lubanParser(stream)
    walker = antlr4.ParseTreeWalker()
    tree = parser.start()
    client = LubanListener()
    walker.walk(client, tree)
    root = client.parameters[0]
    root.name = items[0].strip()
    array = root.to_list()
    return array


def parse_expressions(expressions: list):
    """parse expressions

    Args:
        expressions (list): expressions to parse

    Returns:
        _type_: node list
    """
    arraies = []
    for expression in expressions:
        arraies.append(parse_expression(expression))

    nodes = []
    for array in arraies:
        for node in array:
            exists = False
            for n in nodes:
                if node == n:
                    exists = True
                    break
            if not exists:
                nodes.append(node)
    index = 0
    ret = []
    for node in nodes:
        if node.name == "":
            node.name = f"anonymous_{index}"
            index += 1
        ret.append(node.to_json())
    return ret


def parse_features(feature_configs: list) -> dict:
    features = {}
    for feature in feature_configs:
        dtype = DataType(feature["type"])
        hash = feature.get("hash", False)
        padding = None
        if dtype in (DataType.kInt64, DataType.kInt64s):
            padding = int(feature.get("padding", 0))
        elif dtype in (DataType.kFloat32, DataType.kFloat32s):
            padding = float(feature.get("padding", 0))
        elif dtype == DataType.kString:
            hash = True
            dtype = DataType.kInt64
            padding = int(feature.get("padding", 0))
        elif dtype == DataType.kStrings:
            hash = True
            dtype = DataType.kInt64s
            padding = int(feature.get("padding", 0))

        features[feature["name"]] = {
            "name": feature["name"],
            "type": dtype,
            "padding": padding,
            "group": -1,
            "index": -1,
            "offset": 0,
            "hash": hash,
            "dim": feature.get("dim", 1),
        }

    return features


def parse_group(
    config: dict, user_features_dict: dict, item_features_dict: dict
) -> tuple:
    user_groups_indeces, item_groups_indeces = [], []
    user_groups, item_groups = [], []
    user_features, item_features = [], []

    for i, group in enumerate(config["groups"]):
        if group[0] in user_features_dict:
            dtype = user_features_dict[group[0]]["type"]
            for fea in group:
                if fea not in user_features_dict:
                    raise ValueError(f"{fea} not in user fields")
                if user_features_dict[fea]["type"] != dtype:
                    raise TypeError("different types in a group")
            user_groups_indeces.append((i, dtype))
        elif group[0] in item_features_dict:
            dtype = item_features_dict[group[0]]["type"]
            for fea in group:
                if fea not in item_features_dict:
                    raise ValueError(f"{fea} not in item fields")
                if item_features_dict[fea]["type"] != dtype:
                    raise TypeError("different types in a group")
            item_groups_indeces.append((i, dtype))
        else:
            raise KeyError(f"{fea} not found")

    for i, (gid, dtype) in enumerate(user_groups_indeces):
        offset = 0
        for fea in config["groups"][gid]:
            feature_config = user_features_dict[fea]
            tmp = {
                "name": fea,
                "type": feature_config["type"],
                "padding": feature_config["padding"],
                "group": gid,
                "index": i,
                "offset": offset,
                "hash": feature_config["hash"],
                "dim": feature_config["dim"],
            }

            offset += tmp["dim"]
            user_features.append(tmp)
        user_groups.append({"id": gid, "index": i, "width": offset, "type": dtype})

    for i, (gid, dtype) in enumerate(item_groups_indeces):
        offset = 0
        for fea in config["groups"][gid]:
            feature_config = item_features_dict[fea]
            tmp = {
                "name": fea,
                "type": feature_config["type"],
                "padding": feature_config["padding"],
                "group": gid,
                "index": i,
                "offset": offset,
                "hash": feature_config["hash"],
                "dim": feature_config["dim"],
            }

            offset += tmp["dim"]
            item_features.append(tmp)
        item_groups.append({"id": gid, "index": i, "width": offset, "type": dtype})
    return item_groups, item_features, user_groups, user_features


def parse(input_file: str, output_file: str):
    """do parse

    Args:
        input_file (str): json config
        output_file (str): json config

    Raises:
        ValueError: variable unknown
    """
    configure = json.load(open(input_file, "r"))

    user_expressions, item_expressions = [], []
    for feature in configure["user_features"]:
        expr = feature.get("expr", None)
        if expr is not None:
            user_expressions.append(f'{feature["name"]}={expr}')

    user_transforms = parse_expressions(user_expressions)

    for feature in configure["item_features"]:
        expr = feature.get("expr", None)
        if expr is not None:
            item_expressions.append(f'{feature["name"]}={expr}')

    item_transforms = parse_expressions(item_expressions)

    user_features_dict = parse_features(configure["user_features"])
    item_features_dict = parse_features(configure["item_features"])

    item_groups, item_features, user_groups, user_features = parse_group(
        configure, user_features_dict, item_features_dict
    )
    user = {
        "transforms": user_transforms,
        "groups": user_groups,
        "features": user_features,
    }
    item = {
        "transforms": item_transforms,
        "groups": item_groups,
        "features": item_features,
    }
    json.dump({"user": user, "item": item}, open(output_file, "w"))


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
