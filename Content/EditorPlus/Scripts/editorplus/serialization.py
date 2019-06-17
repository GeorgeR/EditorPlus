# for working with external and unreal serialized data (mostly json stuff)

import unreal

try:
    import jsonpickle

    class UnrealVectorHandler(jsonpickle.handlers.BaseHandler):
        def flatten(self, in_obj, out_data):
            out_data['x'] = in_obj.x
            out_data['y'] = in_obj.y
            out_data['z'] = in_obj.z
            return out_data

        def restore(self, in_data):
            return unreal.Vector(in_data['x'], in_data['x'], in_data['x'])

    jsonpickle.handlers.registry.register(unreal.Vector, UnrealVectorHandler)

except ImportError:
    pass
