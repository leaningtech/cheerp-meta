ifeq ($(OS),Windows_NT)
	CHEERP ?= C:/cheerp
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		CHEERP ?= /opt/cheerp
	else ifeq ($(UNAME_S),Darwin)
		CHEERP ?= /Applications/cheerp
	endif
endif

%.js: %.cpp
	${CHEERP}/bin/clang++ $< -o $@

main.js:
