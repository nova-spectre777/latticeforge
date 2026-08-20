import json
from .benchmark import run
print(json.dumps(run(),indent=2))
