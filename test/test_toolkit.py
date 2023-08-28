#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# `LuBan` - 'c++ tool for transformating and hashing features'
#
# Copyright (C) 2019 - present timepi <timepi123@gmail.com>
# LuBan is provided under: GNU Affero General Public License (AGPL3.0)
# https:#www.gnu.org/licenses/agpl-3.0.html unless stated otherwise.
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
import unittest

import numpy as np
import pyluban


class ToolkitTestCase(unittest.TestCase):
    def setUp(self) -> None:
        self.toolkit = pyluban.Toolkit("config.json")
        fea = {
            "B": {"type": 0, "value": 1},
            "C": {"type": 0, "value": 2},
            "F": {"type": 0, "value": 2100},
        }

        self.features = pyluban.Features(json.dumps(fea))

    def test(self):
        l = pyluban.FeaturesList()
        l.append(self.features)
        print(len(l))
        print(l[0])
        print(l)
        m = self.toolkit.process(l)
        print("m", m)
        print("m", len(m))
        arr = np.asarray(m[0])
        print(arr.dtype)
        print(arr)
        print(np.asarray(m[1]))
        print(l[0])


if __name__ == "__main__":
    unittest.main()
