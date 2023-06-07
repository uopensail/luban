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
import os
import json
import pyluban
import tensorflow as tf
from google.protobuf.json_format import Parse

config = {
    "transforms": [
        "country = lower(u_s_country)",
        "clicks = concat(\"item_id=\", u_r_click)",
        "publish = from_unixtime(d_s_publishTime/1000, \"%Y%m%d\")",
        "now = now()"
    ],
    "outputs": [
        {
            "name": "d_s_id",
            "group": 0
        },
        {
            "name": "country",
            "group": 1
        },
        {
            "name": "clicks",
            "group": 2
        },
        {
            "name": "publish",
            "group": 3
        },
        {
            "name": "now",
            "group": 4
        }
    ]
}

record = {
    "features": {
        "feature": {
            "uc_m_networkType": {
                "bytesList": {
                    "value": [
                        "V0lGSQ=="
                    ]
                }
            },
            "uc_m_timestamp": {
                "int64List": {
                    "value": [
                        "1606447233"
                    ]
                }
            },
            "u_s_id": {
                "bytesList": {
                    "value": [
                        "Yzg1NjZmYjgzZTdlODRiYQ=="
                    ]
                }
            },
            "u_r_click_int": {
                "int64List": {
                    "value": [
                        "12443567",
                        "12434460",
                        "12421510",
                        "12422274",
                        "12427577",
                        "12405095",
                        "12387351",
                        "12416097",
                        "12409586",
                        "12402518",
                        "12399007",
                        "12364393",
                        "12359925",
                        "12319341",
                        "12322425",
                        "12281653",
                        "12248610",
                        "12238856",
                        "12177396",
                        "12176051",
                        "12136341"
                    ]
                }
            },
            "d_r_impValid": {
                "int64List": {
                    "value": [
                        "108008"
                    ]
                }
            },
            "d_r_videoTime": {
                "int64List": {
                    "value": [
                        "0"
                    ]
                }
            },
            "d_r_readTime": {
                "int64List": {
                    "value": [
                        "483213809"
                    ]
                }
            },
            "uc_m_ip": {
                "bytesList": {
                    "value": [
                        "MjAzLjgxLjE3Ny41NA=="
                    ]
                }
            },
            "d_s_publishTime": {
                "int64List": {
                    "value": [
                        "1606169930000"
                    ]
                }
            },
            "u_d_videoSubcats": {
                "bytesList": {
                    "value": [
                        "bWV0cm8=",
                        "ZGFuY2U=",
                        "aHVtb3Vy",
                        "bW92aWVfc2VyaWVz",
                        "Y3JpbWU="
                    ]
                }
            },
            "uc_m_appSource": {
                "bytesList": {
                    "value": [
                        "bGF1bmNoZXI="
                    ]
                }
            },
            "u_r_d_s_cat2": {
                "bytesList": {
                    "value": [
                        "Y2VsZWJyaXR5",
                        "bWV0cm8=",
                        "cmVsYXRpb25zaGlw",
                        "cG9saXRpY2FsX3NjYW5kYWw=",
                        "bG9jYWxfcG9saXRpY3M=",
                        "bWV0cm8=",
                        "bWV0cm8=",
                        "bG9jYWxfcG9saXRpY3M=",
                        "bWV0cm8=",
                        "cmVsYXRpb25zaGlw",
                        "Y2VsZWJyaXR5",
                        "Y2VsZWJyaXR5",
                        "Z2xvYmFsX3BvbGl0aWNz",
                        "Y2hyaXN0aWFuaXR5",
                        "bWV0cm8=",
                        "bWV0cm8=",
                        "YW5pbWFs",
                        "YWNjaWRlbnQ=",
                        "bWV0cm8=",
                        "Y2VsZWJyaXR5",
                        "Y2VsZWJyaXR5"
                    ]
                }
            },
            "uc_m_feedFrom": {
                "int64List": {
                    "value": [
                        "255"
                    ]
                }
            },
            "u_d_textSubcats": {
                "bytesList": {
                    "value": [
                        "Y2VsZWJyaXR5",
                        "bWV0cm8=",
                        "bG9jYWxfcG9saXRpY3M=",
                        "Z2xvYmFsX3BvbGl0aWNz",
                        "cmVsYXRpb25zaGlw"
                    ]
                }
            },
            "d_s_sourceName": {
                "bytesList": {
                    "value": [
                        "VGhlIE9sZCBUaW1lcw=="
                    ]
                }
            },
            "d_s_newsType": {
                "int64List": {
                    "value": [
                        "1"
                    ]
                }
            },
            "u_r_source": {
                "bytesList": {
                    "value": [
                        "Q2VsZWJCdXp6",
                        "U29jaWFsIFN1cnZleQ==",
                        "U3RyYW5nZSBCdXQgVHJ1ZQ==",
                        "TGlmZXN0eWxlTkc=",
                        "IkZha2UiIE5ld3M=",
                        "UmVsaWdpb24gVHJ1dGg=",
                        "VmlkZW9IZWFkbGluZXMgS0U=",
                        "UG9saXRpY3MgTmlnZXJpYQ==",
                        "VmlkZW8gUmVwdWJsaWM=",
                        "QXJvdW5kIFRoZSBXb3JsZA==",
                        "VEhFIFNUQVJT",
                        "TkcgU3RhcnM="
                    ]
                }
            },
            "uc_m_requestId": {
                "bytesList": {
                    "value": [
                        "YTIyMjFhOTgtNTAwMC00M2EyLWFiNGEtYTljNTZhOTdkZGIy"
                    ]
                }
            },
            "d_s_cat1": {
                "bytesList": {
                    "value": [
                        "YXJ0X2N1bHR1cmU="
                    ]
                }
            },
            "u_r_click": {
                "bytesList": {
                    "value": [
                        "MTI0NDM1Njc=",
                        "MTI0MzQ0NjA=",
                        "MTI0MjE1MTA=",
                        "MTI0MjIyNzQ=",
                        "MTI0Mjc1Nzc=",
                        "MTI0MDUwOTU=",
                        "MTIzODczNTE=",
                        "MTI0MTYwOTc=",
                        "MTI0MDk1ODY=",
                        "MTI0MDI1MTg=",
                        "MTIzOTkwMDc=",
                        "MTIzNjQzOTM=",
                        "MTIzNTk5MjU=",
                        "MTIzMTkzNDE=",
                        "MTIzMjI0MjU=",
                        "MTIyODE2NTM=",
                        "MTIyNDg2MTA=",
                        "MTIyMzg4NTY=",
                        "MTIxNzczOTY=",
                        "MTIxNzYwNTE=",
                        "MTIxMzYzNDE="
                    ]
                }
            },
            "u_s_country": {
                "bytesList": {
                    "value": [
                        "S0U="
                    ]
                }
            },
            "d_r_click": {
                "int64List": {
                    "value": [
                        "5536"
                    ]
                }
            },
            "d_s_contentType": {
                "int64List": {
                    "value": [
                        "1"
                    ]
                }
            },
            "d_s_tags": {
                "bytesList": {
                    "value": [
                        "b3ZlcjdkYXlz"
                    ]
                }
            },
            "d_r_clickValid": {
                "int64List": {
                    "value": [
                        "5566"
                    ]
                }
            },
            "d_r_imp": {
                "int64List": {
                    "value": [
                        "296487"
                    ]
                }
            },
            "u_r_d_s_sourceName": {
                "bytesList": {
                    "value": [
                        "Q2VsZWJCdXp6",
                        "U3RyYW5nZSBCdXQgVHJ1ZQ==",
                        "VmlkZW8gUmVwdWJsaWM=",
                        "IkZha2UiIE5ld3M=",
                        "UmVsaWdpb24gVHJ1dGg=",
                        "TkcgU3RhcnM=",
                        "U29jaWFsIFN1cnZleQ==",
                        "UmVsaWdpb24gVHJ1dGg=",
                        "U3RyYW5nZSBCdXQgVHJ1ZQ==",
                        "TGlmZXN0eWxlTkc=",
                        "TkcgU3RhcnM=",
                        "TGlmZXN0eWxlTkc=",
                        "IkZha2UiIE5ld3M=",
                        "UmVsaWdpb24gVHJ1dGg=",
                        "VmlkZW8gUmVwdWJsaWM=",
                        "VmlkZW9IZWFkbGluZXMgS0U=",
                        "UG9saXRpY3MgTmlnZXJpYQ==",
                        "VmlkZW8gUmVwdWJsaWM=",
                        "QXJvdW5kIFRoZSBXb3JsZA==",
                        "VEhFIFNUQVJT",
                        "TkcgU3RhcnM="
                    ]
                }
            },
            "d_s_id": {
                "int64List": {
                    "value": [
                        "12415760"
                    ]
                }
            },
            "uc_m_from": {
                "int64List": {
                    "value": [
                        "1"
                    ]
                }
            },
            "u_r_d_s_cat1": {
                "bytesList": {
                    "value": [
                        "ZW50ZXJ0YWlubWVudA==",
                        "Y29tbXVuaXR5",
                        "bGlmZXN0eWxl",
                        "cG9saXRpY3M=",
                        "cG9saXRpY3M=",
                        "Y29tbXVuaXR5",
                        "Y29tbXVuaXR5",
                        "cG9saXRpY3M=",
                        "Y29tbXVuaXR5",
                        "bGlmZXN0eWxl",
                        "ZW50ZXJ0YWlubWVudA==",
                        "ZW50ZXJ0YWlubWVudA==",
                        "cG9saXRpY3M=",
                        "YXJ0X2N1bHR1cmU=",
                        "Y29tbXVuaXR5",
                        "Y29tbXVuaXR5",
                        "bmF0dXJl",
                        "Y29tbXVuaXR5",
                        "Y29tbXVuaXR5",
                        "ZW50ZXJ0YWlubWVudA==",
                        "ZW50ZXJ0YWlubWVudA=="
                    ]
                }
            },
            "u_d_cats": {
                "bytesList": {
                    "value": [
                        "ZW50ZXJ0YWlubWVudA==",
                        "Y29tbXVuaXR5",
                        "cG9saXRpY3M=",
                        "bGlmZXN0eWxl",
                        "YmVhdXR5"
                    ]
                }
            },
            "u_d_topics": {
                "bytesList": {
                    "value": [
                        "ZW5fdG9waWMzMl8zMQ==",
                        "ZW5fdG9waWMzMl8yOQ==",
                        "ZW5fdG9waWMzMl8zMA==",
                        "ZW5fdG9waWMzMl8xNQ==",
                        "ZW5fdG9waWMzMl81",
                        "ZW5fdG9waWMxMjhfNw==",
                        "ZW5fdG9waWMxMjhfMzg=",
                        "ZW5fdG9waWMxMjhfMw==",
                        "ZW5fdG9waWMxMjhfNjg=",
                        "ZW5fdG9waWMxMjhfNjc=",
                        "ZW5fdG9waWMxMjhfMzE=",
                        "ZW5fdG9waWMxMjhfNDQ=",
                        "ZW5fdG9waWMxMjhfNg==",
                        "ZW5fdG9waWMxMjhfMjU=",
                        "ZW5fdG9waWMxMjhfMTAz"
                    ]
                }
            },
            "label": {
                "int64List": {
                    "value": [
                        "1"
                    ]
                }
            },
            "d_s_channelID": {
                "bytesList": {
                    "value": [
                        "Nzg=",
                        "NjM=",
                        "OTM=",
                        "NTE=",
                        "MTEz",
                        "MTIz",
                        "MTM2",
                        "Mzg=",
                        "MTAw"
                    ]
                }
            },
            "u_r_cat2": {
                "bytesList": {
                    "value": [
                        "bWV0cm8=",
                        "Y2VsZWJyaXR5",
                        "bG9jYWxfcG9saXRpY3M=",
                        "cmVsYXRpb25zaGlw",
                        "cG9saXRpY2FsX3NjYW5kYWw="
                    ]
                }
            },
            "d_s_coverType": {
                "int64List": {
                    "value": [
                        "3"
                    ]
                }
            },
            "uc_m_page": {
                "int64List": {
                    "value": [
                        "8"
                    ]
                }
            },
            "d_s_language": {
                "bytesList": {
                    "value": [
                        "ZW4="
                    ]
                }
            },
            "d_s_countryCode": {
                "bytesList": {
                    "value": [
                        "Tkc=",
                        "S0U=",
                        "T1RIRVI=",
                        "R0g=",
                        "SU4=",
                        "VFo=",
                        "VUc=",
                        "Wk0=",
                        "RVQ=",
                        "Ulc=",
                        "TVc=",
                        "R00=",
                        "Wlc=",
                        "WkE=",
                        "U0w=",
                        "TFI="
                    ]
                }
            },
            "d_s_cat2": {
                "bytesList": {
                    "value": [
                        "aGlzdG9yeQ=="
                    ]
                }
            }
        }
    }
}


def test_process(config_file: str):
    """test process function

    Args:
        config_file (str): json configuration
    """
    example = tf.train.Example()
    Parse(json.dumps(record), example)
    toolkit = pyluban.PyToolKit(config_file=config_file)
    entity_array = pyluban.PyEntityArray()
    toolkit.process(features=example.features.SerializeToString(),
                    entity=entity_array)
    size = entity_array.size()
    for i in range(size):
        entity = entity_array.get(i)
        print(entity)


def generate_tfrecord(data_path: str):
    """use tensorflow to generate tfrecord

    Args:
        data_path (str): tfrecord file 
    """
    print(f'now generating tfrecord: {data_path}')
    writer = tf.io.TFRecordWriter(data_path)
    example = tf.train.Example()
    Parse(json.dumps(record), example)
    data = example.SerializeToString()
    for i in range(10000):
        writer.write(data)
    writer.close()


def test_process_file(config_path: str, data_path: str, output_path: str):
    """test process_file function

    Args:
        config_path (str): toml configuration
        data_path (str): data path of tfrecord
        output_path (str): output file
    """
    generate_tfrecord(data_path)
    toolkit = pyluban.PyToolKit(config_path)
    toolkit.process_file(data_path, output_path)


if __name__ == "__main__":
    pwd = os.getcwd()
    json_config_path = os.path.join(pwd, "config.json")
    toml_config_path = os.path.join(pwd, "config.toml")
    f = open(json_config_path, 'w')
    f.write(json.dumps(config))
    f.close()
    cmd = f"luban_parser -i {json_config_path} -o {toml_config_path}"
    os.system(cmd)
    test_process(toml_config_path)
    data_path = os.path.join(pwd, "data.tfrecord")
    output_path = os.path.join(pwd, "out.txt")
    test_process_file(toml_config_path, data_path, output_path)
