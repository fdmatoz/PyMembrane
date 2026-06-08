from __future__ import annotations

from contextlib import contextmanager
from importlib import import_module
from importlib import resources
from pathlib import Path


@contextmanager
def example_data_dir(package_name: str):
    if hasattr(resources, "files"):
        root = resources.files(package_name) / "data"
        with resources.as_file(root) as path:
            yield Path(path)
        return

    module = import_module(package_name)
    data_dir = Path(module.__file__).resolve().parent / "data"
    if not data_dir.exists():
        raise FileNotFoundError(f"missing data directory for {package_name}")
    yield data_dir


@contextmanager
def example_data_path(package_name: str, relative_path: str):
    with example_data_dir(package_name) as data_dir:
        path = data_dir / relative_path
        if not path.exists():
            raise FileNotFoundError(f"{package_name}:{relative_path}")
        yield path
