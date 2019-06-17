# for interacting with unreal assets

import unreal

class _AssetFactory(object):
    def __init__(self):
        self._asset_class_factories = {
            unreal.DataAsset: unreal.DataAssetFactory()
        }

    def register_factory(self, asset_class, asset_factory):
        self._asset_class_factories[asset_class] = asset_factory

    def get_factory(self, asset_class):
        # unreal.MathLibrary.class_is_child_of(asset_class, unreal.StaticMesh)
        # todo: get parent class
        creatable_asset_class = asset_class # ie. anything that derives from dataasset should be dataasset
        if unreal.MathLibrary.class_is_child_of(asset_class, unreal.DataAsset):
            creatable_asset_class = unreal.DataAsset
        
        return self._asset_class_factories[creatable_asset_class]

    # todo: check that asset class is not abstract
    def _get_or_create(self, asset_path='', asset_class=None, unique_name=True, asset_factory=None):
        if unique_name:
            asset_path, asset_name = unreal.AssetToolsHelpers.get_asset_tools().create_unique_asset_name(base_package_name=asset_path, suffix='')

        asset_path = asset_path.rsplit('.', 1)[0]
        
        if not unreal.EditorAssetLibrary.does_asset_exist(asset_path=asset_path):
            path = asset_path.rsplit('/', 1)[0]
            name = asset_path.rsplit('/', 1)[1]
            return unreal.AssetToolsHelpers.get_asset_tools().create_asset(asset_name=name, package_path=path, asset_class=asset_class, factory=asset_factory)
        
        return unreal.load_asset(asset_path)

    def get_or_create(self, asset_path='', asset_class=None, unique_name=True, asset_factory=None):
        if isinstance(asset_class, str):
            asset_class = unreal.load_class(None, asset_class)

        asset_factory = asset_factory if asset_factory else self.get_factory(asset_class)
        if not asset_factory:
            raise Exception('No factory registered for asset_class: %s' % asset_class.get_name())

        return self._get_or_create(asset_path, asset_class, unique_name, asset_factory)

asset_factory = _AssetFactory()