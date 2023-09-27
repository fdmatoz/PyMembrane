#####################################################################################
# MIT License                                                                       #
#                                                                                   #
# Copyright (c) 2023 Dr. Daniel Alejandro Matoz Fernandez                           #
#               fdamatoz@gmail.com                                                  #
#                    Dr. Rastko Sknepnek                                            #
#               r.sknepnek@dundee.ac.uk                                             #
# Permission is hereby granted, free of charge, to any person obtaining a copy      #
# of this software and associated documentation files (the "Software"), to deal     #
# in the Software without restriction, including without limitation the rights      #
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell         #
# copies of the Software, and to permit persons to whom the Software is             #
# furnished to do so, subject to the following conditions:                          #
#                                                                                   #
# The above copyright notice and this permission notice shall be included in all    #
# copies or substantial portions of the Software.                                   #
#                                                                                   #
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR        #
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,          #
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE       #
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER            #
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,     #
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     #
# SOFTWARE.                                                                         #
#####################################################################################
import subprocess
import setuptools
from setuptools.command.install import install
import site
import multiprocessing

class InstallLocalPackage(install):
    def run(self):
        print()
        install.run(self)
        #subprocess.call("cd pymembrane/cppmodule/build && cmake .. && make -j2 && cp cppmodule.* ../", shell=True)
        #subprocess.call("cd pymembrane/cppmodule/build && cmake -DCMAKE_INSTALL_PREFIX={:s}/pymembrane/cppmodule .. && make -j{:d} && make install".format(site.getsitepackages()[0], multiprocessing.cpu_count()//2 - 1), shell=True)
        cpu_cores = max(1, multiprocessing.cpu_count() // 2)
        subprocess.call("cd pymembrane/cppmodule/build && cmake -DCMAKE_INSTALL_PREFIX={:s}/pymembrane/cppmodule .. && make -j{:d} && make install".format(site.getsitepackages()[0], cpu_cores), shell=True)
with open("README.md", "r") as fh:
    long_description = fh.read()
    #print(long_description)

setuptools.setup(
    name="pymembrane", 
    version="1.0",
    author="Daniel Matoz Fernandez",
    author_email="fdamatoz@gmail.com",
    description="Membrane elasticity simulations",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://fdmatoz.github.io/pymembrane/",
    cmdclass=dict(install=InstallLocalPackage), 
    packages=setuptools.find_packages(),
    package_data={'': ['cppmodule.so']},
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: Copyright Daniel Matoz Fernandez",
        "Operating System :: OS Independent",
    ],
    python_requires='>=3.7',
)
