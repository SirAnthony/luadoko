#!/usr/bin/python
# -*- coding: utf-8 -*-
from codegen import generate
import simplejson
import sys

if __name__ == '__main__':
    filename = 'definitions.json'
    try:
        filename = sys.argv[2]
    except IndexError:
        pass
    with open(filename) as f:
        classes = simplejson.load(f)
    generate(classes)
