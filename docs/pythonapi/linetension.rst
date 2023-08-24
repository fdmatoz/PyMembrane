Line Tension
============

.. contents::
   :local:
   :depth: 2

Definition
----------
.. list-table::
   :widths: 25 75
   :header-rows: 0

   * - **Name**
     - Mesh>Line Tension
   * - **Type**
     - Limit
   * - **Defined in**
     - Vertices

Iterate over all neighbors of a given vertex. If a current neighbor is of a different type, add :math:`\gamma` to the line tension energy. This is the most basic form of the line tension energy. 

The line tension energy associated with a vertex is given by:

.. math::
   U = \sum_{\text{neighboring edges}} \gamma_{\text{type}} \times s

Where:

- :math:`\gamma_{\text{type}}` is determined by:
  - Averaging the :math:`\gamma` values of the two vertices forming the edge if `flag_avg` is true: :math:`\gamma_{\text{type}} = 0.5 \times (\gamma[\text{type_0}] + \gamma[\text{type_1}])`
  - Taking the :math:`\gamma` value of the query vertex if `flag_avg` is false: :math:`\gamma_{\text{type}} = \gamma[\text{type_0}]`
- \(s\) is either:
  - The Euclidean distance between the two vertices if `flag_scale` is true: :math:`s = ||\textbf{r}_{\text{query_vertex}} - \textbf{r}_{\text{neighboring_vertex}}||`
  - 1.0 if `flag_scale` is false.

Python calling
--------------

.. code-block:: python

   evolver.add_force("Mesh>Limit", {
       "gamma": {"1": str(gamma_value)},
        "avg": "True",
        "scale": "True"})
