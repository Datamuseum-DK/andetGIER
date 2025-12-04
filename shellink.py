
import sys
import linklib

args = sys.argv[1:]
if len(args) > 0: (BIND,args) = (args[0],args[1:])

linklib.serve()
