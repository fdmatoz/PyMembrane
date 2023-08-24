Box Class
==========

.. contents::
   :local:
   :depth: 2

Description
-----------

`PyMembrane` uses boxes to implement periodic boundary conditions. A `Box` object is used to represent the simulation box and to wrap coordinates into the box. The box can be controlled with 6 parameters: the box lengths in each direction, and the periodicity in each direction. The box is defined by the lower and upper boundaries in each direction. 

Note: All sides of the simulation box have to be orthogonal to each other. 


Creating a box
--------------

There are several ways to initialize a `Box` object. The simplest way is to specify the box lengths in each direction. For example, the following code creates a box with lengths 10, 20, and 30 in the x, y, and z directions, respectively:


- **Box(Lx, Ly, Lz)**: 
  Initializes the box dimensions. 
  Example:
  .. code-block:: python

     box = Box(10, 20, 30)

In addition to the box lengths, the periodicity in each direction can be specified. For example, the following code creates a box with lengths 10, 20, and 30 in the x, y, and z directions, respectively, and periodicity in the x and z directions:

- **Box(Lx, Ly, Lz, periodic_x, periodic_y, periodic_z)**: 
  Initializes the box dimensions and allows specification of periodicity for each dimension.
  Example:
  .. code-block:: python

     box = Box(10, 20, 30, True, False, True)

The box can also be initialized using pairs for the lower and upper boundaries in each direction. For example, the following code creates a box with lower and upper boundaries (-5, 5), (-10, 10), and (-15, 15) in the x, y, and z directions, respectively:

- **Box(Lxpair, Lypair, Lzpair)**: 
  Initializes the box dimensions using pairs for low and high values for each dimension.
  Example:
  .. code-block:: python

     box = Box((-5,5), (-10,10), (-15,15))


Box methods
-----------

In addition to the initialization methods, the `Box` class has several methods that can be used to manipulate or get information from the box

- **scale()**: 
  Scale the box by a factors scalex, scaley, scalez for each dimension.
  Example:
  .. code-block:: python

     box = Box(10, 20, 30)
     box.scale(2.0, 2.0, 2.0)

- **volume**: 
  Return the volume of the box.
  Example:
  .. code-block:: python

     box = Box(10, 20, 30)
     print("The box volume is: ", box.volume)

Attributes
----------

The following attributes are available for the `Box` class:

- **Lhi**: 
  Upper boundary of the box.
  Example:
  .. code-block:: python

     upper_bound = box.Lhi

- **Llo**: 
  Lower boundary of the box.
  Example:
  .. code-block:: python

     lower_bound = box.Llo

- **L**: 
  Box lengths in each direction.
  Example:
  .. code-block:: python

     length = box.L

- **periodic**: 
  Box periodicity in each direction.
  Example:
  .. code-block:: python

     is_periodic = box.periodic

Other methods
--------------

- **__repr__()**: 
  Returns a string representation of the `Box` object, detailing its dimensions and periodicities.
  Example:
  .. code-block:: python

     print(box)

For further information about the `Box` class, please refer to contextual help in python:

.. code-block:: python
  
   help(mb.Box)