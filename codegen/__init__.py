# -*- coding: utf-8 -*-

from mako.template import Template
from .methods import Method
from os import path as ospath

def generate(defs):

    for value in defs.values():
        for key in ("includes", "methods"):
            if key not in value:
                value[key] = []
            if isinstance(value[key], basestring):
                value[key] = [value[key]]
        value["methods"] = [Method(*x) for x in value["methods"]]

    # Build right inheritance chain
    classes = []
    keys = defs.keys()
    while keys:
        keys_count = len(keys)
        for key in keys[:]:
            value = defs[key]
            parent = value.get("inherit", None)
            if parent == key:
                raise ValueError("{0} inherint itself.".format(key))
            if not parent or parent not in keys:
                print "Adding {0}".format(key)
                classes.append((key, value))
                keys.remove(key)
        if keys_count == len(keys):
            raise ValueError("Recursive inheritance detected: \n\
            In array: {0}".format(keys))

    temp_path = ospath.join("codegen", "templates")

    # Bind data
    data_bind = Template(filename=ospath.join(temp_path, "data_bind.tpl"))
    data_bind_out = data_bind.render_unicode(classes=classes)

    type_list = Template(filename=ospath.join(temp_path, "type_list.tpl"))
    type_list_out = type_list.render_unicode(count=len(classes))

    for item, filename in ((data_bind_out, "CUDataBind.h"),
                           (type_list_out, "TypeListDef.h")):
        path = ospath.join("src", "generated", filename)
        with open(path, 'w') as f:
            f.write(item)

    print "Done."


__all__ = ["generate"]
