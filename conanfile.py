from conan import ConanFile


class TurnipEngine(ConanFile):
    settings = 'os', 'compiler', 'build_type', 'arch'
    generators = 'PremakeDeps'

    def requirements(self):
        self.requires('glfw/3.3.8')
        self.requires('spdlog/1.12.0')
        self.requires('glad/0.1.36')
        self.requires('glm/cci.20230113')
        self.requires('vulkan-loader/1.3.268.0')
        self.requires('entt/3.13.0')
        self.requires('vulkan-memory-allocator/3.0.1')

    def configure(self):
        self.options['glad'].gl_profile = 'core'
        self.options['glad'].gl_version = '4.5'