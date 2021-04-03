import qcalc.sym
import qcalc.mvp

# ! IMPORTANT ! The user may import / export symbol tables, so handle variables, constants and functions with care
# (eg. only in a registered script function.)
# Also the user is able to modify the symbol table through the gui.
# Scripts are not exported / imported by user interaction.
# The addons will be unloaded before import and loaded again after import.


# Our script function callback which is invoked by the expression parser
# when it encounters the defined symbol name in a expression.
def evaluate(*args):
    return 422


# Our script function callback which takes at least one argument
def evaluate_args(*args):
    return args[0]


class Test:
    def evaluate(self):
        pass


test = Test


def load():
    print("Loading sym sample addon")

    global test

    sym = qcalc.mvp.get_presenter_symboltable()

    sym.set_variable("pyVar", 42)
    sym.set_constant("pyConst", 3.141)

    # A native function with no arguments
    sym.set_function("pyFunc", qcalc.sym.Function("42 + sin(32)"))

    # A native function with 1 argument called "arg1"
    sym.set_function("pyFuncArgs", qcalc.sym.Function("133 + cos(arg1)", {"arg1"}))

    # A script function with no arguments
    sym.set_script("pyScript", qcalc.sym.ScriptFunction(evaluate))

    # A script function which accepts at least 1 argument
    sym.set_script("pyScriptArgs", qcalc.sym.ScriptFunction(evaluate_args, True))

    test = Test()

    # Use a member function of a class instance to test cleanup, because a static module function
    # object is never destroyed and therefore no memory is leaked even
    # without decrementing the reference to the callback on the native side.
    sym.set_script_noargs("pyScriptTest", test.evaluate)

    qcalc.mvp.set_presenter_symboltable(sym)


def unload():
    print("Unloading sym sample addon")

    sym = qcalc.mvp.get_presenter_symboltable()

    sym.remove("pyVar")
    sym.remove("pyConst")
    sym.remove("pyFunc")
    sym.remove("pyFuncArgs")
    sym.remove("pyScript")
    sym.remove("pyScriptArgs")
    sym.remove("pyScriptTest")

    qcalc.mvp.set_presenter_symboltable(sym)


print("Importing sym sample addon")
