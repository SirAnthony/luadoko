Luadoko
================

C++ library to bind classes to lua.
The purpose of this library to generate lua bindings for specific C++
classes. It consist from generator and some helpers. The class
information must be specified in json form and passed to generator,
then the binding headers will be generated.

Requirements
------

- python
- mako (python template engine)

Basic usage
------
1. Specify classes definitions in definitions.json (or any other file and
pass it as argument to generator)
#. `Run generator`__. It is python script `generator.py` which will
    generate required headers.
#. `Register classes`__.
#. `Register modules`__.
#. Compile your project.


Data format
================

Source for class information is json object with class defenitions.
Simple example first::
    {
        "SuperClass": {
            "includes": "include/SuperClass.h"
        },
        "Class": {
            "inherit": "SuperClass",
            "includes": [
                "include/Class.h",
                "include/Class.h",
            ],
            "methods": [
                ["getset", "color"],
                ["getset", "ambient"],
                ...
            ]
        }
    }


Classes defines by its name. Name must be same as in c++ code, it will
be used directly to create bindings. Classes may contain attributes.
:includes:   Paths to include files where class can be found. May be
             string or array with strings as well.
:methods:    Description of methods_ avaliable for lua object.
:inherit:    String with name of class current class inherit. All methods
             of superclass will appear in subclass.


.. _methods:

Methods
----------------
The methods declaration is array with two required items and trailing
modifiers. First item of array defines type of binding. Second item
is function name to bind to this binding. Third parameter is function
modifiers. It may be simple string or list with strings. Fourth element
is needed only when `named` used as one of modifiers. It is string,
allow to specify name for lua object function for this method. For
example `["getter", "ItemId", "named", "id"]` must be defined as `ItemId`
method for c++ class but appear as id method in lua object.

Binding types
----------------
There are three basic types of bindings.
:getter:    Bind to getter method. Only returns something.
:getset:    Both getter and setter will be generated. Call with argument
            setups argument as a value, without argument returns it.
:exec:      Treat method as lua function. This binding will pass current
            lua_State* as function parameter. Function must return the
            number of elements on the stack it creates.

Modifiers
-----------------

For getter and getset types next modifiers are avaliable:
:func:      Changes behaviour of binding. Without this modifier all get
            and set parameters will be treat as attributes. Getter
            for `id` will be writen as `Instance->id` without it and as
            `Instance->id()` with it.
:named:     Using with foruth element allows to specify custom name for
            the binding.
.. :checked:   :checked_nil:

Getset specific modifiers:
:flag:      Treat binding as a flag. Class must have `isNAME` `setNAME`
            and `clearNAME` methods to use this. Prefixes can be
            changed in `src\defines.h`.


There only two modifiers avaliable for exec type.
:named:     Works like the same modifier for getter and setter
:noret:     Set executing function to not return anything.


.. _`Run generator`:

Generator
================

The generator is simple python script that take data in json format and
generates bindings as it says. There are two files generates:
`CUDataBind.h` and `TypeListDef.h`. First contains the actual binding
macros and second only helper defines. Both have internal use only so
there nothing interesting here.
Generator may accept filename as first parameter.

Register classes
================

There some work must be done to integrate bindings in code.
1. All classes must have CUDataUser as its public superclass.
2. Class must have protected method `virtual CUData* createUData()`
   defined. It only requires header defenition but never implementation.
   You can implement it though, this code will run when lua user data
   will be created.

Actually you need only `CUDataUser.h` header in class header.
General example, Class.h::
    #include "luadoko/include/CUDataUser.h"

    class CUData;

    class Class : public CUDataUser
    {
    protected:
        virtual CUData* createUData();
    }

The Class is ready to bind to lua. Some methods can be added to make
it useful.


Register modules
================

One more preparation must be done to successfully binding. In lua
initialization in main program the `LuaRoutines::routines_open` method,
avaliable in 'LuaRoutines.h', must be called. It'll initialize all
classes and make it ready for use.


