from __future__ import annotations

import pymembrane
from pymembrane import *  # noqa: F401,F403


def test_import_smoke():
    assert pymembrane.__file__
    b = Box(1.0, 1.0, 1.0, False, False, False)
    system = System(b)
    assert int(system.Numvertices) == 0
