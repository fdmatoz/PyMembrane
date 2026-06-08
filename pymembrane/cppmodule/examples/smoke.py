from __future__ import annotations

import argparse
from pathlib import Path

import pymembrane as mb


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--quick", action="store_true")
    parser.add_argument("--output-dir", type=Path, default=None)
    args = parser.parse_args()

    example_dir = Path(__file__).resolve().parent
    output_dir = args.output_dir or example_dir
    output_dir.mkdir(parents=True, exist_ok=True)
    box = mb.Box(1.0, 1.0, 1.0, False, False, False)
    system = mb.System(box)
    dump_base = output_dir / "smoke_mesh"
    system.dumper.vtk(str(dump_base))
    system.dumper.obj(str(dump_base))

    print("pymembrane:", Path(mb.__file__).resolve())
    print("example_dir:", example_dir)
    print("vtk:", dump_base.with_suffix(".vtk"))
    print("obj:", dump_base.with_suffix(".obj"))
    print("box_volume:", box.volume())
    print("vertices:", system.Numvertices)

    if not args.quick:
        evolver = mb.Evolver(system)
        print("evolver:", type(evolver).__name__)


if __name__ == "__main__":
    main()
