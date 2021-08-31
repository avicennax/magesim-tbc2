from distutils.core import setup, Extension

module = Extension(
    'magesim',
    sources=['src/rl/pybindings.c'],
    include_dirs=["src/sim", "src/rl"],
    extra_objects=["build/sim/interface.o"]
)

setup(
    name = 'MageSim',
    version = '1.0',
    description = 'Python interface to MageSim',
    ext_modules = [module]
)