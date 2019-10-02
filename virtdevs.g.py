#!/usr/bin/env python3

import licant

licant.module("virtdevs",
	sources=["virtdevs/device.cpp"],
	include_paths=["."]
)