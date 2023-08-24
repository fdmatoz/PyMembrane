# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))
from datetime import datetime


# -- Project information -----------------------------------------------------

project = 'PyMembrane'
copyright = f"{datetime.now().year} " + 'Daniel A Matoz-Fernandez and Contributors'
author = 'Daniel A Matoz-Fernandez'

# The full version, including alpha/beta/rc tags
version = '1.0'


# -- General configuration ---------------------------------------------------

extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.autosummary',
    'sphinx.ext.coverage',
    "sphinx_design",
    'sphinx.ext.mathjax',
    'sphinxcontrib.bibtex',
    'breathe',
    'exhale',
    'numpydoc',
]
# Setup the bibtex
bibtex_bibfiles = ['references.bib']


# Setup 'exhale'
exhale_args = {
    "containmentFolder": "./cppapi",
    "rootFileName": "cppapi.rst",
    "rootFileTitle": "C++ API",
    "doxygenStripFromPath": "..",
    "createTreeView": True,
    "exhaleExecutesDoxygen": True,
    "exhaleDoxygenStdin": "INPUT = ../pymembrane/cppmodule/src"
}

# Setup breathe
breathe_projects = {"pymembrane": "./cppapi/xml"}
breathe_default_project = "pymembrane"


# Tell sphinx what the primary language being documented is.
primary_domain = 'python'

# Tell sphinx what the pygments highlight language should be.
highlight_language = 'python'

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'pydata_sphinx_theme'
html_theme_options = {
  #"show_nav_level": 2,
  'navigation_depth': -1,
  "github_user": "fdmatoz",
  "github_repo": "PyMembrane",
  "github_version": "main",
  #"collapse_navigation": True,
  "navbar_end": ["theme-switcher", "navbar-icon-links"],
  "logo": {
      "image_light": "_static/logo-dark.svg",
      "image_dark": "_static/logo-light.svg",
   }
}

html_context = {
    #"doc_path": "docs",
    "default_mode": "light",
}
# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_logo = "_static/logo-dark.svg"
html_favicon = "_static/logo.ico"
html_static_path = ['_static']
html_css_files = ['custom.css']
html_show_sourcelink = False
#Exclude pages
html_additional_pages = {}
html_use_modindex = True
html_domain_indices = False
html_copy_source = False
html_file_suffix = '.html'
htmlhelp_basename = 'pymembrane'

#exclude_patterns = ['installation.rst', 'quickstart.rst', 'tutorial.rst', 'cppapi/library_root.rst','pythonapi/library_root.rst' ]