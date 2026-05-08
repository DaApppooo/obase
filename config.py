import platform

VERBOSE = True
TARGET = platform.system()
RAYLIB_VERSION = "6.0"


if TARGET == "Windows":
  SEPARATOR = "\\"
else:
  SEPARATOR = "/"
