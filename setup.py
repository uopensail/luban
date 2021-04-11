#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon May 13 17:58:27 2019

@author: timepi

@description: this is setup script for pyluban
"""
import platform
import sys

from setuptools import Extension, setup, find_packages

COMPILE_OPTIONS = []
LINK_OPTIONS = []


def is_new_osx():
    """Check whether we're on OSX >= 10.7"""
    if sys.platform != "darwin":
        return False
    mac_ver = platform.mac_ver()[0]
    if mac_ver.startswith("10"):
        minor_version = int(mac_ver.split(".")[1])
        return minor_version >= 7
    return False


if is_new_osx():
    # On Mac, use libc++ because Apple deprecated use of
    # libstdc
    COMPILE_OPTIONS.append("-stdlib=libc++")
    COMPILE_OPTIONS.append("-std=c++14")
    LINK_OPTIONS.append("-lc++")
    LINK_OPTIONS.append("-lprotobuf")

    # g++ (used by unix compiler on mac) links to libstdc++ as a default lib.
    # See: https://stackoverflow.com/questions/1653047/avoid-linking-to-libstdc
    LINK_OPTIONS.append("-nodefaultlibs")

pylubanmodule = Extension(
    name="_pyluban",
    sources=["src/MurmurHash3.cc", "src/feature.pb.cc", "src/pyluban.cpp", "src/luban_wrap.cxx"],
    extra_compile_args=COMPILE_OPTIONS,
    extra_link_args=LINK_OPTIONS,
)

setup(
    name="pyluban",
    version="1.0.0",
    description="Python wrapper for luban, a set of fast and robust hash functions.",
    license="License :: CC0 1.0 Universal (CC0 1.0) Public Domain Dedication",
    author="timepi",
    author_email="",
    url="",
    packages=find_packages(),
    py_modules=["pyluban"],
    ext_modules=[pylubanmodule],
    keywords="utility hash",
    long_description="",
    long_description_content_type="text/markdown",
    classifiers=[
        "Development Status :: 5 - Production/Stable",
        "Intended Audience :: Developers",
        "License :: CC0 1.0 Universal (CC0 1.0) Public Domain Dedication",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.6",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Topic :: Software Development :: Libraries",
        "Topic :: Utilities",
    ],
)