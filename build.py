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
@setup.linux
@setup.windows
def nanosvg():
  ensure_dir("include")
  ensure_dir("licenses")
  need_program("git")
  if os.path.exists("nanosvg"):
    rm("nanosvg")
  shell("git clone https://github.com/memononen/nanosvg.git")
  mv("nanosvg/LICENSE.txt", "licenses/nanosvg.txt")
  rm("nanosvg")
  wget("https://raw.githubusercontent.com/raylib-extras/examples-c/refs/heads/main/textures_svg_loading/nanosvg.h", "include/nanosvg.h")
  wget("https://raw.githubusercontent.com/raylib-extras/examples-c/refs/heads/main/textures_svg_loading/nanosvgrast.h", "include/nanosvgrast.h")
@setup.linux
@setup.windows
def eva_icons():
  wget("https://akveo.github.io/eva-icons/eva-icons.zip", "eva-icons.zip")
  extract("eva-icons.zip", "tmp")
  mv("tmp/outline/svg/*", "res")
  pinfo("You can find these icons here: https://github.com/akveo/eva-icons")
  pinfo("Downloading license...")
  if os.path.exists("eva-icons"):
    rm("eva-icons")
  shell("git clone --depth=1 https://github.com/akveo/eva-icons.git")
  mv("eva-icons/LICENSE.txt", "licenses/eva-icons.txt")
  rm("tmp")
  rm("eva-icons.zip")
  rm("eva-icons")

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
