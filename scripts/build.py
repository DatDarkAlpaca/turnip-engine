import os
import sys
import platform
import subprocess

mono_directory = os.environ["MONO_SDK"]
mono_sgen = os.path.join(mono_directory, '')

def prepare_mono_dependencies():
    



def execute_premake(*args):
    action = sys.argv[1]
    options = list(sys.argv[2:])
    subprocess.run(['premake5', action] + options)


def main():
    # Arguments:
    if len(sys.argv) < 2:
        return print('Usage: build <action>')

    # Mono:


    # Premake:
    print('Running premake...')
    execute_premake(sys.argv)


if __name__ == '__main__':
    main()
