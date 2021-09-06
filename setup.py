from setuptools import Extension, setup
from Cython.Build import cythonize


with open("README.md", "r", encoding="utf-8") as fh:
    long_description = fh.read()

VERSION = 0.1
NAME = "magesim"
LICENSE = "MIT"

setup(
    name=NAME,
    version=VERSION,
    packages=[NAME],
    license=LICENSE,
    package_dir={"": "src"},
    ext_modules=cythonize(
        Extension(
            "*",
            ["src/magesim/simulation.pyx"],
            language="c++",
            extra_compile_args=["-std=c++11"],
            extra_link_args=["-std=c++11"]
        )
    ),
    install_requires=["gym"]
)