create_project("turnip-engine", "StaticLib")
    externalwarnings "off"
    links { "turnip-script" }

    setup_vendors()
    define_graphics_api()

    setup_platform()
    detect_platform()
