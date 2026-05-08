import shutil
import os
import pathlib
import config
import subprocess as subp

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
    if config.VERBOSE and not hide:
        print(f"\x1b[1;32m$\x1b[0m {s}")
    if config.TARGET == 'Windows':
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
  if config.TARGET == 'Windows':
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
    print(content)
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
    shutil.rmtree(p)
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
def mv(src: str, dst: str, shush = False):
  if d := os.path.dirname(dst):
    ensure_dir(d)
  else:
    ensure_dir(dst)
  if not shush:
    pinfo(f"Moving '{src}' to '{dst}'.")
  if src.endswith('*'):
    path, search = src.rsplit(config.SEPARATOR, 1)
    search = search[:-1]
    srcs = [
      i
      for i in ls(path)
      if i.rsplit(config.SEPARATOR,1)[-1].startswith(search)
    ]
    for src in srcs:
      mv(src, dst, shush=True)
  else:
    os.rename(src, os.path.join(dst, src.rsplit(config.SEPARATOR, 1)[-1]))
def ls(p: str):
    if not p:
        p = '.'
    return [ os.path.join(p, i) for i in os.listdir(p) ]
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

class Setup:
  def __init__(self):
    self.setups = {'linux':[], 'windows':[]}
  def linux(self, f):
    self.setups['linux'].append(f)
  def windows(self, f):
    self.setups['windows'].append(f)
  def run(self, target: str):
    if target == 'Windows':
      arr = self.setups['windows']
    else:
      arr = self.setups['linux']
    for s in arr:
      pinfo(f"Setting up {repr(s.__name__)}...")
      s()

