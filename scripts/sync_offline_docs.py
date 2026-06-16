from __future__ import annotations

import shutil
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "docs" / "_build" / "html"
DESTINATION = ROOT / "pymembrane" / "_offline_docs"


def main() -> int:
    if not SOURCE.joinpath("index.html").exists():
        raise FileNotFoundError(
            f"Missing built HTML docs at {SOURCE}. Run "
            "`sphinx-build -b html ./docs ./docs/_build/html` first."
        )

    if DESTINATION.exists():
        shutil.rmtree(DESTINATION)
    shutil.copytree(SOURCE, DESTINATION)
    print(f"Copied offline docs to {DESTINATION}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
