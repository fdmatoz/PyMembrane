from __future__ import annotations

from contextlib import contextmanager
import pkgutil
import shutil
import tempfile
from pathlib import Path


@contextmanager
def example_data_path(relative_path: str):
    repo_root = Path(__file__).resolve().parents[2]
    source_path = repo_root / "docs" / "examples" / relative_path
    if source_path.exists():
        yield source_path
        return

    data = pkgutil.get_data("pymembrane.examples.data", relative_path)
    if data is None:
        raise FileNotFoundError(relative_path)

    temp_root = Path(tempfile.mkdtemp(prefix="pymembrane-data-"))
    try:
        path = temp_root / relative_path
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(data)
        yield path
    finally:
        shutil.rmtree(temp_root, ignore_errors=True)
