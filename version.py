# from https://hatita.com/2018/12/ue4-git-sematic-versioning-tool/#new-version
# edited by George Rolfe

import sys
import subprocess
import os

CONFIG = {
    'Prefix': 'SEMVER_',
    'Meta': 'meta-',
    'Version': 'v'
}

BUILD_TAG_QUERY = 'git describe --always --tags --abbrev=0 --match {Meta}*'.format(Meta=CONFIG['Meta'])
VERSION_TAG_QUERY = 'git describe --always --tags --match {Version}*'.format(Version=CONFIG['Version'])
BUILD_HASH_QUERY = 'git describe --always --abbrev=12 --exclude "*"'

# Versioning via 

# Get last tag-* that describes a version tag
# git describe --always --tags --abbrev=0 --match {Meta}*
# -> last Meta

# Get last version tag
# git describe --always --tags --match v*
# -> v{Major}.{Minor}.{Patch}-{buildmeta}

class SemanticVersion(object):
    def __init__(self):
        self.major = 0
        self.minor = 0
        self.patch = 0
        self.build_meta = 0        
        self._build()

        self.build_tag = ''
        self.use_build_tag = self._get_build_tag()

        self.build_hash = self._get_build_hash()

        self.meta_tag = self.build_tag

    @property
    def version_string(self):
        params = {
            'major': self.major,
            'minor': self.minor,
            'patch': self.patch,
            'meta_tag': self.build_tag,
            'build_meta': self.build_meta
        }
        if self.use_build_tag:
            return '{major}.{minor}.{patch}-{meta_tag}+{build_meta}'.format_map(params)
        else:
            return '{major}.{minor}.{patch}+{build_meta}'.format_map(params)

    def _build(self):
        semver_process = subprocess.run(VERSION_TAG_QUERY, text=True, capture_output=True)
        process_stdout = semver_process.stdout
        if process_stdout[:1] == CONFIG['Version']: # is valid version tag
            raw_version = process_stdout[len(CONFIG['Version']):]
            version_array = raw_version.split('.')
            build_meta = raw_version.split('-')
            if len(build_meta) > 1:
                version_array[2] = version_array[2].split('-')[0]
                build_meta = int(build_meta[1])
            else:
                build_meta = 0

            self.major = int(version_array[0])
            self.minor = int(version_array[1])
            self.patch = int(version_array[2])
            self.build_meta = build_meta

    def _get_build_tag(self):
        build_tag_process = subprocess.run(BUILD_TAG_QUERY, text=True, capture_output=True)
        build_tag = build_tag_process.stdout[:len(CONFIG['Meta'])] # Cut tag-
        if build_tag == CONFIG['Meta']: 
            build_tag = build_tag_process.stdout[len(CONFIG['Meta']):] # Cut tag-
            self.build_tag = build_tag[:-1] # also remove the \n
            return True
        return False

    def _get_build_hash(self):
        build_hash_process = subprocess.run(BUILD_HASH_QUERY, text=True, capture_output=True)
        return str(build_hash_process.stdout)[:-1]

def populate_template(version_major, version_minor, version_patch, product_name='SomeModule', header="Copyright Header"):
    product_name = product_name.upper()
    content = '''
    // {header}

    #pragma once

    // See https://semver.org/

    #define {product_name}_VERSION_MAJOR {version_major}
    #define {product_name}_VERSION_MINOR {version_minor}
    #define {product_name}_VERSION_PATCH {version_patch}

    #define {product_name}_VERSION_STRING "{version_string}"

    /** Extra identifier string (like: "alpha", "beta.1", "beta.2", "test", "featureX", etc.). */
    //#define {product_name}_VERSION_ID_STRING "alpha"

    #ifdef {product_name}_VERSION_ID_STRING
    #define {product_name}_VERSION_STRING_EX "v" {product_name}_VERSION_STRING "-" {product_name}_VERSION_ID_STRING
    #else
    #define {product_name}_VERSION_STRING_EX "v" {product_name}_VERSION_STRING
    #endif
    '''

    version_string = 'abc'

    params = {
        'header': copyright,
        'product_name': product_name,
        'version_major': version_major,
        'version_minor': version_minor,
        'version_patch': version_patch,
        'version_string': version_string
    }

    return content.format_map(params)


def main(output_module_name=None):
    path = os.path.dirname(os.path.realpath(__file__))
    path = os.path.join(path, 'Source', output_module_name, 'Public')
    os.chdir(path)

    version = SemanticVersion()

    output_dir = path
    print(output_dir)
    output_file_path = os.path.join(output_dir, 'Version.h')

    # if os.path.exists(output_file_path):
    #     with open(output_file_path, 'r') as file:
    #         first_line = file.readline()
    #         if version_string + " " + build_hash in first_line:
    #             print('No Version Update required.')
    #             exit(0)

    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    print(version)

    content = populate_template(version.major, version.minor, version.patch, output_module_name)

    with open(output_file_path, 'w') as file:
        file.write(content)

if __name__ == '__main__':
    main(sys.argv[1:])