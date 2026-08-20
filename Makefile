.PHONY: check build
check:
	./scripts/check_all.sh
build:
	cmake -S engine-cpp -B engine-cpp/build && cmake --build engine-cpp/build
