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

hello.js: hello.cpp
	${CHEERP}/bin/clang++ $< -o $@
