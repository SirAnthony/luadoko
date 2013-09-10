# -*- coding: utf-8 -*-


class Method(object):

    def __init__(self, function, name, modifier="", method_name=None):
        self.function = function
        self.name = name
        self.modifier = modifier
        self.modifier_s = '_'.join(self.modifier) if isinstance(
                            modifier, (list, tuple)) else self.modifier
        self.method_name = method_name
        if self.modifier_s:
            self.modifier_s += '_'


    def decl(self):
        named = ""
        if self.method_name and "named" in self.modifier:
            named = ", {0}".format(self.method_name)
        return "{0}_{2}METHOD_DECL({1}{3})".format(
            self.function.upper(), self.name, self.modifier_s.upper(),
            named)

    def entry(self):
        return "{0}_METHOD_ENTRY(ID, {1})".format(
            self.function.upper(), self.name)
