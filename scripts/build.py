import os
import sys
import shutil
import platform
import subprocess

turnip_editor = './turnip-editor'
vendor_directory = './vendor'

mono_directory = os.environ["MONO_SDK"]
mono_sgen_filename = 'mono-2.0-sgen.dll'
mono_sgen_directory = os.path.join(mono_directory, 'bin', mono_sgen_filename)


def prepare_mono_dependencies():
    if platform.system() == 'Windows':
        shutil.copyfile(mono_sgen_directory, os.path.join(turnip_editor, mono_sgen_filename))    


def execute_premake():
    action = sys.argv[1]
    options = list(sys.argv[2:])
    subprocess.run(['premake5', action] + options)


def main():
    # Arguments:
    if len(sys.argv) < 2:
        return print('Usage: build <action>')

    # Mono:
    prepare_mono_dependencies()

    # Premake:
    print('Running premake...')
    execute_premake()


if __name__ == '__main__':
    main()
