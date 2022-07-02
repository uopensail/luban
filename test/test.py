#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Created on Mon May 13 17:58:27 2019

@author: timepi

@description: test
"""
import pyluban
import base64
import tensorflow as tf
from google.protobuf.json_format import Parse


def gen_test_record(filename):
    """
    生成测试
    :param filename:
    :return:
    """
    data = open(filename, 'r').read()
    
    example = tf.train.Example()
    Parse(data, example)
    record = example.features.SerializeToString()
    #return base64.b64decode(data)
    return record


def test(filename):
    """
    测试
    :param filename:
    :return:
    """
    config_file = 'test.toml'
    record = gen_test_record(filename)

    toolkit = pyluban.PyToolKit(config_file)
    entity = pyluban.PyEntity()
    toolkit.process(record, entity)
    size = entity.size()
    for i in range(size):
        e = entity.get(i)
        print(e)
        
       


test("test.txt")
