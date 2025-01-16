import sys
import subprocess


def execute_premake(*args):
    action = sys.argv[1]
    options = list(sys.argv[2:])
    subprocess.run(['premake5', action] + options)


def main():
    # Arguments:
    if len(sys.argv) < 2:
        print('Usage: build <action>')
        return

    # Premake:
    print('Running premake...')
    
    execute_premake(sys.argv)


if __name__ == '__main__':
    main()
