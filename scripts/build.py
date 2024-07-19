import sys
import subprocess


def execute_premake(action: str):
    subprocess.run(['premake5', action])


def main():
    # Arguments:
    if len(sys.argv) < 2:
        print('Usage: build <action>')
        return

    # Vulkan:
    # TODO: Detect vulkan

    # Premake:
    print('Running premake...')
    action = sys.argv[1]
    execute_premake(action)


if __name__ == '__main__':
    main()
