project ?= helloworld

.PHONY: build

.SILENT:
build: bin
	$(CXX) $(project)/**.cpp -std=c++17 -o bin/$(project)

clean:
	rm -rf bin

bin:
	mkdir bin

run:
	./bin/$(project)
