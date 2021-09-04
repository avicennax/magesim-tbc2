from setuptools import Extension, setup
from Cython.Build import cythonize


setup(
    name="cython_test",
    ext_modules=cythonize(
        Extension(
            "*",
            ["src/testsim/simulation.pyx"],
            language="c++",
            extra_compile_args=["-std=c++11"],
            extra_link_args=["-std=c++11"]
        )
    )
)