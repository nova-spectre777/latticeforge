#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cmake -S "$ROOT/engine-cpp" -B "$ROOT/engine-cpp/build" >/dev/null
cmake --build "$ROOT/engine-cpp/build" -j2 >/dev/null
ctest --test-dir "$ROOT/engine-cpp/build" --output-on-failure
PYTHONPATH="$ROOT/lab-python" python3 -m unittest discover -s "$ROOT/lab-python/tests" -v
(cd "$ROOT/studio-ts" && npx --yes tsc -p tsconfig.json && npx --yes tsc -p tsconfig.test.json && node --test build-tests/tests/*.test.js)
"$ROOT/engine-cpp/build/latticeforge-search" "$ROOT/examples/search.json" > "$ROOT/examples/search-output.json"
grep -q 'SPECTRA-Omega' "$ROOT/examples/search-output.json"
echo 'ALL VERIFIED'
