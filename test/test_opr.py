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

import json
import random
import time
import unittest
from datetime import datetime

import numpy as np
import pyluban


def add(a, b):
    return np.float32(a) + np.float32(b)


def sub(a, b):
    return np.float32(a) - np.float32(b)


def mul(a, b):
    return np.float32(a) * np.float32(b)


def div(a, b):
    return np.float32(a) / np.float32(b)


def sigmoid(a):
    return 1.0 / (1.0 + np.exp(-np.float32(a)))


def average(a):
    return np.sum(np.array(a, dtype=np.float32)) / np.float32(len(a))


def max(a):
    return np.max(np.array(a, dtype=np.float32))


def min(a):
    return np.min(np.array(a, dtype=np.float32))


def variance(a):
    return np.var(np.array(a, dtype=np.float32))


def stddev(a):
    return np.std(np.array(a, dtype=np.float32))


class OperatorTestCase(unittest.TestCase):
    def setUp(self) -> None:
        self.funcs = []

    def create_function(self, func, name, flag, args, vars):
        return pyluban.Function(
            json.dumps(
                {
                    "func": func,
                    "name": name,
                    "flag": flag,
                    "args": args,
                    "vars": vars,
                }
            )
        )

    def create_floats(self, names):
        args = {}
        for name in names:
            args[name] = {"type": 1, "value": random.random() * 100}
        return pyluban.Features(json.dumps(args))

    def create_ints(self, names):
        args = {}
        for name in names:
            args[name] = {"type": 0, "value": random.randint(1000, 10000)}
        return pyluban.Features(json.dumps(args))

    def test_add(self):
        func1 = self.create_function("addf", "X", 3, [], ["A", "B"])
        arg = [{"type": 1, "value": random.random() * 100}]
        func2 = self.create_function("addf", "Y", 1, arg, ["A"])
        func3 = self.create_function("addf", "Z", 1, arg, ["A"])
        feas = self.create_floats(["A", "B"])
        opr = pyluban.Operator()
        opr.call(func1, feas)
        opr.call(func2, feas)
        opr.call(func3, feas)
        dic = json.loads(str(feas))
        tmp = {}
        for k, v in dic.items():
            tmp[k] = v["value"]
        dic = tmp
        self.assertLess(abs(dic["X"] - add(dic["A"], dic["B"])), 1e-6)
        self.assertLess(abs(dic["Z"] - add(dic["A"], arg[0]["value"])), 1e-6)
        self.assertLess(abs(dic["Y"] - add(dic["A"], arg[0]["value"])), 1e-6)

    def test_add_int(self):
        func1 = self.create_function("addi", "X", 3, [], ["A", "B"])
        arg = [{"type": 1, "value": random.randint(1000, 100000)}]
        func2 = self.create_function("addi", "Y", 1, arg, ["A"])
        func3 = self.create_function("addi", "Z", 1, arg, ["A"])
        feas = self.create_ints(["A", "B"])
        opr = pyluban.Operator()
        opr.call(func1, feas)
        opr.call(func2, feas)
        opr.call(func3, feas)
        dic = json.loads(str(feas))
        tmp = {}
        for k, v in dic.items():
            tmp[k] = v["value"]
        dic = tmp
        self.assertLess(abs(dic["X"] - add(dic["A"], dic["B"])), 1e-6)
        self.assertLess(abs(dic["Z"] - add(dic["A"], arg[0]["value"])), 1e-6)
        self.assertLess(abs(dic["Y"] - add(dic["A"], arg[0]["value"])), 1e-6)

    def test_add_cast_float(self):
        func1 = self.create_function("_add<int64_t>", "X", 3, [], ["A", "B"])
        arg = [{"type": 1, "value": random.random() * 100}]
        func2 = self.create_function("_add<int64_t>", "Y", 1, arg, ["A"])
        func3 = self.create_function("_add<int64_t>", "Z", 1, arg, ["A"])
        feas = self.create_floats(["A", "B"])
        opr = pyluban.Operator()
        opr.call(func1, feas)
        opr.call(func2, feas)
        opr.call(func3, feas)
        dic = json.loads(str(feas))
        tmp = {}
        for k, v in dic.items():
            tmp[k] = v["value"]
        dic = tmp
        self.assertLess(abs(dic["X"] - add(int(dic["A"]), int(dic["B"]))), 1e-6)
        self.assertLess(abs(dic["Z"] - add(int(dic["A"]), int(arg[0]["value"]))), 1e-6)
        self.assertLess(abs(dic["Y"] - add(int(dic["A"]), int(arg[0]["value"]))), 1e-6)

    def test_add_cast_float(self):
        func1 = self.create_function("addf", "X", 3, [], ["A", "B"])
        arg = [{"type": 1, "value": random.randint(1000, 10000)}]
        func2 = self.create_function("addf", "Y", 1, arg, ["A"])
        func3 = self.create_function("addf", "Z", 1, arg, ["A"])
        feas = self.create_ints(["A", "B"])
        opr = pyluban.Operator()
        opr.call(func1, feas)
        opr.call(func2, feas)
        opr.call(func3, feas)
        dic = json.loads(str(feas))
        tmp = {}
        for k, v in dic.items():
            tmp[k] = v["value"]
        dic = tmp
        self.assertLess(abs(dic["X"] - add(dic["A"], dic["B"])), 1e-6)
        self.assertLess(abs(dic["Z"] - add(dic["A"], arg[0]["value"])), 1e-6)
        self.assertLess(abs(dic["Y"] - add(dic["A"], arg[0]["value"])), 1e-6)

    def test_sub(self):
        func1 = self.create_function("subf", "X", 3, [], ["A", "B"])
        arg = [{"type": 1, "value": random.random() * 100}]
        func2 = self.create_function("subf", "Y", 1, arg, ["A"])
        func3 = self.create_function("subf", "Z", 2, arg, ["A"])
        feas = self.create_floats(["A", "B"])
        opr = pyluban.Operator()
        opr.call(func1, feas)
        opr.call(func2, feas)
        opr.call(func3, feas)
        dic = json.loads(str(feas))
        tmp = {}
        for k, v in dic.items():
            tmp[k] = v["value"]
        dic = tmp
        self.assertLess(abs(dic["X"] - sub(dic["A"], dic["B"])), 1e-6)
        self.assertLess(abs(dic["Z"] - sub(arg[0]["value"], dic["A"])), 1e-6)
        self.assertLess(abs(dic["Y"] - sub(dic["A"], arg[0]["value"])), 1e-6)

    def test_mul(self):
        func1 = self.create_function("mulf", "X", 3, [], ["A", "B"])
        arg = [{"type": 1, "value": random.random() * 100}]
        func2 = self.create_function("mulf", "Y", 1, arg, ["A"])
        func3 = self.create_function("mulf", "Z", 2, arg, ["A"])
        feas = self.create_floats(["A", "B"])
        opr = pyluban.Operator()
        opr.call(func1, feas)
        opr.call(func2, feas)
        opr.call(func3, feas)
        dic = json.loads(str(feas))
        tmp = {}
        for k, v in dic.items():
            tmp[k] = v["value"]
        dic = tmp
        self.assertLess(abs(dic["X"] - mul(dic["A"], dic["B"])), 1e-6)
        self.assertLess(abs(dic["Z"] - mul(arg[0]["value"], dic["A"])), 1e-6)
        self.assertLess(abs(dic["Y"] - mul(dic["A"], arg[0]["value"])), 1e-6)

    def test_div(self):
        func1 = self.create_function("divf", "X", 3, [], ["A", "B"])
        arg = [{"type": 1, "value": random.random() * 100}]
        func2 = self.create_function("divf", "Y", 1, arg, ["A"])
        func3 = self.create_function("divf", "Z", 2, arg, ["A"])
        feas = self.create_floats(["A", "B"])
        opr = pyluban.Operator()
        opr.call(func1, feas)
        opr.call(func2, feas)
        opr.call(func3, feas)
        dic = json.loads(str(feas))
        tmp = {}
        for k, v in dic.items():
            tmp[k] = v["value"]
        dic = tmp
        self.assertLess(abs(dic["X"] - div(dic["A"], dic["B"])), 1e-6)
        self.assertLess(abs(dic["Z"] - div(arg[0]["value"], dic["A"])), 1e-6)
        self.assertLess(abs(dic["Y"] - div(dic["A"], arg[0]["value"])), 1e-6)

    def test_log(self):
        func1 = self.create_function("log", "X", 1, [], ["A"])
        arg = [{"type": 1, "value": random.random() * 100}]
        func2 = self.create_function("log", "Y", 1, [], ["A"])
        func3 = self.create_function("log", "Z", 0, arg, [])
        feas = self.create_floats(["A"])
        opr = pyluban.Operator()
        opr.call(func1, feas)
        opr.call(func2, feas)
        opr.call(func3, feas)
        dic = json.loads(str(feas))
        tmp = {}
        for k, v in dic.items():
            tmp[k] = v["value"]
        dic = tmp
        self.assertLess(abs(dic["X"] - np.log(dic["A"])), 1e-6)
        self.assertLess(abs(dic["Z"] - np.log(arg[0]["value"])), 1e-6)
        self.assertLess(abs(dic["Y"] - np.log(dic["A"])), 1e-6)

    def test_sigmoid(self):
        func1 = self.create_function("sigmoid", "X", 1, [], ["A"])
        arg = [{"type": 1, "value": random.random()}]
        func2 = self.create_function("sigmoid", "Y", 1, [], ["A"])
        func3 = self.create_function("sigmoid", "Z", 0, arg, [])
        feas = self.create_floats(["A"])
        opr = pyluban.Operator()
        opr.call(func1, feas)
        opr.call(func2, feas)
        opr.call(func3, feas)
        dic = json.loads(str(feas))
        tmp = {}
        for k, v in dic.items():
            tmp[k] = v["value"]
        dic = tmp
        self.assertLess(abs(dic["X"] - sigmoid(dic["A"])), 1e-6)
        self.assertLess(abs(dic["Z"] - sigmoid(arg[0]["value"])), 1e-6)
        self.assertLess(abs(dic["Y"] - sigmoid(dic["A"])), 1e-6)

    def test_average(self):
        arg = [{"type": 4, "value": [random.random() for _ in range(10)]}]
        func = self.create_function("average", "X", 0, arg, [])
        feas = pyluban.Features()
        opr = pyluban.Operator()
        opr.call(func, feas)
        dic = json.loads(str(feas))
        tmp = {}
        for k, v in dic.items():
            tmp[k] = v["value"]
        dic = tmp
        self.assertLess(abs(dic["X"] - average(arg[0]["value"])), 1e-6)

    def test_max(self):
        arg = [{"type": 4, "value": [random.random() for _ in range(10)]}]
        func = self.create_function("maxf", "X", 0, arg, [])
        feas = pyluban.Features()
        opr = pyluban.Operator()
        opr.call(func, feas)
        dic = json.loads(str(feas))
        tmp = {}
        for k, v in dic.items():
            tmp[k] = v["value"]
        dic = tmp
        self.assertLess(abs(dic["X"] - max(arg[0]["value"])), 1e-6)

    def test_min(self):
        arg = [{"type": 4, "value": [random.random() for _ in range(10)]}]
        func = self.create_function("minf", "X", 0, arg, [])
        feas = pyluban.Features()
        opr = pyluban.Operator()
        opr.call(func, feas)
        dic = json.loads(str(feas))
        tmp = {}
        for k, v in dic.items():
            tmp[k] = v["value"]
        dic = tmp
        self.assertLess(abs(dic["X"] - min(arg[0]["value"])), 1e-6)

    def test_variance(self):
        arg = [{"type": 4, "value": [random.random()] * 10}]
        func = self.create_function("variancef", "X", 0, arg, [])
        feas = pyluban.Features()
        opr = pyluban.Operator()
        opr.call(func, feas)
        dic = json.loads(str(feas))
        tmp = {}
        for k, v in dic.items():
            tmp[k] = v["value"]
        dic = tmp
        self.assertLess(abs(dic["X"] - variance(arg[0]["value"])), 1e-6)

    def test_stddev(self):
        arg = [{"type": 4, "value": [random.random() for _ in range(10)]}]
        func = self.create_function("stddevf", "X", 0, arg, [])
        feas = pyluban.Features()
        opr = pyluban.Operator()
        opr.call(func, feas)
        dic = json.loads(str(feas))
        tmp = {}
        for k, v in dic.items():
            tmp[k] = v["value"]
        dic = tmp
        self.assertLess(abs(dic["X"] - stddev(arg[0]["value"])), 1e-6)

    def test_year(self):
        func = self.create_function("year", "X", 0, [], [])
        feas = pyluban.Features()
        opr = pyluban.Operator()
        opr.call(func, feas)
        dic = json.loads(str(feas))
        tmp = {}
        for k, v in dic.items():
            tmp[k] = v["value"]
        dic = tmp
        now = datetime.now()
        self.assertEqual(int(dic["X"]), now.year)

    def test_month(self):
        func = self.create_function("month", "X", 0, [], [])
        feas = pyluban.Features()
        opr = pyluban.Operator()
        opr.call(func, feas)
        dic = json.loads(str(feas))
        tmp = {}
        for k, v in dic.items():
            tmp[k] = v["value"]
        dic = tmp
        now = datetime.now()
        self.assertEqual(int(dic["X"]), now.month)

    def test_day(self):
        func = self.create_function("day", "X", 0, [], [])
        feas = pyluban.Features()
        opr = pyluban.Operator()
        opr.call(func, feas)
        dic = json.loads(str(feas))
        tmp = {}
        for k, v in dic.items():
            tmp[k] = v["value"]
        dic = tmp
        now = datetime.now()
        self.assertEqual(int(dic["X"]), now.day)

    def test_hour(self):
        func = self.create_function("hour", "X", 0, [], [])
        feas = pyluban.Features()
        opr = pyluban.Operator()
        opr.call(func, feas)
        dic = json.loads(str(feas))
        tmp = {}
        for k, v in dic.items():
            tmp[k] = v["value"]
        dic = tmp
        now = datetime.now()
        self.assertEqual(int(dic["X"]), now.hour)

    def test_now(self):
        func = self.create_function("now", "X", 0, [], [])
        feas = pyluban.Features()
        opr = pyluban.Operator()
        opr.call(func, feas)
        dic = json.loads(str(feas))
        tmp = {}
        for k, v in dic.items():
            tmp[k] = v["value"]
        dic = tmp
        now = datetime.now()
        self.assertEqual(int(dic["X"]), int(now.timestamp() * 1000))

    def test_from_unixtime(self):
        now = datetime.now()
        arg = [
            {"type": 0, "value": int(now.timestamp())},
            {"type": 2, "value": "%Y-%m-%d"},
        ]
        func = self.create_function("from_unixtime", "X", 0, arg, [])
        feas = pyluban.Features()
        opr = pyluban.Operator()
        opr.call(func, feas)
        dic = json.loads(str(feas))
        tmp = {}
        for k, v in dic.items():
            tmp[k] = v["value"]
        dic = tmp
        self.assertEqual(dic["X"], now.date().isoformat())

    def test_unix_timestamp(self):
        now = datetime.now()
        arg = [
            {"type": 2, "value": now.date().isoformat()},
            {"type": 2, "value": "%Y-%m-%d"},
        ]
        func = self.create_function("unix_timestamp", "X", 0, arg, [])
        feas = pyluban.Features()
        opr = pyluban.Operator()
        opr.call(func, feas)
        dic = json.loads(str(feas))
        tmp = {}
        for k, v in dic.items():
            tmp[k] = v["value"]
        dic = tmp

        self.assertEqual(
            dic["X"], int(time.mktime(time.strptime(arg[0]["value"], "%Y-%m-%d")))
        )

    def test_date_add(self):
        arg = [{"type": 2, "value": "2023-01-01"}, {"type": 0, "value": 10}]
        func = self.create_function("date_add", "X", 0, arg, [])
        feas = pyluban.Features()
        opr = pyluban.Operator()
        opr.call(func, feas)
        dic = json.loads(str(feas))
        tmp = {}
        for k, v in dic.items():
            tmp[k] = v["value"]
        dic = tmp

        self.assertEqual(dic["X"], "2023-01-11")

    def test_date_sub(self):
        arg = [{"type": 2, "value": "2023-01-11"}, {"type": 0, "value": 10}]
        func = self.create_function("date_sub", "X", 0, arg, [])
        feas = pyluban.Features()
        opr = pyluban.Operator()
        opr.call(func, feas)
        dic = json.loads(str(feas))
        tmp = {}
        for k, v in dic.items():
            tmp[k] = v["value"]
        dic = tmp

        self.assertEqual(dic["X"], "2023-01-01")

    def test_date_diff(self):
        arg = [{"type": 2, "value": "2023-01-11"}, {"type": 2, "value": "2023-01-01"}]
        func = self.create_function("date_diff", "X", 0, arg, [])
        feas = pyluban.Features()
        opr = pyluban.Operator()
        opr.call(func, feas)
        dic = json.loads(str(feas))
        tmp = {}
        for k, v in dic.items():
            tmp[k] = v["value"]
        dic = tmp

        self.assertEqual(dic["X"], 10)

    def test_reverse(self):
        arg = [{"type": 2, "value": "2023-01-11"}]
        func = self.create_function("reverse", "X", 0, arg, [])
        feas = pyluban.Features()
        opr = pyluban.Operator()
        opr.call(func, feas)
        dic = json.loads(str(feas))
        tmp = {}
        for k, v in dic.items():
            tmp[k] = v["value"]
        dic = tmp

        self.assertEqual(dic["X"], "11-10-3202")

    def test_upper(self):
        arg = [{"type": 2, "value": "abcdef"}]
        func = self.create_function("upper", "X", 0, arg, [])
        feas = pyluban.Features()
        opr = pyluban.Operator()
        opr.call(func, feas)
        dic = json.loads(str(feas))
        tmp = {}
        for k, v in dic.items():
            tmp[k] = v["value"]
        dic = tmp

        self.assertEqual(dic["X"], "ABCDEF")

    def test_lower(self):
        arg = [{"type": 2, "value": "ABCDEF"}]
        func = self.create_function("lower", "X", 0, arg, [])
        feas = pyluban.Features()
        opr = pyluban.Operator()
        opr.call(func, feas)
        dic = json.loads(str(feas))
        tmp = {}
        for k, v in dic.items():
            tmp[k] = v["value"]
        dic = tmp

        self.assertEqual(dic["X"], "abcdef")

    def test_substr(self):
        arg = [
            {"type": 2, "value": "ABCDEF"},
            {"type": 0, "value": 2},
            {"type": 0, "value": 3},
        ]
        func = self.create_function("substr", "X", 0, arg, [])
        feas = pyluban.Features()
        opr = pyluban.Operator()
        opr.call(func, feas)
        dic = json.loads(str(feas))
        tmp = {}
        for k, v in dic.items():
            tmp[k] = v["value"]
        dic = tmp

        self.assertEqual(dic["X"], "CDE")

    def test_concat(self):
        arg = [{"type": 2, "value": 'ABCD"EF'}, {"type": 2, "value": "-123456"}]
        func = self.create_function("concat", "X", 0, arg, [])
        feas = pyluban.Features()
        opr = pyluban.Operator()
        opr.call(func, feas)
        dic = json.loads(str(feas))
        tmp = {}
        for k, v in dic.items():
            tmp[k] = v["value"]
        dic = tmp

        self.assertEqual(dic["X"], 'ABCD"EF-123456')


if __name__ == "__main__":
    unittest.main()
