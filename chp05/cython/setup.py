from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

ext_modules = [Extension("test", ["test.pyx"])]
setup(
   name = 'random number sum application',
   cmdclass = {'build_ext' : build_ext },
   ext_modules = ext_modules
)

