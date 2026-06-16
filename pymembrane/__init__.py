# Copyright 2023 Daniel Matoz Fernandez, fdamatoz@gmail.com
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
# documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
# the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
# and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or substantial portions 
#  of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
# TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
# CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
# DEALINGS IN THE SOFTWARE.

from __future__ import absolute_import

import sys

from . import cppmodule as _cppmodule
from .cppmodule import dump as dump
from .cppmodule import md as md
from .cppmodule import *  # noqa: F401,F403
from .docs import docs_path, offline_docs_available, offline_docs_root, open_docs

sys.modules[__name__ + ".dump"] = dump
sys.modules[__name__ + ".md"] = md

__version__ = _cppmodule.__version__
__release_date__ = _cppmodule.__release_date__
__compiled_date__ = _cppmodule.__compiled_date__
__OS__ = _cppmodule.__OS__

__info__ = (
    "PyMembrane\nversion: {}\nrelease date: {}\ncompiled date:{}\ncompiled OS:{}\n".format(
        __version__, __release_date__, __compiled_date__, __OS__
    )
)

__all__ = [name for name in dir(_cppmodule) if not name.startswith("_")]
__all__.extend(
    [
        "dump",
        "md",
        "docs_path",
        "offline_docs_available",
        "offline_docs_root",
        "open_docs",
        "__info__",
        "__version__",
        "__release_date__",
        "__compiled_date__",
        "__OS__",
    ]
)
