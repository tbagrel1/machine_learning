#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from distutils.core import setup, Extension
import numpy as np

ext_modules = [Extension("binimage", sources=["binimage.c"])]

setup(
    name="binimage",
    version="0.1.0",
    include_dirs=[np.get_include()],
    ext_modules=ext_modules
)
