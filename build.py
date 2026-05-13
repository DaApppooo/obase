#!/bin/env python3
# Multi-platform build script.

from sys import argv
import config
import os
from utils import (
  pinfo, pwarn, perr, has_program, need_program,
  Setup, shell, chdir, wget, extract, archive, rm, ensure_dir,
  cp, mv, ls, lsr

)
setup = Setup()

@setup.linux
def raylib():
  ensure_dir("include")
  ensure_dir("lib")
  if 'libraylib.so' in ls("lib"):
    return
  wget(
    f"https://github.com/raysan5/raylib/releases/download/{config.RAYLIB_VERSION}/raylib-{config.RAYLIB_VERSION}_linux_amd64.tar.gz",
    "raylib.tar.gz"
  )
  extract("raylib.tar.gz", "tmp")
  mv("tmp/include/*", "include")
  mv("tmp/lib/*", "lib")
  rm("tmp")
  rm("raylib.tar.gz")
@setup.windows
def raylib():
  assert False, "TODO"

def main(argv):
  match argv[1:]:
    case ('setup',):
      setup.run(config.TARGET)
    case ('test',):
      need_program("cmake")
      shell("cmake -DCMAKE_BUILD_TYPE=Debug -B bin")
      shell("cmake --build bin -j 8 --target test")
      shell(os.path.join("bin", "test"))
    # Add other compilation commands here
    case ('db-test',):
      need_program("cmake")
      need_program("gdb")
      shell("cmake -DCMAKE_BUILD_TYPE=Debug -B bin")
      shell("cmake --build bin --target test")
      shell("gdb --args " + os.path.join("bin", "test"))
    case ('asan-test',):
      need_program("cmake")
      shell("cmake -DCMAKE_BUILD_TYPE=Debug -B bin")
      shell("cmake --build bin --target asan_test")
      shell(os.path.join("bin", "asan_test"))
    case _:
      perr("No command specified.")
      quit(1)


if __name__ == "__main__":
  main(argv)
