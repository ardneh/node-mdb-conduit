#!/usr/bin/env bash

# TODO: probably want to replace this with a git submodule.
if [ ! -d third_party/mdb-conduit ]; then
	git clone -b master --depth 1 --single-branch ../mdb-conduit # https://github.com/mongodb/mongo third_party/mongo
	# git clone -b v2.6 --depth 1 --single-branch https://github.com/mongodb/mongo third_party/mongo
fi
