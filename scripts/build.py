import sys
import subprocess


def execute_conan(build_types: str):
    build_type_list = build_types.split()

    for build_type in build_type_list:
        subprocess.run(['conan', 'install', '.', '--build', 'missing',
                    '-of', './dependencies', f"--settings=build_type={build_type}"])


def execute_premake(action: str):
    subprocess.run(['premake5', action])


def main():
    # Arguments:
    if len(sys.argv) != 2:
        print('Usage: build <action>')
        return
    
    # Conan:
    print('> Running conan...')
    build_type = sys.argv[2] if len(sys.argv) > 2 else 'Debug Release'
    execute_conan(build_types=build_type)
    
    # Premake:
    print('Running premake...')
    action = sys.argv[1]
    execute_premake(action)


if __name__ == '__main__':
    main()
