# -----------------------------------------------------------
# export to txt file the position and rotation of selected
# items in the scene
#
# 1. select Scripting on top menu bar
# 2. open this scipt in editor
# 3. select the items you want to export
# 4. press the run script
# -----------------------------------------------------------

import bpy
import sys

original_stdout = sys.stdout

#output to same directory as the opened .blend file
scenefilePath = bpy.path.abspath("//scene.txt")

with open(scenefilePath, 'w') as f:
    sys.stdout = f
    for ob in bpy.context.selected_objects:
        print("Object:", ob.name)
        print("%f,%f,%f" % ob.location[:])
        print("%f,%f,%f" % ob.rotation_euler[:])
        print('')
    sys.stdout = original_stdout