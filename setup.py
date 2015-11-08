from distutils.core import setup, Extension
from distutils.command.build import build as DistutilsBuild
import os, platform

make = 'make'
ext = 'so'
if platform.system() == 'Darwin':
    ext = 'dylib'
elif platform.system().contains('BSD'):
    make = 'gmake'

class Build(DistutilsBuild):
    def run(self):
        os.system(make)
        DistutilsBuild.run(self)

setup(name='twombly',
    cmdclass={'build': Build},
    version='0.1',
    description='vector graphics library',
    author='Zach Shipko',
    author_email ='zachshipko@gmail.com',
    url='https://github.com/zshipko/libtwombly',
    packages=['twombly'],
    package_dir = {'twombly': 'py/twombly'},
    data_files=[('lib', ('libtwombly.' + ext,))]
)
