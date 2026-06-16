from __future__ import annotations

import argparse
import webbrowser
from importlib import resources
from pathlib import Path


def _source_checkout_docs_root() -> Path | None:
    repo_root = Path(__file__).resolve().parents[1]
    docs_root = repo_root / "docs" / "_build" / "html"
    if docs_root.joinpath("index.html").exists():
        return docs_root
    return None


def offline_docs_root() -> Path:
    candidate = resources.files("pymembrane") / "_offline_docs"
    docs_root = Path(candidate)
    if docs_root.joinpath("index.html").exists():
        return docs_root

    source_docs = _source_checkout_docs_root()
    if source_docs is not None:
        return source_docs

    raise FileNotFoundError(
        "Offline HTML documentation is not bundled. Build the docs with "
        "`sphinx-build -b html ./docs ./docs/_build/html` or populate "
        "`pymembrane/_offline_docs` before building the wheel."
    )


def offline_docs_available() -> bool:
    try:
        offline_docs_root()
    except FileNotFoundError:
        return False
    return True


def docs_path(page: str = "index.html") -> Path:
    return offline_docs_root() / page


def open_docs(page: str = "index.html") -> Path:
    target = docs_path(page).resolve()
    webbrowser.open(target.as_uri())
    return target


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(
        description="Locate or open the bundled offline PyMembrane HTML documentation.",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
    )
    parser.add_argument(
        "--page",
        default="index.html",
        help="Relative HTML page inside the offline documentation bundle.",
    )
    parser.add_argument(
        "--open",
        action="store_true",
        help="Open the requested page in the default web browser.",
    )
    args = parser.parse_args(argv)

    target = open_docs(args.page) if args.open else docs_path(args.page)
    print(target)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
