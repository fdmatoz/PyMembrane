from __future__ import annotations

import subprocess
import sys
from pathlib import Path

import pytest


def test_edge_flip_benchmark_tiny_smoke():
    repo_root = Path(__file__).resolve().parents[1]
    script = repo_root / "benchmarks" / "benchmark_edge_flip.py"
    result = subprocess.run(
        [sys.executable, str(script), "--preset", "tiny", "--steps", "1", "--seed", "1"],
        cwd="/tmp",
        text=True,
        capture_output=True,
        check=False,
    )
    if result.returncode == 2 and "Could not import pymembrane" in result.stdout:
        pytest.skip("pymembrane native extension is not installed in this environment")
    assert result.returncode == 0, result.stdout + result.stderr
    assert "preset: tiny" in result.stdout
    assert "steps: 1" in result.stdout
