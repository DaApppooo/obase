#!/bin/env python3
# Multi-platform build script.

from sys import argv
import platform 
import shutil
import os
import pathlib
import subprocess as subp

VERBOSE = True
TARGET = platform.system()

def pinfo(msg):
  print("[INFO]", msg)
def pwarn(msg):
  print("[WARN]", msg)
def perr(msg):
  print("[ERR]", msg)
def has_program(name: str):
  return bool(shutil.which(name))
def need_program(name: str):
  if not has_program(name):
    perr(f"Missing program {name!r}.")
    quit(1)

def shell(s: str | list[str], crash = True, hide = False, help: str | None = None, direct_output: bool = True):
    if not isinstance(s, str):
        s = ' && '.join(s)
    if VERBOSE and not hide:
        print(f"\x1b[1;32m$\x1b[0m {s}")
    if TARGET == 'Windows':
        res = subp.run(s, encoding='utf-8', check=False, capture_output=not direct_output, shell=True, executable='C:\\Windows\\system32\\cmd.exe')
    else:
        res = subp.run(s, encoding='utf-8', check=False, capture_output=not direct_output, shell=True)
    out = res.stdout
    if out and not hide:
      print(res.stdout)
    if res.stderr and not hide:
      print(res.stderr)
    if res.returncode:
        perr(f"Exited with status code {res.returncode}.")
        if help is not None:
            pinfo(help)
        if crash:
            quit(res.returncode)
        return subp.CalledProcessError(res.returncode, s, out, res.stderr)
    if out:
      return out.strip()
    else:
      return out
class chdir:
  def __init__(self, p: str):
    self.prev = os.getcwd()
    os.chdir(p or '.')
  def __enter__(self):
    pass
  def __exit__(self, *a):
    os.chdir(self.prev)
def wget(src: str, dst: str):
  if TARGET == 'Windows':
    shell(f"powershell wget '{src}' -OutFile '{dst}'")
  else:
    shell(f"wget '{src}' -O '{dst}'")
def extract(src: str, dst: str, archive_ext: str | None = None):
  """
  Will extract archive 'src' in directory 'dst'.
  If 'src' contains a folder which contains X, X will be moved into 'dst'.
  Otherwise, the content of 'src' is moved into 'dst'.
  """
  tmp = f'_extract_tmp{os.getpid()}'
  ensure_dir(tmp)
  try:
    shutil.unpack_archive(src, tmp, archive_ext)
    content = ls(tmp)
    if len(content) > 1:
      ensure_dir(dst)
      mv(os.path.join(tmp, '*'), dst)
    elif len(content) == 0:
      pwarn(f"Extracted empty archive '{src}'.")
    elif os.path.isdir(content[0]):
      mv(os.path.join(content[0], '*'), dst)
    else:
      mv(os.path.join(content[0], '*'), dst)
  except:
    raise
  finally:
    rm(tmp)
def archive(src: str | list[str], dst: str):
  """
  Will archive 'src' in 'dst'.
  """
  tmp = f'_archive_tmp{os.getpid()}'
  ensure_dir(tmp)
  try:
    _dst = pathlib.Path(dst)
    if isinstance(src, str):
      if os.path.isdir(src):
        cp(os.path.join(src, '*'), tmp)
      else:
        cp(src, tmp)
    else:
      for p in src:
        cp(p, tmp)
    shutil.make_archive(_dst.stem, _dst.suffix[1:], tmp, tmp)
  except:
    raise
  finally:
    rm(tmp)
def rm(p: str):
  if not os.path.exists(p):
    return
  pinfo(f"Removing {p}.")
  if os.path.isdir(p):
    os.rmtree(p)
  else:
    os.remove(p)
def ensure_dir(path):
  if not os.path.exists(path):
    os.makedirs(path)
  elif not os.path.isdir(path):
    perr(f"Path {path} should be a directory.")
    quit(1)
def cp(src: str, dst: str):
  ensure_dir(os.path.dirname(dst))
  pinfo(f"Copying '{src}' to '{dst}'.")
  if os.path.isdir(src):
    shutil.copytree(src, dst)
  else:
    shutil.copyfile(src, dst)
def mv(src: str, dst: str):
  ensure_dir(os.path.dirname(dst))
  pinfo(f"Moving '{src}' to '{dst}'.")
  os.rename(src, dst)
def ls(p: str):
    if not p:
        p = '.'
    return [ os.spath.join(p, i) for i in os.listdir(p) ]
def lsr(p: str, fltr = lambda x : True, force_recurse = False):
  l = []
  for i in ls(p):
    if os.path.exists(os.path.join(i, '.git')):
        continue
    if os.path.isdir(i):
        l.extend(lsr(i, fltr))
    elif fltr(i):
        l.append(i)
  return l

match argv[1:]:
  case ('setup',):
    if TARGET == "Windows":
      assert False, "TODO"
    elif has_program('apt'): # debian based, looking for qt6
      pinfo("Installing qt6 library...")
      if not os.getuid() == 0:
        perr("Please run this command as root.")
        quit(1)
      shell("apt install qt6-base-dev")
    else:
      assert False, "TODO"
  case ('test',):
    need_program("cmake")
    shell("cmake -DCMAKE_BUILD_TYPE=Debug -B bin")
    shell("cmake --build bin --target test")
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

